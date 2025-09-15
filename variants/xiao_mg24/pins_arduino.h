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

#ifndef PINS_ARDUINO_H
#define PINS_ARDUINO_H

#include "Arduino.h"

#define PINS_COUNT        (getPinCount())
#define NUM_DIGITAL_PINS  (19u)
#define NUM_ANALOG_INPUTS (19u)

// LEDs
// ----
#define LED_BUILTIN   (20u)
#define LED_BUILTIN_ACTIVE   LOW
#define LED_BUILTIN_INACTIVE !LED_BUILTIN_ACTIVE

#define PIN_LED     (LED_BUILTIN)

// Digital pins
// ------------
#define D0  (0u)
#define D1  (1u)
#define D2  (2u)
#define D3  (3u)
#define D4  (4u)
#define D5  (5u)
#define D6  (6u)
#define D7  (7u)
#define D8  (8u)
#define D9  (9u)
#define D10 (10u)
#define D11 (11u)
#define D12 (12u)
#define D13 (13u)
#define D14 (14u)
#define D15 (15u)
#define D16 (16u)
#define D17 (17u)
#define D18 (18u)

// Serial
// ------
#define SERIAL_HOWMANY 2

#define PIN_SERIAL_RX (D11) // PA9
#define PIN_SERIAL_TX (D12) // PA8

#define PIN_SERIAL_RX1 (D7) // PC7
#define PIN_SERIAL_TX1 (D6) // PC6

// SPI
// ---
#define SPI_HOWMANY   2

#define PIN_SPI_SS    (D0)
#define PIN_SPI_MOSI  (D10)
#define PIN_SPI_MISO  (D9)
#define PIN_SPI_SCK   (D8)
static const uint8_t SS    = D0; // PC0
static const uint8_t MOSI  = D10; // PA5
static const uint8_t MISO  = D9; // PA4
static const uint8_t SCK   = D8; // PA3

#define PIN_SPI_SS1    (21)
#define PIN_SPI_MOSI1  (D15)
#define PIN_SPI_MISO1  (D16)
#define PIN_SPI_SCK1   (D17)
static const uint8_t SS1    = 21; // PA6
static const uint8_t MOSI1  = D15; // PB0
static const uint8_t MISO1  = D16; // PB1
static const uint8_t SCK1   = D17; // PA0

// Wire
// ----
#define WIRE_HOWMANY  2

#define PIN_WIRE_SDA  (D4)
#define PIN_WIRE_SCL  (D5)
static const uint8_t SDA = D4; // PC4
static const uint8_t SCL = D5; // PC5

#define PIN_WIRE_SDA1  (D14)
#define PIN_WIRE_SCL1  (D13)
static const uint8_t SDA1 = D14; // PB3
static const uint8_t SCL1 = D13; // PB2

// Deep Sleep escape pin
// ---------------------
#define DEEP_SLEEP_ESCAPE_PIN   D1 // PC1

#endif // PINS_ARDUINO_H
