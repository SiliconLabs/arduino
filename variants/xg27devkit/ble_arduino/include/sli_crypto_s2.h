/***************************************************************************//**
 * @file
 * @brief Hardware accelerated cryptographic defintions specific to series-2
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
#ifndef SLI_CRYPTO_S2_H
#define SLI_CRYPTO_S2_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Standard buffer size in bytes
#define SLI_CRYPTO_AES_BLOCK_SIZE  16
/// Location value for keys stored in plaintext
#define SLI_CRYPTO_KEY_LOCATION_PLAINTEXT ((sli_crypto_key_location_t)0x00000000UL)
/// The SLI Crypto API supports only the RADIOAES crypto engine on Series-2
#define SLI_CRYPTO_ENGINE_RADIOAES       ((sli_crypto_engine_t)0x00000001UL)
#define SLI_CRYPTO_ENGINE_DEFAULT        (SLI_CRYPTO_ENGINE_RADIOAES)

/// Used to choose a crypto engine.
/// @ref SLI_CRYPTO_LPWAES.
typedef uint32_t sli_crypto_engine_t;

/// Key storage location. Can either
/// @ref SLI_CRYPTO_KEY_LOCATION_PLAINTEXT
typedef uint32_t sli_crypto_key_location_t;

/// Describes where the plaintext key is stored
typedef struct {
  uint8_t* pointer; ///< Pointer to a key buffer.
  uint32_t size;    ///< Size of buffer.
} sli_crypto_key_buffer_t;

/// Describes the plaintext key
typedef struct {
  sli_crypto_key_buffer_t buffer; ///< Key buffer.
  uint32_t key_size;              ///< Key size.
} sli_crypto_plaintext_key_t;

typedef struct {
  /// Key storage location.
  sli_crypto_key_location_t location;
  /// Crypto engine.
  sli_crypto_engine_t engine;
  /// Describes key storage location.
  union {
    sli_crypto_plaintext_key_t plaintext_key;
  } key;
} sli_crypto_descriptor_t;

#ifdef __cplusplus
}
#endif

#endif // SLI_CRYPTO_S2_H
