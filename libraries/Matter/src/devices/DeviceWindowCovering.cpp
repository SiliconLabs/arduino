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

#include "DeviceWindowCovering.h"

DeviceWindowCovering::DeviceWindowCovering(const char* device_name) :
  Device(device_name),
  current_operational_status(kOperationalStatus_Stopped),
  requested_lift_pos(0u),
  actual_lift_pos(0u)
{
  this->SetDeviceType(device_type_t::kDeviceType_WindowCovering);
}

void DeviceWindowCovering::SetCurrentOperationalStatus(OperationalStatus_t operational_status)
{
  uint8_t opstate_map = 0u;

  switch (operational_status) {
    case kOperationalStatus_Opening:
      ChipLogProgress(DeviceLayer, "WindowCoveringDevice[%s]: operational status='opening'", this->device_name);
      opstate_map = 0x05u;
      break;
    case kOperationalStatus_Closing:
      ChipLogProgress(DeviceLayer, "WindowCoveringDevice[%s]: operational status='closing'", this->device_name);
      opstate_map = 0x0Au;
      break;
    case kOperationalStatus_Stopped:
      ChipLogProgress(DeviceLayer, "WindowCoveringDevice[%s]: operational status='stopped'", this->device_name);
      opstate_map = 0x00u;
      break;
    default:
      ChipLogError(DeviceLayer, "WindowCoveringDevice[%s]: unknown operational status='%d'", this->device_name, operational_status);
      opstate_map = 0x00u;
      break;
  }

  this->current_operational_status = opstate_map;
  this->HandleWindowCoveringDeviceStatusChanged(kChanged_OperationalStatus);
  CallDeviceChangeCallback();
}

void DeviceWindowCovering::SetRequestedLiftPosition(uint16_t lift_position)
{
  if (lift_position > this->max_lift_position) {
    lift_position = this->max_lift_position;
  }
  ChipLogProgress(DeviceLayer, "WindowCoveringDevice[%s]: new requested position='%d'", this->device_name, lift_position);
  this->requested_lift_pos = lift_position;
  this->HandleWindowCoveringDeviceStatusChanged(kChanged_LiftPositionTargetPercent);
  CallDeviceChangeCallback();
}

uint16_t DeviceWindowCovering::GetRequestedLiftPosition()
{
  return this->requested_lift_pos;
}

void DeviceWindowCovering::SetActualLiftPosition(uint16_t lift_position)
{
  if (lift_position > this->max_lift_position) {
    lift_position = this->max_lift_position;
  }
  ChipLogProgress(DeviceLayer, "WindowCoveringDevice[%s]: new actual position='%d'", this->device_name, lift_position);
  this->actual_lift_pos = lift_position;
  this->HandleWindowCoveringDeviceStatusChanged(kChanged_LiftPositionCurrentPercent);
  CallDeviceChangeCallback();
}

uint16_t DeviceWindowCovering::GetActualLiftPosition()
{
  return this->actual_lift_pos;
}

uint32_t DeviceWindowCovering::GetWindowCoveringClusterFeatureMap()
{
  return this->window_covering_cluster_feature_map;
}

uint16_t DeviceWindowCovering::GetWindowCoveringClusterRevision()
{
  return this->window_covering_cluster_revision;
}

CHIP_ERROR DeviceWindowCovering::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                            chip::AttributeId attributeId,
                                                            uint8_t* buffer,
                                                            uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::WindowCovering::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadWindowCoveringAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::WindowCovering::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == Type::Id) && (maxReadLength == 1)) {
    uint8_t type = 0u; // roller shade
    memcpy(buffer, &type, sizeof(type));
  } else if ((attributeId == CurrentPositionLiftPercent100ths::Id) && (maxReadLength == 2)) {
    uint16_t currentPercent100s = this->actual_lift_pos;
    memcpy(buffer, &currentPercent100s, sizeof(currentPercent100s));
  } else if ((attributeId == TargetPositionLiftPercent100ths::Id) && (maxReadLength == 2)) {
    uint16_t currentPercent100s = this->requested_lift_pos;
    memcpy(buffer, &currentPercent100s, sizeof(currentPercent100s));
  } else if ((attributeId == ConfigStatus::Id) && (maxReadLength == 1)) {
    uint8_t configStatus = 1u; // operational
    memcpy(buffer, &configStatus, sizeof(configStatus));
  } else if ((attributeId == OperationalStatus::Id) && (maxReadLength == 1)) {
    uint8_t opStatus = this->current_operational_status;
    memcpy(buffer, &opStatus, sizeof(opStatus));
  } else if ((attributeId == EndProductType::Id) && (maxReadLength == 1)) {
    uint8_t endProductType = 0u; // roller shade
    memcpy(buffer, &endProductType, sizeof(endProductType));
  } else if ((attributeId == Mode::Id) && (maxReadLength == 1)) {
    uint8_t mode = 0u; // no special mode
    memcpy(buffer, &mode, sizeof(mode));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetWindowCoveringClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetWindowCoveringClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceWindowCovering::HandleWindowCoveringDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & DeviceWindowCovering::kChanged_LiftPosition) {
    ScheduleMatterReportingCallback(this->endpoint_id, WindowCovering::Id, WindowCovering::Attributes::CurrentPositionLift::Id);
  }
  if (itemChangedMask & DeviceWindowCovering::kChanged_LiftPositionCurrentPercent) {
    ScheduleMatterReportingCallback(this->endpoint_id, WindowCovering::Id, WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Id);
  }
  if (itemChangedMask & DeviceWindowCovering::kChanged_LiftPositionTargetPercent) {
    ScheduleMatterReportingCallback(this->endpoint_id, WindowCovering::Id, WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id);
  }
  if (itemChangedMask & DeviceWindowCovering::kChanged_OperationalStatus) {
    ScheduleMatterReportingCallback(this->endpoint_id, WindowCovering::Id, WindowCovering::Attributes::OperationalStatus::Id);
  }
}
