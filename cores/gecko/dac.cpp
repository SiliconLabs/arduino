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

#include "dac.h"

#ifdef NUM_DAC_HW

#include "arduino_dac_config.h"

using namespace arduino;

DacClass::DacClass(VDAC_TypeDef *vdac_peripheral, PinName ch0_pin, PinName ch1_pin) :
  dac_initialized(false),
  ch0_pin(ch0_pin),
  ch1_pin(ch1_pin),
  ch0_initialized(false),
  ch1_initialized(false),
  ch0_value(0),
  ch1_value(0),
  auto_deinit(true),
  write_resolution(8),
  dac_max_value(255),
  voltage_ref(vdacRef1V25)
{
  this->vdac_peripheral = vdac_peripheral;
}

void DacClass::set_output(uint8_t channel_num, uint32_t value)
{
  if (value > this->dac_max_value || channel_num > 1) {
    return;
  }

  if (value == 0 && this->auto_deinit) {
    this->deinit(channel_num);
    return;
  }

  // Map the value from the current write resolution to 12 bits (true resolution)
  uint32_t value_out = value;
  if (this->write_resolution != this->dac_true_bit_resolution) {
    value_out = map(value, 0, this->dac_max_value, 0, this->dac_true_max_value);
  }

  this->init(channel_num);
  VDAC_ChannelOutputSet(this->vdac_peripheral, channel_num, value_out);

  if (channel_num == 0) {
    this->ch0_value = value_out;
  }
  if (channel_num == 1) {
    this->ch1_value = value_out;
  }
}

void DacClass::init(uint8_t channel_num)
{
  // Initialize the peripheral
  if (!this->dac_initialized) {
    // Use default settings
    VDAC_Init_TypeDef init = VDAC_INIT_DEFAULT;

    // Set the configured voltage reference
    init.reference = this->voltage_ref;

    // Use the HFRCOEM23 to clock the VDAC in order to operate in EM3 mode
    if (this->vdac_peripheral == VDAC0) {
      CMU_ClockSelectSet(cmuClock_VDAC0, cmuSelect_HFRCOEM23);
    } else if (this->vdac_peripheral == VDAC1) {
      CMU_ClockSelectSet(cmuClock_VDAC1, cmuSelect_HFRCOEM23);
    } else {
      return;
    }

    // Enable the HFRCOEM23 and VDAC clocks
    CMU_ClockEnable(cmuClock_HFRCOEM23, true);

    // Enable the VDAC peripheral clock
    if (this->vdac_peripheral == VDAC0) {
      CMU_ClockEnable(cmuClock_VDAC0, true);
    } else if (this->vdac_peripheral == VDAC1) {
      CMU_ClockEnable(cmuClock_VDAC1, true);
    } else {
      return;
    }

    // Calculate the VDAC clock prescaler value resulting in a 1 MHz VDAC clock
    init.prescaler = VDAC_PrescaleCalc(this->vdac_peripheral, this->vdac_max_freq);

    // Clocking is requested on demand
    init.onDemandClk = false;

    // Initialize the VDAC and VDAC channel
    VDAC_Init(this->vdac_peripheral, &init);

    this->dac_initialized = true;
  }

  // Initialize the requested channel
  if (channel_num == 0 && !this->ch0_initialized) {
    this->init_channel(0);
  }
  if (channel_num == 1 && !this->ch1_initialized) {
    this->init_channel(1);
  }
}

void DacClass::init_channel(uint8_t channel_num)
{
  if (channel_num > 1) {
    return;
  }

  // Set the DAC output pin's mode to 'gpioModeWiredOr'
  if (channel_num == 0) {
    GPIO_PinModeSet(getSilabsPortFromArduinoPin(this->ch0_pin), getSilabsPinFromArduinoPin(this->ch0_pin), gpioModeWiredOr, 0);
  }
  if (channel_num == 1) {
    GPIO_PinModeSet(getSilabsPortFromArduinoPin(this->ch1_pin), getSilabsPinFromArduinoPin(this->ch1_pin), gpioModeWiredOr, 0);
  }

  // Use default settings
  VDAC_InitChannel_TypeDef initChannel = VDAC_INITCHANNEL_DEFAULT;

  // Disable High Capacitance Load mode
  initChannel.highCapLoadEnable = false;

  // Use Low Power mode
  initChannel.powerMode = vdacPowerModeLowPower;

  VDAC_InitChannel(this->vdac_peripheral, &initChannel, channel_num);

  // Enable the VDAC
  VDAC_Enable(this->vdac_peripheral, channel_num, true);

  if (channel_num == 0) {
    this->ch0_initialized = true;
  }
  if (channel_num == 1) {
    this->ch1_initialized = true;
  }
}

void DacClass::deinit(uint8_t channel_num)
{
  if (channel_num > 1) {
    return;
  }
  if (channel_num == 0 && !this->ch0_initialized) {
    return;
  }
  if (channel_num == 1 && !this->ch1_initialized) {
    return;
  }

  // Reset the whole hardware - we don't have the means to deinitialize a separate channel
  // The other channel which is still enabled will jump to 0V for a brief moment while it's reinitialized
  VDAC_Reset(this->vdac_peripheral);
  this->dac_initialized = false;

  // Reinitialize the other channel if it's still enabled
  if (channel_num == 0) {
    this->ch0_initialized = false;
    if (this->ch1_initialized) {
      this->ch1_initialized = false;
      this->set_output(1, this->ch1_value);
    }
  }

  if (channel_num == 1) {
    this->ch1_initialized = false;
    if (this->ch0_initialized) {
      this->ch0_initialized = false;
      this->set_output(0, this->ch0_value);
    }
  }
}

void DacClass::set_auto_deinit(bool auto_deinit)
{
  this->auto_deinit = auto_deinit;
}

void DacClass::set_write_resolution(uint8_t resolution)
{
  if (resolution < 1 || resolution > this->dac_true_bit_resolution) {
    return;
  }
  this->write_resolution = resolution;
  this->dac_max_value = pow(2, this->write_resolution) - 1;
}

void DacClass::set_voltage_reference(dac_voltage_ref_t reference)
{
  // Deinitialize both channels
  this->deinit(0);
  this->deinit(1);

  switch (reference) {
    case DAC_VREF_1V25:
      this->voltage_ref = vdacRef1V25;
      break;
    case DAC_VREF_2V5:
      this->voltage_ref = vdacRef2V5;
      break;
    case DAC_VREF_AVDD:
      this->voltage_ref = vdacRefAvdd;
      break;
    case DAC_VREF_EXTERNAL_PIN:
      this->voltage_ref = vdacRefExtPin;
      break;

    default:
      this->voltage_ref = vdacRef1V25;
      break;
  }
}

#if (NUM_DAC_HW > 0)
arduino::DacClass DAC_0(VDAC0, SL_DAC0_CH0_PIN, SL_DAC0_CH1_PIN);
#endif

#if (NUM_DAC_HW > 1)
arduino::DacClass DAC_1(VDAC1, SL_DAC1_CH0_PIN, SL_DAC1_CH1_PIN);
#endif

#endif // NUM_DAC_HW
