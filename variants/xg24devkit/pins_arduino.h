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
#define NUM_DIGITAL_PINS  (12u)
#define NUM_ANALOG_INPUTS (12u)

// LEDs
// ----
#define LED_BUILTIN   (12u) // PD2
#define LED_BUILTIN_1 (13u) // PA4
#define LED_BUILTIN_2 (14u) // PB0
#define LED_BUILTIN_ACTIVE   LOW
#define LED_BUILTIN_INACTIVE !LED_BUILTIN_ACTIVE

#define PIN_LED     (LED_BUILTIN)
#define LEDR        (LED_BUILTIN)
#define LEDG        (LED_BUILTIN_1)
#define LEDB        (LED_BUILTIN_2)

// Buttons
// -------
#define BTN_BUILTIN   (15) // PB2
#define BTN_BUILTIN_1 (16) // PB3

// Other peripherals
// -----------------
#define PIN_SENSOR_ENABLE (17) // PC9
#define PIN_MIC_ENABLE    (18) // PC8
#define PIN_SPI_FLASH_CS  (19) // PC0

// Analog pins
// -----------
#define PIN_A0 (7u)
#define PIN_A1 (8u)
#define PIN_A2 (9u)
#define PIN_A3 (10u)
#define PIN_A4 (11u)
static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;

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
#define SERIAL_HOWMANY 1
#define PIN_SERIAL_RX  (D5) // PA6
#define PIN_SERIAL_TX  (D4) // PA5

// SPI
// ---
#define SPI_HOWMANY   1
#define PIN_SPI_SS    (D3)
#define PIN_SPI_MOSI  (D0)
#define PIN_SPI_MISO  (D1)
#define PIN_SPI_SCK   (D2)
static const uint8_t SS    = D3; // PA7
static const uint8_t MOSI  = D0; // PC3
static const uint8_t MISO  = D1; // PC2
static const uint8_t SCK   = D2; // PD2

// Wire
// ----
#define WIRE_HOWMANY  1
#define PIN_WIRE_SDA  (D6)
#define PIN_WIRE_SCL  (A4)
static const uint8_t SDA = D6; // PC5
static const uint8_t SCL = A4; // PC4

// Deep Sleep escape pin
// ---------------------
#define DEEP_SLEEP_ESCAPE_PIN   BTN_BUILTIN

#endif // PINS_ARDUINO_H
