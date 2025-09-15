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

uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder)
{
  PinName pin_name_data = pinToPinName(dataPin);
  PinName pin_name_clock = pinToPinName(clockPin);
  if (pin_name_data == PIN_NAME_NC || pin_name_clock == PIN_NAME_NC) {
    return 0;
  }
  return shiftIn(pin_name_data, pin_name_clock, bitOrder);
}

uint8_t shiftIn(PinName dataPin, PinName clockPin, BitOrder bitOrder)
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i) {
    digitalWrite(clockPin, HIGH);
    if (bitOrder == LSBFIRST) {
      value |= digitalRead(dataPin) << i;
    } else {
      value |= digitalRead(dataPin) << (7 - i);
    }
    digitalWrite(clockPin, LOW);
  }
  return value;
}

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val)
{
  PinName pin_name_data = pinToPinName(dataPin);
  PinName pin_name_clock = pinToPinName(clockPin);
  if (pin_name_data == PIN_NAME_NC || pin_name_clock == PIN_NAME_NC) {
    return;
  }
  shiftOut(pin_name_data, pin_name_clock, bitOrder, val);
}

void shiftOut(PinName dataPin, PinName clockPin, BitOrder bitOrder, uint8_t val)
{
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if (bitOrder == LSBFIRST) {
      digitalWrite(dataPin, val & 1);
      val >>= 1;
    } else {
      digitalWrite(dataPin, (val & 128) != 0);
      val <<= 1;
    }

    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}
