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

#include "Arduino.h"
#include "arduino_variant.h"

void init_arduino_variant()
{
  ;
}

// Variant pin mapping - maps Arduino pin numbers to Silabs ports/pins
// D0 -> Dmax -> A0 -> Amax -> Other peripherals
PinName gPinNames[] = {
  PA0, // D0
  PC0, // D1 - SPI SDO - WU
  PC1, // D2 - SPI SDI
  PC2, // D3 - SPI SCK
  PC3, // D4 - SPI CS
  PC6, // D5
  PB0, // D6
  PC7, // A0 - Button - WU - 7
  PA4, // A1 - LED - 8
  PD3, // A2 - SDA
  PD2, // A3 - SCL - WU
  PB1, // A4 - Tx1 - WU - 11
  PB2, // A5 - Rx1 - 12
  PB3, // A6 - WU
  PB4, // A7
  PA6, // Rx - 15
  PA5, // Tx - WU - 16
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
