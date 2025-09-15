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

#ifndef __ARDUINO_DAC_H
#define __ARDUINO_DAC_H

#ifdef NUM_DAC_HW

#include "em_cmu.h"
#include "em_vdac.h"

enum dac_voltage_ref_t {
  DAC_VREF_1V25 = 0,          // 1.25V
  DAC_VREF_2V5,               // 2.5V
  DAC_VREF_AVDD,              // Analog VDD
  DAC_VREF_EXTERNAL_PIN       // External VREF pin (PA00 if available)
};

namespace arduino {
class DacClass {
public:
  /***************************************************************************//**
   * Constructor for DacClass
   * Takes a DAC peripheral.
   * Note, that each DAC peripheral has 2 independent channels.
   *
   * @param[in] vdac_peripheral The DAC peripheral to be used
   * @param[in] ch0_pin The output pin for channel 0
   * @param[in] ch1_pin The output pin for channel 1
   ******************************************************************************/
  DacClass(VDAC_TypeDef* vdac_peripheral, PinName ch0_pin, PinName ch1_pin);

  /***************************************************************************//**
   * Sets the specified DAC channel's output to the desired value
   *
   * @param[in] channel_num the DAC channel to be set
   * @param[in] value the value to set the DAC channel to
   ******************************************************************************/
  void set_output(uint8_t channel_num, uint32_t value);

  /***************************************************************************//**
   * Initializes the DAC hardware and the requested channel
   *
   * @param[in] channel_num the DAC channel to be initialized
   ******************************************************************************/
  void init(uint8_t channel_num);

  /***************************************************************************//**
   * Deintializes the requested DAC channel
   *
   * @param[in] channel_num the DAC channel to be deinitialized
   ******************************************************************************/
  void deinit(uint8_t channel_num);

  /***************************************************************************//**
   * Sets whether the DAC channels should automatically deinitialize
   * when a 0 value is written to it. When deinitializing a channel and an
   * other channel is still active - the active channel will also
   * deinitialize momentarily. This is on by default, but it can
   * interfere with certain applications, so it can be turned off.
   * Once turned off the user is responsible for deinitializing the DAC and it's
   * channels by calling deinit().
   *
   * @param[in] auto_deinit sets whether the auto deinit feature is on or off
   ******************************************************************************/
  void set_auto_deinit(bool auto_deinit);

  /***************************************************************************//**
   * Sets the write resolution in bits.
   * The default is 8 bits, the maximum is 12 bits.
   *
   * @param[in] resolution the requested write resolution in bits
   ******************************************************************************/
  void set_write_resolution(uint8_t resolution);

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
  void set_voltage_reference(dac_voltage_ref_t reference);

private:
  /***************************************************************************//**
   * Initializes a specific channel of the DAC hardware
   *
   * @param[in] channel_num the DAC channel to be deinitialized
   ******************************************************************************/
  void init_channel(uint8_t channel_num);

  bool dac_initialized;
  PinName ch0_pin;
  PinName ch1_pin;
  bool ch0_initialized;
  bool ch1_initialized;
  uint32_t ch0_value;
  uint32_t ch1_value;
  bool auto_deinit;
  uint8_t write_resolution;
  VDAC_TypeDef* vdac_peripheral;
  uint32_t dac_max_value;
  VDAC_Ref_TypeDef voltage_ref;

  // VDAC to max frequency (1 MHz)
  static const uint32_t vdac_max_freq = 1000000u;
  // The DAC has a 12 bit resolution - the max accepted value is 4095
  static const uint8_t dac_true_bit_resolution = 12u;
  static const uint32_t dac_true_max_value = 4095u;
};
} // namespace arduino

#if (NUM_DAC_HW > 0)
extern arduino::DacClass DAC_0;
#endif // (NUM_DAC_HW > 0)

#if (NUM_DAC_HW > 1)
extern arduino::DacClass DAC_1;
#endif // (NUM_DAC_HW > 1)

#endif // NUM_DAC_HW
#endif // __ARDUINO_DAC_H
