/***************************************************************************//**
 * @file bthome_v2_nvm3.h
 * @brief bthome v2 nvm3 header file.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
 * 1. The origin of this software must not be misrepresented{} you must not
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

#ifndef BTHOME_V2_NVM3_H_
#define BTHOME_V2_NVM3_H_

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "nvm3_default.h"
#include "sl_status.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------
#define BTHOME_BLOCK_SIZE               22

/***************************************************************************//**
 * @brief
 *    Typedef database of BThome advertisement data.
 ******************************************************************************/
typedef union {
  uint8_t data[BTHOME_BLOCK_SIZE];
  struct {
    uint8_t mac[6];
    uint8_t key[16];
  } bthome_nvm3;
} bthome_v2_server_data_t;

/***************************************************************************//**
 * @brief
 *    NVM3 read block data.
 *
 * @param[in] index
 *    Object index, start index is 0
 * @param[out] value
 *    Pointer to read value, value size equal to 22
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_read(uint8_t index, uint8_t *value);

/***************************************************************************//**
 * @brief
 *    NVM3 read block data.
 *
 * @param[in] index
 *    Object index, start index is 0
 * @param[in] value
 *    Pointer to write value, value size equal to 22
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_write(uint8_t index, uint8_t *value);

/***************************************************************************//**
 * @brief
 *    Find object index by MAC.
 *
 * @param[in] mac
 *    Device MAC address
 * @param[in] value
 *    Pointer to write value, value size equal to 22
 *
 * @return
 *    Object index or -1 if not found
 ******************************************************************************/
int bthome_v2_server_nvm3_find_index(uint8_t *mac);

/***************************************************************************//**
 * @brief
 *    NVM3 save device encrypt key with MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[in] key
 *    Device encrypt key, 16 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_save_device_key(uint8_t *mac, uint8_t *key);

/***************************************************************************//**
 * @brief
 *    NVM3 find device encrypt key by MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[out] key
 *    Pointer to device encrypt key, 16 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_find_device_key(uint8_t *mac, uint8_t *key);

/***************************************************************************//**
 * @brief
 *    NVM3 remove device encrypt key by MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_nvm3_remove_device_key(uint8_t *mac);

#endif /* BTHOME_V2_NVM3_H_ */

#ifdef __cplusplus
}
#endif
