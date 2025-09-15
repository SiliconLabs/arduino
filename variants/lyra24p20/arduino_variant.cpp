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
  PA4, // D0
  PA5, // D1 - WU
  PC4, // D2 - SPI SDO
  PC5, // D3 - SPI SDI - WU
  PC2, // D4 - SPI SCK
  PC3, // D5 - SPI CS
  PC6, // D6 - SPI1 SS
  PB0, // D7 - SPI1 SCK - DAC0
  PA6, // A0 - 8
  PA8, // A1 - LED - 9
  PD3, // A2 - SDA - 10
  PD2, // A3 - SCL - WU - 11
  PB1, // A4 - SPI1 SDO - Tx1 - DAC1 - WU - 12
  PB2, // A5 - SPI1 SDI - Rx1 - DAC2 - 13
  PB3, // A6 - DAC3 - WU - 14
  PB4, // A7 - SPI1 SCK - 15
  PC7, // Button - WU - 16
  PA0, // Rx - 17
  PA7  // Tx - 18
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
