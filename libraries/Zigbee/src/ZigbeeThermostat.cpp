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

#include "ZigbeeThermostat.h"
#include <math.h>

ZigbeeThermostat::ZigbeeThermostat() :
  thermostat_device(nullptr),
  initialized(false)
{
}

ZigbeeThermostat::~ZigbeeThermostat()
{
  end();
}

bool ZigbeeThermostat::begin()
{
  if (this->initialized) {
    return false;
  }

  uint8_t ep = Zigbee.allocateEndpoint(ZIGBEE_THERMOSTAT);
  if (ep == 0) {
    return false;
  }

  if (!this->start_on_endpoint(ep)) {
    Zigbee.freeEndpoint(ep);
    return false;
  }
  return true;
}

bool ZigbeeThermostat::start_on_endpoint(uint8_t endpoint_id)
{
  if (this->initialized) {
    return false;
  }

  this->thermostat_device = new DeviceThermostat("Zigbee Thermostat", endpoint_id, 2000, 2000);
  if (!this->thermostat_device) {
    return false;
  }

  this->base_zigbee_device = this->thermostat_device;

  if (!zigbee_endpoint_register(this->thermostat_device)) {
    delete this->thermostat_device;
    this->thermostat_device = nullptr;
    this->base_zigbee_device = nullptr;
    return false;
  }

  this->thermostat_device->InitializeAttributes();
  this->thermostat_device->SetOnline(true);
  this->initialized = true;
  return true;
}

void ZigbeeThermostat::end()
{
  if (!this->initialized) {
    return;
  }

  Zigbee.freeEndpoint(this->thermostat_device->GetEndpointId());
  zigbee_endpoint_unregister(this->thermostat_device);
  delete this->thermostat_device;
  this->thermostat_device = nullptr;
  this->base_zigbee_device = nullptr;
  this->initialized = false;
}

int16_t ZigbeeThermostat::get_local_temperature_raw()
{
  if (!this->thermostat_device) {
    return 0;
  }
  return this->thermostat_device->GetLocalTemperatureValue();
}

void ZigbeeThermostat::set_local_temperature_raw(int16_t local_temp)
{
  if (this->thermostat_device) {
    this->thermostat_device->SetLocalTemperatureValue(local_temp);
  }
}

int16_t ZigbeeThermostat::get_heating_setpoint_raw()
{
  if (!this->thermostat_device) {
    return 0;
  }
  return this->thermostat_device->GetHeatingSetpointValue();
}

void ZigbeeThermostat::set_heating_setpoint_raw(int16_t heating_setpoint)
{
  if (this->thermostat_device) {
    this->thermostat_device->SetHeatingSetpointValue(heating_setpoint);
  }
}

float ZigbeeThermostat::get_local_temperature()
{
  return (float)this->get_local_temperature_raw() / 100.0f;
}

void ZigbeeThermostat::set_local_temperature(float local_temp)
{
  if (!isfinite(local_temp)) {
    return;
  }
  this->set_local_temperature_raw(temperature_to_raw(local_temp));
}

float ZigbeeThermostat::get_heating_setpoint()
{
  return (float)this->get_heating_setpoint_raw() / 100.0f;
}

void ZigbeeThermostat::set_heating_setpoint(float heating_setpoint)
{
  if (!isfinite(heating_setpoint)) {
    return;
  }
  this->set_heating_setpoint_raw(temperature_to_raw(heating_setpoint));
}

void ZigbeeThermostat::set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint)
{
  if (this->thermostat_device && isfinite(abs_min_heating_setpoint)) {
    this->thermostat_device->SetAbsMinHeatingSetpoint(temperature_to_raw(abs_min_heating_setpoint));
  }
}

void ZigbeeThermostat::set_minimum_heating_setpoint(float min_heating_setpoint)
{
  if (this->thermostat_device && isfinite(min_heating_setpoint)) {
    this->thermostat_device->SetMinHeatingSetpoint(temperature_to_raw(min_heating_setpoint));
  }
}

void ZigbeeThermostat::set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint)
{
  if (this->thermostat_device && isfinite(abs_max_heating_setpoint)) {
    this->thermostat_device->SetAbsMaxHeatingSetpoint(temperature_to_raw(abs_max_heating_setpoint));
  }
}

void ZigbeeThermostat::set_maximum_heating_setpoint(float max_heating_setpoint)
{
  if (this->thermostat_device && isfinite(max_heating_setpoint)) {
    this->thermostat_device->SetMaxHeatingSetpoint(temperature_to_raw(max_heating_setpoint));
  }
}

float ZigbeeThermostat::get_absolute_minimum_heating_setpoint()
{
  if (!this->thermostat_device) {
    return 0.0f;
  }
  return (float)this->thermostat_device->GetAbsMinHeatingSetpoint() / 100.0f;
}

float ZigbeeThermostat::get_minimum_heating_setpoint()
{
  if (!this->thermostat_device) {
    return 0.0f;
  }
  return (float)this->thermostat_device->GetMinHeatingSetpoint() / 100.0f;
}

float ZigbeeThermostat::get_absolute_maximum_heating_setpoint()
{
  if (!this->thermostat_device) {
    return 0.0f;
  }
  return (float)this->thermostat_device->GetAbsMaxHeatingSetpoint() / 100.0f;
}

float ZigbeeThermostat::get_maximum_heating_setpoint()
{
  if (!this->thermostat_device) {
    return 0.0f;
  }
  return (float)this->thermostat_device->GetMaxHeatingSetpoint() / 100.0f;
}

ZigbeeThermostat::thermostat_mode_t ZigbeeThermostat::get_system_mode()
{
  if (!this->thermostat_device) {
    return OFF;
  }
  return (thermostat_mode_t)this->thermostat_device->GetSystemMode();
}

void ZigbeeThermostat::set_system_mode(ZigbeeThermostat::thermostat_mode_t system_mode)
{
  if (this->thermostat_device) {
    this->thermostat_device->SetSystemMode((uint8_t)system_mode);
  }
}

ZigbeeThermostat::thermostat_hvac_action_t ZigbeeThermostat::get_hvac_action()
{
  if (!this->thermostat_device) {
    return IDLE;
  }
  return (thermostat_hvac_action_t)this->thermostat_device->GetHvacAction();
}

void ZigbeeThermostat::set_hvac_action(ZigbeeThermostat::thermostat_hvac_action_t hvac_action)
{
  if (this->thermostat_device) {
    this->thermostat_device->SetHvacAction((uint8_t)hvac_action);
  }
}

bool ZigbeeThermostat::send_attribute_report()
{
  if (!this->thermostat_device) {
    return false;
  }
  return this->thermostat_device->SendAttributeReport();
}

int16_t ZigbeeThermostat::temperature_to_raw(float temperature)
{
  float scaled = temperature * 100.0f;
  if (scaled < -27315.0f) {
    scaled = -27315.0f;
  } else if (scaled > 32767.0f) {
    scaled = 32767.0f;
  }
  return static_cast<int16_t>(scaled);
}
