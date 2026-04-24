/***************************************************************************//**
 * @file bthome_v2_nvm3.c
 * @brief BTHome v2 NVM3.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include "sl_status.h"


#include "bthome_v2_server_nvm3.h"
#include "nvm3_generic.h"


#ifndef MAX_ENCRYPT_DEVICE
#define MAX_ENCRYPT_DEVICE    5
#endif

// <o MAX_DEVICE> The maximum number of device that can be managed.
// <i> Default: 10
#ifndef MAX_DEVICE
#define MAX_DEVICE            10
#endif



/***************************************************************************//**
 * Find object index by MAC.
 ******************************************************************************/
int bthome_v2_server_nvm3_find_index(uint8_t *mac)
{
  bthome_v2_server_data_t tmp;
  int index;
  uint32_t type;
  size_t len;

  // mac == NULL then find empty
  if (mac == NULL) {
    for (index = 0; index < MAX_ENCRYPT_DEVICE; index++) {
      nvm3_getObjectInfo(nvm3_defaultHandle, index, &type, &len);
      if (len == 0) {
        return index;
      }
    }
  }

  // mac != NULL then find index
  index = 0;
  while (index < MAX_ENCRYPT_DEVICE) {
    bthome_v2_server_nvm3_read(index, tmp.data);

    if (memcmp(tmp.bthome_nvm3.mac, mac, 6) == 0) {
      return index;
    }
    index++;
  }

  return -1;
}

/***************************************************************************//**
 * Save device encrypt key with MAC address.
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_save_device_key(uint8_t *mac, uint8_t *key)
{
  sl_status_t sc;
  bthome_v2_server_data_t tmp;
  int index;

  if ((mac == NULL) || (key == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  index = bthome_v2_server_nvm3_find_index(mac);
  if (index == -1) {
    index = bthome_v2_server_nvm3_find_index(NULL);
    if (index >= MAX_ENCRYPT_DEVICE) {
      return SL_STATUS_FULL;
    }
  }

  memcpy(tmp.bthome_nvm3.mac, mac, 6);
  memcpy(tmp.bthome_nvm3.key, key, 16);
  sc = bthome_v2_server_nvm3_write(index, tmp.data);

  return sc;
}

/***************************************************************************//**
 * Find device encrypt key by MAC address.
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_find_device_key(uint8_t *mac, uint8_t *key)
{
  sl_status_t sc;
  bthome_v2_server_data_t tmp;
  int index;

  if ((mac == NULL) || (key == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  index = bthome_v2_server_nvm3_find_index(mac);
  if (index == -1) {
    return SL_STATUS_NOT_FOUND;
  }

  sc = bthome_v2_server_nvm3_read(index, tmp.data);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  memcpy(key, tmp.bthome_nvm3.key, 16);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Remove device encrypt key by MAC address.
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_remove_device_key(uint8_t *mac)
{
  sl_status_t sc;
  int index;

  if (mac == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  index = bthome_v2_server_nvm3_find_index(mac);
  if (index == -1) {
    return SL_STATUS_OK;
  }

  sc = nvm3_deleteObject(nvm3_defaultHandle,
                         index);

  return sc;
}

/***************************************************************************//**
 * NVM3 read block data.
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_read(uint8_t index, uint8_t *value)
{
  sl_status_t sc;
  uint32_t type;
  size_t len;

  if (value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // check if the index contain data
  sc = nvm3_getObjectInfo(nvm3_defaultHandle,
                          index,
                          &type,
                          &len);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  if ((type == NVM3_OBJECTTYPE_DATA)
      && (len == BTHOME_BLOCK_SIZE)) {
    return nvm3_readData(nvm3_defaultHandle,
                         index,
                         value,
                         BTHOME_BLOCK_SIZE);
  }

  return SL_STATUS_INVALID_TYPE;
}

/***************************************************************************//**
 * NVM3 write block data.
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_write(uint8_t index, uint8_t *value)
{
  sl_status_t sc;

  if (value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = nvm3_writeData(nvm3_defaultHandle,
                      index,
                      value,
                      BTHOME_BLOCK_SIZE);

  return sc;
}

#ifdef __cplusplus
}
#endif
