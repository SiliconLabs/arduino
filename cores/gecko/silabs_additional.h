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

// Additional Silicon Labs functions

#ifndef SILABS_ADDITIONAL_H
#define SILABS_ADDITIONAL_H

#include "Arduino.h"
#include "em_common.h"

typedef enum {
  CPU_39MHZ,
  CPU_76MHZ,
  CPU_78MHZ,
  CPU_80MHZ
} cpu_clock_t;

typedef enum {
  BOARD_UNKNOWN = 0,
  BOARD_ARDUINO_NANO_MATTER,
  BOARD_SPARKFUN_THINGPLUS_MATTER,
  BOARD_SILABS_XG24_EXPLORER_KIT,
  BOARD_SILABS_XG24_DEV_KIT,
  BOARD_SILABS_XG27_DEV_KIT,
  BOARD_SILABS_BGM220_EXPLORER_KIT,
  BOARD_EURIO_LYRA_24P_20_DBM_DEV_KIT,
  BOARD_SEEED_STUDIO_XIAO_MG24
} silabs_board_t;

typedef enum {
  RADIO_STACK_UNKNOWN = 0,
  RADIO_STACK_NONE,
  RADIO_STACK_BLE_SILABS,
  RADIO_STACK_BLE_ARDUINO,
  RADIO_STACK_MATTER
} silabs_radio_stack_t;

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

/***************************************************************************//**
 * Gets the current CPU cycle count
 *
 * The CPU cycle counter is a 32-bit counter that increments every CPU cycle
 * and overflows quite often. Useful for precision timing.
 *
 * @return the current CPU cycle count
 ******************************************************************************/
inline __attribute__((always_inline))
uint32_t getCPUCycleCount()
{
  return DWT->CYCCNT;
}

/***************************************************************************//**
 * Returns the current board / hardware platform the sketch is running on
 *
 * @return the value corresponding to the current board
 ******************************************************************************/
silabs_board_t getCurrentBoardType();

/***************************************************************************//**
 * Returns the radio stack type the sketch was compiled with
 *
 * @return the value corresponding to the current radio stack
 ******************************************************************************/
silabs_radio_stack_t getCurrentRadioStackType();

/***************************************************************************//**
 * Returns whether the board is AI/ML capable
 *
 * Note, that not all radio stacks support AI/ML capabilities even
 * if the hardware does.
 *
 * @return true if the board is AI/ML capable, false otherwise
 ******************************************************************************/
bool isBoardAiMlCapable();

/***************************************************************************//**
 * Returns the total heap (dynamic memory) size in bytes
 *
 * @return the total heap size available in bytes
 ******************************************************************************/
size_t getTotalHeapSize();

/***************************************************************************//**
 * Returns the current free heap (dynamic memory) size in bytes
 *
 * @return the current free heap size in bytes
 ******************************************************************************/
size_t getFreeHeapSize();

/***************************************************************************//**
 * Returns the current used heap (dynamic memory) size in bytes
 *
 * @return the current used heap size in bytes
 ******************************************************************************/
size_t getUsedHeapSize();

/***************************************************************************//**
 * Returns the highest recorded heap (dynamic memory) usage in bytes
 *
 * @return the highest recorded used heap size in bytes
 ******************************************************************************/
size_t getHeapHighWatermark();

/***************************************************************************//**
 * Resets the highest recorded heap (dynamic memory) usage
 ******************************************************************************/
void resetHeapHighWatermark();

/***************************************************************************//**
 * Deinitializes a selected I2C peripheral
 *
 * @param i2c_peripheral Pointer to the I2C peripheral to be deinitialized
 ******************************************************************************/
void I2C_Deinit(I2C_TypeDef* i2c_peripheral);

#endif // SILABS_ADDITIONAL_H
