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

#ifndef SILABS_MICROPHONE_ANALOG
#define SILABS_MICROPHONE_ANALOG

#include <Arduino.h>

class MicrophoneAnalog {
public:
  MicrophoneAnalog(PinName data_pin, PinName enable_pin = PIN_NAME_NC);
  MicrophoneAnalog(pin_size_t data_pin, pin_size_t enable_pin = UINT8_MAX);
  ~MicrophoneAnalog();

  void begin(uint32_t *buffer, uint32_t num_samples);
  void end();

  void startSampling(void (*user_onsampling_finished_callback)());
  void stopSampling();

  float getAverage(uint32_t *buffer, uint32_t buf_size);
  uint32_t getSingleSample();

private:
  PinName data_pin;
  PinName enable_pin;
  uint32_t num_samples;
  uint32_t *buffer;
};

#endif
