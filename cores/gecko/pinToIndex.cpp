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
#include "pinDefinitions.h"

PinName pinToPinName(pin_size_t pin)
{
  // If the pin is already in the PinName range - no mapping needed
  if (pin >= PIN_NAME_MIN && pin <= PIN_NAME_MAX) {
    return (PinName)pin;
  }
  // If the pin is in the Arduino pin range - convert to PinName
  if (pin >= PINS_COUNT) {
    return PIN_NAME_NC;
  }
  return gPinNames[pin];
}

pin_size_t digitalPinToInterrupt(pin_size_t pin)
{
  // We don't need to do any mapping, it's done in attachInterrupt
  // This function is only here for compatibility
  return pin;
}

GPIO_Port_TypeDef getSilabsPortFromArduinoPin(PinName pin_name)
{
  if (pin_name >= PD0) {
    return gpioPortD;
  } else if (pin_name >= PC0) {
    return gpioPortC;
  } else if (pin_name >= PB0) {
    return gpioPortB;
  }
  return gpioPortA;
}

uint32_t getSilabsPinFromArduinoPin(PinName pin_name)
{
  return (pin_name - PIN_NAME_MIN) % 16;
}
