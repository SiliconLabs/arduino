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

#ifdef ARDUINO_MATTER

 #include "AppTask.h"
using namespace ::chip;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::StartAppTask()
{
  return CHIP_NO_ERROR;
}

#endif // ARDUINO_MATTER

void init_arduino_variant()
{
  ;
}

// Variant pin mapping - maps Arduino pin numbers to Silabs ports/pins
// D0 -> Dmax -> A0 -> Amax -> Other peripherals
PinName gPinNames[] = {
  PA4, // D0 - Tx1 - SPI1 SDO
  PA5, // D1 - Rx1 - SPI1 SDI - WU
  PA3, // D2 - SPI1 SCK
  PC6, // D3 - SPI1 SS
  PC7, // D4 - SDA1 - WU
  PC8, // D5 - SCL1
  PC9, // D6
  PD2, // D7 - WU
  PD3, // D8
  PD4, // D9
  PD5, // D10 - SPI SS
  PA9, // D11 - SPI SDO
  PA8, // D12 - SPI SDI
  PB4, // D13 - SPI SCK
  PB0, // A0 - DAC0
  PB2, // A1 - DAC2
  PB5, // A2
  PC0, // A3 - WU
  PA6, // A4 - SDA
  PA7, // A5 - SCL
  PB1, // A6 - DAC1 - WU
  PB3, // A7 - DAC3 - WU
  PC1, // LED R - 22
  PC2, // LED G - 23
  PC3, // LED B - 24
  PA0, // Button - 25
  PC4, // Serial Tx - 26
  PC5, // Serial Rx - WU - 27
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
