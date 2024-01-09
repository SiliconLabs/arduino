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

#include "wiring.h"
#include "adc.h"
#include "dac.h"
#include "pwm.h"

extern AdcClass ADC;
extern PwmClass PWM;

uint16_t analogRead(uint8_t pin)
{
  return ADC.get_sample(pin);
}

void analogReference(uint8_t reference)
{
  ADC.set_reference(reference);
}

void analogReferenceDAC(uint8_t reference)
{
  #if (NUM_DAC_HW > 0)
  DAC_0.set_voltage_reference((dac_voltage_ref_t)reference);
  #endif // (NUM_DAC_HW > 0)

  #if (NUM_DAC_HW > 1)
  DAC_1.set_voltage_reference((dac_voltage_ref_t)reference);
  #endif // (NUM_DAC_HW > 1)

  #ifndef NUM_DAC_HW
  (void)reference;
  #endif // NUM_DAC_HW
}

void analogWrite(uint8_t pin, int value)
{
  PWM.duty_cycle_mode(pin, value);
}

void analogWrite(dac_channel_t dac_channel, int value)
{
  // If we have at least one DAC peripheral
  #if (NUM_DAC_HW > 0)

  if (value < 0) {
    return;
  }

  switch (dac_channel) {
    case dac_channel_t::DAC0:
      DAC_0.set_output(0, (uint32_t)value);
      break;

    case dac_channel_t::DAC1:
      DAC_0.set_output(1, (uint32_t)value);
      break;

    #if (NUM_DAC_HW > 1)
    // If we have at least two DAC peripherals
    case dac_channel_t::DAC2:
      DAC_1.set_output(0, (uint32_t)value);
      break;

    case dac_channel_t::DAC3:
      DAC_1.set_output(1, (uint32_t)value);
      break;
    #endif // #if (NUM_DAC_HW > 1)

    default:
      break;
  }

  #else // #if (NUM_DAC_HW > 0)

  // No DAC peripherals
  (void)dac_channel;
  (void)value;

  #endif // #if (NUM_DAC_HW > 0)
}

void analogWriteResolution(int resolution)
{
  PWM.duty_cycle_mode_set_write_resolution(resolution);

  #if (NUM_DAC_HW > 0)
  DAC_0.set_write_resolution((uint8_t)resolution);
  #endif // (NUM_DAC_HW > 0)

  #if (NUM_DAC_HW > 1)
  DAC_1.set_write_resolution((uint8_t)resolution);
  #endif // (NUM_DAC_HW > 1)
}

void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
  PWM.frequency_mode(pin, frequency);
  if (duration == 0) {
    return;
  }
  uint32_t tone_end_time = millis() + duration;
  while (millis() < tone_end_time) {
    yield();
  }
  noTone(pin);
}

void noTone(uint8_t pin)
{
  PWM.frequency_mode(pin, 0);
}
