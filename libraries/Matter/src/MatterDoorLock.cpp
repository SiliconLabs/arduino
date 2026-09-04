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

#include "MatterDoorLock.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gDoorLockDeviceTypes[] = { { DEVICE_TYPE_DOOR_LOCK, DEVICE_VERSION_DEFAULT } };

constexpr CommandId doorLockIncomingCommands[] = {
  app::Clusters::DoorLock::Commands::LockDoor::Id,
  app::Clusters::DoorLock::Commands::UnlockDoor::Id,
  app::Clusters::DoorLock::Commands::SetUser::Id,
  app::Clusters::DoorLock::Commands::GetUser::Id,
  app::Clusters::DoorLock::Commands::ClearUser::Id,
  app::Clusters::DoorLock::Commands::SetCredential::Id,
  app::Clusters::DoorLock::Commands::GetCredentialStatus::Id,
  app::Clusters::DoorLock::Commands::ClearCredential::Id,
  kInvalidCommandId,
};

constexpr uint32_t doorLockSupportedFeatures =
  static_cast<uint32_t>(DoorLock::Feature::kPinCredential)
  | static_cast<uint32_t>(DoorLock::Feature::kRfidCredential)
  | static_cast<uint32_t>(DoorLock::Feature::kUser);

// Door lock cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(doorLockAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::LockState::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),          // LockState
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::LockType::Id, INT8U, 1, 0),                                 // LockType
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::ActuatorEnabled::Id, BOOLEAN, 1, 0),                        // ActuatorEnabled
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::OperatingMode::Id, INT8U, 1, 0),                            // OperatingMode
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::SupportedOperatingModes::Id, INT16U, 2, 0),                 // SupportedOperatingModes
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::NumberOfTotalUsersSupported::Id, INT16U, 2, 0),             // NumberOfTotalUsersSupported
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::NumberOfPINUsersSupported::Id, INT16U, 2, 0),               // NumberOfPINUsersSupported
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::NumberOfRFIDUsersSupported::Id, INT16U, 2, 0),              // NumberOfRFIDUsersSupported
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::NumberOfCredentialsSupportedPerUser::Id, INT8U, 1, 0),      // NumberOfCredentialsSupportedPerUser
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::MaxPINCodeLength::Id, INT8U, 1, 0),                         // MaxPINCodeLength
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::MinPINCodeLength::Id, INT8U, 1, 0),                         // MinPINCodeLength
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::MaxRFIDCodeLength::Id, INT8U, 1, 0),                        // MaxRFIDCodeLength
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::MinRFIDCodeLength::Id, INT8U, 1, 0),                        // MinRFIDCodeLength
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::CredentialRulesSupport::Id, BITMAP8, 1, 0),                 // CredentialRulesSupport
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                            // FeatureMap
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                       // ClusterRevision auto added by LIST_END

// Door lock endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(doorLockEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(DoorLock::Id, doorLockAttrs, ZAP_CLUSTER_MASK(SERVER), doorLockIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterDoorLock
 ******************************************************************************/
MatterDoorLock::MatterDoorLock() :
  door_lock_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  feature_map(doorLockSupportedFeatures),
  initialized(false)
{
  ;
}

bool MatterDoorLock::setFeatures(uint32_t feature_map)
{
  if (this->initialized || (feature_map & ~doorLockSupportedFeatures) != 0) {
    return false;
  }
  this->feature_map = feature_map;
  return true;
}

bool MatterDoorLock::setFeatures(Feature feature)
{
  return this->setFeatures(static_cast<uint32_t>(feature));
}

bool MatterDoorLock::setFeatures(Feature feature1, Feature feature2)
{
  return this->setFeatures(static_cast<uint32_t>(feature1) | static_cast<uint32_t>(feature2));
}

bool MatterDoorLock::setFeatures(std::initializer_list<Feature> features)
{
  uint32_t feature_map = 0;
  for (Feature feature : features) {
    feature_map |= static_cast<uint32_t>(feature);
  }
  return this->setFeatures(feature_map);
}

/***************************************************************************//**
 * Destructor for MatterDoorLock
 ******************************************************************************/
MatterDoorLock::~MatterDoorLock()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterDoorLock instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterDoorLock::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceDoorLock* door_lock_device = new (std::nothrow)DeviceDoorLock("Door lock", this->feature_map);
  if (door_lock_device == nullptr) {
    return false;
  }
  door_lock_device->SetReachable(true);
  door_lock_device->SetProductName("Door lock");

  // Set the device instance pointer in the base class
  this->base_matter_device = door_lock_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(door_lock_device);
    return false;
  }
  new_endpoint->cluster = doorLockEndpointClusters;
  new_endpoint->clusterCount = ArraySize(doorLockEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(doorLockEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_device_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_device_data_version == nullptr) {
    delete(door_lock_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(door_lock_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gDoorLockDeviceTypes),
                                 Span<DataVersion>(new_device_data_version, ArraySize(doorLockEndpointClusters)),
                                 1);
  if (result < 0) {
    delete(door_lock_device);
    free(new_endpoint);
    free(new_device_data_version);
    return false;
  }

  this->door_lock_device = door_lock_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_device_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterDoorLock instance
 ******************************************************************************/
void MatterDoorLock::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->door_lock_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->door_lock_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Returns the lock state of the door lock
 *
 * @return true if locked, false otherwise
 ******************************************************************************/
bool MatterDoorLock::is_locked()
{
  if (!this->initialized) {
    return false;
  }
  return this->door_lock_device->GetLockState() == DeviceDoorLock::lock_state_t::LOCKED;
}

/***************************************************************************//**
 * Sets the lock state of the door lock
 *
 * @param[in] locked true to lock, false to unlock
 ******************************************************************************/
void MatterDoorLock::set_locked(bool locked)
{
  if (!this->initialized) {
    return;
  }
  this->door_lock_device->SetLockState(locked ? DeviceDoorLock::lock_state_t::LOCKED : DeviceDoorLock::lock_state_t::UNLOCKED);
}

/***************************************************************************//**
 * Toggles the lock state of the door lock
 ******************************************************************************/
void MatterDoorLock::toggle()
{
  if (!this->initialized) {
    return;
  }
  DeviceDoorLock::lock_state_t state_out;
  if (this->door_lock_device->GetLockState() == DeviceDoorLock::lock_state_t::LOCKED) {
    state_out = DeviceDoorLock::lock_state_t::UNLOCKED;
  } else {
    state_out = DeviceDoorLock::lock_state_t::LOCKED;
  }
  this->door_lock_device->SetLockState(state_out);
}

/***************************************************************************//**
 * Assignment operator overload for MatterDoorLock
 *
 * @param[in] state true to lock, false to unlock
 ******************************************************************************/
void MatterDoorLock::operator=(bool state)
{
  this->set_locked(state);
}

/***************************************************************************//**
 * Boolean operator overload for MatterDoorLock
 *
 * @return true if locked, false otherwise
 ******************************************************************************/
MatterDoorLock::operator bool()
{
  return this->is_locked();
}

/***************************************************************************//**
 * Provisions a PIN or RFID credential from the sketch
 *
 * @param[in] type the credential type (CredentialTypeEnum::kPin or kRfid)
 * @param[in] index the 1-based credential index
 * @param[in] data the raw credential bytes (PIN digits or RFID UID)
 * @param[in] len the length of 'data' in bytes
 *
 * @return true if the credential was provisioned, false otherwise
 ******************************************************************************/
bool MatterDoorLock::set_credential(CredentialTypeEnum type, uint16_t index, const uint8_t* data, uint8_t len)
{
  if (!this->initialized) {
    return false;
  }
  return this->door_lock_device->SetLocalCredential(type, index, data, len);
}

/***************************************************************************//**
 * Clears a previously provisioned PIN or RFID credential
 *
 * @param[in] type the credential type (CredentialTypeEnum::kPin or kRfid)
 * @param[in] index the 1-based credential index
 *
 * @return true if the credential was cleared, false otherwise
 ******************************************************************************/
bool MatterDoorLock::clear_credential(CredentialTypeEnum type, uint16_t index)
{
  if (!this->initialized) {
    return false;
  }
  return this->door_lock_device->ClearLocalCredential(type, index);
}

/***************************************************************************//**
 * Sets the callback that's called whenever a credential is provisioned or cleared
 *
 * @param[in] credential_changed_cb the callback to call
 ******************************************************************************/
void MatterDoorLock::set_credential_changed_callback(void (*credential_changed_cb)(CredentialTypeEnum type, uint16_t index))
{
  if (!this->initialized) {
    return;
  }
  this->door_lock_device->SetCredentialChangedCallback(credential_changed_cb);
}

/***************************************************************************//**
 * Reports that a physically-presented credential (matched by the sketch itself) authorized an unlock
 *
 * @param[in] type the credential type that was matched (e.g. CredentialTypeEnum::kRfid)
 * @param[in] index the 1-based index of the previously provisioned credential that was matched
 ******************************************************************************/
void MatterDoorLock::report_credential_unlock(CredentialTypeEnum type, uint16_t index)
{
  if (!this->initialized) {
    return;
  }
  this->door_lock_device->ReportCredentialUnlock(type, index);
}
