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

#ifndef _MICROPHONE_PDM_
#define _MICROPHONE_PDM_
#include <Arduino.h>

class MicrophonePDM {
public:
  MicrophonePDM(PinName dinPin, PinName clkPin, PinName pwrPin);
  MicrophonePDM(PinName dinPin, PinName clkPin);
  MicrophonePDM(pin_size_t dinPin, pin_size_t clkPin, pin_size_t pwrPin);
  MicrophonePDM(pin_size_t dinPin, pin_size_t clkPin);
  ~MicrophonePDM();

  bool begin(uint8_t numChannel, uint32_t sampleRate, int16_t *buffer, uint32_t bufferCount);
  void end();

  void enable();
  void disable();

  bool getSamples();
  bool startStreaming(void (*bufferReadyCallback)(const void *, uint32_t));
  bool start();
  bool stop();
  bool sampleBufferReady();
  bool calculateSoundLevel(float &soundLevel, uint8_t channel);

private:
  PinName _dinPin;
  PinName _clkPin;
  PinName _pwrPin;
  uint32_t _sampleRate;
  uint8_t _numChannel;
  uint32_t _numFrames;
  int16_t *_buffer;
};

#endif
