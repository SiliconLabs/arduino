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

// Simple WS2812 driver utilizing the hardware SPI

#include <Arduino.h>
#include <SPI.h>

class ezWS2812 {
public:
  ezWS2812(uint32_t num_leds, SPIClass& SPI_peripheral = SPI) :
    num_leds(num_leds),
    SPI_peripheral(SPI_peripheral)
  {
    ;
  }

  void begin()
  {
    this->SPI_peripheral.begin();
    SPISettings settings(8000000, MSBFIRST, SPI_MODE1);
    this->SPI_peripheral.beginTransaction(settings);
  }

  void end()
  {
    this->SPI_peripheral.endTransaction();
    this->SPI_peripheral.end();
  }

  void set_pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100)
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
          this->one();
        } else {
          this->zero();
        }
      }
    }
  }

  // Allows individual LEDs to be set to different colors
  void set_pixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100, bool end_transfer = true)
  {
    for (uint32_t i = 0; i < num; i++) {
      this->set_pixel(red, green, blue, brightness);
    }
    if (end_transfer) {
      this->end_transfer();
    }
  }

  void set_all(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 100)
  {
    for (uint32_t i = 0; i < this->num_leds; i++) {
      this->set_pixel(red, green, blue, brightness);
    }
    this->end_transfer();
  }

  void end_transfer()
  {
    this->SPI_peripheral.transfer(0);
    delay(1);
  }

private:
  inline void one()
  {
    this->SPI_peripheral.transfer(0xF8);
  }

  inline void zero()
  {
    this->SPI_peripheral.transfer(0xC0);
  }

  uint32_t num_leds;
  SPIClass& SPI_peripheral;
};
