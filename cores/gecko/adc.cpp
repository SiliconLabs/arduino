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

static bool dma_transfer_finished_cb(unsigned int channel, unsigned int sequenceNo, void *userParam);

AdcClass::AdcClass() :
  initialized_single(false),
  initialized_scan(false),
  paused_transfer(false),
  current_adc_pin(PD2),
  current_adc_reference(AR_VDD),
  current_read_resolution(this->max_read_resolution_bits),
  user_onsampling_finished_callback(nullptr),
  adc_mutex(nullptr)
{
  this->adc_mutex = xSemaphoreCreateMutexStatic(&this->adc_mutex_buf);
  configASSERT(this->adc_mutex);
}

void AdcClass::init_single(PinName pin, uint8_t reference)
{
  // Set up the ADC pin as an input
  pinMode(pin, INPUT);

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

  this->initialized_scan = false;
  this->initialized_single = true;
}

void AdcClass::init_scan(PinName pin, uint8_t reference)
{
  // Set up the ADC pin as an input
  pinMode(pin, INPUT);

  // Create ADC init structs with default values
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t all_configs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitScan_t init_scan = IADC_INITSCAN_DEFAULT;

  // Scan table structure
  IADC_ScanTable_t scanTable = IADC_SCANTABLE_DEFAULT;

  // Enable IADC0, GPIO and PRS clock branches
  CMU_ClockEnable(cmuClock_IADC0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PRS, true);

  // Shutdown between conversions to reduce current
  init.warmup = iadcWarmupNormal;

  // Set the HFSCLK prescale value here
  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, 20000000, 0);

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

  // Set the voltage reference
  all_configs.configs[0].reference = sl_adc_reference;
  all_configs.configs[0].vRef = sl_adc_vref;
  all_configs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  all_configs.configs[0].analogGain = iadcCfgAnalogGain1x;

  /*
   * CLK_SRC_ADC must be prescaled by some value greater than 1 to
   * derive the intended CLK_ADC frequency.
   * Based on the default 2x oversampling rate (OSRHS)...
   * conversion time = ((4 * OSRHS) + 2) / fCLK_ADC
   * ...which results in a maximum sampling rate of 833 ksps with the
   * 2-clock input multiplexer switching time is included.
   */
  all_configs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,
                                                                  10000000,
                                                                  0,
                                                                  iadcCfgModeNormal,
                                                                  init.srcClkPrescale);

  // Reset the ADC
  IADC_reset(IADC0);

  // Only configure the ADC if it is not already running
  if (IADC0->CTRL == _IADC_CTRL_RESETVALUE) {
    IADC_init(IADC0, &init, &all_configs);
  }

  // Assign the input pin
  uint32_t pin_index = pin - PIN_NAME_MIN;

  // Trigger continuously once scan is started
  init_scan.triggerAction = iadcTriggerActionContinuous;
  // Set the SCANFIFODVL flag when scan FIFO holds 2 entries
  // The interrupt associated with the SCANFIFODVL flag in the IADC_IF register is not used
  init_scan.dataValidLevel = iadcFifoCfgDvl1;
  // Enable DMA wake-up to save the results when the specified FIFO level is hit
  init_scan.fifoDmaWakeup = true;

  scanTable.entries[0].posInput = GPIO_to_ADC_pin_map[pin_index];
  scanTable.entries[0].includeInScan = true;

  // Initialize scan
  IADC_initScan(IADC0, &init_scan, &scanTable);
  IADC_enableInt(IADC0, IADC_IEN_SCANTABLEDONE);

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

  this->initialized_single = false;
  this->initialized_scan = true;
}

sl_status_t AdcClass::init_dma(uint32_t *buffer, uint32_t size)
{
  sl_status_t status;
  if (!this->initialized_scan) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  // Initialize DMA with default parameters
  DMADRV_Init();

  // Allocate DMA channel
  status = DMADRV_AllocateChannel(&this->dma_channel, NULL);
  if (status != ECODE_EMDRV_DMADRV_OK) {
    return SL_STATUS_FAIL;
  }

  // Trigger LDMA transfer on IADC scan completion
  LDMA_TransferCfg_t transferCfg = LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_IADC0_IADC_SCAN);

  /*
   * Set up a linked descriptor to save scan results to the
   * user-specified buffer. By linking the descriptor to itself
   * (the last argument is the relative jump in terms of the number of
   * descriptors), transfers will run continuously.
   */
  #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
  this->ldma_descriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_WORD(&(IADC0->SCANFIFODATA), buffer, size, 0);

  DMADRV_LdmaStartTransfer((int)this->dma_channel, &transferCfg, &this->ldma_descriptor, dma_transfer_finished_cb, NULL);
  return SL_STATUS_OK;
}

uint16_t AdcClass::get_sample(PinName pin)
{
  xSemaphoreTake(this->adc_mutex, portMAX_DELAY);

  if (this->initialized_scan) {
    this->scan_stop();
  }

  if (!this->initialized_single || (pin != this->current_adc_pin)) {
    this->current_adc_pin = pin;
    this->init_single(this->current_adc_pin, this->current_adc_reference);
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

  // Apply the configured read resolution
  result = result >> (this->max_read_resolution_bits - this->current_read_resolution);

  return result;
}

void AdcClass::set_reference(uint8_t reference)
{
  if (reference >= AR_MAX || reference == this->current_adc_reference) {
    return;
  }
  xSemaphoreTake(this->adc_mutex, portMAX_DELAY);
  this->current_adc_reference = reference;
  if (this->initialized_single) {
    this->init_single(this->current_adc_pin, this->current_adc_reference);
  } else if (this->initialized_scan) {
    this->init_scan(this->current_adc_pin, this->current_adc_reference);
  }
  xSemaphoreGive(this->adc_mutex);
}

void AdcClass::set_read_resolution(uint8_t resolution)
{
  if (resolution > this->max_read_resolution_bits) {
    this->current_read_resolution = this->max_read_resolution_bits;
    return;
  }
  this->current_read_resolution = resolution;
}

sl_status_t AdcClass::scan_start(PinName pin, uint32_t *buffer, uint32_t size, void (*user_onsampling_finished_callback)())
{
  sl_status_t status = SL_STATUS_FAIL;
  xSemaphoreTake(this->adc_mutex, portMAX_DELAY);

  if ((!this->initialized_scan && !this->initialized_single) || (pin != this->current_adc_pin)) {
    // Initialize in scan mode
    this->current_adc_pin = pin;
    this->user_onsampling_finished_callback = user_onsampling_finished_callback;
    this->init_scan(this->current_adc_pin, this->current_adc_reference);
    status = this->init_dma(buffer, size);
  } else if (this->initialized_scan && this->paused_transfer) {
    // Resume DMA transfer if paused
    status = DMADRV_ResumeTransfer(this->dma_channel);
    this->paused_transfer = false;
  } else if (this->initialized_single) {
    // Initialize in scan mode if it was initialized in single mode
    this->deinit();
    this->current_adc_pin = pin;
    this->user_onsampling_finished_callback = user_onsampling_finished_callback;
    this->init_scan(this->current_adc_pin, this->current_adc_reference);
    status = this->init_dma(buffer, size);
  } else {
    xSemaphoreGive(this->adc_mutex);
    return status;
  }

  // Start the conversion and wait for results
  IADC_command(IADC0, iadcCmdStartScan);

  xSemaphoreGive(this->adc_mutex);
  return status;
}

void AdcClass::scan_stop()
{
  // Pause sampling
  DMADRV_PauseTransfer(this->dma_channel);
  this->paused_transfer = true;
}

void AdcClass::deinit()
{
  // Stop sampling
  DMADRV_StopTransfer(this->dma_channel);

  // Free resources
  DMADRV_FreeChannel(this->dma_channel);

  // Reset the ADC
  IADC_reset(IADC0);

  this->initialized_scan = false;
  this->initialized_single = false;
  this->current_adc_pin = PIN_NAME_NC;
}

void AdcClass::handle_dma_finished_callback()
{
  if (!this->user_onsampling_finished_callback) {
    return;
  }

  this->user_onsampling_finished_callback();
}

bool dma_transfer_finished_cb(unsigned int channel, unsigned int sequenceNo, void *userParam)
{
  (void)channel;
  (void)sequenceNo;
  (void)userParam;

  ADC.handle_dma_finished_callback();
  return false;
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
