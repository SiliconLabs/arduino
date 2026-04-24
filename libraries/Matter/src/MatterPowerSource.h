/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2026 Silicon Laboratories Inc. www.silabs.com
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

#ifndef MATTER_POWER_SOURCE_H
#define MATTER_POWER_SOURCE_H

#include "Matter.h"
#include "devices/DevicePowerSource.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterSimplePowerSource : public ArduinoMatterAppliance {
public:
  enum charge_level_t : uint8_t {
    CHARGE_LEVEL_OK = 0x00,
    CHARGE_LEVEL_WARNING = 0x01,
    CHARGE_LEVEL_CRITICAL = 0x02
  };

  MatterSimplePowerSource();
  ~MatterSimplePowerSource();
  bool begin();
  void end();
  void set_battery_percent(uint8_t value);
  uint8_t get_battery_percent();
  void set_battery_charge_level(charge_level_t value);
  charge_level_t get_battery_charge_level();
  operator uint8_t();
  void operator=(uint8_t value);

protected:
  DevicePowerSource* power_source_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

class MatterPowerSource : public MatterSimplePowerSource {
public:
  enum status_t : uint8_t {
    UNSPECIFIED = 0x00,
    ACTIVE = 0x01,
    STANDBY = 0x02,
    UNAVAILABLE = 0x03
  };

  MatterPowerSource();
  ~MatterPowerSource();
  bool begin();

  void set_status(status_t value);
  status_t get_status();

  void set_order(uint8_t value);
  uint8_t get_order();

  void set_description(const char* value);
  const char* get_description();

  void set_bat_voltage(uint32_t value);
  uint32_t get_bat_voltage();

  void set_bat_percent_remaining(uint8_t value);
  uint8_t get_bat_percent_remaining();

  void set_bat_time_remaining(uint32_t value);
  uint32_t get_bat_time_remaining();

  void set_bat_charge_level(charge_level_t value);
  charge_level_t get_bat_charge_level();

  void set_bat_replacement_needed(bool value);
  bool get_bat_replacement_needed();

  void set_bat_replaceability(uint8_t value);
  uint8_t get_bat_replaceability();

  void set_bat_present(bool value);
  bool get_bat_present();
};

#endif // MATTER_POWER_SOURCE_H
