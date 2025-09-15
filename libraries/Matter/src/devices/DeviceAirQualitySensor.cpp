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

#include "DeviceAirQualitySensor.h"

DeviceAirQualitySensor::DeviceAirQualitySensor(const char* device_name, uint8_t measured_value) :
  Device(device_name),
  measured_value(measured_value)
{
  this->SetDeviceType(device_type_t::kDeviceType_AirQualitySensor);
}

uint8_t DeviceAirQualitySensor::GetMeasuredValue()
{
  return this->measured_value;
}

void DeviceAirQualitySensor::SetMeasuredValue(uint8_t measurement)
{
  bool changed = this->measured_value != measurement;
  ChipLogProgress(DeviceLayer, "AirQualitySensorDevice[%s]: new measurement='%d'", this->device_name, measurement);
  this->measured_value = measurement;

  if (changed) {
    this->HandleAirQualitySensorDeviceStatusChanged(kChanged_MeasurementValue);
    CallDeviceChangeCallback();
  }
}

uint32_t DeviceAirQualitySensor::GetAirQualitySensorClusterFeatureMap()
{
  return this->air_quality_sensor_cluster_feature_map;
}

uint16_t DeviceAirQualitySensor::GetAirQualitySensorClusterRevision()
{
  return this->air_quality_sensor_cluster_revision;
}

CHIP_ERROR DeviceAirQualitySensor::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                              chip::AttributeId attributeId,
                                                              uint8_t* buffer,
                                                              uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::AirQuality::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadAirQualitySensorAttribute: clusterId=%lu attrId=%ld, length:%d", clusterId, attributeId, maxReadLength);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::AirQuality::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == AirQuality::Id) && (maxReadLength == 1)) {
    uint8_t measuredValue = this->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetAirQualitySensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetAirQualitySensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceAirQualitySensor::HandleAirQualitySensorDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & DeviceAirQualitySensor::kChanged_MeasurementValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, AirQuality::Id, AirQuality::Attributes::AirQuality::Id);
  }
}
