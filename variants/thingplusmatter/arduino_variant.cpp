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
  PC7, // D0 - WU
  PA5, // D1 - Tx - WU
  PA6, // D2 - Rx
  PC6, // D3 - SPI SDI
  PC3, // D4 - SPI SDO
  PC2, // D5 - SPI SCK
  PC1, // D6 - SPI SS
  PC0, // D7 - WU
  PD0, // D8
  PD1, // D9
  PD2, // D10 - WU
  PD3, // D11
  PB4, // A0 - SDA
  PB3, // A1 - SCL - DAC3 - WU
  PB2, // A2 - SPI1 SDI - Rx1 - DAC2
  PB1, // A3 - SPI1 SDO - Tx1 - DAC1 - WU
  PB0, // A4 - SPI1 SCK - DAC0
  PA0, // A5 - SPI1 SS
  PA4, // A6
  PC4, // A7
  PC5, // A8 - WU
  PA8, // LED - 21
  PA7, // SD card SPI CS - 22
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
