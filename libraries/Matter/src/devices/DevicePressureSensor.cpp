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

#include "DevicePressureSensor.h"

DevicePressureSensor::DevicePressureSensor(const char* device_name,
                                           int16_t min,
                                           int16_t max,
                                           int16_t measured_value) :
  Device(device_name),
  min_value(min),
  max_value(max),
  measured_value(measured_value)
{
  this->SetDeviceType(device_type_t::kDeviceType_PressureSensor);
}

int16_t DevicePressureSensor::GetMeasuredValue()
{
  return this->measured_value;
}

void DevicePressureSensor::SetMeasuredValue(int16_t measurement)
{
  if (measurement < this->min_value) {
    measurement = this->min_value;
  } else if (measurement > this->max_value) {
    measurement = this->max_value;
  }

  bool changed = this->measured_value != measurement;
  ChipLogProgress(DeviceLayer, "PressureSensorDevice[%s]: new measurement='%d'", this->device_name, measurement);
  this->measured_value = measurement;

  if (changed) {
    this->HandlePressureSensorDeviceStatusChanged(kChanged_MeasurementValue);
    CallDeviceChangeCallback();
  }
}

uint32_t DevicePressureSensor::GetPressureSensorClusterFeatureMap()
{
  return this->pressure_sensor_cluster_feature_map;
}

uint16_t DevicePressureSensor::GetPressureSensorClusterRevision()
{
  return this->pressure_sensor_cluster_revision;
}

CHIP_ERROR DevicePressureSensor::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                            chip::AttributeId attributeId,
                                                            uint8_t* buffer,
                                                            uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::PressureMeasurement::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadPressureMeasurementAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::PressureMeasurement::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == MeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t measuredValue = this->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == MinMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t minValue = this->min_value;
    memcpy(buffer, &minValue, sizeof(minValue));
  } else if ((attributeId == MaxMeasuredValue::Id) && (maxReadLength == 2)) {
    int16_t maxValue = this->max_value;
    memcpy(buffer, &maxValue, sizeof(maxValue));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetPressureSensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetPressureSensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DevicePressureSensor::HandlePressureSensorDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_MeasurementValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, PressureMeasurement::Id, PressureMeasurement::Attributes::MeasuredValue::Id);
  }
}
