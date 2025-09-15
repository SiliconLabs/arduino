/***************************************************************************//**
 * @file
 * @brief Silicon Labs PSA Crypto utility functions.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_PSA_CRYPTO_H
#define SL_PSA_CRYPTO_H

#include "psa/crypto.h"

#include "sl_psa_values.h"

#include <stddef.h>

// -----------------------------------------------------------------------------
// Functions

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * \addtogroup sl_psa_key_management
 * \{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Set the location attribute of a key in PSA Crypto according to a given
 *   persistence level, and a preferred location. If the preferred location is
 *   not available, perhaps because the device does not support this location,
 *   the primary local storage (PSA_KEY_LOCATION_LOCAL_STORAGE) will be used.
 *
 * @param[out] attributes
 *   The attribute structure to write to.
 *
 * @param[in] persistence
 *   The persistence level of the key. If this is #PSA_KEY_PERSISTENCE_VOLATILE,
 *   the key will be volatile, and the key identifier attribute is reset to 0.
 *
 * @param[in] preferred_location
 *   The location of the key. Can be \ref SL_PSA_KEY_LOCATION_WRAPPED,
 *   \ref SL_PSA_KEY_LOCATION_BUILTIN, or PSA_KEY_LOCATION_LOCAL_STORAGE.
 ******************************************************************************/
void sl_psa_set_key_lifetime_with_location_preference(
  psa_key_attributes_t *attributes,
  psa_key_persistence_t persistence,
  psa_key_location_t preferred_location);

/***************************************************************************//**
 * @brief
 *   Get the 'most secure' location attribute of a key usable in this
 *   implementation of PSA Crypto.
 *
 * @return
 *   The 'most secure' usable location of a key. In order of preference, the
 *   following values can be returned: \ref SL_PSA_KEY_LOCATION_WRAPPED,
 *   or PSA_KEY_LOCATION_LOCAL_STORAGE.
 ******************************************************************************/
psa_key_location_t sl_psa_get_most_secure_key_location(void);

/** \} (end addtogroup sl_psa_key_management) */

#ifdef __cplusplus
}
#endif

#ifdef DOXYGEN
/***************************************************************************//**
 * \defgroup sl_psa_crypto PSA Crypto Extensions
 *  @brief Silicon Labs specific extensions to the PSA Crypto API
 *  @{
 ******************************************************************************/

/***************************************************************************//**
 * \defgroup sl_psa_key_derivation Key Derivation
 *  @brief Key Derivation extensions to the PSA Crypto API
 *  @{
 ******************************************************************************/

// This function is declared in psa/crypto.h, which currently is not included with
// doxygen. Declared here for visibility on docs.silabs.com.

/** Perform a single-shot key derivation operation and output the resulting key.
 *
 * This function supports HKDF and PBKDF2.
 *
 * \note
 * - PBKDF2-CMAC is not suported on xG21
 * - PBKDF2-CMAC is only KDF supported for xG27
 *
 * This function obtains its secret input from a key object, and any additional
 * inputs such as buffers and integers. The output of this function is a key
 * object containing the output of the selected key derivation function.
 *
 *
 * \param alg                     The key derivation algorithm to compute
 *                                (\c PSA_ALG_XXX value such that
 *                                #PSA_ALG_IS_KEY_DERIVATION(\p alg) is true).
 * \param key_in                  Identifier of the secret key to input to the
 *                                operation. It must allow the usage
 *                                PSA_KEY_USAGE_DERIVE and be of a symmetric
 *                                type.
 * \param[in] info                A context- and application specific
 *                                information string. Only used for HKDF, but
 *                                can be omitted.
 * \param info_length             The length of the provided info in bytes.
 * \param[in] salt                An optional salt value (a non-secret random value).
 *                                Used for both HKDF and PBKDF2. Recommended for
 *                                PBKDF2.
 * \param salt_length             The length of the provided salt in bytes.
 * \param iterations              The number of iterations to use. Maximum
 *                                supported value is 16384. Only used for PBKDF2.
 * \param[in] key_out_attributes  The attributes for the new key output by the
 *                                derivation operation. The key must be of a
 *                                symmetric type.
 * \param[out] key_out            The identifier of the new key output by the
 *                                derivation operation.
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_HANDLE
 * \retval #PSA_ERROR_NOT_PERMITTED
 *         The input key does not have the required usage policy set.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The input- or output key is not of a symmetric type.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The input- or output key is larger than what the SE can handle.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         The requested algorithm is not supported.
 * \retval #PSA_ERROR_HARDWARE_FAILURE
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY
 * \retval #PSA_ERROR_STORAGE_FAILURE
 * \retval #PSA_ERROR_BAD_STATE
 *         The library has not been previously initialized by psa_crypto_init().
 *         It is implementation-dependent whether a failure to initialize
 *         results in this error code.
 */
psa_status_t sl_psa_key_derivation_single_shot(
  psa_algorithm_t alg,
  mbedtls_svc_key_id_t key_in,
  const uint8_t *info,
  size_t info_length,
  const uint8_t *salt,
  size_t salt_length,
  size_t iterations,
  const psa_key_attributes_t *key_out_attributes,
  mbedtls_svc_key_id_t *key_out);

/** @} */ // end defgroup sl_psa_key_derivation
/** @} */ // end defgroup sl_psa_crypto

#endif // DOXYGEN
#endif // SL_PSA_CRYPTO_H
