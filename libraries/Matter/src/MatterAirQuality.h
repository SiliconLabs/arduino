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

#ifndef MATTER_AIRQUALITY_H
#define MATTER_AIRQUALITY_H

#include "Matter.h"
#include "devices/DeviceAirQualitySensor.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterAirQuality : public ArduinoMatterAppliance {
public:

  // Enum for air quality value
  enum class AirQuality_t : uint8_t {
    UNKNOWN        = 0x00,
    GOOD           = 0x01,
    FAIR           = 0x02,
    MODERATE       = 0x03,
    POOR           = 0x04,
    VERY_POOR      = 0x05,
    EXTREMELY_POOR = 0x06,
    // All received enum values that are not listed above will be mapped
    // to UNDEFINED_VALUE. This is a helper enum value that should only
    // be used by code to process how it handles receiving and undefined
    // enum value.
    UNDEFINED_VALUE   = 0x07,
  };

  MatterAirQuality();
  ~MatterAirQuality();
  bool begin();
  void end();
  void set_air_quality(MatterAirQuality::AirQuality_t value);
  MatterAirQuality::AirQuality_t get_air_quality();
  void operator=(MatterAirQuality::AirQuality_t value);

private:
  DeviceAirQualitySensor* sensor_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

#endif // MATTER_AIRQUALITY_H
