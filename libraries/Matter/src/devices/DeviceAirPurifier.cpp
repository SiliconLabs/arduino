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

#include "DeviceAirPurifier.h"

DeviceAirPurifier::DeviceAirPurifier(const char* device_name,
                                     uint8_t air_quality_value) :
  Device(device_name),
  air_quality_value(air_quality_value),
  current_fan_percent(0),
  current_fan_mode(DeviceFan::fan_mode_t::Off)
{
  this->SetDeviceType(device_type_t::kDeviceType_AirPurifier);
}

uint8_t DeviceAirPurifier::GetMeasuredAirQualityValue()
{
  return this->air_quality_value;
}

void DeviceAirPurifier::SetMeasuredAirQualityValue(uint8_t value)
{
  bool changed = this->air_quality_value != value;
  ChipLogProgress(DeviceLayer, "AirPurifierDevice[%s]: new air quality measurement='%u'", this->device_name, value);
  this->air_quality_value = value;

  if (changed) {
    this->HandleAirPurifierDeviceStatusChanged(kChanged_AirQualityMeasurementValue);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceAirPurifier::GetFanPercentSetting()
{
  return this->current_fan_percent;
}

void DeviceAirPurifier::SetFanPercentSetting(uint8_t percent)
{
  if (percent > 100) {
    percent = 100;
  }

  bool changed = this->current_fan_percent != percent;
  ChipLogProgress(DeviceLayer, "AirPurifierDevice[%s]: new fan percent='%d'", this->device_name, percent);
  this->current_fan_percent = percent;

  if (changed) {
    this->HandleAirPurifierDeviceStatusChanged(kChanged_FanPercentSetting);
    this->HandleAirPurifierDeviceStatusChanged(kChanged_FanPercentCurrent);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceAirPurifier::GetFanPercentCurrent()
{
  return this->current_fan_percent;
}

void DeviceAirPurifier::SetFanPercentCurrent(uint8_t percent)
{
  this->SetFanPercentSetting(percent);
}

void DeviceAirPurifier::SetFanMode(uint8_t fan_mode)
{
  bool changed = this->current_fan_mode != fan_mode;
  ChipLogProgress(DeviceLayer, "AirPurifierDevice[%s]: new fan mode='%d'", this->device_name, fan_mode);
  this->current_fan_mode = (DeviceFan::fan_mode_t)fan_mode;

  if (changed) {
    this->HandleAirPurifierDeviceStatusChanged(kChanged_FanModeSetting);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceAirPurifier::GetFanMode()
{
  return this->current_fan_mode;
}

uint8_t DeviceAirPurifier::GetFanModeSequence()
{
  return this->fan_mode_sequence;
}

uint8_t DeviceAirPurifier::GetFanSpeedMax()
{
  return this->fan_speed_max;
}

uint32_t DeviceAirPurifier::GetAirQualitySensorClusterFeatureMap()
{
  return this->air_quality_sensor_cluster_feature_map;
}

uint16_t DeviceAirPurifier::GetAirQualitySensorClusterRevision()
{
  return this->air_quality_sensor_cluster_revision;
}

uint32_t DeviceAirPurifier::GetFanClusterFeatureMap()
{
  return this->fan_cluster_feature_map;
}

uint16_t DeviceAirPurifier::GetFanClusterRevision()
{
  return this->fan_cluster_revision;
}

CHIP_ERROR DeviceAirPurifier::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                         chip::AttributeId attributeId,
                                                         uint8_t* buffer,
                                                         uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::AirQuality::Attributes;
  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadAirPurifierAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId == chip::app::Clusters::AirQuality::Id) {
    if ((attributeId == AirQuality::Id) && (maxReadLength == 1)) {
      uint8_t measuredValue = this->GetMeasuredAirQualityValue();
      memcpy(buffer, &measuredValue, sizeof(measuredValue));
      return CHIP_NO_ERROR;
    } else if ((attributeId == ::chip::app::Clusters::AirQuality::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = this->GetAirQualitySensorClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
      return CHIP_NO_ERROR;
    } else if ((attributeId == ::chip::app::Clusters::AirQuality::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = this->GetAirQualitySensorClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
      return CHIP_NO_ERROR;
    }
  }

  if (clusterId == chip::app::Clusters::FanControl::Id) {
    if ((attributeId == FanMode::Id) && (maxReadLength == 1)) {
      uint8_t fanmode = this->GetFanMode();
      memcpy(buffer, &fanmode, sizeof(fanmode));
      return CHIP_NO_ERROR;
    } else if ((attributeId == FanModeSequence::Id) && (maxReadLength == 1)) {
      uint8_t fanmode_sequence = this->GetFanModeSequence();
      memcpy(buffer, &fanmode_sequence, sizeof(fanmode_sequence));
      return CHIP_NO_ERROR;
    } else if ((attributeId == PercentSetting::Id) && (maxReadLength == 1)) {
      uint8_t percent_setting = this->GetFanPercentSetting();
      memcpy(buffer, &percent_setting, sizeof(percent_setting));
      return CHIP_NO_ERROR;
    } else if ((attributeId == PercentCurrent::Id) && (maxReadLength == 1)) {
      uint8_t percent_current = this->GetFanPercentCurrent();
      memcpy(buffer, &percent_current, sizeof(percent_current));
      return CHIP_NO_ERROR;
    } else if ((attributeId == SpeedMax::Id) && (maxReadLength == 1)) {
      uint8_t speed_max = this->GetFanSpeedMax();
      memcpy(buffer, &speed_max, sizeof(speed_max));
      return CHIP_NO_ERROR;
    } else if ((attributeId == SpeedSetting::Id) && (maxReadLength == 1)) {
      uint8_t speed_setting = this->GetFanPercentSetting();
      memcpy(buffer, &speed_setting, sizeof(speed_setting));
      return CHIP_NO_ERROR;
    } else if ((attributeId == SpeedCurrent::Id) && (maxReadLength == 1)) {
      uint8_t speed_current = this->GetFanPercentCurrent();
      memcpy(buffer, &speed_current, sizeof(speed_current));
      return CHIP_NO_ERROR;
    } else if ((attributeId == ::chip::app::Clusters::FanControl::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = this->GetFanClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
      return CHIP_NO_ERROR;
    } else if ((attributeId == ::chip::app::Clusters::FanControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = this->GetFanClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
      return CHIP_NO_ERROR;
    }
  }

  return CHIP_ERROR_INVALID_ARGUMENT;
}

CHIP_ERROR DeviceAirPurifier::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                          chip::AttributeId attributeId,
                                                          uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteAirPurifierAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId != chip::app::Clusters::FanControl::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if (attributeId == PercentSetting::Id) {
    this->SetFanPercentSetting(*buffer);
  } else if (attributeId == SpeedSetting::Id) {
    this->SetFanPercentSetting(*buffer);
  } else if (attributeId == FanMode::Id) {
    this->SetFanMode(*buffer);
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceAirPurifier::HandleAirPurifierDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_AirQualityMeasurementValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, AirQuality::Id, AirQuality::Attributes::AirQuality::Id);
  }
  if (itemChangedMask & kChanged_FanPercentSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentSetting::Id);
  }
  if (itemChangedMask & kChanged_FanPercentCurrent) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::PercentCurrent::Id);
  }
  if (itemChangedMask & kChanged_FanModeSetting) {
    ScheduleMatterReportingCallback(this->endpoint_id, FanControl::Id, FanControl::Attributes::FanMode::Id);
  }
}
