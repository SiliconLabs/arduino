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

#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

#include "api/Common.h"
#include <cmath>

extern "C" {
  #include "em_common.h"
  #include "em_gpio.h"
  #include "sl_sleeptimer.h"
  #include "sl_udelay.h"
}

#undef PA0
#undef PA1
#undef PA2
#undef PA3
#undef PA4
#undef PA5
#undef PA6
#undef PA7
#undef PA8
#undef PA9
#undef PA10
#undef PA11
#undef PA12
#undef PA13
#undef PA14
#undef PA15
#undef PB0
#undef PB1
#undef PB2
#undef PB3
#undef PB4
#undef PB5
#undef PB6
#undef PB7
#undef PB8
#undef PB9
#undef PB10
#undef PB11
#undef PB12
#undef PB13
#undef PB14
#undef PB15
#undef PC0
#undef PC1
#undef PC2
#undef PC3
#undef PC4
#undef PC5
#undef PC6
#undef PC7
#undef PC8
#undef PC9
#undef PC10
#undef PC11
#undef PC12
#undef PC13
#undef PC14
#undef PC15
#undef PD0
#undef PD1
#undef PD2
#undef PD3
#undef PD4
#undef PD5
#undef PD6
#undef PD7
#undef PD8
#undef PD9
#undef PD10
#undef PD11
#undef PD12
#undef PD13
#undef PD14
#undef PD15

typedef enum {
  PIN_NAME_MIN = 50,
  // Port A
  PA0 = 50,
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
  PB0 = 66,
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
  PC0 = 82,
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
  PD0 = 98,
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
  PIN_NAME_MAX = 114,
  PIN_NAME_NC = 0xFFFFFFFF
} PinName;

enum _dac_channel_t {
  DAC0,
  DAC1,
  DAC2,
  DAC3
};

PinName pinToPinName(pin_size_t pin);
pin_size_t digitalPinToInterrupt(pin_size_t pin);

GPIO_Port_TypeDef getSilabsPortFromArduinoPin(PinName pin_name);
uint32_t getSilabsPinFromArduinoPin(PinName pin_name);

#endif // PIN_DEFINITIONS_H
