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

DeviceDoorLock::DeviceDoorLock(const char* device_name) :
  Device(device_name),
  lock_state(lock_state_t::UNLOCKED)
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

uint32_t DeviceDoorLock::GetDoorLockClusterFeatureMap()
{
  return this->door_lock_cluster_feature_map;
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
