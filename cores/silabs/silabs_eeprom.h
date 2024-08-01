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

#include "Arduino.h"

/**
 * @brief Reads a byte from the EEPROM at the specified address.
 *
 * This function reads and returns the byte stored in the EEPROM at the given address.
 *
 * @param addr The address in the EEPROM from which to read the byte.
 * @return The byte read from the EEPROM.
 */
uint8_t eeprom_read_byte(uint32_t addr);

/**
 * @brief Writes a byte to the EEPROM at the specified address.
 *
 * This function writes the given byte to the EEPROM at the specified address.
 *
 * @param addr The address in the EEPROM where the byte will be written.
 * @param value The byte value to be written to the EEPROM.
 */
void eeprom_write_byte(uint32_t addr, uint8_t value);

/**
 * @brief Gets the length of the EEPROM.
 *
 * This function returns the total number of bytes available in the EEPROM.
 *
 * @return The total number of bytes in the EEPROM.
 */
uint16_t eeprom_get_length();
