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
#include "pinDefinitions.h"

#ifndef __ARDUINO_ADC_H
#define __ARDUINO_ADC_H

#include <cmath>
#include <inttypes.h>
#include "em_cmu.h"
#include "em_iadc.h"
#include "em_ldma.h"
#include "dmadrv.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "sl_status.h"

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

  /***************************************************************************//**
   * Sets the ADC read resolution
   *
   * @param[in] resolution The selected read resolution in bits
   ******************************************************************************/
  void set_read_resolution(uint8_t resolution);

  /***************************************************************************//**
   * Starts ADC in scan (continuous) mode
   *
   * @param[in] buffer The buffer where the sampled data is stored
   * @param[in] size The size of the buffer
   * @param[in] channel The number of the LDMA channel used
   *
   * @return Status of the scan init process
   ******************************************************************************/
  sl_status_t scan_start(PinName pin, uint32_t *buffer, uint32_t size, void (*user_onsampling_finished_callback)());

  /***************************************************************************//**
   * Stops ADC scan
   ******************************************************************************/
  void scan_stop();

  /***************************************************************************//**
   * De-initialize the ADC
   ******************************************************************************/
  void deinit();

  /***************************************************************************//**
   * Callback handler for the DMA transfer
   ******************************************************************************/
  void handle_dma_finished_callback();

  // The maximum read resolution of the ADC
  static const uint8_t max_read_resolution_bits = 12u;

private:
  /***************************************************************************//**
   * Initializes the ADC hardware as single shot
   *
   * @param[in] pin The pin number of the ADC input
   * @param[in] reference The selected voltage reference from 'analog_references'
   ******************************************************************************/
  void init_single(PinName pin, uint8_t reference);

  /***************************************************************************//**
   * Initializes the ADC hardware in scan (continuous) mode
   *
   * @param[in] pin The pin number of the ADC input
   ******************************************************************************/
  void init_scan(PinName pin, uint8_t reference);

  /**************************************************************************//**
   * Initializes the DMA hardware
   *
   * @param[in] buffer Pointer to the array where ADC results will be stored
   * @param[in] size Size of the array
   * @param[in] channel Channel to use for transfer
   *
   * @return Status of the DMA init process
   *****************************************************************************/
  sl_status_t init_dma(uint32_t *buffer, uint32_t size);

  bool initialized_single;
  bool initialized_scan;
  bool paused_transfer;

  PinName current_adc_pin;
  uint8_t current_adc_reference;
  uint8_t current_read_resolution;

  LDMA_Descriptor_t ldma_descriptor;
  unsigned int dma_channel;
  unsigned int dma_sequence_number;

  void (*user_onsampling_finished_callback)(void);

  static const IADC_PosInput_t GPIO_to_ADC_pin_map[64];

  SemaphoreHandle_t adc_mutex;
  StaticSemaphore_t adc_mutex_buf;
};
} // namespace arduino

extern arduino::AdcClass ADC;

#endif // __ARDUINO_ADC_H
