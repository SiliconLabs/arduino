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

#include "SilabsMicrophonePDM.h"
#include "dmadrv.h"
#include "sl_mic.h"
#include "sl_status.h"

MicrophonePDM::MicrophonePDM(PinName dinPin, PinName clkPin, PinName pwrPin) :
  _dinPin(dinPin),
  _clkPin(clkPin),
  _pwrPin(pwrPin),
  _sampleRate(44100),
  _numChannel(1),
  _numFrames(0),
  _buffer(NULL)
{
  ;
}

MicrophonePDM::MicrophonePDM(PinName dinPin, PinName clkPin) :
  _dinPin(dinPin),
  _clkPin(clkPin),
  _pwrPin(PIN_NAME_NC),
  _sampleRate(44100),
  _numChannel(1),
  _numFrames(0),
  _buffer(NULL)
{
  ;
}

MicrophonePDM::MicrophonePDM(pin_size_t dinPin, pin_size_t clkPin, pin_size_t pwrPin) :
  _sampleRate(44100),
  _numChannel(1),
  _numFrames(0),
  _buffer(NULL)
{
  _dinPin = pinToPinName(dinPin);
  _clkPin = pinToPinName(clkPin);
  _pwrPin = pinToPinName(pwrPin);
}

MicrophonePDM::MicrophonePDM(pin_size_t dinPin, pin_size_t clkPin) :
  _pwrPin(PIN_NAME_NC),
  _sampleRate(44100),
  _numChannel(1),
  _numFrames(0),
  _buffer(NULL)
{
  _dinPin = pinToPinName(dinPin);
  _clkPin = pinToPinName(clkPin);
}

MicrophonePDM::~MicrophonePDM()
{
  ;
}

/***************************************************************************//**
 * @brief
 *    Initialize the microphone.
 *
 * @param[in] sample_rate
 *    The desired sample rate in Hz
 *
 * @param[in] channels
 *    Number of audio channels (1 or 2)
 *
 * @return
 *    Returns true on success, false otherwise
 ******************************************************************************/
bool MicrophonePDM::begin(uint8_t numChannel,
                          uint32_t sampleRate,
                          int16_t *buffer,
                          uint32_t bufferCount)
{
  if (!buffer || !bufferCount || numChannel == 0) {
    return false;
  }
  pinMode(_dinPin, INPUT);
  pinMode(_clkPin, OUTPUT);
  if (_pwrPin != PIN_NAME_NC) {
    pinMode(_pwrPin, OUTPUT);
    digitalWrite(_pwrPin, 1);
  }
  _numChannel = numChannel;
  _sampleRate = sampleRate;
  _buffer = buffer;
  _numFrames = bufferCount / numChannel;
  if (_numFrames > DMADRV_MAX_XFER_COUNT) {
    // numFrames is too large.
    return false;
  }
  sl_status_t sc = sl_mic_init(getSilabsPortFromArduinoPin(_dinPin),
                               getSilabsPinFromArduinoPin(_dinPin),
                               getSilabsPortFromArduinoPin(_clkPin),
                               getSilabsPinFromArduinoPin(_clkPin),
                               _sampleRate,
                               _numChannel,
                               32);
  if (SL_STATUS_OK != sc) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 * @brief
 *    De-initialize the microphone.
 *
 ******************************************************************************/
void MicrophonePDM::end()
{
  _buffer = NULL;
  _numFrames = 0;
  sl_mic_deinit();
}

/***************************************************************************//**
 * @brief
 *    Enable the microphone.
 *
 ******************************************************************************/
void MicrophonePDM::enable()
{
  if (_pwrPin != PIN_NAME_NC) {
    digitalWrite(_pwrPin, 1);
  }
}

/***************************************************************************//**
 * @brief
 *    Disable the microphone.
 *
 ******************************************************************************/
void MicrophonePDM::disable()
{
  if (_pwrPin != PIN_NAME_NC) {
    digitalWrite(_pwrPin, 0);
  }
}

/***************************************************************************//**
 * @brief
 *    Read samples from the microphone into a sample buffer.
 *
 * @details
 *    This function starts the microphone sampling and stops the sampling after
 *    reading the configured number of samples.
 *
 * @retval true: Success.
 * @retval false:
 *    - Not initialized.
 *    - Sampling is already in progress.
 ******************************************************************************/
bool MicrophonePDM::getSamples()
{
  if (!_buffer || !_numFrames) {
    return false;
  }
  sl_status_t sc = sl_mic_get_n_samples(_buffer, _numFrames);
  if (SL_STATUS_OK != sc) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 * @brief
 *    Read samples from the microphone into a sample buffer continuously.
 *
 * @details
 *    This function starts the microphone sampling and stops only upon calling
 *    @ref sl_mic_stop or @ref sl_mic_deinit. The buffer is used in a "ping-pong"
 *    manner meaning that one half of the buffer is used for sampling while the
 *    other half is being processed.
 *
 * @param[in] callback
 *    Callback is called when n_frames in the sample buffer is ready.
 *
 * @retval true: Success.
 * @retval false:
 *    - Not initialized.
 *    - Sampling is already in progress.
 ******************************************************************************/
bool MicrophonePDM::startStreaming(void (*bufferReadyCallback)(const void *, uint32_t))
{
  if (!_buffer || !_numFrames) {
    return false;
  }
  sl_status_t sc = sl_mic_start_streaming(_buffer, _numFrames, bufferReadyCallback);
  if (SL_STATUS_OK != sc) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 * @brief
 *    Start the microphone.
 *
 * @retval true: Success.
 * @retval false: Not initialized or Microphone is already running.
 ******************************************************************************/
bool MicrophonePDM::start()
{
  sl_status_t sc = sl_mic_start();
  if (SL_STATUS_OK != sc) {
    return false;
  }
  return true;
}

/***************************************************************************//**
 * @brief
 *    Stop the microphone.
 *
 * @retval true: Success.
 * @retval false: Microphone is not running.
 ******************************************************************************/
bool MicrophonePDM::stop()
{
  sl_status_t sc = sl_mic_stop();
  if (SL_STATUS_OK != sc) {
    return false;
  }
  return true;
}

/***************************************************************************//**
 * @brief
 *    Check whether the sample buffer is ready.
 *
 * @retval true: All samples requested from microphone are ready.
 * @retval false: Samples are still being read into the buffer.
 ******************************************************************************/
bool MicrophonePDM::sampleBufferReady()
{
  return sl_mic_sample_buffer_ready();
}

/***************************************************************************//**
 * @brief
 *    Calculate the dBSPL value for a channel from a sample buffer.
 *
 * @param[out] sound_level
 *    The calculated sound level
 *
 * @param[in] channel
 *    The channel to get the sound level for
 *
 * @retval true: Success
 * @retval false: Invalid channel num
 ******************************************************************************/
bool MicrophonePDM::calculateSoundLevel(float &soundLevel, uint8_t channel)
{
  if (!_buffer || !_numFrames) {
    return false;
  }
  sl_status_t sc = sl_mic_calculate_sound_level(&soundLevel, _buffer, _numFrames, channel);
  if (SL_STATUS_OK != sc) {
    return false;
  }

  return true;
}
