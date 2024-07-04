/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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

#include "DeviceFan.h"

DeviceFan::DeviceFan(const char* device_name) :
  Device(device_name),
  current_percent(0),
  current_fan_mode(fan_mode_t::Off),
  speedmax(100)
{
  ;
}

uint8_t DeviceFan::GetPercentSetting()
{
  return this->current_percent;
}

void DeviceFan::SetPercentSetting(uint8_t percent)
{
  if (percent > 100) {
    percent = 100;
  }

  bool changed = this->current_percent != percent;
  ChipLogProgress(DeviceLayer, "FanDevice[%s]: new percent='%d'", this->device_name, percent);
  this->current_percent = percent;

  if (changed) {
    this->HandleFanDeviceStatusChanged(kChanged_PercentSetting);
    this->HandleFanDeviceStatusChanged(kChanged_PercentCurrent);
  }
}

uint8_t DeviceFan::GetPercentCurrent()
{
  return this->current_percent;
}

void DeviceFan::SetPercentCurrent(uint8_t percent)
{
  (void)percent;
}

uint8_t DeviceFan::GetSpeedMax()
{
  return this->speedmax;
}

void DeviceFan::SetFanMode(uint8_t fan_mode)
{
  bool changed = this->current_fan_mode != fan_mode;
  ChipLogProgress(DeviceLayer, "FanDevice[%s]: new mode='%d'", this->device_name, fan_mode);
  this->current_fan_mode = (fan_mode_t)fan_mode;

  if (changed) {
    this->HandleFanDeviceStatusChanged(kChanged_ModeSetting);
  }

  if (!changed) {
    return;
  }

  // Adjust the percentage to the selected mode
  switch ((fan_mode_t)fan_mode) {
    case fan_mode_t::Low:
      this->SetPercentSetting(20);
      break;

    case fan_mode_t::Med:
      this->SetPercentSetting(50);
      break;

    case fan_mode_t::High:
      this->SetPercentSetting(100);
      break;

    default:
      break;
  }
}

uint8_t DeviceFan::GetFanMode()
{
  return this->current_fan_mode;
}

uint8_t DeviceFan::GetFanModeSequence()
{
  return this->fan_mode_sequence;
}

uint32_t DeviceFan::GetFanClusterFeatureMap()
{
  return this->fan_cluster_feature_map;
}

uint16_t DeviceFan::GetFanClusterRevision()
{
  return this->fan_cluster_revision;
}

EmberAfStatus DeviceFan::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                    chip::AttributeId attributeId,
                                                    uint8_t* buffer,
                                                    uint16_t maxReadLength)
{
  if (!this->reachable) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadFanControlAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::FanControl::Id) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  if ((attributeId == FanMode::Id) && (maxReadLength == 1)) {
    uint8_t fanmode = this->GetFanMode();
    memcpy(buffer, &fanmode, sizeof(fanmode));
  } else if ((attributeId == FanModeSequence::Id) && (maxReadLength == 1)) {
    uint8_t fanmode_sequence = this->GetFanModeSequence();
    memcpy(buffer, &fanmode_sequence, sizeof(fanmode_sequence));
  } else if ((attributeId == PercentSetting::Id) && (maxReadLength == 1)) {
    uint8_t percent_setting = this->GetPercentSetting();
    memcpy(buffer, &percent_setting, sizeof(percent_setting));
  } else if ((attributeId == PercentCurrent::Id) && (maxReadLength == 1)) {
    uint8_t percent_current = this->GetPercentCurrent();
    memcpy(buffer, &percent_current, sizeof(percent_current));
  } else if ((attributeId == SpeedMax::Id) && (maxReadLength == 1)) {    
    uint8_t speedmax = this->GetSpeedMax();
    memcpy(buffer, &speedmax, sizeof(speedmax));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetFanClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetFanClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus DeviceFan::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                     chip::AttributeId attributeId,
                                                     uint8_t* buffer)
{
  if (!this->reachable) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteFanControlAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId != chip::app::Clusters::FanControl::Id) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  if (attributeId == PercentSetting::Id) {
    this->SetPercentSetting(*buffer);
  } else if (attributeId == FanMode::Id) {
    this->SetFanMode(*buffer);
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

void DeviceFan::HandleFanDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_PercentSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentSetting::Id);
  }
  if (itemChangedMask & kChanged_PercentCurrent) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentCurrent::Id);
  }
  if (itemChangedMask & kChanged_ModeSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::FanMode::Id);
  }
}
