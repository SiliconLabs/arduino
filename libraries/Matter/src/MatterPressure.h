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

#ifndef MATTER_PRESSURE_H
#define MATTER_PRESSURE_H

#include "Matter.h"
#include "devices/DevicePressureSensor.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterPressure : public ArduinoMatterAppliance {
public:
  MatterPressure();
  ~MatterPressure();
  bool begin();
  void end();
  void set_measured_value(int16_t value);
  void set_measured_value(float value);
  void set_measured_value(double value);
  int16_t get_measured_value();
  operator uint16_t();
  operator float();
  operator double();
  void operator=(int16_t value);
  void operator=(float value);
  void operator=(double value);

private:
  DevicePressureSensor* sensor_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

#endif // MATTER_PRESSURE_H
