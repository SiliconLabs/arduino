/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
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
#define NUM_DIGITAL_PINS  (15u)
#define NUM_ANALOG_INPUTS (15u)

// LEDs
// ----
#define LED_BUILTIN   (15u) // PA4
#define LED_BUILTIN_1 (16u) // PA7
#define PIN_LED       (LED_BUILTIN)
#define LED_BUILTIN_ACTIVE   HIGH
#define LED_BUILTIN_INACTIVE !LED_BUILTIN_ACTIVE

// Buttons
// -------
#define BTN_BUILTIN   (17) // PB2
#define BTN_BUILTIN_1 (18) // PB3

// Analog pins
// -----------
#define PIN_A0 (7u)
#define PIN_A1 (8u)
#define PIN_A2 (9u)
#define PIN_A3 (10u)
#define PIN_A4 (11u)
#define PIN_A5 (12u)
#define PIN_A6 (13u)
#define PIN_A7 (14u)
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

// Serial
// ------
#define SERIAL_HOWMANY 2

#define PIN_SERIAL_RX (21) // PA6
#define PIN_SERIAL_TX (22) // PA5

#define PIN_SERIAL_RX1 (A5) // PD5
#define PIN_SERIAL_TX1 (A4) // PD4

// SPI
// ---
#define SPI_HOWMANY   2

#define PIN_SPI_SS    (D4)
#define PIN_SPI_MOSI  (D1)
#define PIN_SPI_MISO  (D2)
#define PIN_SPI_SCK   (D3)
static const uint8_t SS    = D4; // PC0
static const uint8_t MOSI  = D1; // PC3
static const uint8_t MISO  = D2; // PC2
static const uint8_t SCK   = D3; // PC1

#define PIN_SPI_SS1    (A6)
#define PIN_SPI_MOSI1  (A4)
#define PIN_SPI_MISO1  (A5)
#define PIN_SPI_SCK1   (A7)
static const uint8_t SS1    = A6; // PB1
static const uint8_t MOSI1  = A4; // PD4
static const uint8_t MISO1  = A5; // PD5
static const uint8_t SCK1   = A7; // PA0

// Wire
// ----
#define WIRE_HOWMANY  2

// Wire is on the mikroBUS header
#define PIN_WIRE_SDA  (A2)
#define PIN_WIRE_SCL  (A3)
static const uint8_t SDA = A2; // PB5
static const uint8_t SCL = A3; // PB4

// Wire1 is on the QWIIC connector
#define PIN_WIRE_SDA1  (20)
#define PIN_WIRE_SCL1  (19)
static const uint8_t SDA1 = 20; // PC5
static const uint8_t SCL1 = 19; // PC4

// Deep Sleep escape pin
// ---------------------
#define DEEP_SLEEP_ESCAPE_PIN   BTN_BUILTIN

#endif // PINS_ARDUINO_H
