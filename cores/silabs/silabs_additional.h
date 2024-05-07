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

// Additional Silicon Labs functions

#ifndef SILABS_ADDITIONAL_H
#define SILABS_ADDITIONAL_H

#include "Arduino.h"
#include "em_common.h"

typedef enum {
  CPU_40MHZ,
  CPU_76MHZ,
  CPU_80MHZ
} cpu_clock_t;

/***************************************************************************//**
 * Returns the internal die temperature sensor's measured value in Celsius
 * The sensor is factory calibrated and has an accuracy of +/- 1.5 degrees and
 * it's range is from -40 to 125.
 *
 * @return the MCU die temperature in Celsius
 ******************************************************************************/
float getCPUTemp();

/***************************************************************************//**
 * Issues a system reset
 ******************************************************************************/
void systemReset();

/***************************************************************************//**
 * Returns the unique system ID of the microcontroller
 *
 * @return the device's unique ID
 ******************************************************************************/
uint64_t getDeviceUniqueId();

/***************************************************************************//**
 * Returns the unique system ID of the microcontroller in hexadecimal as a string
 *
 * @return the device's unique ID in hex as a string
 ******************************************************************************/
String getDeviceUniqueIdStr();

/***************************************************************************//**
 * Returns a string containing the current Silicon Labs Arduino Core version
 *
 * @return the current core version as a string
 ******************************************************************************/
String getCoreVersion();

/***************************************************************************//**
 * Sets the CPU clock to the specified frequency
 *
 * @param[in] clock the desired clock speed's identifier in cpu_clock_t
 ******************************************************************************/
void setCPUClock(cpu_clock_t clock);

/***************************************************************************//**
 * Gets the current CPU clock speed in hertz (Hz)
 *
 * @return the current CPU clock speed in hertz (Hz)
 ******************************************************************************/
uint32_t getCPUClock();

void I2C_Deinit(I2C_TypeDef* i2c_peripheral);

#endif // SILABS_ADDITIONAL_H