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
  PC3, // D0 - SPI SDO
  PC2, // D1 - SPI SDI
  PC1, // D2 - SPI SCK
  PA7, // D3 - SPI CS
  PA5, // D4 - Tx - WU
  PA6, // D5 - Rx
  PC5, // D6 - SDA - WU
  PB2, // A0 - DAC2
  PB0, // A1 - DAC0
  PB3, // A2 - DAC3
  PD2, // A3 - WU
  PC4, // A4 - SCL
  PD2, // LED R - 12
  PA4, // LED G - 13
  PB0, // LED B - 14
  PB2, // Button - DAC2 - 15
  PB3, // Button - DAC3 - WU - 16
  PC9, // Sensor array power - 17
  PC8, // Microphone power - 18
  PC0, // SPI flash CS - WU - 19
  PD3, // I2S SCK - 20
  PD4, // I2S SD - 21
  PD5, // I2S WS - WU - 22
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
