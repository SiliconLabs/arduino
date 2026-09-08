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

#pragma once

#include "MatterDevice.h"
#include <door-lock-server.h>

class DeviceDoorLock : public Device
{
public:
  enum Changed_t{
    kChanged_LockState = kChanged_Last << 1,
  } Changed;

  DeviceDoorLock(const char* device_name, uint32_t feature_map);

  enum lock_state_t : uint8_t {
    NOT_FULLY_LOCKED = 0,
    LOCKED,
    UNLOCKED,
    UNLATCHED,
    UNKNOWN = 255,
  };

  using CredentialTypeEnum = chip::app::Clusters::DoorLock::CredentialTypeEnum;
  using OperationSourceEnum = chip::app::Clusters::DoorLock::OperationSourceEnum;
  using UserStatusEnum = chip::app::Clusters::DoorLock::UserStatusEnum;
  using UserTypeEnum = chip::app::Clusters::DoorLock::UserTypeEnum;
  using CredentialRuleEnum = chip::app::Clusters::DoorLock::CredentialRuleEnum;
  using CredentialStructType = chip::app::Clusters::DoorLock::Structs::CredentialStruct::Type;
  using Feature = chip::app::Clusters::DoorLock::Feature;

  // Credential/user storage capacity. Fixed-size, no dynamic allocation.
  static const uint16_t kMaxUsers = 10u;
  static const uint16_t kMaxPinCredentials = 10u;
  static const uint16_t kMaxRfidCredentials = 10u;
  static const uint8_t kMaxCredentialsPerUser = 2u;
  static const uint8_t kMaxCredentialDataLength = 20u;
  static const uint8_t kMaxUserNameLength = 10u;

  lock_state_t GetLockState();

  // Plain attribute-write style state update (no LockOperation event, no credential attribution).
  void SetLockState(lock_state_t state);

  // Rich state update used for LockDoor/UnlockDoor command handling and for locally-reported
  // credential presentations: updates the LockState attribute the same way as the overload above,
  // and additionally reports the operation through the Door Lock cluster's LockOperation event via
  // DoorLockServer, with the given operation source and (optional) user/credential attribution.
  bool SetLockState(lock_state_t state,
                    OperationSourceEnum opSource,
                    const chip::app::DataModel::Nullable<uint16_t>& userIndex = chip::app::DataModel::NullNullable,
                    const chip::app::DataModel::Nullable<chip::app::DataModel::List<const CredentialStructType> >& credentials = chip::app::DataModel::NullNullable);

  // Compares a presented PIN against the stored PIN credentials. On a match, returns true and fills
  // in the matching credential index and the index of the user it belongs to (0 if the credential
  // is not associated with any user).
  bool ValidatePIN(const chip::ByteSpan& pin, uint16_t& outUserIndex, uint16_t& outCredentialIndex);

  // Backing storage for the emberAfPluginDoorLock{Get,Set}{User,Credential} weak callbacks.
  bool GetUser(uint16_t userIndex, EmberAfPluginDoorLockUserInfo& user);
  bool SetUser(uint16_t userIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
               const chip::CharSpan& userName, uint32_t uniqueId, UserStatusEnum userStatus,
               UserTypeEnum userType, CredentialRuleEnum credentialRule,
               const CredentialStructType* credentials, size_t totalCredentials);
  bool GetCredential(uint16_t credentialIndex, CredentialTypeEnum credentialType, EmberAfPluginDoorLockCredentialInfo& credential);
  bool SetCredential(uint16_t credentialIndex, chip::FabricIndex creator, chip::FabricIndex modifier,
                     DlCredentialStatus credentialStatus, CredentialTypeEnum credentialType,
                     const chip::ByteSpan& credentialData);

  // Sketch-facing helpers behind MatterDoorLock::set_credential()/clear_credential()/report_credential_unlock().
  // Provision/clear a PIN or RFID credential outside of a Matter controller's SetUser/SetCredential flow.
  bool SetLocalCredential(CredentialTypeEnum type, uint16_t index, const uint8_t* data, uint8_t len);
  bool ClearLocalCredential(CredentialTypeEnum type, uint16_t index);

  // Reports that a previously provisioned credential (matched by the sketch itself, e.g. against a
  // scanned RFID tag) just authorized an unlock.
  bool ReportCredentialUnlock(CredentialTypeEnum type, uint16_t index);

  void SetCredentialChangedCallback(void (*credential_changed_cb)(CredentialTypeEnum type, uint16_t index));

  uint32_t GetDoorLockClusterFeatureMap();
  bool HasFeature(Feature feature) const;
  uint16_t GetDoorLockClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

private:
  struct CredentialSlot {
    bool occupied = false;
    uint8_t data[kMaxCredentialDataLength] = { 0 };
    uint8_t dataLength = 0;
    chip::FabricIndex createdBy = chip::kUndefinedFabricIndex;
    chip::FabricIndex lastModifiedBy = chip::kUndefinedFabricIndex;
  };

  struct UserSlot {
    bool occupied = false;
    char name[kMaxUserNameLength + 1] = { 0 };
    uint8_t nameLength = 0;
    uint32_t uniqueId = 0;
    UserStatusEnum status = UserStatusEnum::kAvailable;
    UserTypeEnum type = UserTypeEnum::kUnrestrictedUser;
    CredentialRuleEnum credentialRule = CredentialRuleEnum::kSingle;
    CredentialStructType credentials[kMaxCredentialsPerUser];
    uint8_t totalCredentials = 0;
    chip::FabricIndex createdBy = chip::kUndefinedFabricIndex;
    chip::FabricIndex lastModifiedBy = chip::kUndefinedFabricIndex;
  };

  void HandleDoorLockDeviceStatusChanged(Changed_t itemChangedMask);

  // Returns the storage slot for a given (type, 1-based index), or nullptr if the type/index is
  // not backed by this implementation (only PIN and RFID credentials are stored).
  CredentialSlot* GetCredentialSlot(CredentialTypeEnum type, uint16_t index);

  // Scans all users for a credential reference matching (type, index); returns the 1-based user
  // index, or 0 if the credential is not associated with any user.
  uint16_t FindUserForCredential(CredentialTypeEnum type, uint16_t credentialIndex);

  lock_state_t lock_state;

  static const uint8_t lock_type = 2u;                      // Other type
  static const uint8_t actuator_enabled = 1u;               // Actuator enabled - able to process lock/unlock commands
  static const uint8_t operating_mode = 0u;                 // Normal
  static const uint16_t supported_operating_modes = 0xFFF6; // Normal and NoRemoteUnlock supported

  static const uint8_t min_pin_code_length = 4u;
  static const uint8_t max_pin_code_length = 8u;
  static const uint8_t min_rfid_code_length = 4u;
  static const uint8_t max_rfid_code_length = 20u;
  static const uint8_t credential_rules_support = 0x01u; // 'Single' credential rule only

  CredentialSlot pin_credentials[kMaxPinCredentials];
  CredentialSlot rfid_credentials[kMaxRfidCredentials];
  UserSlot users[kMaxUsers];

  void (*credential_changed_cb)(CredentialTypeEnum type, uint16_t index);

  uint32_t door_lock_cluster_feature_map;
  static const uint16_t door_lock_cluster_revision    = 7u;
};
