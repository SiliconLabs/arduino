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

#ifndef ARDUINO_H
#define ARDUINO_H

#include "api/ArduinoAPI.h"

using namespace arduino;

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

  #if defined(SL_CATALOG_BLUETOOTH_PRESENT)
    #include "sl_bluetooth.h"
  #endif // SL_CATALOG_BLUETOOTH_PRESENT

  #include "em_common.h"
  #include "em_rmu.h"
  #include "app_assert.h"
  #include "em_gpio.h"
  #include "app_log.h"
  #include "psa_crypto_core.h"
  #include "sl_udelay.h"
}

#include "pinDefinitions.h"
#include "wiring_private.h"
#include "pins_arduino.h"
#include "stdlib_noniso.h"
#include "Serial.h"
#include "adc.h"
#include "pwm.h"
#include "silabs_additional.h"

#include "overloads.h"

#ifdef NUM_DAC_HW
#include "dac.h"
#endif // NUM_DAC_HW

using std::round;
using std::isinf;
using std::isnan;
using std::min;
using std::max;
using std::abs;

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

/***************************************************************************//**
 * Sets the DAC voltage reference
 * Possible values:
 *  - DAC_VREF_1V25
 *  - DAC_VREF_2V5,
 *  - DAC_VREF_AVDD,
 *  - DAC_VREF_EXTERNAL_PIN
 *
 * @param[in] reference The selected reference from 'dac_voltage_references'
 ******************************************************************************/
void analogReferenceDAC(uint8_t reference);

typedef enum _dac_channel_t dac_channel_t;
void analogWrite(dac_channel_t dac_channel, int value);
void analogWriteResolution(int resolution);
void analogReadResolution(int resolution);

/***************************************************************************//**
 * Starts continuous ADC sample acquisition using DMA
 *
 * @param[in] pin The selected analog input pin
 * @param[in] buffer Pointer to the sampling buffer
 * @param[in] size The size of the sampling buffer
 * @param[in] user_onsampling_finished_callback Callback that gets called when an
 *            acquisition finishes - pass 'nullptr' to stop sampling
 ******************************************************************************/
void analogReadDMA(PinName pin, uint32_t *buffer, uint32_t size, void (*user_onsampling_finished_callback)());
void analogReadDMA(pin_size_t pin, uint32_t *buffer, uint32_t size, void (*user_onsampling_finished_callback)());

bool get_system_init_finished();
uint32_t get_system_reset_cause();
void escape_hatch();
void gpio_interrupt_handler_init();

#endif // ARDUINO_H
