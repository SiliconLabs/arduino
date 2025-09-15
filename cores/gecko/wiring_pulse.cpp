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

inline static bool wait_for_pin_state(PinName pin_name, bool state, unsigned long timeout)
{
  while (digitalRead(pin_name) != state) {
    if (micros() > timeout) {
      return false;
    }
    yield();
  }
  return true;
}

unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  PinName pin_name = pinToPinName(pin);
  if (pin_name == PIN_NAME_NC) {
    return 0;
  }
  return pulseIn(pin_name, state, timeout);
}

unsigned long pulseIn(PinName pin_name, uint8_t state, unsigned long timeout)
{
  if (pin_name >= PIN_NAME_MAX || state > HIGH) {
    return 0;
  }
  unsigned long timing_start;
  unsigned long timing_result;
  unsigned long timeout_end = micros() + timeout;
  // Wait for the pin to change to the requested state
  bool res = wait_for_pin_state(pin_name, state, timeout_end);
  // Start measurement
  timing_start = micros();
  // Return 0 if we timed out
  if (!res) {
    return 0;
  }
  // Wait for the pin to change to the opposite of the requested state
  res = wait_for_pin_state(pin_name, !state, timeout_end);
  // Calculate the measurement result
  timing_result = micros() - timing_start;
  // Return 0 if we timed out
  if (!res) {
    return 0;
  }
  return timing_result;
}

unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  PinName pin_name = pinToPinName(pin);
  if (pin_name == PIN_NAME_NC) {
    return 0;
  }
  return pulseInLong(pin_name, state, timeout);
}

unsigned long pulseInLong(PinName pin_name, uint8_t state, unsigned long timeout)
{
  return pulseIn(pin_name, state, timeout);
}
