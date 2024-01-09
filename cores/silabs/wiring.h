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

#ifndef WIRING_H
#define WIRING_H

#include <cmath>
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
  #include "em_common.h"
  #include "em_gpio.h"
  #include "sl_sleeptimer.h"
  #include "sl_udelay.h"
}

typedef enum {
  LOW = 0,
  HIGH = 1
} digital_pin_state;

typedef enum {
  CHANGE = 1,
  FALLING,
  RISING
} pin_state_transition;

typedef enum {
  INPUT = 0,
  OUTPUT = 1,
  INPUT_PULLUP = 2
} pinmode;

typedef enum {
  DAC0,
  DAC1,
  DAC2,
  DAC3
} dac_channel_t;

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

uint32_t millis();
uint32_t micros();
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
void yield();

uint16_t analogRead(uint8_t pin);
void analogReference(uint8_t reference);
void analogWrite(uint8_t pin, int value);
void analogWrite(dac_channel_t dac_channel, int value);
void analogWriteResolution(int resolution);
void tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t pin);

void pinMode(uint8_t pin, uint8_t mode);
int digitalRead(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t state);
uint8_t digitalPinToInterrupt(uint8_t pin);
void attachInterrupt(uint8_t pin, void (*callback)(void), int mode);
void detachInterrupt(uint8_t pin);

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 10000000);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 10000000);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

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
void analogReferenceDAC(uint8_t reference);

typedef enum : uint8_t {
  // Port A
  PA0 = 0,
  PA1,
  PA2,
  PA3,
  PA4,
  PA5,
  PA6,
  PA7,
  PA8,
  PA9,
  PA10,
  PA11,
  PA12,
  PA13,
  PA14,
  PA15,
  // Port B
  PB0 = 16,
  PB1,
  PB2,
  PB3,
  PB4,
  PB5,
  PB6,
  PB7,
  PB8,
  PB9,
  PB10,
  PB11,
  PB12,
  PB13,
  PB14,
  PB15,
  // Port C
  PC0 = 32,
  PC1,
  PC2,
  PC3,
  PC4,
  PC5,
  PC6,
  PC7,
  PC8,
  PC9,
  PC10,
  PC11,
  PC12,
  PC13,
  PC14,
  PC15,
  // Port D
  PD0 = 48,
  PD1,
  PD2,
  PD3,
  PD4,
  PD5,
  PD6,
  PD7,
  PD8,
  PD9,
  PD10,
  PD11,
  PD12,
  PD13,
  PD14,
  PD15,
  PIN_MAX = 64
} hw_pins;

// Hardware pin names with leading zeroes
typedef enum : uint8_t {
  // Port A
  PA00 = 0,
  PA01,
  PA02,
  PA03,
  PA04,
  PA05,
  PA06,
  PA07,
  PA08,
  PA09,
  // Port B
  PB00 = 16,
  PB01,
  PB02,
  PB03,
  PB04,
  PB05,
  PB06,
  PB07,
  PB08,
  PB09,
  // Port C
  PC00 = 32,
  PC01,
  PC02,
  PC03,
  PC04,
  PC05,
  PC06,
  PC07,
  PC08,
  PC09,
  // Port D
  PD00 = 48,
  PD01,
  PD02,
  PD03,
  PD04,
  PD05,
  PD06,
  PD07,
  PD08,
  PD09,
} hw_pins_leading_zeroes;

// Alternate names for pins without the 'P'
typedef enum : uint8_t {
  // Port A
  A0 = 0,
  A1,
  A2,
  A3,
  A4,
  A5,
  A6,
  A7,
  A8,
  A9,
  A10,
  A11,
  A12,
  A13,
  A14,
  A15,
  // Port B
  B0 = 16,
  B1,
  B2,
  B3,
  B4,
  B5,
  B6,
  B7,
  B8,
  B9,
  B10,
  B11,
  B12,
  B13,
  B14,
  B15,
  // Port C
  C0 = 32,
  C1,
  C2,
  C3,
  C4,
  C5,
  C6,
  C7,
  C8,
  C9,
  C10,
  C11,
  C12,
  C13,
  C14,
  C15,
  // Port D
  D0 = 48,
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8,
  D9,
  D10,
  D11,
  D12,
  D13,
  D14,
  D15
} hw_pins_alternative;

// // Alternate names for pins without the 'P' with leading zeroes
typedef enum : uint8_t {
  // Port A
  A00 = 0,
  A01,
  A02,
  A03,
  A04,
  A05,
  A06,
  A07,
  A08,
  A09,
  // Port B
  B00 = 16,
  B01,
  B02,
  B03,
  B04,
  B05,
  B06,
  B07,
  B08,
  B09,
  // Port C
  C00 = 32,
  C01,
  C02,
  C03,
  C04,
  C05,
  C06,
  C07,
  C08,
  C09,
  // Port D
  D00 = 48,
  D01,
  D02,
  D03,
  D04,
  D05,
  D06,
  D07,
  D08,
  D09,
} hw_pins_alternative_leading_zeroes;

#endif // WIRING_H
