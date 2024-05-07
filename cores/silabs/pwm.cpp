/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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

#include "pwm.h"

using namespace arduino;

PwmClass::PwmClass() :
  pwm_mode(pwm_mode_t::DUTY_CYCLE),
  auto_deinit(true),
  pwm_mutex(nullptr),
  duty_cycle_set_time(0u),
  duty_cycle_mode_write_resolution(8),
  duty_cycle_mode_max_value(255)
{
  pwm_config = {
    .frequency = this->duty_cycle_mode_default_freq,
    .polarity  = PWM_ACTIVE_HIGH,
  };

  for (auto& pwm_pin : pwm_pins) {
    pwm_pin.pin = PIN_NAME_MAX;
    pwm_pin.inst.timer = TIMER0;
    pwm_pin.inst.channel = 0;
    pwm_pin.inst.port = gpioPortA;
    pwm_pin.inst.pin = 0;
    pwm_pin.inst.location = 0;
  }

  this->pwm_mutex = xSemaphoreCreateMutexStatic(&this->pwm_mutex_buf);
  configASSERT(this->pwm_mutex);
}

bool PwmClass::init(PinName pin, int frequency)
{
  uint8_t pwm_channel_idx = get_next_free_pwm_channel_idx();
  if (pwm_channel_idx == UINT8_MAX) {
    // No more free PWM channels available
    return false;
  }

  #ifdef SL_CATALOG_POWER_MANAGER_PRESENT
  // Require at least EM1 to keep the timer peripheral running
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
  #endif // SL_CATALOG_POWER_MANAGER_PRESENT

  this->pwm_pins[pwm_channel_idx].pin = pin;
  this->pwm_pins[pwm_channel_idx].duty_cycle_percent = 101;
  this->pwm_pins[pwm_channel_idx].inst.port = getSilabsPortFromArduinoPin(pin);
  this->pwm_pins[pwm_channel_idx].inst.pin = getSilabsPinFromArduinoPin(pin);
  this->pwm_pins[pwm_channel_idx].inst.channel = pwm_channel_idx;

  GPIO_PinModeSet(this->pwm_pins[pwm_channel_idx].inst.port, this->pwm_pins[pwm_channel_idx].inst.pin, gpioModePushPull, 0);
  pwm_config.frequency = frequency;
  sl_pwm_init(&this->pwm_pins[pwm_channel_idx].inst, &pwm_config);
  sl_pwm_start(&this->pwm_pins[pwm_channel_idx].inst);
  return true;
}

void PwmClass::duty_cycle_mode(PinName pin, int duty_cycle)
{
  if (duty_cycle < 0 || duty_cycle > (int)this->duty_cycle_mode_max_value || pin >= PIN_NAME_MAX) {
    return;
  }

  // If the previous duty cycle setting was within the stabilization time - we wait for the stabilization time to elapse
  // If different channels's duty cycles are set in quick succession they won't take effect - therefore we have to wait in between
  while (millis() < this->duty_cycle_set_time + this->pwm_stabilization_time_ms) {
    yield();
  }

  xSemaphoreTake(this->pwm_mutex, portMAX_DELAY);

  // If the PWM was running in a different mode before - deinitialize it
  if (this->pwm_mode != pwm_mode_t::DUTY_CYCLE) {
    deinit_all_pwm_channels();
    this->pwm_mode = pwm_mode_t::DUTY_CYCLE;
  }

  // Initialize PWM if the pin doesn't have an initialized instance
  if (get_pwm_channel_idx_for_pin(pin) == UINT8_MAX) {
    bool res = this->init(pin, this->duty_cycle_mode_default_freq);
    // Return if PWM could not be initialized
    if (!res) {
      xSemaphoreGive(this->pwm_mutex);
      return;
    }
  }
  // Calculate the duty cycle percent from the provided value
  // Arduino passes the duty cycle as a number from 0 to the configured write resolution's max (255 by default).
  // We need the duty cycle to be expressed as a number from 0-100.
  int duty_cycle_percent = duty_cycle * 100 / this->duty_cycle_mode_max_value;

  uint8_t pwm_channel_idx = get_pwm_channel_idx_for_pin(pin);
  // Don't change anything if the requested duty cycle is the same as the currently set
  if (this->pwm_pins[pwm_channel_idx].duty_cycle_percent == (uint8_t)duty_cycle_percent) {
    xSemaphoreGive(this->pwm_mutex);
    return;
  }
  this->pwm_pins[pwm_channel_idx].duty_cycle_percent = (uint8_t)duty_cycle_percent;

  // Stop the PWM on 0 duty cycle (if auto deinit is enabled), set the requested duty cycle otherwise
  if (duty_cycle_percent == 0 && this->auto_deinit) {
    this->stop(pin);
  } else {
    sl_pwm_set_duty_cycle(&this->pwm_pins[pwm_channel_idx].inst, (uint8_t)duty_cycle_percent);
    this->duty_cycle_set_time = millis();
  }

  xSemaphoreGive(this->pwm_mutex);
}

void PwmClass::frequency_mode(PinName pin, int frequency)
{
  // Frequency mode handles only one channel
  if (pin >= PIN_NAME_MAX) {
    return;
  }
  xSemaphoreTake(this->pwm_mutex, portMAX_DELAY);

  // If the PWM was running in a different mode before - deinitialize it
  if (this->pwm_mode != pwm_mode_t::FREQUENCY) {
    deinit_all_pwm_channels();
  }
  this->pwm_mode = pwm_mode_t::FREQUENCY;
  // Stop waveform generation if the frequency is zero
  if (frequency == 0) {
    this->stop(pin);
    xSemaphoreGive(this->pwm_mutex);
    return;
  }
  // Initialize PWM with the requested frequency
  if (!this->init(pin, frequency)) {
    xSemaphoreGive(this->pwm_mutex);
    return;
  }
  // Set the requested frequency
  // Arduino requires a 50% duty cycle in tone mode
  uint8_t pwm_channel_idx = get_pwm_channel_idx_for_pin(pin);
  sl_pwm_set_duty_cycle(&this->pwm_pins[pwm_channel_idx].inst, 50);

  xSemaphoreGive(this->pwm_mutex);
}

void PwmClass::stop(PinName pin)
{
  uint8_t pwm_channel_idx = this->get_pwm_channel_idx_for_pin(pin);
  if (pwm_channel_idx == UINT8_MAX) {
    return;
  }
  sl_pwm_stop(&this->pwm_pins[pwm_channel_idx].inst);

  // Deinit the PWM peripheral if there are no users left
  if (this->get_num_of_pwm_channels_in_use() == 0) {
    sl_pwm_deinit(&this->pwm_pins[pwm_channel_idx].inst);

    #ifdef SL_CATALOG_POWER_MANAGER_PRESENT
    // Remove the energy mode requirement
    sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
    #endif // SL_CATALOG_POWER_MANAGER_PRESENT
  }

  this->pwm_pins[pwm_channel_idx].pin = PIN_NAME_MAX;
}

void PwmClass::duty_cycle_mode_set_write_resolution(uint8_t resolution)
{
  if (resolution < 1 || resolution > this->duty_cycle_mode_write_resolution_max) {
    return;
  }
  this->duty_cycle_mode_write_resolution = resolution;
  this->duty_cycle_mode_max_value = pow(2, this->duty_cycle_mode_write_resolution) - 1;
}

void PwmClass::set_auto_deinit(bool auto_deinit)
{
  this->auto_deinit = auto_deinit;
}

uint8_t PwmClass::get_next_free_pwm_channel_idx()
{
  for (uint8_t i = 0; i < this->max_pwm_channels; i++) {
    if (this->pwm_pins[i].pin == PIN_NAME_MAX) {
      return i;
    }
  }
  return UINT8_MAX;
}

uint8_t PwmClass::get_pwm_channel_idx_for_pin(PinName pin)
{
  for (uint8_t i = 0; i < this->max_pwm_channels; i++) {
    if (this->pwm_pins[i].pin == pin) {
      return i;
    }
  }
  return UINT8_MAX;
}

uint8_t PwmClass::get_num_of_pwm_channels_in_use()
{
  uint8_t count = 0u;
  for (auto& pwm_pin : this->pwm_pins) {
    if (pwm_pin.pin != PIN_NAME_MAX) {
      count++;
    }
  }
  return count;
}

void PwmClass::deinit_all_pwm_channels()
{
  for (auto& pwm_pin : this->pwm_pins) {
    if (pwm_pin.pin != PIN_NAME_MAX) {
      this->stop(pwm_pin.pin);
    }
  }
}

arduino::PwmClass PWM;
