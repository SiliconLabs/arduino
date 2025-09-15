/***************************************************************************//**
 * @file
 * @brief Provides hardware accelerated cryptographic primitives.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
 ******************************************************************************/
#ifndef SLI_CRYPTO_H
#define SLI_CRYPTO_H

#include "em_device.h"
#if defined(_SILICON_LABS_32B_SERIES_2)
  #include "sli_crypto_s2.h"
#elif defined(_SILICON_LABS_32B_SERIES_3)
  #include "sli_crypto_s3.h"
#elif
  #error Unsupported device.
#endif
#include "sl_status.h"
#include "sl_code_classification.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief                CCM buffer authenticated decryption optimized for BLE
 *
 * @param key_descriptor AES key descriptor
 * @param data           Input/output buffer of payload data of BLE packet
 * @param length         length of input data
 * @param iv             nonce (initialization vector)
 *                       must be 13 bytes
 * @param header         header of BLE packet (1 byte)
 * @param tag            authentication tag of BLE packet (4 bytes)
 *
 * @return               SL_STATUS_OK if successful and authenticated,
 *                       SL_STATUS_INVALID_SIGNATURE if tag does not match payload,
 *                       relevant status code on other error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_ccm_auth_decrypt_ble(sli_crypto_descriptor_t     *key_descriptor,
                                            unsigned char               *data,
                                            size_t                      length,
                                            const unsigned char         *iv,
                                            unsigned char               header,
                                            unsigned char               *tag);

/***************************************************************************//**
 * @brief                CCM buffer encryption optimized for BLE
 *
 * @param key_descriptor AES key descriptor
 * @param data           Input/output buffer of payload data of BLE packet
 * @param length         length of input data
 * @param iv             nonce (initialization vector)
 *                       must be 13 bytes
 * @param header         header of BLE packet (1 byte)
 * @param tag            buffer where the BLE packet tag (4 bytes) will be written
 *
 * @return               SL_STATUS_OK if successful, relevant status code on error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_ccm_encrypt_and_tag_ble(sli_crypto_descriptor_t     *key_descriptor,
                                               unsigned char               *data,
                                               size_t                      length,
                                               const unsigned char         *iv,
                                               unsigned char               header,
                                               unsigned char               *tag);

/***************************************************************************//**
 * @brief                CCM buffer authenticated decryption optimized for Zigbee
 *
 * @param key_descriptor AES key descriptor
 * @param encrypt        Encrypt operation
 * @param data_in        Input buffer of payload data (decrypt-in-place)
 * @param data_out       output buffer of payload data (decrypt-in-place)
 * @param length         length of input data
 * @param iv             nonce (initialization vector)
 *                       must be 13 bytes
 * @param aad            Input buffer of Additional Authenticated Data
 * @param aad_len        Length of buffer aad
 * @param tag            authentication tag
 * @param tag_len        Length of authentication tag
 *
 * @return               SL_STATUS_OK if successful and authenticated,
 *                       SL_STATUS_INVALID_SIGNATURE if tag does not match payload,
 *                       relevant status code on other error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_ccm_zigbee(sli_crypto_descriptor_t     *key_descriptor,
                                  bool                        encrypt,
                                  const unsigned char         *data_in,
                                  unsigned char               *data_out,
                                  size_t                      length,
                                  const unsigned char         *iv,
                                  const unsigned char         *aad,
                                  size_t                      aad_len,
                                  unsigned char               *tag,
                                  size_t                      tag_len);

/***************************************************************************//**
 * @brief                Process a table of BLE RPA device keys and look for a
 *                       match against the supplied hash
 *
 * @param key_descriptor SLI crypto descriptor. If plaintext keys are used the
 *                       descriptor provides a pointer to an array of AES-128 keys.
 *                       If KSU stored keys are used, the descriptor provides the
 *                       starting key slot ID of the KSU RAM where the IRK list is
 *                       located
 * @param irk_len        Number of IRK to be resolved for the RPA operation
 * @param keymask        Bitmask indicating with key indices in key table are valid
 * @param prand          24-bit BLE nonce to encrypt with each key and match against
 *                       hash
 * @param hash           BLE RPA hash to match against (last 24 bits of AES result)
 * @param irk_index      0-based index of matching key if a match is found,
 *                       -1 for no match or error
 *
 * @return               SL_STATUS_OK if successful, relevant status code on error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_process_rpa(sli_crypto_descriptor_t     *key_descriptor,
                                   size_t                      irk_len,
                                   uint64_t                    keymask,
                                   uint32_t                    prand,
                                   uint32_t                    hash,
                                   int                         *irk_index);

/***************************************************************************//**
 * @brief                AES-CTR block encryption/decryption optimized for radio
 *
 * @param key_descriptor AES key descriptor
 * @param keybits        must be 128 or 256
 * @param input          16-byte input block
 * @param iv_in          16-byte counter/IV starting value
 * @param iv_out         16-byte counter/IV output after block round
 * @param output         16-byte output block
 *
 * @return               SL_STATUS_OK if successful, relevant status code on error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_aes_ctr_radio(sli_crypto_descriptor_t   *key_descriptor,
                                     unsigned int              keybits,
                                     const unsigned char       input[SLI_CRYPTO_AES_BLOCK_SIZE],
                                     const unsigned char       iv_in[SLI_CRYPTO_AES_BLOCK_SIZE],
                                     volatile unsigned char    iv_out[SLI_CRYPTO_AES_BLOCK_SIZE],
                                     volatile unsigned char    output[SLI_CRYPTO_AES_BLOCK_SIZE]);

/***************************************************************************//**
 * @brief                AES-ECB block encryption/decryption optimized for radio
 *
 * @param encrypt        true for encryption, false for decryption
 * @param key_descriptor AES key descriptor
 * @param keybits        must be 128 or 256
 * @param input          16-byte input block
 * @param output         16-byte output block
 *
 * @return               SL_STATUS_OK if successful, relevant status code on error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_aes_ecb_radio(bool                    encrypt,
                                     sli_crypto_descriptor_t *key_descriptor,
                                     unsigned int            keybits,
                                     const unsigned char     input[SLI_CRYPTO_AES_BLOCK_SIZE],
                                     volatile unsigned char  output[SLI_CRYPTO_AES_BLOCK_SIZE]);

/***************************************************************************//**
 * @brief                AES-CMAC calculation optimized for radio
 *
 * @param key_descriptor AES key descriptor
 * @param keybits        Must be 128 or 256
 * @param input          Input buffer containing the message to be signed
 * @param length         Amount of bytes in the input buffer
 * @param output         16-byte output block for calculated CMAC
 *
 * @return               SL_STATUS_OK if successful, relevant status code on error
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLI_CRYPTO, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_crypto_aes_cmac_radio(sli_crypto_descriptor_t *key_descriptor,
                                      unsigned int            keybits,
                                      const unsigned char     *input,
                                      unsigned int            length,
                                      volatile unsigned char  output[SLI_CRYPTO_AES_BLOCK_SIZE]);
// #endif

#ifdef __cplusplus
}
#endif

#endif // SLI_CRYPTO_H
