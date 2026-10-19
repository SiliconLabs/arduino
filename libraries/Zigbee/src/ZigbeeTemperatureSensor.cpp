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

#include "ZigbeeTemperatureSensor.h"
#include <math.h>

ZigbeeTemperatureSensor::ZigbeeTemperatureSensor() :
  sensor_device(nullptr),
  initialized(false)
{
}

ZigbeeTemperatureSensor::~ZigbeeTemperatureSensor()
{
  end();
}

bool ZigbeeTemperatureSensor::begin()
{
  if (this->initialized) {
    return false;
  }

  uint8_t ep = Zigbee.allocateEndpoint(ZIGBEE_TEMPERATURE_SENSOR);
  if (ep == 0) {
    return false;
  }
  if (!start_on_endpoint(ep)) {
    Zigbee.freeEndpoint(ep);
    return false;
  }
  return true;
}

bool ZigbeeTemperatureSensor::start_on_endpoint(uint8_t endpoint_id)
{
  if (this->initialized) {
    return false;
  }

  this->sensor_device = new DeviceTempSensor("Zigbee Temp Sensor", endpoint_id);
  if (!this->sensor_device) {
    return false;
  }

  this->base_zigbee_device = this->sensor_device;

  if (!zigbee_endpoint_register(this->sensor_device)) {
    delete this->sensor_device;
    this->sensor_device = nullptr;
    this->base_zigbee_device = nullptr;
    return false;
  }

  this->sensor_device->SetOnline(true);
  this->initialized = true;
  return true;
}

void ZigbeeTemperatureSensor::end()
{
  if (!this->initialized) {
    return;
  }
  Zigbee.freeEndpoint(this->sensor_device->GetEndpointId());
  zigbee_endpoint_unregister(this->sensor_device);
  delete this->sensor_device;
  this->sensor_device = nullptr;
  this->base_zigbee_device = nullptr;
  this->initialized = false;
}

void ZigbeeTemperatureSensor::set_measured_value_raw(int16_t value)
{
  if (this->sensor_device) {
    this->sensor_device->SetMeasuredValue(value);
  }
}

void ZigbeeTemperatureSensor::set_measured_value_celsius(float celsius)
{
  if (!isfinite(celsius)) {
    return;
  }
  float scaled = celsius * 100.0f;
  if (scaled < -27315.0f) {
    scaled = -27315.0f;
  } else if (scaled > 32767.0f) {
    scaled = 32767.0f;
  }
  this->set_measured_value_raw(static_cast<int16_t>(scaled));
}

int16_t ZigbeeTemperatureSensor::get_measured_value_raw()
{
  if (!this->sensor_device) {
    return 0;
  }
  return this->sensor_device->GetMeasuredValue();
}

float ZigbeeTemperatureSensor::get_measured_value_celsius()
{
  return (float)get_measured_value_raw() / 100.0f;
}

void ZigbeeTemperatureSensor::set_min_value_raw(int16_t value)
{
  if (this->sensor_device) {
    this->sensor_device->SetMinMeasuredValue(value);
  }
}

void ZigbeeTemperatureSensor::set_max_value_raw(int16_t value)
{
  if (this->sensor_device) {
    this->sensor_device->SetMaxMeasuredValue(value);
  }
}

void ZigbeeTemperatureSensor::set_min_value_celsius(float celsius)
{
  if (!isfinite(celsius)) {
    return;
  }
  float scaled = celsius * 100.0f;
  if (scaled < -27315.0f) {
    scaled = -27315.0f;
  } else if (scaled > 32767.0f) {
    scaled = 32767.0f;
  }
  this->set_min_value_raw(static_cast<int16_t>(scaled));
}

void ZigbeeTemperatureSensor::set_max_value_celsius(float celsius)
{
  if (!isfinite(celsius)) {
    return;
  }
  float scaled = celsius * 100.0f;
  if (scaled < -27315.0f) {
    scaled = -27315.0f;
  } else if (scaled > 32767.0f) {
    scaled = 32767.0f;
  }
  this->set_max_value_raw(static_cast<int16_t>(scaled));
}

bool ZigbeeTemperatureSensor::send_attribute_report()
{
  if (!this->sensor_device) {
    return false;
  }
  return this->sensor_device->SendAttributeReport();
}

bool ZigbeeTemperatureSensor::get_attribute_report_sent()
{
  if (!this->sensor_device) {
    return false;
  }
  return this->sensor_device->GetAttributeReportSent();
}

bool ZigbeeTemperatureSensor::set_reporting_interval(uint16_t min_interval_s, uint16_t max_interval_s)
{
  if (!this->sensor_device) {
    return false;
  }
  return this->sensor_device->SetReportingInterval(min_interval_s, max_interval_s);
}

ZigbeeTemperatureSensor::operator float()
{
  return get_measured_value_celsius();
}

void ZigbeeTemperatureSensor::operator=(float celsius)
{
  set_measured_value_celsius(celsius);
}
