/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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

#include "arduino_variant.h"
extern "C" {
  #include "sl_system_init.h"
}
#include "HardwareSerial.h"
#include "Wire.h"
#include "SPI.h"

extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern TwoWire Wire;
extern SPIClass SPI;
extern SPIClass SPI1;

void init_arduino_variant()
{
  sl_system_init();
  // Deinit Serial, Wire and SPI by default - sl_system_init() initializes it
  Serial.end();
  Serial1.end();
  Wire.end();
  SPI.end();

  // Reenable the clock for EUSART0 in CMU (cmuClock_EUSART0) for the SPI1 to successfully deinit
  // This clock is turned off in Serial.end() as they share the same EUSART0 peripheral.
  // We turn it back on so that the SPI1 can deinitialize without running to a fault when accessing the EUSART0 registers.
  CMU_ClockEnable(cmuClock_EUSART0, true);
  SPI1.end();
}
