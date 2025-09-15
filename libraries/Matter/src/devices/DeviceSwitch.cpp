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

#include "DeviceSwitch.h"
#include <platform/CHIPDeviceLayer.h>
#include <app/clusters/switch-server/switch-server.h>

DeviceSwitch::DeviceSwitch(const char* device_name) :
  Device(device_name),
  number_of_positions(2),
  current_position(0),
  multi_press_max(2)
{
  this->SetDeviceType(device_type_t::kDeviceType_Switch);
}

void DeviceSwitch::SetNumberOfPositions(uint8_t number_of_positions)
{
  bool changed = this->number_of_positions != number_of_positions;
  this->number_of_positions = number_of_positions;
  if (changed) {
    this->HandleSwitchDeviceStatusChanged(kChanged_NumberOfPositions);
    CallDeviceChangeCallback();
  }
}

void DeviceSwitch::SetCurrentPosition(uint8_t current_position)
{
  bool changed = this->current_position != current_position;
  this->current_position = current_position;
  if (changed) {
    this->HandleSwitchDeviceStatusChanged(kChanged_CurrentPosition);
    CallDeviceChangeCallback();
  }
}

void DeviceSwitch::SetMultiPressMax(uint8_t multi_press_max)
{
  bool changed = this->multi_press_max != multi_press_max;
  this->multi_press_max = multi_press_max;
  if (changed) {
    this->HandleSwitchDeviceStatusChanged(kChanged_MultiPressMax);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceSwitch::GetNumberOfPositions()
{
  return this->number_of_positions;
}

uint8_t DeviceSwitch::GetCurrentPosition()
{
  return this->current_position;
}

uint8_t DeviceSwitch::GetMultiPressMax()
{
  return this->multi_press_max;
}

uint32_t DeviceSwitch::GetFeatureMap()
{
  return this->switch_cluster_feature_map;
}

uint16_t DeviceSwitch::GetSwitchClusterRevision()
{
  return this->switch_cluster_revision;
}

CHIP_ERROR DeviceSwitch::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                    chip::AttributeId attributeId,
                                                    uint8_t* buffer,
                                                    uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::Switch::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadSwitchAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::Switch::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == CurrentPosition::Id) && (maxReadLength == 1)) {
    uint8_t current_pos = this->GetCurrentPosition();
    memcpy(buffer, &current_pos, sizeof(current_pos));
  } else if ((attributeId == NumberOfPositions::Id) && (maxReadLength == 1)) {
    uint8_t max_positions = this->GetNumberOfPositions();
    memcpy(buffer, &max_positions, sizeof(max_positions));
  } else if ((attributeId == MultiPressMax::Id) && (maxReadLength == 1)) {
    uint8_t multipress_max = this->GetMultiPressMax();
    memcpy(buffer, &multipress_max, sizeof(multipress_max));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetSwitchClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceSwitch::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                     chip::AttributeId attributeId,
                                                     uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::Switch::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteSwitchAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId != chip::app::Clusters::Switch::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if (attributeId == CurrentPosition::Id) {
    this->SetCurrentPosition(*buffer);
  } else if (attributeId == NumberOfPositions::Id) {
    this->SetNumberOfPositions(*buffer);
  } else if (attributeId == MultiPressMax::Id) {
    this->SetMultiPressMax(*buffer);
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceSwitch::HandleSwitchDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;
  using namespace ::chip::DeviceLayer;

  if (itemChangedMask & kChanged_CurrentPosition) {
    ScheduleMatterReportingCallback(this->endpoint_id, Switch::Id, Switch::Attributes::CurrentPosition::Id);

    // Send the corresponding event for press or release
    PlatformMgr().LockChipStack();
    if (this->current_position) {
      SwitchServer::Instance().OnInitialPress(this->endpoint_id, this->current_position);
    } else {
      SwitchServer::Instance().OnShortRelease(this->endpoint_id, this->current_position);
    }
    PlatformMgr().UnlockChipStack();
  }
  if (itemChangedMask & kChanged_NumberOfPositions) {
    ScheduleMatterReportingCallback(this->endpoint_id, Switch::Id, Switch::Attributes::NumberOfPositions::Id);
  }
  if (itemChangedMask & kChanged_MultiPressMax) {
    ScheduleMatterReportingCallback(this->endpoint_id, Switch::Id, Switch::Attributes::MultiPressMax::Id);
  }
}
