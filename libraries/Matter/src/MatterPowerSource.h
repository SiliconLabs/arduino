// SPDX-License-Identifier: MIT

#ifndef MATTER_POWER_SOURCE_H
#define MATTER_POWER_SOURCE_H

#include "Matter.h"
#include "devices/DevicePowerSource.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterPowerSource : public ArduinoMatterAppliance {
public:
  MatterPowerSource();
  ~MatterPowerSource();
  bool begin();
  void end();
  void set_bat_percent_remaining_raw(uint8_t value);
  void set_bat_percent_remaining(double percent);
  uint8_t get_bat_percent_remaining();
  void operator=(uint8_t value);

private:
  DevicePowerSource* power_source_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

#endif // MATTER_POWER_SOURCE_H
