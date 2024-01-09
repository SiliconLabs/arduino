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

#pragma once

#include "MatterDevice.h"

class DeviceThermostat : public Device
{
public:
  enum Changed_t{
    kChanged_HeatingSetpointValue = kChanged_Last << 1,
    kChanged_LocalTemperatureValue = kChanged_Last << 2,
    kChanged_SystemModeValue = kChanged_Last << 3,
  } Changed;

  DeviceThermostat(const char* device_name, std::string location, int16_t local_temperature, int16_t heating_setpoint);
  using DeviceCallback_fn = std::function<void(DeviceThermostat*, DeviceThermostat::Changed_t)>;

  int16_t GetLocalTemperatureValue();
  void SetLocalTemperatureValue(int16_t local_temp);
  int16_t GetHeatingSetpointValue();
  void SetHeatingSetpointValue(int16_t heating_setpoint);
  uint8_t GetSystemMode();
  void SetSystemMode(uint8_t system_mode);

  void SetAbsMinHeatingSetpoint(int16_t abs_min_heating_setpoint);
  int16_t GetAbsMinHeatingSetpoint();
  void SetMinHeatingSetpoint(int16_t min_heating_setpoint);
  int16_t GetMinHeatingSetpoint();

  void SetAbsMaxHeatingSetpoint(int16_t abs_max_heating_setpoint);
  int16_t GetAbsMaxHeatingSetpoint();
  void SetMaxHeatingSetpoint(int16_t max_heating_setpoint);
  int16_t GetMaxHeatingSetpoint();

  void SetChangeCallback(DeviceCallback_fn device_changed_callback);
  uint8_t GetControlSequenceOfOperation();
  uint32_t GetThermostatClusterFeatureMap();
  uint16_t GetThermostatClusterRevision();

private:
  void HandleDeviceChange(Device* device, Device::Changed_t change_mask);

  int16_t local_temperature;
  int16_t heating_setpoint;
  uint8_t system_mode;
  int16_t abs_min_heating_setpoint;
  int16_t min_heating_setpoint;
  int16_t abs_max_heating_setpoint;
  int16_t max_heating_setpoint;
  DeviceCallback_fn device_changed_callback;

  static const uint8_t thermostat_control_sequence_of_operation = 2u; // Heating only
  static const uint32_t thermostat_cluster_feature_map          = 1u; // Heating capability enabled
  static const uint16_t thermostat_cluster_revision             = 5u;
};
