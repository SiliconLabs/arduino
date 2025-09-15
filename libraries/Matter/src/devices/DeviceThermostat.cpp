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

#include "DeviceThermostat.h"

DeviceThermostat::DeviceThermostat(const char* device_name,
                                   int16_t local_temperature,
                                   int16_t heating_setpoint) :
  Device(device_name),
  local_temperature(local_temperature),
  heating_setpoint(heating_setpoint),
  system_mode(0),
  abs_min_heating_setpoint(700),
  min_heating_setpoint(1600),
  abs_max_heating_setpoint(3200),
  max_heating_setpoint(3000)
{
  this->SetDeviceType(device_type_t::kDeviceType_Thermostat);
}

int16_t DeviceThermostat::GetLocalTemperatureValue()
{
  return this->local_temperature;
}

void DeviceThermostat::SetLocalTemperatureValue(int16_t local_temp)
{
  bool changed = this->local_temperature != local_temp;
  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new local temp='%d'", this->device_name, local_temp);
  this->local_temperature = local_temp;

  if (changed) {
    this->HandleThermostatDeviceStatusChanged(kChanged_LocalTemperatureValue);
    CallDeviceChangeCallback();
  }
}

int16_t DeviceThermostat::GetHeatingSetpointValue()
{
  return this->heating_setpoint;
}

void DeviceThermostat::SetHeatingSetpointValue(int16_t heating_setpoint)
{
  bool changed = this->heating_setpoint != heating_setpoint;

  if (heating_setpoint < this->abs_min_heating_setpoint) {
    heating_setpoint = this->abs_min_heating_setpoint;
  }
  if (heating_setpoint > this->abs_max_heating_setpoint) {
    heating_setpoint = this->abs_max_heating_setpoint;
  }

  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new heating setpoint='%d'", this->device_name, heating_setpoint);
  this->heating_setpoint = heating_setpoint;

  if (changed) {
    this->HandleThermostatDeviceStatusChanged(kChanged_HeatingSetpointValue);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceThermostat::GetSystemMode()
{
  return this->system_mode;
}

void DeviceThermostat::SetSystemMode(uint8_t system_mode)
{
  bool changed = this->system_mode != system_mode;
  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new system mode='%u'", this->device_name, system_mode);
  this->system_mode = system_mode;

  if (changed) {
    this->HandleThermostatDeviceStatusChanged(kChanged_SystemModeValue);
    CallDeviceChangeCallback();
  }
}

uint8_t DeviceThermostat::GetControlSequenceOfOperation()
{
  return this->thermostat_control_sequence_of_operation;
}

uint32_t DeviceThermostat::GetThermostatClusterFeatureMap()
{
  return this->thermostat_cluster_feature_map;
}

uint16_t DeviceThermostat::GetThermostatClusterRevision()
{
  return this->thermostat_cluster_revision;
}

void DeviceThermostat::SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint)
{
  if (abs_min_heating_setpoint > this->min_heating_setpoint) {
    return;
  }
  this->abs_min_heating_setpoint = abs_min_heating_setpoint;
}

int16_t DeviceThermostat::GetAbsMinHeatingSetpoint()
{
  return this->abs_min_heating_setpoint;
}

void DeviceThermostat::SetMinHeatingSetpoint(int16_t min_heating_setpoint)
{
  if (min_heating_setpoint < this->abs_min_heating_setpoint) {
    return;
  }
  this->min_heating_setpoint = min_heating_setpoint;
}

int16_t DeviceThermostat::GetMinHeatingSetpoint()
{
  return this->min_heating_setpoint;
}

void DeviceThermostat::SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint)
{
  if (this->max_heating_setpoint > abs_max_heating_setpoint) {
    return;
  }
  this->abs_max_heating_setpoint = abs_max_heating_setpoint;
}

int16_t DeviceThermostat::GetAbsMaxHeatingSetpoint()
{
  return this->abs_max_heating_setpoint;
}

void DeviceThermostat::SetMaxHeatingSetpoint(int16_t max_heating_setpoint)
{
  if (max_heating_setpoint > this->abs_max_heating_setpoint) {
    return;
  }
  this->max_heating_setpoint = max_heating_setpoint;
}

int16_t DeviceThermostat::GetMaxHeatingSetpoint()
{
  return this->max_heating_setpoint;
}

CHIP_ERROR DeviceThermostat::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                        chip::AttributeId attributeId,
                                                        uint8_t* buffer,
                                                        uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::Thermostat::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadThermostatAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::Thermostat::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == LocalTemperature::Id) && (maxReadLength == 2)) {
    int16_t localTemp = this->GetLocalTemperatureValue();
    memcpy(buffer, &localTemp, sizeof(localTemp));
  } else if ((attributeId == OccupiedHeatingSetpoint::Id) && (maxReadLength == 2)) {
    int16_t heatingSetpoint = this->GetHeatingSetpointValue();
    memcpy(buffer, &heatingSetpoint, sizeof(heatingSetpoint));
  } else if ((attributeId == SystemMode::Id) && (maxReadLength == 1)) {
    uint8_t systemMode = this->GetSystemMode();
    memcpy(buffer, &systemMode, sizeof(systemMode));
  } else if ((attributeId == ControlSequenceOfOperation::Id) && (maxReadLength == 1)) {
    uint8_t seq_op = this->GetControlSequenceOfOperation();
    memcpy(buffer, &seq_op, sizeof(seq_op));
  } else if ((attributeId == AbsMinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t abs_min_heat_setpoint = this->GetAbsMinHeatingSetpoint();
    memcpy(buffer, &abs_min_heat_setpoint, sizeof(abs_min_heat_setpoint));
  } else if ((attributeId == AbsMaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t abs_max_heat_setpoint = this->GetAbsMaxHeatingSetpoint();
    memcpy(buffer, &abs_max_heat_setpoint, sizeof(abs_max_heat_setpoint));
  } else if ((attributeId == MinHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t min_heat_setpoint = this->GetMinHeatingSetpoint();
    memcpy(buffer, &min_heat_setpoint, sizeof(min_heat_setpoint));
  } else if ((attributeId == MaxHeatSetpointLimit::Id) && (maxReadLength == 2)) {
    int16_t max_heat_setpoint = this->GetMaxHeatingSetpoint();
    memcpy(buffer, &max_heat_setpoint, sizeof(max_heat_setpoint));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetThermostatClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetThermostatClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceThermostat::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                         chip::AttributeId attributeId,
                                                         uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters;
  using namespace ::chip::app::Clusters::Thermostat::Attributes;
  ChipLogProgress(DeviceLayer, "HandleWriteThermostatAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId != chip::app::Clusters::Thermostat::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if (attributeId == OccupiedHeatingSetpoint::Id) {
    this->SetHeatingSetpointValue(*((int16_t*)buffer));
  } else if (attributeId == SystemMode::Id) {
    this->SetSystemMode(*buffer);
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceThermostat::HandleThermostatDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_HeatingSetpointValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::OccupiedHeatingSetpoint::Id);
  }
  if (itemChangedMask & kChanged_LocalTemperatureValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::LocalTemperature::Id);
  }
  if (itemChangedMask & kChanged_SystemModeValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, Thermostat::Id, Thermostat::Attributes::SystemMode::Id);
  }
}
