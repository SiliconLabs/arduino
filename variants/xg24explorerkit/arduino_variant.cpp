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
  PC9, // D0
  PC3, // D1 - SPI SDO
  PC2, // D2 - SPI SDI
  PC1, // D3 - SPI SCK
  PC0, // D4 - SPI CS - WU
  PC8, // D5 - SPI1 SS
  PB0, // D6 - SPI1 SCK - DAC0
  PD2, // A0 - WU - 7
  PD3, // A1 - 8
  PB5, // A2 - SDA - 9
  PB4, // A3 - SCL - 10
  PD4, // A4 - SPI1 SDO - Tx1 - 11
  PD5, // A5 - SPI1 SDI - Rx1 - WU - 12
  PB1, // A6 - DAC1 - WU - 13
  PA0, // A7 - SPI1 SCK - 14
  PA4, // LED - 15
  PA7, // LED - 16
  PB2, // Button - DAC2 - 17
  PB3, // Button - DAC3 - WU - 18
  PC4, // SCL1 - 19
  PC5, // SDA1 - WU - 20
  PA6, // Rx - 21
  PA5, // Tx - WU - 22
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
