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

#include "SilabsMicrophoneAnalog.h"
#include "sl_status.h"
#include "dmadrv.h"
#include "adc.h"

/***************************************************************************//**
 * Constructor for MicrophoneAnalog class
 ******************************************************************************/
MicrophoneAnalog::MicrophoneAnalog(PinName data_pin, PinName enable_pin) :
  data_pin(data_pin),
  enable_pin(enable_pin),
  num_samples(0),
  buffer(nullptr)
{
  ;
}

/***************************************************************************//**
 * Constructor for MicrophoneAnalog class
 ******************************************************************************/
MicrophoneAnalog::MicrophoneAnalog(pin_size_t data_pin, pin_size_t enable_pin) :
  num_samples(0),
  buffer(nullptr)
{
  this->data_pin = pinToPinName(data_pin);
  this->enable_pin = pinToPinName(enable_pin);
}

/***************************************************************************//**
 * Destructor for MicrophoneAnalog class
 ******************************************************************************/
MicrophoneAnalog::~MicrophoneAnalog()
{
  ;
}

/***************************************************************************//**
 * Initializes the microphone
 *
 * @param[in] buffer The data buffer for streaming
 ******************************************************************************/
void MicrophoneAnalog::begin(uint32_t *buffer, uint32_t num_samples)
{
  if (!buffer) {
    return;
  }

  pinMode(data_pin, INPUT);
  if (this->enable_pin != PIN_NAME_NC) {
    pinMode(this->enable_pin, OUTPUT);
    digitalWrite(this->enable_pin, HIGH);
  }
  this->num_samples = num_samples;
  this->buffer = buffer;
}

/***************************************************************************//**
 * Deinitializes the microphone
 ******************************************************************************/
void MicrophoneAnalog::end()
{
  ADC.deinit();

  this->num_samples = 0;
  this->buffer = nullptr;

  if (this->enable_pin != PIN_NAME_NC) {
    digitalWrite(this->enable_pin, LOW);
  }
}

/***************************************************************************//**
 * Starts sample acquisition from the microphone
 ******************************************************************************/
void MicrophoneAnalog::startSampling(void (*user_onsampling_finished_callback)())
{
  if (!user_onsampling_finished_callback) {
    return;
  }
  analogReadDMA(this->data_pin, this->buffer, this->num_samples, user_onsampling_finished_callback);
}

/***************************************************************************//**
 * Stops sampling
 ******************************************************************************/
void MicrophoneAnalog::stopSampling()
{
  ADC.scan_stop();
}

/***************************************************************************//**
 * Gets a single sample from the microphone
 *
 * @return The raw value of the sample
 ******************************************************************************/
uint32_t MicrophoneAnalog::getSingleSample()
{
  return (uint32_t)ADC.get_sample(this->data_pin);
}

/***************************************************************************//**
 * Gets the average value of the provided samples
 *
 * @return The average value of the samples
 ******************************************************************************/
float MicrophoneAnalog::getAverage(uint32_t *buffer, uint32_t buf_size)
{
  if (!buffer) {
    return 0.0f;
  }

  float sum = 0.0f;
  for (uint32_t i = 0u; i < buf_size; i++) {
    sum += buffer[i];
  }
  return sum / buf_size;
}
