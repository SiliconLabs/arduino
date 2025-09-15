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

#include "Arduino.h"

#ifndef __ARDUINO_PWM_H
#define __ARDUINO_PWM_H

#include <cmath>
#include <inttypes.h>
#include "pinDefinitions.h"
#include "wiring_private.h"
#include "sl_pwm.h"
#include "em_gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"

extern "C" {
  #include "sl_power_manager.h"
}

namespace arduino {
class PwmClass {
public:
  /**************************************************************************//**
   * Constructor for PwmClass
   *****************************************************************************/
  PwmClass();

  /**************************************************************************//**
   * PWM signal generation in duty cycle mode
   * In this mode the frequency is fixed at a constant value and the duty
   * cycle is variable by the user. Used for 'analogWrite'.
   * Can handle multiple channels.
   *
   * @param[in] pin output pin for the PWM signal
   * @param[in] duty_cycle duty cycle for the PWM signal (0-255)
   *****************************************************************************/
  void duty_cycle_mode(PinName pin, int duty_cycle);

  /**************************************************************************//**
   * PWM signal generation in frequency mode
   * In this mode the duty cycle is fixed at 50% and the frequency
   * is variable by the user. Used for 'tone'.
   * Can only handle one channel at a time.
   *
   * @param[in] pin output pin for the PWM signal
   * @param[in] frequency the desired frequency of the PWM signal
   *****************************************************************************/
  void frequency_mode(PinName pin, int frequency);

  /**************************************************************************//**
   * Stops any ongoing PWM signal generation and output
   *
   * @param[in] pin PWM pin number to deinitialize
   *****************************************************************************/
  void stop(PinName pin);

  /***************************************************************************//**
   * Sets the write resolution in bits.
   * The default is 8 bits, the maximum is 12 bits.
   *
   * @param[in] resolution the requested write resolution in bits
   ******************************************************************************/
  void duty_cycle_mode_set_write_resolution(uint8_t resolution);

  /***************************************************************************//**
   * Turns the automatic deinitialization feature on or off.
   * When it's on the PWM hardware will be deinitialized when 0 duty cycle is
   * requested. If multiple channels are active, then this deinitialization will
   * momentarily interrupt the other channels.
   * When auto deinit is off PWM can still be stopped by calling stop() explicitly.
   * It's on by default. This setting is only relevant in duty cycle mode.
   *
   * @param[in] auto_deinit the requested auto deinit state
   ******************************************************************************/
  void set_auto_deinit(bool auto_deinit);

private:
  /**************************************************************************//**
   * Initializes PWM signal generation
   *
   * @param[in] pin output pin for the PWM signal
   * @param[in] frequency the desired frequency of the PWM signal
   *
   * @return true if the initialization was successful, false otherwise
   *****************************************************************************/
  bool init(PinName pin, int frequency);

  enum pwm_mode_t {
    DUTY_CYCLE,
    FREQUENCY
  };

  sl_pwm_config_t pwm_config;
  pwm_mode_t pwm_mode;
  bool auto_deinit;

  static const int duty_cycle_mode_default_freq = 1000;

  SemaphoreHandle_t pwm_mutex;
  StaticSemaphore_t pwm_mutex_buf;

  static const uint8_t max_pwm_channels = 3u;
  static const uint32_t pwm_stabilization_time_ms = 2u;

  uint32_t duty_cycle_set_time;

  uint8_t duty_cycle_mode_write_resolution;
  uint32_t duty_cycle_mode_max_value;
  static const uint8_t duty_cycle_mode_write_resolution_max = 12u;

  typedef struct {
    PinName pin;
    uint8_t duty_cycle_percent;
    sl_pwm_instance_t inst;
  } pwm_pin_t;

  pwm_pin_t pwm_pins[max_pwm_channels];

  /**************************************************************************//**
   * Provides the next free PWM channel index if available
   *
   * @return the next free PWM channel index - UINT8_MAX if no channels available
   *****************************************************************************/
  uint8_t get_next_free_pwm_channel_idx();

  /**************************************************************************//**
   * Returns the PWM channel index for the provided pin
   *
   * @param[in] pin the pin to get the PWM channel index for
   *
   * @return the index in 'pwm_pins' corresponding to the provided pin,
   *         UINT8_MAX if not found
   *****************************************************************************/
  uint8_t get_pwm_channel_idx_for_pin(PinName pin);

  /**************************************************************************//**
   * Provides the number of PWM channels in use
   *
   * @return the number of PWM channels in use
   *****************************************************************************/
  uint8_t get_num_of_pwm_channels_in_use();

  /**************************************************************************//**
   * Deinitializes all active PWM channels
   *****************************************************************************/
  void deinit_all_pwm_channels();
};
} // namespace arduino

extern arduino::PwmClass PWM;

#endif // __ARDUINO_PWM_H
