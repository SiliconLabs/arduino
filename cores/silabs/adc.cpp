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

#include "adc.h"

using namespace arduino;

AdcClass::AdcClass() :
  initialized(false),
  current_adc_pin(PD2),
  current_adc_reference(AR_VDD),
  adc_mutex(nullptr)
{
  this->adc_mutex = xSemaphoreCreateMutexStatic(&this->adc_mutex_buf);
  configASSERT(this->adc_mutex);
}

void AdcClass::init(PinName pin, uint8_t reference)
{
  // Create ADC init structs with default values
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t all_configs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t init_single = IADC_INITSINGLE_DEFAULT;
  IADC_SingleInput_t input = IADC_SINGLEINPUT_DEFAULT;

  // Enable IADC0, GPIO and PRS clock branches
  CMU_ClockEnable(cmuClock_IADC0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PRS, true);

  IADC_CfgReference_t sl_adc_reference;
  uint32_t sl_adc_vref;

  // Set the voltage reference
  switch (reference) {
    case AR_INTERNAL1V2:
      sl_adc_reference = iadcCfgReferenceInt1V2;
      sl_adc_vref = 1200;
      break;

    case AR_EXTERNAL_1V25:
      sl_adc_reference = iadcCfgReferenceExt1V25;
      sl_adc_vref = 1250;
      break;

    case AR_VDD:
      sl_adc_reference = iadcCfgReferenceVddx;
      sl_adc_vref = 3300;
      break;

    case AR_08VDD:
      sl_adc_reference = iadcCfgReferenceVddX0P8Buf;
      sl_adc_vref = 2640;
      break;

    default:
      return;
  }
  all_configs.configs[0].reference = sl_adc_reference;
  all_configs.configs[0].vRef = sl_adc_vref;

  // Reset the ADC
  IADC_reset(IADC0);

  // Only configure the ADC if it is not already running
  if (IADC0->CTRL == _IADC_CTRL_RESETVALUE) {
    IADC_init(IADC0, &init, &all_configs);
  }

  // Assign the input pin
  uint32_t pin_index = pin - PIN_NAME_MIN;
  input.posInput = GPIO_to_ADC_pin_map[pin_index];

  // Initialize the ADC
  IADC_initSingle(IADC0, &init_single, &input);
  IADC_enableInt(IADC0, IADC_IEN_SINGLEDONE);

  // Allocate the analog bus for ADC0 inputs
  // Port C and D are handled together
  // Even and odd pins on the same port have a different register value
  bool pin_is_even = (pin % 2 == 0);
  if (pin >= PD0 || pin >= PC0) {
    if (pin_is_even) {
      GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDEVEN0_ADC0;
    } else {
      GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDODD0_ADC0;
    }
  } else if (pin >= PB0) {
    if (pin_is_even) {
      GPIO->BBUSALLOC |= GPIO_BBUSALLOC_BEVEN0_ADC0;
    } else {
      GPIO->BBUSALLOC |= GPIO_BBUSALLOC_BODD0_ADC0;
    }
  } else {
    if (pin_is_even) {
      GPIO->ABUSALLOC |= GPIO_ABUSALLOC_AEVEN0_ADC0;
    } else {
      GPIO->ABUSALLOC |= GPIO_ABUSALLOC_AODD0_ADC0;
    }
  }

  this->initialized = true;
}

uint16_t AdcClass::get_sample(PinName pin)
{
  xSemaphoreTake(this->adc_mutex, portMAX_DELAY);

  if (!this->initialized || pin != this->current_adc_pin) {
    this->current_adc_pin = pin;
    this->init(this->current_adc_pin, this->current_adc_reference);
  }
  // Clear single done interrupt
  IADC_clearInt(IADC0, IADC_IF_SINGLEDONE);

  // Start conversion and wait for result
  IADC_command(IADC0, iadcCmdStartSingle);
  while (!(IADC_getInt(IADC0) & IADC_IF_SINGLEDONE)) {
    yield();
  }
  uint16_t result = IADC_readSingleData(IADC0);

  xSemaphoreGive(this->adc_mutex);
  return result;
}

void AdcClass::set_reference(uint8_t reference)
{
  if (reference >= AR_MAX || reference == this->current_adc_reference) {
    return;
  }
  xSemaphoreTake(this->adc_mutex, portMAX_DELAY);
  this->current_adc_reference = reference;
  this->init(this->current_adc_pin, this->current_adc_reference);
  xSemaphoreGive(this->adc_mutex);
}

const IADC_PosInput_t AdcClass::GPIO_to_ADC_pin_map[64] = {
  // Port A
  iadcPosInputPortAPin0,
  iadcPosInputPortAPin1,
  iadcPosInputPortAPin2,
  iadcPosInputPortAPin3,
  iadcPosInputPortAPin4,
  iadcPosInputPortAPin5,
  iadcPosInputPortAPin6,
  iadcPosInputPortAPin7,
  iadcPosInputPortAPin8,
  iadcPosInputPortAPin9,
  iadcPosInputPortAPin10,
  iadcPosInputPortAPin11,
  iadcPosInputPortAPin12,
  iadcPosInputPortAPin13,
  iadcPosInputPortAPin14,
  iadcPosInputPortAPin15,
  // Port B
  iadcPosInputPortBPin0,
  iadcPosInputPortBPin1,
  iadcPosInputPortBPin2,
  iadcPosInputPortBPin3,
  iadcPosInputPortBPin4,
  iadcPosInputPortBPin5,
  iadcPosInputPortBPin6,
  iadcPosInputPortBPin7,
  iadcPosInputPortBPin8,
  iadcPosInputPortBPin9,
  iadcPosInputPortBPin10,
  iadcPosInputPortBPin11,
  iadcPosInputPortBPin12,
  iadcPosInputPortBPin13,
  iadcPosInputPortBPin14,
  iadcPosInputPortBPin15,
  // Port C
  iadcPosInputPortCPin0,
  iadcPosInputPortCPin1,
  iadcPosInputPortCPin2,
  iadcPosInputPortCPin3,
  iadcPosInputPortCPin4,
  iadcPosInputPortCPin5,
  iadcPosInputPortCPin6,
  iadcPosInputPortCPin7,
  iadcPosInputPortCPin8,
  iadcPosInputPortCPin9,
  iadcPosInputPortCPin10,
  iadcPosInputPortCPin11,
  iadcPosInputPortCPin12,
  iadcPosInputPortCPin13,
  iadcPosInputPortCPin14,
  iadcPosInputPortCPin15,
  // Port D
  iadcPosInputPortDPin0,
  iadcPosInputPortDPin1,
  iadcPosInputPortDPin2,
  iadcPosInputPortDPin3,
  iadcPosInputPortDPin4,
  iadcPosInputPortDPin5,
  iadcPosInputPortDPin6,
  iadcPosInputPortDPin7,
  iadcPosInputPortDPin8,
  iadcPosInputPortDPin9,
  iadcPosInputPortDPin10,
  iadcPosInputPortDPin11,
  iadcPosInputPortDPin12,
  iadcPosInputPortDPin13,
  iadcPosInputPortDPin14,
  iadcPosInputPortDPin15
};

arduino::AdcClass ADC;
