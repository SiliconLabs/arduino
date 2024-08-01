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

#include "silabs_eeprom.h"

#include "nvm3.h"
#include "nvm3_hal_flash.h"
#include "nvm3_default_config.h"

// The NVM3 key space is divided into several domains, the usable user domain is 0x00000 - 0x0FFFF
#define NVM3_USER_KEY_SPACE_END 0x0FFFFu

#define NVM3_OBJECT_SIZE (NVM3_DEFAULT_MAX_OBJECT_SIZE)
#define NVM3_MAX_SIZE 10240u

uint8_t eeprom_read_byte(uint32_t addr)
{
  if (addr >= NVM3_MAX_SIZE) {
    return 0xFFu;
  }

  uint32_t object_idx = addr / NVM3_OBJECT_SIZE;
  uint32_t data_idx = addr % NVM3_OBJECT_SIZE;

  if (object_idx > NVM3_USER_KEY_SPACE_END) {
    return 0xFFu;
  }

  uint8_t object_cache[NVM3_OBJECT_SIZE];
  Ecode_t status = nvm3_readData(nvm3_defaultHandle, object_idx, (void*)object_cache, NVM3_OBJECT_SIZE);

  if (status != ECODE_NVM3_OK) {
    return 0xFFu;
  }
  return object_cache[data_idx];
}

void eeprom_write_byte(uint32_t addr, uint8_t value)
{
  if (addr >= NVM3_MAX_SIZE) {
    return;
  }

  uint32_t object_idx = addr / NVM3_OBJECT_SIZE;
  uint32_t data_idx = addr % NVM3_OBJECT_SIZE;

  if (object_idx > NVM3_USER_KEY_SPACE_END) {
    return;
  }

  // Read the object from NVM3
  uint8_t object_cache[NVM3_OBJECT_SIZE];
  Ecode_t status = nvm3_readData(nvm3_defaultHandle, object_idx, (void*)object_cache, NVM3_OBJECT_SIZE);
  if (status != ECODE_NVM3_OK && status != ECODE_NVM3_ERR_KEY_NOT_FOUND) {
    return;
  }

  // Initialize the object if it's currently non-existent
  if (status == ECODE_NVM3_ERR_KEY_NOT_FOUND) {
    memset(object_cache, 0xFFu, NVM3_OBJECT_SIZE);
  }

  // Update the requested value
  object_cache[data_idx] = value;

  // Write the object back to NVM3
  nvm3_writeData(nvm3_defaultHandle, object_idx, object_cache, NVM3_OBJECT_SIZE);

  // Do repacking if needed
  if (nvm3_repackNeeded(nvm3_defaultHandle)) {
    nvm3_repack(nvm3_defaultHandle);
  }
}

uint16_t eeprom_get_length()
{
  return NVM3_MAX_SIZE;
}
