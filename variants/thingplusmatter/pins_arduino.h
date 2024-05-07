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
#define NUM_DIGITAL_PINS  (21u)
#define NUM_ANALOG_INPUTS (21u)

// LEDs
// ----
#define LED_BUILTIN   (21u) // PA8
#define PIN_LED       (LED_BUILTIN)
#define LED_BUILTIN_ACTIVE   HIGH
#define LED_BUILTIN_INACTIVE !LED_BUILTIN_ACTIVE

// Other peripherals
// -----------------
#define PIN_SD_CARD_CS (22) // PA7

// Analog pins
// -----------
#define PIN_A0 (12u)
#define PIN_A1 (13u)
#define PIN_A2 (14u)
#define PIN_A3 (15u)
#define PIN_A4 (16u)
#define PIN_A5 (17u)
#define PIN_A6 (18u)
#define PIN_A7 (19u)
#define PIN_A8 (20u)
static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t A6  = PIN_A6;
static const uint8_t A7  = PIN_A7;

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

// Serial
// ------
#define SERIAL_HOWMANY 2

#define PIN_SERIAL_RX (D2) // PA6
#define PIN_SERIAL_TX (D1) // PA5

#define PIN_SERIAL_RX1 (A2) // PB2
#define PIN_SERIAL_TX1 (A3) // PB1

// SPI
// ---
#define SPI_HOWMANY   2

#define PIN_SPI_SS    (D6)
#define PIN_SPI_MOSI  (D4)
#define PIN_SPI_MISO  (D3)
#define PIN_SPI_SCK   (D5)
static const uint8_t SS    = D6; // PC1
static const uint8_t MOSI  = D4; // PC3
static const uint8_t MISO  = D3; // PC6
static const uint8_t SCK   = D5; // PC2

#define PIN_SPI_SS1    (A5)
#define PIN_SPI_MOSI1  (A3)
#define PIN_SPI_MISO1  (A2)
#define PIN_SPI_SCK1   (A4)
static const uint8_t SS1    = A5; // PA0
static const uint8_t MOSI1  = A3; // PB1
static const uint8_t MISO1  = A2; // PB2
static const uint8_t SCK1   = A4; // PB0

// Wire
// ----
#define WIRE_HOWMANY  1

#define PIN_WIRE_SDA  (A0)
#define PIN_WIRE_SCL  (A1)
static const uint8_t SDA = A0; // PB4
static const uint8_t SCL = A1; // PB3

#endif // PINS_ARDUINO_H
