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

// Simple WS2812 driver utilizing the GPIO

#include <Arduino.h>

class ezWS2812gpio {
public:
  ezWS2812gpio(uint32_t num_leds, pin_size_t pin) :
    num_leds(num_leds),
    cpu_clock_hz(0)
  {
    this->led_pin = pinToPinName(pin);
    this->led_port = getSilabsPortFromArduinoPin(this->led_pin);
    uint8_t pin_mapped = this->led_pin - PIN_NAME_MIN;
    this->led_pin_mask = 1 << (pin_mapped % 16);
  }

  ezWS2812gpio(uint32_t num_leds, PinName pin) :
    num_leds(num_leds),
    led_pin(pin),
    cpu_clock_hz(0)
  {
    this->led_port = getSilabsPortFromArduinoPin(this->led_pin);
    uint8_t pin_mapped = this->led_pin - PIN_NAME_MIN;
    this->led_pin_mask = 1 << (pin_mapped % 16);
  }

  void begin()
  {
    sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
    pinMode(this->led_pin, OUTPUT);
    this->cpu_clock_hz = getCPUClock();
  }

  void end()
  {
    sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
  }

  void set_pixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100, bool end_transfer = true)
  {
    for (uint32_t i = 0; i < num; i++) {
      this->set_next_pixel(red, green, blue, brightness);
    }
    if (end_transfer) {
      this->end_transfer();
    }
  }

  void set_all(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100)
  {
    for (uint32_t i = 0; i < this->num_leds; i++) {
      this->set_next_pixel(red, green, blue, brightness);
    }
    this->end_transfer();
  }

  void end_transfer()
  {
    this->direct_write_low();
    delayMicroseconds(100);
  }

private:

  void set_next_pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100)
  {
    // Assign the colors in the output array in GRB order (instead of RGB)
    uint8_t colors[3] = { green, red, blue };

    // If brightness is not 100% we take away from the color intensity of each component proportionally
    if (brightness > 0 && brightness <= 100) {
      for (uint8_t c = 0; c < 3; c++) {
        uint32_t color_adj = (uint32_t)colors[c] * brightness / 100;
        colors[c] = (uint8_t)color_adj;
      }
    } else if (brightness == 0) {
      // If brightness is zero - set all color components to zero
      memset(colors, 0, sizeof(colors));
    }

    // Go through the output array
    for (uint8_t c = 0; c < 3; c++) {
      // Go through the bits of each byte
      for (int i = 8; i > 0; i--) {
        // If the current bit is set send a 1, otherwise send a 0
        uint8_t curr = ((colors[c] >> i) & 1);
        if (curr) {
          if (this->cpu_clock_hz == 39000000) {
            this->one_39mhz();
          } else if (this->cpu_clock_hz == 78000000 || this->cpu_clock_hz == 76800000 || this->cpu_clock_hz == 80000000) {
            this->one_78mhz();
          }
        } else {
          if (this->cpu_clock_hz == 39000000) {
            this->zero_39mhz();
          } else if (this->cpu_clock_hz == 78000000 || this->cpu_clock_hz == 76800000 || this->cpu_clock_hz == 80000000) {
            this->zero_78mhz();
          }
        }
      }
    }
  }

  inline __attribute__((always_inline))
  void one_39mhz()
  {
    // T1H - 700ns
    this->direct_write_high();
    uint32_t cycle_start;
    cycle_start = DWT->CYCCNT;
    this->direct_write_high();
    while ((DWT->CYCCNT - cycle_start) < 15) {
      ;
    }

    // T1L - 600ns
    this->direct_write_low();
    delayMicroseconds(1);
  }

  inline __attribute__((always_inline))
  void zero_39mhz()
  {
    // T0H - 350ns
    this->direct_write_high();
    __NOP();

    // T0L - 800ns
    this->direct_write_low();
    delayMicroseconds(1);
  }

  inline __attribute__((always_inline))
  void one_78mhz()
  {
    // T1H - 700ns
    uint32_t cycle_start;
    cycle_start = DWT->CYCCNT;
    this->direct_write_high();
    while ((DWT->CYCCNT - cycle_start) < 45) {
      ;
    }

    // T1L - 600ns
    this->direct_write_low();
    delayMicroseconds(1);
  }

  inline __attribute__((always_inline))
  void zero_78mhz()
  {
    // T0H - 350ns
    this->direct_write_high();
    __NOP(); __NOP();

    // T0L - 800ns
    this->direct_write_low();
    delayMicroseconds(1);
  }

  inline __attribute__((always_inline))
  void direct_write_low()
  {
    BUS_RegMaskedClear(&GPIO->P[this->led_port].DOUT, this->led_pin_mask);
  }

  inline __attribute__((always_inline))
  void direct_write_high()
  {
    BUS_RegMaskedSet(&GPIO->P[this->led_port].DOUT, this->led_pin_mask);
  }

  uint32_t num_leds;
  PinName led_pin;
  GPIO_Port_TypeDef led_port;
  uint32_t led_pin_mask;
  uint32_t cpu_clock_hz;
};
