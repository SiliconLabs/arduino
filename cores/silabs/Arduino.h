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

#ifndef ARDUINO_H
#define ARDUINO_H

#include <cmath>
#include <algorithm>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "arduino_variant.h"

extern "C" {
  #include "sl_component_catalog.h"
  #include "sl_system_init.h"
  #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    #include "sl_power_manager.h"
  #endif // SL_CATALOG_POWER_MANAGER_PRESENT
  #if defined(SL_CATALOG_KERNEL_PRESENT)
    #include "sl_system_kernel.h"
  #else // SL_CATALOG_KERNEL_PRESENT
    #include "sl_system_process_action.h"
  #endif // SL_CATALOG_KERNEL_PRESENT

  #include "em_common.h"
  #include "app_assert.h"
  #include "sl_bluetooth.h"
  #include "em_gpio.h"
  #include "app_log.h"
  #include "psa_crypto_core.h"
  #include "sl_udelay.h"
}

#ifdef ARDUINO_MATTER
#include "efr32_utils.h"
#endif // ARDUINO_MATTER

typedef uint16_t word;
typedef uint8_t byte;
typedef bool boolean;

#include "wiring.h"
#include "wiring_private.h"
#include "pins_arduino.h"
#include "binary.h"
#include "WCharacter.h"
#include "WMath.h"
#include "stdlib_noniso.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "Wire.h"
#include "SPI.h"
#include "adc.h"
#include "pwm.h"
#include "silabs_additional.h"

#ifdef NUM_DAC_HW
#include "dac.h"
#endif // NUM_DAC_HW

using std::round;
using std::isinf;
using std::isnan;
using std::min;
using std::max;
using std::abs;

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define bit(b) (1UL << (b))
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

extern HardwareSerial Serial;
extern TwoWire Wire;
extern SPIClass SPI;
extern AdcClass ADC;
extern PwmClass PWM;

#if (NUM_HW_SERIAL > 1)
extern HardwareSerial Serial1;
#endif // (NUM_HW_SERIAL > 1)

#if (NUM_HW_SPI > 1)
extern SPIClass SPI1;
#endif // (NUM_HW_SPI > 1)

#if (NUM_DAC_HW > 0)
extern DacClass DAC_0;
#endif // (NUM_DAC_HW > 0)

#if (NUM_DAC_HW > 1)
extern DacClass DAC_1;
#endif // (NUM_DAC_HW > 1)

void setup();
void loop();

#endif // ARDUINO_H
