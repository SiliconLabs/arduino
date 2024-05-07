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
#define NUM_DIGITAL_PINS  (22u)
#define NUM_ANALOG_INPUTS (22u)

// LEDs
// ----
#define LED_BUILTIN   (22u)
#define LED_BUILTIN_1 (23u)
#define LED_BUILTIN_2 (24u)
#define LED_BUILTIN_ACTIVE   LOW
#define LED_BUILTIN_INACTIVE !LED_BUILTIN_ACTIVE

#define PIN_LED     (LED_BUILTIN)
#define LEDR        (LED_BUILTIN)
#define LEDG        (LED_BUILTIN_1)
#define LEDB        (LED_BUILTIN_2)

// Buttons
// -------
#define BTN_BUILTIN (25)

// Analog pins
// -----------
#define PIN_A0 (14u)
#define PIN_A1 (15u)
#define PIN_A2 (16u)
#define PIN_A3 (17u)
#define PIN_A4 (18u)
#define PIN_A5 (19u)
#define PIN_A6 (20u)
#define PIN_A7 (21u)
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
#define D12 (12u)
#define D13 (13u)

// Serial
// ------
#define SERIAL_HOWMANY 2

// The pins of Serial are not exposed on the header - they're only connected to the board controller
#define PIN_SERIAL_RX (27) // PC5
#define PIN_SERIAL_TX (26) // PC4

#define PIN_SERIAL_RX1 (D1) // PA5
#define PIN_SERIAL_TX1 (D0) // PA4

// SPI
// ---
#define SPI_HOWMANY   2

#define PIN_SPI_SS    (D10)
#define PIN_SPI_MOSI  (D11)
#define PIN_SPI_MISO  (D12)
#define PIN_SPI_SCK   (D13)
static const uint8_t SS    = D10; // PD5
static const uint8_t MOSI  = D11; // PA9
static const uint8_t MISO  = D12; // PA8
static const uint8_t SCK   = D13; // PB4

#define PIN_SPI_SS1    (D3)
#define PIN_SPI_MOSI1  (D0)
#define PIN_SPI_MISO1  (D1)
#define PIN_SPI_SCK1   (D2)
static const uint8_t SS1    = D3; // PC6
static const uint8_t MOSI1  = D0; // PA4
static const uint8_t MISO1  = D1; // PA5
static const uint8_t SCK1   = D2; // PA3

// Wire
// ----
#define WIRE_HOWMANY  2

#define PIN_WIRE_SDA  (A4)
#define PIN_WIRE_SCL  (A5)
static const uint8_t SDA = A4; // PA6
static const uint8_t SCL = A5; // PA7

#define PIN_WIRE_SDA1  (D4)
#define PIN_WIRE_SCL1  (D5)
static const uint8_t SDA1 = D4; // PC7
static const uint8_t SCL1 = D5; // PC8

#endif // PINS_ARDUINO_H
