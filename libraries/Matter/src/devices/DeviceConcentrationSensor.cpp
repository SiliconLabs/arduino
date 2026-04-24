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

#include "DeviceConcentrationSensor.h"

DeviceConcentrationMeasurementSensor::DeviceConcentrationMeasurementSensor(const char* device_name,
                                                                           device_type_t device_type,
                                                                           measurement_unit_t measurement_unit,
                                                                           float min_measured_value,
                                                                           float max_measured_value,
                                                                           float measured_value) :
  Device(device_name),
  measured_value(measured_value),
  min_measured_value(min_measured_value),
  max_measured_value(max_measured_value)
{
  this->SetDeviceType(device_type);
  this->concentration_measurement_cluster_measurement_unit = static_cast<uint8_t>(measurement_unit);
}

float DeviceConcentrationMeasurementSensor::GetMeasuredValue()
{
  return this->measured_value;
}

void DeviceConcentrationMeasurementSensor::SetMeasuredValue(float measurement)
{
  bool changed = this->measured_value != measurement;
  ChipLogProgress(DeviceLayer, "ConcentrationMeasurementSensorDevice[%s]: new measurement='%f'", this->device_name, measurement);
  this->measured_value = measurement;
  if (changed) {
    this->HandleConcentrationMeasurementSensorDeviceStatusChanged(kChanged_MeasurementValue);
    CallDeviceChangeCallback();
  }
}

float DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterMinMeasurementValue()
{
  return this->min_measured_value;
}

float DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterMaxMeasurementValue()
{
  return this->max_measured_value;
}

uint8_t DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterMeasurementUnit()
{
  return this->concentration_measurement_cluster_measurement_unit;
}

uint8_t DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterMeasurementMedium()
{
  return this->concentration_measurement_cluster_measurement_medium;
}

uint32_t DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterFeatureMap()
{
  return this->concentration_measurement_cluster_feature_map;
}

uint16_t DeviceConcentrationMeasurementSensor::GetConcentrationMeasurementClusterRevision()
{
  return this->concentration_measurement_cluster_revision;
}

CHIP_ERROR DeviceConcentrationMeasurementSensor::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                                            chip::AttributeId attributeId,
                                                                            uint8_t* buffer,
                                                                            uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  ChipLogProgress(DeviceLayer, "HandleReadConcentrationMeasurementSensorAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if ((this->device_type == device_type_t::kDeviceType_CO2Sensor) && (clusterId != chip::app::Clusters::CarbonDioxideConcentrationMeasurement::Id)) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }
  if ((this->device_type == device_type_t::kDeviceType_COSensor) && (clusterId != chip::app::Clusters::CarbonMonoxideConcentrationMeasurement::Id)) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }
  if ((this->device_type == device_type_t::kDeviceType_TVOCSensor) && (clusterId != chip::app::Clusters::TotalVolatileOrganicCompoundsConcentrationMeasurement::Id)) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  const AttributeId ConcentrationMeasurementMeasuredValueId = 0x00000000;
  const AttributeId ConcentrationMeasurementMinMeasuredValueId = 0x00000001;
  const AttributeId ConcentrationMeasurementMaxMeasuredValueId = 0x00000002;
  const AttributeId ConcentrationMeasurementMeasurementUnitId = 0x00000008;
  const AttributeId ConcentrationMeasurementMeasurementMediumId = 0x00000009;
  const AttributeId ConcentrationMeasurementFeatureMapId = chip::app::Clusters::Globals::Attributes::FeatureMap::Id;
  const AttributeId ConcentrationMeasurementClusterRevisionId = chip::app::Clusters::Globals::Attributes::ClusterRevision::Id;

  if ((attributeId == ConcentrationMeasurementMeasuredValueId) && (maxReadLength == 4)) {
    float measuredValue = this->GetMeasuredValue();
    memcpy(buffer, &measuredValue, sizeof(measuredValue));
  } else if ((attributeId == ConcentrationMeasurementMinMeasuredValueId) && (maxReadLength == 4)) {
    float minMeasuredValue = this->GetConcentrationMeasurementClusterMinMeasurementValue();
    memcpy(buffer, &minMeasuredValue, sizeof(minMeasuredValue));
  } else if ((attributeId == ConcentrationMeasurementMaxMeasuredValueId) && (maxReadLength == 4)) {
    float maxMeasuredValue = this->GetConcentrationMeasurementClusterMaxMeasurementValue();
    memcpy(buffer, &maxMeasuredValue, sizeof(maxMeasuredValue));
  } else if ((attributeId == ConcentrationMeasurementMeasurementUnitId) && (maxReadLength == 1)) {
    uint8_t measurement_unit = this->GetConcentrationMeasurementClusterMeasurementUnit();
    memcpy(buffer, &measurement_unit, sizeof(measurement_unit));
  } else if ((attributeId == ConcentrationMeasurementMeasurementMediumId) && (maxReadLength == 1)) {
    uint8_t measurement_medium = this->GetConcentrationMeasurementClusterMeasurementMedium();
    memcpy(buffer, &measurement_medium, sizeof(measurement_medium));
  } else if ((attributeId == ConcentrationMeasurementFeatureMapId) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetConcentrationMeasurementClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ConcentrationMeasurementClusterRevisionId) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetConcentrationMeasurementClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceConcentrationMeasurementSensor::HandleConcentrationMeasurementSensorDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_MeasurementValue) {
    if (this->device_type == device_type_t::kDeviceType_CO2Sensor) {
      ScheduleMatterReportingCallback(this->endpoint_id, CarbonDioxideConcentrationMeasurement::Id, CarbonDioxideConcentrationMeasurement::Attributes::MeasuredValue::Id);
    } else if (this->device_type == device_type_t::kDeviceType_COSensor) {
      ScheduleMatterReportingCallback(this->endpoint_id, CarbonMonoxideConcentrationMeasurement::Id, CarbonMonoxideConcentrationMeasurement::Attributes::MeasuredValue::Id);
    } else if (this->device_type == device_type_t::kDeviceType_TVOCSensor) {
      ScheduleMatterReportingCallback(this->endpoint_id, TotalVolatileOrganicCompoundsConcentrationMeasurement::Id, TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MeasuredValue::Id);
    }
  }
}
