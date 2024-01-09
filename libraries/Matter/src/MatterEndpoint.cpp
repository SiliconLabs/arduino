/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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

#include "MatterEndpoint.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

template<typename Base, typename T>
inline bool instanceof(const T* ptr)
{
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

// Current ZCL implementation of Struct uses a max-size array of 254 bytes
const uint16_t kDescriptorAttributeArraySize = 254;

void CallReportingCallback(intptr_t closure)
{
  auto path = reinterpret_cast<app::ConcreteAttributePath*>(closure);
  MatterReportingAttributeChangeCallback(*path);
  Platform::Delete(path);
}

void ScheduleReportingCallback(Device* dev, ClusterId cluster, AttributeId attribute)
{
  auto* path = Platform::New<app::ConcreteAttributePath>(dev->GetEndpointId(), cluster, attribute);
  chip::DeviceLayer::PlatformMgr().ScheduleWork(CallReportingCallback, reinterpret_cast<intptr_t>(path));
}

void HandleDeviceStatusChanged(Device* dev, Device::Changed_t itemChangedMask)
{
  if (itemChangedMask & Device::kChanged_Reachable) {
    ScheduleReportingCallback(dev, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
  }

  if (itemChangedMask & Device::kChanged_Name) {
    ScheduleReportingCallback(dev, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::NodeLabel::Id);
  }
}

void HandleDeviceOnOffStatusChanged(DeviceOnOff* dev, DeviceOnOff::Changed_t itemChangedMask)
{
  if (itemChangedMask & (DeviceOnOff::kChanged_Reachable | DeviceOnOff::kChanged_Name | DeviceOnOff::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }

  if (itemChangedMask & DeviceOnOff::kChanged_OnOff) {
    ScheduleReportingCallback(dev, OnOff::Id, OnOff::Attributes::OnOff::Id);
  }
}

void HandleDeviceLightbulbStatusChanged(DeviceLightbulb* dev, DeviceLightbulb::Changed_t itemChangedMask)
{
  if (itemChangedMask & (DeviceLightbulb::kChanged_Reachable | DeviceLightbulb::kChanged_Name | DeviceLightbulb::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }

  if (itemChangedMask & DeviceLightbulb::kChanged_OnOff) {
    ScheduleReportingCallback(dev, OnOff::Id, OnOff::Attributes::OnOff::Id);
  }
  if (itemChangedMask & DeviceLightbulb::kChanged_Level) {
    ScheduleReportingCallback(dev, LevelControl::Id, LevelControl::Attributes::CurrentLevel::Id);
  }
  if (itemChangedMask & DeviceLightbulb::kChanged_Color) {
    ScheduleReportingCallback(dev, ColorControl::Id, ColorControl::Attributes::CurrentHue::Id);
    ScheduleReportingCallback(dev, ColorControl::Id, ColorControl::Attributes::CurrentSaturation::Id);
  }
}

void HandleDeviceTempSensorStatusChanged(DeviceTempSensor* dev, DeviceTempSensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceTempSensor::kChanged_Reachable | DeviceTempSensor::kChanged_Name | DeviceTempSensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceTempSensor::kChanged_MeasurementValue) {
    ScheduleReportingCallback(dev, TemperatureMeasurement::Id, TemperatureMeasurement::Attributes::MeasuredValue::Id);
  }
}

void HandleDeviceHumiditySensorStatusChanged(DeviceHumiditySensor* dev, DeviceHumiditySensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceHumiditySensor::kChanged_Reachable | DeviceHumiditySensor::kChanged_Name | DeviceHumiditySensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceHumiditySensor::kChanged_MeasurementValue) {
    ScheduleReportingCallback(dev, RelativeHumidityMeasurement::Id, RelativeHumidityMeasurement::Attributes::MeasuredValue::Id);
  }
}

void HandleDeviceSwitchStatusChanged(DeviceSwitch* dev, DeviceSwitch::Changed_t itemChangedMask)
{
  if (itemChangedMask & (DeviceSwitch::kChanged_Reachable | DeviceSwitch::kChanged_Name | DeviceSwitch::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }

  if (itemChangedMask & DeviceSwitch::kChanged_CurrentPosition) {
    ScheduleReportingCallback(dev, Switch::Id, Switch::Attributes::CurrentPosition::Id);
  }
  if (itemChangedMask & DeviceSwitch::kChanged_NumberOfPositions) {
    ScheduleReportingCallback(dev, Switch::Id, Switch::Attributes::NumberOfPositions::Id);
  }
  if (itemChangedMask & DeviceSwitch::kChanged_MultiPressMax) {
    ScheduleReportingCallback(dev, Switch::Id, Switch::Attributes::MultiPressMax::Id);
  }
}

void HandleDeviceIlluminanceSensorStatusChanged(DeviceIlluminanceSensor* dev, DeviceIlluminanceSensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceIlluminanceSensor::kChanged_Reachable | DeviceIlluminanceSensor::kChanged_Name | DeviceIlluminanceSensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceIlluminanceSensor::kChanged_MeasurementValue) {
    ScheduleReportingCallback(dev, IlluminanceMeasurement::Id, IlluminanceMeasurement::Attributes::MeasuredValue::Id);
  }
}

void HandleDevicePressureSensorStatusChanged(DevicePressureSensor* dev, DevicePressureSensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DevicePressureSensor::kChanged_Reachable | DevicePressureSensor::kChanged_Name | DevicePressureSensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DevicePressureSensor::kChanged_MeasurementValue) {
    ScheduleReportingCallback(dev, PressureMeasurement::Id, PressureMeasurement::Attributes::MeasuredValue::Id);
  }
}

void HandleDeviceOccupancySensorStatusChanged(DeviceOccupancySensor* dev, DeviceOccupancySensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceOccupancySensor::kChanged_Reachable | DeviceOccupancySensor::kChanged_Name | DeviceOccupancySensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceOccupancySensor::kChanged_OccupancyValue) {
    ScheduleReportingCallback(dev, OccupancySensing::Id, OccupancySensing::Attributes::Occupancy::Id);
  }
}

void HandleDeviceFlowSensorStatusChanged(DeviceFlowSensor* dev, DeviceFlowSensor::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceFlowSensor::kChanged_Reachable | DeviceFlowSensor::kChanged_Name | DeviceFlowSensor::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceFlowSensor::kChanged_MeasurementValue) {
    ScheduleReportingCallback(dev, FlowMeasurement::Id, FlowMeasurement::Attributes::MeasuredValue::Id);
  }
}

void HandleDeviceFanStatusChanged(DeviceFan* dev, DeviceFan::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceFan::kChanged_Reachable | DeviceFan::kChanged_Name | DeviceFan::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceFan::kChanged_PercentSetting) {
    ScheduleReportingCallback(dev, FanControl::Id, FanControl::Attributes::PercentSetting::Id);
  }
  if (itemChangedMask & DeviceFan::kChanged_PercentCurrent) {
    ScheduleReportingCallback(dev, FanControl::Id, FanControl::Attributes::PercentCurrent::Id);
  }
  if (itemChangedMask & DeviceFan::kChanged_ModeSetting) {
    ScheduleReportingCallback(dev, FanControl::Id, FanControl::Attributes::FanMode::Id);
  }
}

void HandleDeviceThermostatStatusChanged(DeviceThermostat* dev, DeviceThermostat::Changed_t itemChangedMask)
{
  if (itemChangedMask
      & (DeviceThermostat::kChanged_Reachable | DeviceThermostat::kChanged_Name | DeviceThermostat::kChanged_Location)) {
    HandleDeviceStatusChanged(static_cast<Device*>(dev), (Device::Changed_t) itemChangedMask);
  }
  if (itemChangedMask & DeviceThermostat::kChanged_HeatingSetpointValue) {
    ScheduleReportingCallback(dev, Thermostat::Id, Thermostat::Attributes::OccupiedHeatingSetpoint::Id);
  }
  if (itemChangedMask & DeviceThermostat::kChanged_LocalTemperatureValue) {
    ScheduleReportingCallback(dev, Thermostat::Id, Thermostat::Attributes::LocalTemperature::Id);
  }
  if (itemChangedMask & DeviceThermostat::kChanged_SystemModeValue) {
    ScheduleReportingCallback(dev, Thermostat::Id, Thermostat::Attributes::SystemMode::Id);
  }
}

EmberAfStatus HandleReadOnOffAttribute(DeviceOnOff* dev, chip::AttributeId attributeId, uint8_t* buffer, uint16_t maxReadLength)
{
  ChipLogProgress(DeviceLayer, "HandleReadOnOffAttribute: attrId=%ld, maxReadLength=%d", attributeId, maxReadLength);

  if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1)) {
    *buffer = dev->IsOn() ? 1 : 0;
  } else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t rev = ZCL_ON_OFF_CLUSTER_REVISION;
    memcpy(buffer, &rev, sizeof(rev));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleWriteOnOffAttribute(DeviceOnOff* dev, chip::AttributeId attributeId, uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "HandleWriteOnOffAttribute: attrId=%ld", attributeId);

  if (attributeId == OnOff::Attributes::OnOff::Id) {
    if (*buffer) {
      dev->SetOnOff(true);
    } else {
      dev->SetOnOff(false);
    }
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadLightbulbAttribute(DeviceLightbulb* dev,
                                           ClusterId clusterId,
                                           chip::AttributeId attributeId,
                                           uint8_t* buffer,
                                           uint16_t maxReadLength)
{
  using namespace LevelControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == LevelControl::Id) {
    if ((attributeId == CurrentLevel::Id) && (maxReadLength == 1)) {
      uint8_t level = dev->GetLevel();
      memcpy(buffer, &level, sizeof(level));
    } else if ((attributeId == Options::Id) && (maxReadLength == 1)) {
      uint8_t options = dev->GetLevelControlOptions();
      memcpy(buffer, &options, sizeof(options));
    } else if ((attributeId == OnLevel::Id) && (maxReadLength == 1)) {
      uint8_t onLevel = dev->GetLevelControlOnLevel();
      memcpy(buffer, &onLevel, sizeof(onLevel));
    } else if ((attributeId == MaxLevel::Id) && (maxReadLength == 1)) {
      uint8_t maxLevel = dev->GetLevelControlMaxLevel();
      memcpy(buffer, &maxLevel, sizeof(maxLevel));
    } else if ((attributeId == MinLevel::Id) && (maxReadLength == 1)) {
      uint8_t minLevel = dev->GetLevelControlMinLevel();
      memcpy(buffer, &minLevel, sizeof(minLevel));
    } else if ((attributeId == RemainingTime::Id) && (maxReadLength == 2)) {
      uint16_t remainingTime = dev->GetLevelControlRemainingTime();
      memcpy(buffer, &remainingTime, sizeof(remainingTime));
    } else if ((attributeId == StartUpCurrentLevel::Id) && (maxReadLength == 1)) {
      uint8_t startupCurrentLevel = dev->GetLevelControlStartupCurrentLevel();
      memcpy(buffer, &startupCurrentLevel, sizeof(startupCurrentLevel));
    } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = dev->GetLevelControlClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = dev->GetLevelControlClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - LevelControl: invalid read");
      return EMBER_ZCL_STATUS_FAILURE;
    }
  }

  if (clusterId == ColorControl::Id) {
    if ((attributeId == ColorControl::Attributes::CurrentHue::Id) && (maxReadLength == 1)) {
      uint8_t hue = dev->GetHue();
      memcpy(buffer, &hue, sizeof(hue));
    } else if ((attributeId == ColorControl::Attributes::CurrentSaturation::Id) && (maxReadLength == 1)) {
      uint8_t saturation = dev->GetSaturation();
      memcpy(buffer, &saturation, sizeof(saturation));
    } else if ((attributeId == ColorControl::Attributes::Options::Id) && (maxReadLength == 1)) {
      uint8_t options = dev->GetColorControlOptions();
      memcpy(buffer, &options, sizeof(options));
    } else if ((attributeId == ColorControl::Attributes::ColorMode::Id) && (maxReadLength == 1)) {
      uint8_t colorMode = dev->GetColorControlColorMode();
      memcpy(buffer, &colorMode, sizeof(colorMode));
    } else if ((attributeId == ColorControl::Attributes::EnhancedColorMode::Id) && (maxReadLength == 1)) {
      uint8_t enhancedColorMode = dev->GetColorControlEnhancedColorMode();
      memcpy(buffer, &enhancedColorMode, sizeof(enhancedColorMode));
    } else if ((attributeId == ColorControl::Attributes::ColorCapabilities::Id) && (maxReadLength == 2)) {
      uint16_t colorCapabilities = dev->GetColorControlColorCapabilities();
      memcpy(buffer, &colorCapabilities, sizeof(colorCapabilities));
    } else if ((attributeId == ColorControl::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = dev->GetColorControlClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == ColorControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t clusterRevision = dev->GetColorControlClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - ColorControl: invalid read");
      return EMBER_ZCL_STATUS_FAILURE;
    }
  }

  if (clusterId == OnOff::Id) {
    if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1)) {
      *buffer = dev->IsOn() ? 1 : 0;
      //ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute: read onoff=%u", dev->IsOn());
    } else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t rev = dev->GetOnoffClusterRevision();
      memcpy(buffer, &rev, sizeof(rev));
    } else if ((attributeId == OnOff::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = dev->GetOnoffClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - OnOff: invalid read");
      return EMBER_ZCL_STATUS_FAILURE;
    }
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleWriteLightbulbAttribute(DeviceLightbulb* dev,
                                            ClusterId clusterId,
                                            chip::AttributeId attributeId,
                                            uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == OnOff::Attributes::OnOff::Id) && (clusterId == OnOff::Id)) {
    if (*buffer) {
      dev->SetOnOff(true);
    } else {
      dev->SetOnOff(false);
    }
  } else if ((attributeId == LevelControl::Attributes::CurrentLevel::Id) && (clusterId == LevelControl::Id)) {
    dev->SetLevel(*buffer);
  } else if ((attributeId == ColorControl::Attributes::CurrentHue::Id) && (clusterId == ColorControl::Id)) {
    dev->SetHue(*buffer);
  } else if ((attributeId == ColorControl::Attributes::CurrentSaturation::Id) && (clusterId == ColorControl::Id)) {
    dev->SetSaturation(*buffer);
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadTempMeasurementAttribute(DeviceTempSensor* dev,
                                                 chip::AttributeId attributeId,
                                                 uint8_t* buffer,
                                                 uint16_t maxReadLength)
{
  using namespace TemperatureMeasurement::Attributes;

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t measuredValue = dev->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t minValue = dev->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t maxValue = dev->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetTempSensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetTempSensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadHumidityMeasurementAttribute(DeviceHumiditySensor* dev,
                                                     chip::AttributeId attributeId,
                                                     uint8_t* buffer,
                                                     uint16_t maxReadLength)
{
  using namespace RelativeHumidityMeasurement::Attributes;

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t measuredValue = dev->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t minValue = dev->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t maxValue = dev->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetHumiditySensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetHumiditySensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadSwitchAttribute(DeviceSwitch* dev,
                                        ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength)
{
  using namespace Switch::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadSwitchAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == CurrentPosition::Id) && (maxReadLength == 1)) {
    uint8_t current_pos = dev->GetCurrentPosition();
    memcpy(buffer, &current_pos, sizeof(current_pos));
  } else if ((attributeId == NumberOfPositions::Id) && (maxReadLength == 1)) {
    uint8_t max_positions = dev->GetNumberOfPositions();
    memcpy(buffer, &max_positions, sizeof(max_positions));
  } else if ((attributeId == MultiPressMax::Id) && (maxReadLength == 1)) {
    uint8_t multipress_max = dev->GetMultiPressMax();
    memcpy(buffer, &multipress_max, sizeof(multipress_max));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetSwitchClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleWriteSwitchAttribute(DeviceSwitch* dev,
                                         ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "HandleWriteSwitchAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == Switch::Attributes::CurrentPosition::Id) && (clusterId == Switch::Id)) {
    dev->SetCurrentPosition(*buffer);
  } else if ((attributeId == Switch::Attributes::NumberOfPositions::Id) && (clusterId == Switch::Id)) {
    dev->SetNumberOfPositions(*buffer);
  } else if ((attributeId == Switch::Attributes::MultiPressMax::Id) && (clusterId == Switch::Id)) {
    dev->SetMultiPressMax(*buffer);
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadIlluminanceMeasurementAttribute(DeviceIlluminanceSensor* dev,
                                                        ClusterId clusterId,
                                                        chip::AttributeId attributeId,
                                                        uint8_t* buffer,
                                                        uint16_t maxReadLength)
{
  using namespace IlluminanceMeasurement::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadIlluminanceMeasurementAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t measuredValue = dev->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t minValue = dev->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t maxValue = dev->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetIlluminanceSensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetIlluminanceSensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadPressureMeasurementAttribute(DevicePressureSensor* dev,
                                                     ClusterId clusterId,
                                                     chip::AttributeId attributeId,
                                                     uint8_t* buffer,
                                                     uint16_t maxReadLength)
{
  using namespace PressureMeasurement::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadPressureMeasurementAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t measuredValue = dev->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t minValue = dev->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t maxValue = dev->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetPressureSensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetPressureSensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadOccupancySensingAttribute(DeviceOccupancySensor* dev,
                                                  ClusterId clusterId,
                                                  chip::AttributeId attributeId,
                                                  uint8_t* buffer,
                                                  uint16_t maxReadLength)
{
  using namespace OccupancySensing::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadOccupancySensingAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == Occupancy::Id) && (maxReadLength == 1)) {
    bool occupied = dev->GetOccupancy();
    memcpy(buffer, &occupied, sizeof(occupied));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetOccupancySensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetOccupancySensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadFlowMeasurementAttribute(DeviceFlowSensor* dev,
                                                 ClusterId clusterId,
                                                 chip::AttributeId attributeId,
                                                 uint8_t* buffer,
                                                 uint16_t maxReadLength)
{
  using namespace FlowMeasurement::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadFlowMeasurementAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t measuredValue = dev->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t minValue = dev->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    uint16_t maxValue = dev->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetFlowSensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetFlowSensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadFanControlAttribute(DeviceFan* dev,
                                            ClusterId clusterId,
                                            chip::AttributeId attributeId,
                                            uint8_t* buffer,
                                            uint16_t maxReadLength)
{
  using namespace FanControl::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadFanControlAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == FanMode::Id) && (maxReadLength == 1)) {
    uint8_t fanmode = dev->GetFanMode();
    memcpy(buffer, &fanmode, sizeof(fanmode));
  } else if ((attributeId == FanModeSequence::Id) && (maxReadLength == 1)) {
    uint8_t fanmode_sequence = dev->GetFanModeSequence();
    memcpy(buffer, &fanmode_sequence, sizeof(fanmode_sequence));
  } else if ((attributeId == PercentSetting::Id) && (maxReadLength == 1)) {
    uint8_t percent_setting = dev->GetPercentSetting();
    memcpy(buffer, &percent_setting, sizeof(percent_setting));
  } else if ((attributeId == PercentCurrent::Id) && (maxReadLength == 1)) {
    uint8_t percent_current = dev->GetPercentCurrent();
    memcpy(buffer, &percent_current, sizeof(percent_current));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetFanClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetFanClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleWriteFanControlAttribute(DeviceFan* dev,
                                             ClusterId clusterId,
                                             chip::AttributeId attributeId,
                                             uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "HandleWriteFanControlAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == FanControl::Attributes::PercentSetting::Id) && (clusterId == FanControl::Id)) {
    dev->SetPercentSetting(*buffer);
  } else if ((attributeId == FanControl::Attributes::FanMode::Id) && (clusterId == FanControl::Id)) {
    dev->SetFanMode(*buffer);
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleReadThermostatAttribute(DeviceThermostat* dev,
                                            ClusterId clusterId,
                                            chip::AttributeId attributeId,
                                            uint8_t* buffer,
                                            uint16_t maxReadLength)
{
  using namespace Thermostat::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadThermostatAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == LocalTemperature::Id) && (maxReadLength == 2)) {
    int16_t localTemp = dev->GetLocalTemperatureValue();
    memcpy(buffer, &localTemp, sizeof(localTemp));
  } else if ((attributeId == OccupiedHeatingSetpoint::Id) && (maxReadLength == 2)) {
    int16_t heatingSetpoint = dev->GetHeatingSetpointValue();
    memcpy(buffer, &heatingSetpoint, sizeof(heatingSetpoint));
  } else if ((attributeId == SystemMode::Id) && (maxReadLength == 1)) {
    uint8_t systemMode = dev->GetSystemMode();
    memcpy(buffer, &systemMode, sizeof(systemMode));
  } else if ((attributeId == ControlSequenceOfOperation::Id) && (maxReadLength == 1)) {
    uint8_t seq_op = dev->GetControlSequenceOfOperation();
    memcpy(buffer, &seq_op, sizeof(seq_op));
  } else if ((attributeId == AbsMinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t abs_min_heat_setpoint = dev->GetAbsMinHeatingSetpoint();
    memcpy(buffer, &abs_min_heat_setpoint, sizeof(abs_min_heat_setpoint));
  } else if ((attributeId == AbsMaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t abs_max_heat_setpoint = dev->GetAbsMaxHeatingSetpoint();
    memcpy(buffer, &abs_max_heat_setpoint, sizeof(abs_max_heat_setpoint));
  } else if ((attributeId == MinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t min_heat_setpoint = dev->GetMinHeatingSetpoint();
    memcpy(buffer, &min_heat_setpoint, sizeof(min_heat_setpoint));
  } else if ((attributeId == MaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t max_heat_setpoint = dev->GetMaxHeatingSetpoint();
    memcpy(buffer, &max_heat_setpoint, sizeof(max_heat_setpoint));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = dev->GetThermostatClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = dev->GetThermostatClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus HandleWriteThermostatAttribute(DeviceThermostat* dev,
                                             ClusterId clusterId,
                                             chip::AttributeId attributeId,
                                             uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "HandleWriteThermostatAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == Thermostat::Attributes::OccupiedHeatingSetpoint::Id) && (clusterId == Thermostat::Id)) {
    dev->SetHeatingSetpointValue(*((int16_t*)buffer));
  } else if ((attributeId == Thermostat::Attributes::SystemMode::Id) && (clusterId == Thermostat::Id)) {
    dev->SetSystemMode(*buffer);
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

EmberAfStatus emberAfExternalAttributeReadCallback(EndpointId endpoint,
                                                   ClusterId clusterId,
                                                   const EmberAfAttributeMetadata* attributeMetadata,
                                                   uint8_t* buffer,
                                                   uint16_t maxReadLength)
{
  ChipLogProgress(DeviceLayer, "emberAfExternalAttributeReadCallback");
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);
  EmberAfStatus ret = EMBER_ZCL_STATUS_FAILURE;

  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    return ret;
  }

  if (instanceof<DeviceLightbulb>(dev)) {
    ret = HandleReadLightbulbAttribute(static_cast<DeviceLightbulb*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceTempSensor>(dev)) {
    ret = HandleReadTempMeasurementAttribute(static_cast<DeviceTempSensor*>(dev), attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceHumiditySensor>(dev)) {
    ret = HandleReadHumidityMeasurementAttribute(static_cast<DeviceHumiditySensor*>(dev), attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceSwitch>(dev)) {
    ret = HandleReadSwitchAttribute(static_cast<DeviceSwitch*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceIlluminanceSensor>(dev)) {
    ret = HandleReadIlluminanceMeasurementAttribute(static_cast<DeviceIlluminanceSensor*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DevicePressureSensor>(dev)) {
    ret = HandleReadPressureMeasurementAttribute(static_cast<DevicePressureSensor*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceOccupancySensor>(dev)) {
    ret = HandleReadOccupancySensingAttribute(static_cast<DeviceOccupancySensor*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceFlowSensor>(dev)) {
    ret = HandleReadFlowMeasurementAttribute(static_cast<DeviceFlowSensor*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceFan>(dev)) {
    ret = HandleReadFanControlAttribute(static_cast<DeviceFan*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  } else if (instanceof<DeviceThermostat>(dev)) {
    ret = HandleReadThermostatAttribute(static_cast<DeviceThermostat*>(dev), clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  }

  return ret;
}

EmberAfStatus emberAfExternalAttributeWriteCallback(EndpointId endpoint,
                                                    ClusterId clusterId,
                                                    const EmberAfAttributeMetadata* attributeMetadata,
                                                    uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "emberAfExternalAttributeWriteCallback");
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);
  EmberAfStatus ret = EMBER_ZCL_STATUS_FAILURE;

  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    return ret;
  }

  if (instanceof<DeviceLightbulb>(dev)) {
    ret = HandleWriteLightbulbAttribute(static_cast<DeviceLightbulb*>(dev), clusterId, attributeMetadata->attributeId, buffer);
  }
  if (instanceof<DeviceSwitch>(dev)) {
    ret = HandleWriteSwitchAttribute(static_cast<DeviceSwitch*>(dev), clusterId, attributeMetadata->attributeId, buffer);
  }
  if (instanceof<DeviceFan>(dev)) {
    ret = HandleWriteFanControlAttribute(static_cast<DeviceFan*>(dev), clusterId, attributeMetadata->attributeId, buffer);
  }
  if (instanceof<DeviceThermostat>(dev)) {
    ret = HandleWriteThermostatAttribute(static_cast<DeviceThermostat*>(dev), clusterId, attributeMetadata->attributeId, buffer);
  }
  return ret;
}
