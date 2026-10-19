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

#ifndef ZIGBEE_TEMPERATURE_SENSOR_H
#define ZIGBEE_TEMPERATURE_SENSOR_H

#include "Zigbee.h"
#include "devices/DeviceTempSensor.h"

class ZigbeeTemperatureSensor : public ArduinoZigbeeAppliance {
public:
  ZigbeeTemperatureSensor();
  ~ZigbeeTemperatureSensor();
  bool begin() override;
  void end() override;

  void set_measured_value_raw(int16_t value);
  void set_measured_value_celsius(float celsius);
  int16_t get_measured_value_raw();
  float get_measured_value_celsius();
  void set_min_value_raw(int16_t value);
  void set_max_value_raw(int16_t value);
  void set_min_value_celsius(float celsius);
  void set_max_value_celsius(float celsius);

  bool send_attribute_report();
  bool get_attribute_report_sent();
  bool set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s);

  operator float();
  void operator=(float celsius);

private:
  bool start_on_endpoint(uint8_t endpoint_id);

  DeviceTempSensor* sensor_device;
  bool initialized;
};

#endif // ZIGBEE_TEMPERATURE_SENSOR_H
