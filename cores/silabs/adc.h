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

#include "Arduino.h"

#ifndef __ARDUINO_ADC_H
#define __ARDUINO_ADC_H

#include <cmath>
#include <inttypes.h>
#include "em_cmu.h"
#include "em_iadc.h"
#include "FreeRTOS.h"
#include "semphr.h"

enum analog_references {
  AR_INTERNAL1V2 = 0, // Internal 1.2V reference
  AR_EXTERNAL_1V25,   // External 1.25V reference
  AR_VDD,             // VDD (unbuffered to ground)
  AR_08VDD,           // 0.8 * VDD (buffered to ground)
  AR_MAX              // Maximum value
};

namespace arduino {
class AdcClass {
public:
  /***************************************************************************//**
   * Constructor for AdcClass
   ******************************************************************************/
  AdcClass();

  /***************************************************************************//**
   * Performs a single ADC measurement on the provided pin and returns the sample
   *
   * @param[in] pin The pin number of the ADC input
   *
   * @return the measured ADC sample
   ******************************************************************************/
  uint16_t get_sample(PinName pin);

  /***************************************************************************//**
   * Sets the ADC voltage reference
   *
   * @param[in] reference The selected voltage reference from 'analog_references'
   ******************************************************************************/
  void set_reference(uint8_t reference);

private:
  /***************************************************************************//**
   * Initializes the ADC hardware
   *
   * @param[in] pin The pin number of the ADC input
   * @param[in] reference The selected voltage reference from 'analog_references'
   ******************************************************************************/
  void init(PinName pin, uint8_t reference);

  bool initialized;
  PinName current_adc_pin;
  uint8_t current_adc_reference;
  static const IADC_PosInput_t GPIO_to_ADC_pin_map[64];

  SemaphoreHandle_t adc_mutex;
  StaticSemaphore_t adc_mutex_buf;
};
} // namespace arduino

extern arduino::AdcClass ADC;

#endif // __ARDUINO_ADC_H
