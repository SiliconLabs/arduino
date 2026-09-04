/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "DeviceDoorLock.h"

using chip::app::DataModel::List;
using chip::app::DataModel::Nullable;
using chip::app::DataModel::NullNullable;

DeviceDoorLock::DeviceDoorLock(const char* device_name, uint32_t feature_map) :
  Device(device_name),
  lock_state(lock_state_t::UNLOCKED),
  credential_changed_cb(nullptr),
  door_lock_cluster_feature_map(feature_map)
{
  this->SetDeviceType(device_type_t::kDeviceType_DoorLock);
}

DeviceDoorLock::lock_state_t DeviceDoorLock::GetLockState()
{
  return this->lock_state;
}

void DeviceDoorLock::SetLockState(DeviceDoorLock::lock_state_t state)
{
  bool changed = this->lock_state != state;
  ChipLogProgress(DeviceLayer, "DoorLockDevice[%s]: new lock state='%d'", this->device_name, state);
  this->lock_state = state;

  if (changed) {
    this->HandleDoorLockDeviceStatusChanged(kChanged_LockState);
    CallDeviceChangeCallback();
  }
}

bool DeviceDoorLock::SetLockState(DeviceDoorLock::lock_state_t state,
                                  OperationSourceEnum opSource,
                                  const Nullable<uint16_t>& userIndex,
                                  const Nullable<List<const CredentialStructType>>& credentials)
{
  this->SetLockState(state);
  return DoorLockServer::Instance().SetLockState(this->endpoint_id,
                                                 static_cast<DlLockState>(state),
                                                 opSource, userIndex, credentials);
}

bool DeviceDoorLock::ValidatePIN(const chip::ByteSpan& pin, uint16_t& outUserIndex, uint16_t& outCredentialIndex)
{
  if (!this->HasFeature(Feature::kPinCredential)) {
    return false;
  }
  for (uint16_t i = 0; i < kMaxPinCredentials; i++) {
    CredentialSlot& slot = this->pin_credentials[i];
    if (!slot.occupied) {
      continue;
    }
    if ((slot.dataLength == pin.size()) && (memcmp(slot.data, pin.data(), pin.size()) == 0)) {
      outCredentialIndex = i + 1;
      outUserIndex = this->FindUserForCredential(CredentialTypeEnum::kPin, outCredentialIndex);
      return true;
    }
  }
  return false;
}

DeviceDoorLock::CredentialSlot* DeviceDoorLock::GetCredentialSlot(CredentialTypeEnum type, uint16_t index)
{
  if (index == 0) {
    return nullptr; // Programming PIN (index 0) is not supported by this implementation
  }
  if (type == CredentialTypeEnum::kPin) {
    if (!this->HasFeature(Feature::kPinCredential)) {
      return nullptr;
    }
    return (index <= kMaxPinCredentials) ? &this->pin_credentials[index - 1] : nullptr;
  }
  if (type == CredentialTypeEnum::kRfid) {
    if (!this->HasFeature(Feature::kRfidCredential)) {
      return nullptr;
    }
    return (index <= kMaxRfidCredentials) ? &this->rfid_credentials[index - 1] : nullptr;
  }
  return nullptr;
}

uint16_t DeviceDoorLock::FindUserForCredential(CredentialTypeEnum type, uint16_t credentialIndex)
{
  for (uint16_t i = 0; i < kMaxUsers; i++) {
    UserSlot& slot = this->users[i];
    if (!slot.occupied) {
      continue;
    }
    for (uint8_t c = 0; c < slot.totalCredentials; c++) {
      if ((slot.credentials[c].credentialType == type) && (slot.credentials[c].credentialIndex == credentialIndex)) {
        return i + 1;
      }
    }
  }
  return 0;
}

bool DeviceDoorLock::GetUser(uint16_t userIndex, EmberAfPluginDoorLockUserInfo& user)
{
  if (!this->HasFeature(Feature::kUser)) {
    return false;
  }
  if ((userIndex == 0) || (userIndex > kMaxUsers)) {
    return false;
  }
  UserSlot& slot = this->users[userIndex - 1];

  if (!slot.occupied) {
    user.userStatus = UserStatusEnum::kAvailable;
    return true;
  }

  user.userName = chip::CharSpan(slot.name, slot.nameLength);
  user.credentials = chip::Span<const CredentialStructType>(slot.credentials, slot.totalCredentials);
  user.userUniqueId = slot.uniqueId;
  user.userStatus = slot.status;
  user.userType = slot.type;
  user.credentialRule = slot.credentialRule;
  user.creationSource = (slot.createdBy != chip::kUndefinedFabricIndex) ? DlAssetSource::kMatterIM : DlAssetSource::kUnspecified;
  user.createdBy = slot.createdBy;
  user.modificationSource = (slot.lastModifiedBy != chip::kUndefinedFabricIndex) ? DlAssetSource::kMatterIM : DlAssetSource::kUnspecified;
  user.lastModifiedBy = slot.lastModifiedBy;
  return true;
}

bool DeviceDoorLock::SetUser(uint16_t userIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
                             const chip::CharSpan& userName, uint32_t uniqueId, UserStatusEnum userStatus,
                             UserTypeEnum userType, CredentialRuleEnum credentialRule,
                             const CredentialStructType* credentials, size_t totalCredentials)
{
  if (!this->HasFeature(Feature::kUser)) {
    return false;
  }
  if ((userIndex == 0) || (userIndex > kMaxUsers)) {
    return false;
  }
  UserSlot& slot = this->users[userIndex - 1];

  if (userStatus == UserStatusEnum::kAvailable) {
    slot = UserSlot();
    CallDeviceChangeCallback();
    return true;
  }

  slot.occupied = true;
  size_t nameLength = userName.size();
  if (nameLength > kMaxUserNameLength) {
    nameLength = kMaxUserNameLength;
  }
  memcpy(slot.name, userName.data(), nameLength);
  slot.name[nameLength] = '\0';
  slot.nameLength = static_cast<uint8_t>(nameLength);
  slot.uniqueId = uniqueId;
  slot.status = userStatus;
  slot.type = userType;
  slot.credentialRule = credentialRule;

  slot.totalCredentials = 0;
  if (credentials != nullptr) {
    size_t count = totalCredentials;
    if (count > kMaxCredentialsPerUser) {
      count = kMaxCredentialsPerUser;
    }
    for (size_t i = 0; i < count; i++) {
      slot.credentials[i] = credentials[i];
    }
    slot.totalCredentials = static_cast<uint8_t>(count);
  }
  slot.createdBy = creator;
  slot.lastModifiedBy = modifier;

  CallDeviceChangeCallback();
  return true;
}

bool DeviceDoorLock::GetCredential(uint16_t credentialIndex, CredentialTypeEnum credentialType, EmberAfPluginDoorLockCredentialInfo& credential)
{
  CredentialSlot* slot = this->GetCredentialSlot(credentialType, credentialIndex);
  if (slot == nullptr) {
    return false;
  }

  credential.credentialType = credentialType;
  if (!slot->occupied) {
    credential.status = DlCredentialStatus::kAvailable;
    return true;
  }

  credential.status = DlCredentialStatus::kOccupied;
  credential.credentialData = chip::ByteSpan(slot->data, slot->dataLength);
  credential.creationSource = (slot->createdBy != chip::kUndefinedFabricIndex) ? DlAssetSource::kMatterIM : DlAssetSource::kUnspecified;
  credential.createdBy = slot->createdBy;
  credential.modificationSource = (slot->lastModifiedBy != chip::kUndefinedFabricIndex) ? DlAssetSource::kMatterIM : DlAssetSource::kUnspecified;
  credential.lastModifiedBy = slot->lastModifiedBy;
  return true;
}

bool DeviceDoorLock::SetCredential(uint16_t credentialIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
                                   DlCredentialStatus credentialStatus, CredentialTypeEnum credentialType,
                                   const chip::ByteSpan& credentialData)
{
  CredentialSlot* slot = this->GetCredentialSlot(credentialType, credentialIndex);
  if (slot == nullptr) {
    return false;
  }

  if (credentialStatus == DlCredentialStatus::kAvailable) {
    *slot = CredentialSlot();
    if (this->credential_changed_cb) {
      this->credential_changed_cb(credentialType, credentialIndex);
    }
    CallDeviceChangeCallback();
    return true;
  }

  size_t dataLength = credentialData.size();
  if (dataLength > kMaxCredentialDataLength) {
    dataLength = kMaxCredentialDataLength;
  }
  slot->occupied = true;
  memcpy(slot->data, credentialData.data(), dataLength);
  slot->dataLength = static_cast<uint8_t>(dataLength);
  slot->createdBy = creator;
  slot->lastModifiedBy = modifier;

  if (this->credential_changed_cb) {
    this->credential_changed_cb(credentialType, credentialIndex);
  }
  CallDeviceChangeCallback();
  return true;
}

bool DeviceDoorLock::SetLocalCredential(CredentialTypeEnum type, uint16_t index, const uint8_t* data, uint8_t len)
{
  if (data == nullptr) {
    return false;
  }
  return this->SetCredential(index, chip::kUndefinedFabricIndex, chip::kUndefinedFabricIndex,
                             DlCredentialStatus::kOccupied, type, chip::ByteSpan(data, len));
}

bool DeviceDoorLock::ClearLocalCredential(CredentialTypeEnum type, uint16_t index)
{
  return this->SetCredential(index, chip::kUndefinedFabricIndex, chip::kUndefinedFabricIndex,
                             DlCredentialStatus::kAvailable, type, chip::ByteSpan());
}

bool DeviceDoorLock::ReportCredentialUnlock(CredentialTypeEnum type, uint16_t index)
{
  CredentialSlot* slot = this->GetCredentialSlot(type, index);
  if ((slot == nullptr) || !slot->occupied) {
    return false;
  }

  OperationSourceEnum opSource = OperationSourceEnum::kUnspecified;
  switch (type) {
    case CredentialTypeEnum::kPin:
    case CredentialTypeEnum::kProgrammingPIN:
      opSource = OperationSourceEnum::kKeypad;
      break;
    case CredentialTypeEnum::kRfid:
      opSource = OperationSourceEnum::kRfid;
      break;
    case CredentialTypeEnum::kFingerprint:
    case CredentialTypeEnum::kFingerVein:
    case CredentialTypeEnum::kFace:
      opSource = OperationSourceEnum::kBiometric;
      break;
    default:
      break;
  }

  uint16_t foundUserIndex = this->FindUserForCredential(type, index);
  Nullable<uint16_t> userIndex = (foundUserIndex == 0) ? Nullable<uint16_t>() : Nullable<uint16_t>(foundUserIndex);

  CredentialStructType credential;
  credential.credentialType = type;
  credential.credentialIndex = index;
  List<const CredentialStructType> credentialList(&credential, 1);

  return this->SetLockState(lock_state_t::UNLOCKED, opSource, userIndex, Nullable<List<const CredentialStructType>>(credentialList));
}

void DeviceDoorLock::SetCredentialChangedCallback(void (*credential_changed_cb)(CredentialTypeEnum type, uint16_t index))
{
  this->credential_changed_cb = credential_changed_cb;
}

uint32_t DeviceDoorLock::GetDoorLockClusterFeatureMap()
{
  return this->door_lock_cluster_feature_map;
}

bool DeviceDoorLock::HasFeature(Feature feature) const
{
  return (this->door_lock_cluster_feature_map & static_cast<uint32_t>(feature)) != 0;
}

uint16_t DeviceDoorLock::GetDoorLockClusterRevision()
{
  return this->door_lock_cluster_revision;
}

CHIP_ERROR DeviceDoorLock::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                      chip::AttributeId attributeId,
                                                      uint8_t* buffer,
                                                      uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::DoorLock::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadDoorLockAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::DoorLock::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == LockState::Id) && (maxReadLength == 1)) {
    uint8_t lock_state = this->GetLockState();
    memcpy(buffer, &lock_state, sizeof(lock_state));
  } else if ((attributeId == LockType::Id) && (maxReadLength == 1)) {
    uint8_t lock_type = this->lock_type;
    memcpy(buffer, &lock_type, sizeof(lock_type));
  } else if ((attributeId == ActuatorEnabled::Id) && (maxReadLength == 1)) {
    uint8_t actuator_enabled = this->actuator_enabled;
    memcpy(buffer, &actuator_enabled, sizeof(actuator_enabled));
  } else if ((attributeId == OperatingMode::Id) && (maxReadLength == 1)) {
    uint8_t operating_mode = this->operating_mode;
    memcpy(buffer, &operating_mode, sizeof(operating_mode));
  } else if ((attributeId == SupportedOperatingModes::Id) && (maxReadLength == 2)) {
    uint16_t supported_operating_modes = this->supported_operating_modes;
    memcpy(buffer, &supported_operating_modes, sizeof(supported_operating_modes));
  } else if ((attributeId == NumberOfTotalUsersSupported::Id) && (maxReadLength == 2)) {
    uint16_t number_of_total_users_supported = kMaxUsers;
    memcpy(buffer, &number_of_total_users_supported, sizeof(number_of_total_users_supported));
  } else if ((attributeId == NumberOfPINUsersSupported::Id) && (maxReadLength == 2)) {
    uint16_t number_of_pin_users_supported = kMaxUsers;
    memcpy(buffer, &number_of_pin_users_supported, sizeof(number_of_pin_users_supported));
  } else if ((attributeId == NumberOfRFIDUsersSupported::Id) && (maxReadLength == 2)) {
    uint16_t number_of_rfid_users_supported = kMaxUsers;
    memcpy(buffer, &number_of_rfid_users_supported, sizeof(number_of_rfid_users_supported));
  } else if ((attributeId == NumberOfCredentialsSupportedPerUser::Id) && (maxReadLength == 1)) {
    uint8_t number_of_credentials_supported_per_user = kMaxCredentialsPerUser;
    memcpy(buffer, &number_of_credentials_supported_per_user, sizeof(number_of_credentials_supported_per_user));
  } else if ((attributeId == MaxPINCodeLength::Id) && (maxReadLength == 1)) {
    uint8_t max_pin_code_length = this->max_pin_code_length;
    memcpy(buffer, &max_pin_code_length, sizeof(max_pin_code_length));
  } else if ((attributeId == MinPINCodeLength::Id) && (maxReadLength == 1)) {
    uint8_t min_pin_code_length = this->min_pin_code_length;
    memcpy(buffer, &min_pin_code_length, sizeof(min_pin_code_length));
  } else if ((attributeId == MaxRFIDCodeLength::Id) && (maxReadLength == 1)) {
    uint8_t max_rfid_code_length = this->max_rfid_code_length;
    memcpy(buffer, &max_rfid_code_length, sizeof(max_rfid_code_length));
  } else if ((attributeId == MinRFIDCodeLength::Id) && (maxReadLength == 1)) {
    uint8_t min_rfid_code_length = this->min_rfid_code_length;
    memcpy(buffer, &min_rfid_code_length, sizeof(min_rfid_code_length));
  } else if ((attributeId == CredentialRulesSupport::Id) && (maxReadLength == 1)) {
    uint8_t credential_rules_support = this->credential_rules_support;
    memcpy(buffer, &credential_rules_support, sizeof(credential_rules_support));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetDoorLockClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetDoorLockClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceDoorLock::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                       chip::AttributeId attributeId,
                                                       uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::DoorLock::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteDoorLockAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId != chip::app::Clusters::DoorLock::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if (attributeId == LockState::Id) {
    this->SetLockState((DeviceDoorLock::lock_state_t)(*buffer));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceDoorLock::HandleDoorLockDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_LockState) {
    ScheduleMatterReportingCallback(this->endpoint_id, DoorLock::Id, DoorLock::Attributes::LockState::Id);
  }
}
