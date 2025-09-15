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

#ifndef MATTER_THERMOSTAT_H
#define MATTER_THERMOSTAT_H

#include "Matter.h"
#include "devices/DeviceThermostat.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterThermostat : public ArduinoMatterAppliance {
public:

  enum thermostat_mode_t {
    OFF = 0,
    HEAT = 4
  };

  MatterThermostat();
  ~MatterThermostat();
  bool begin();
  void end();
  int16_t get_local_temperature_raw();
  void set_local_temperature_raw(int16_t local_temp);
  int16_t get_heating_setpoint_raw();
  void set_heating_setpoint_raw(int16_t heating_setpoint);

  float get_local_temperature();
  void set_local_temperature(float local_temp);
  float get_heating_setpoint();
  void set_heating_setpoint(float heating_setpoint);

  void set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint);
  void set_minimum_heating_setpoint(float min_heating_setpoint);
  void set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint);
  void set_maximum_heating_setpoint(float max_heating_setpoint);
  float get_absolute_minimum_heating_setpoint();
  float get_minimum_heating_setpoint();
  float get_absolute_maximum_heating_setpoint();
  float get_maximum_heating_setpoint();

  thermostat_mode_t get_system_mode();
  void set_system_mode(thermostat_mode_t system_mode);

private:
  DeviceThermostat* thermostat_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

#endif // MATTER_THERMOSTAT_H
