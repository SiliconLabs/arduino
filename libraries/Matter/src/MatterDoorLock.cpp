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
  kInvalidCommandId,
};

// Door lock cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(doorLockAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::LockState::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),  // LockState
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::LockType::Id, INT8U, 1, 0),                         // LockType
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::ActuatorEnabled::Id, INT8U, 1, 0),                  // ActuatorEnabled
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::OperatingMode::Id, INT8U, 1, 0),                    // OperatingMode
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::SupportedOperatingModes::Id, INT16U, 2, 0),         // SupportedOperatingModes
DECLARE_DYNAMIC_ATTRIBUTE(DoorLock::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                    // FeatureMap
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                               // ClusterRevision auto added by LIST_END

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
  initialized(false)
{
  ;
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
  DeviceDoorLock* door_lock_device = new (std::nothrow)DeviceDoorLock("Door lock");
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
                                 Span<DataVersion>(new_device_data_version, dataversion_size), 1);
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
