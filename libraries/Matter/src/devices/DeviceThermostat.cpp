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

#include "DeviceThermostat.h"

DeviceThermostat::DeviceThermostat(const char* device_name,
                                   std::string location,
                                   int16_t local_temperature,
                                   int16_t heating_setpoint) :
  Device(device_name, location),
  local_temperature(local_temperature),
  heating_setpoint(heating_setpoint),
  system_mode(0),
  abs_min_heating_setpoint(700),
  min_heating_setpoint(1600),
  abs_max_heating_setpoint(3200),
  max_heating_setpoint(3000),
  device_changed_callback(nullptr)
{
  ;
}

int16_t DeviceThermostat::GetLocalTemperatureValue()
{
  return this->local_temperature;
}

void DeviceThermostat::SetLocalTemperatureValue(int16_t local_temp)
{
  bool changed = this->local_temperature != local_temp;
  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new local temp='%d'", mName, local_temp);
  this->local_temperature = local_temp;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_LocalTemperatureValue);
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

  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new heating setpoint='%d'", mName, heating_setpoint);
  this->heating_setpoint = heating_setpoint;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_HeatingSetpointValue);
  }
}

uint8_t DeviceThermostat::GetSystemMode()
{
  return this->system_mode;
}

void DeviceThermostat::SetSystemMode(uint8_t system_mode)
{
  bool changed = this->system_mode != system_mode;
  ChipLogProgress(DeviceLayer, "ThermostatDevice[%s]: new system mode='%u'", mName, system_mode);
  this->system_mode = system_mode;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_SystemModeValue);
  }
}

void DeviceThermostat::SetChangeCallback(DeviceCallback_fn device_changed_callback)
{
  this->device_changed_callback = device_changed_callback;
}

void DeviceThermostat::HandleDeviceChange(Device* device, Device::Changed_t change_mask)
{
  if (this->device_changed_callback) {
    this->device_changed_callback(this, (DeviceThermostat::Changed_t) change_mask);
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
