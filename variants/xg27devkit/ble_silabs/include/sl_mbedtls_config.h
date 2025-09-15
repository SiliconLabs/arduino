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

#ifndef SL_MBEDTLS_CONFIG_H
#define SL_MBEDTLS_CONFIG_H

// -----------------------------------------------------------------------------
// User exposed config options

// <<< Use Configuration Wizard in Context Menu >>>

// <h> TLS/DTLS configuration

// <o MBEDTLS_SSL_CIPHERSUITES> Complete list of ciphersuites to use, in order of preference.
// <i> Default: MBEDTLS_TLS_ECJPAKE_WITH_AES_128_CCM_8
// <i> Complete list of ciphersuites to use, in order of preference.
// <i> The value of this configuration should be updated for the application needs.
#define MBEDTLS_SSL_CIPHERSUITES         MBEDTLS_TLS_ECJPAKE_WITH_AES_128_CCM_8

// <o SL_MBEDTLS_SSL_IN_CONTENT_LEN> Maximum TLS/DTLS fragment length in bytes (input).
// <i> Default: 768
// <i> The size configured here determines the size of the internal I/O
// <i> buffer used in mbedTLS when receiving data.
#define SL_MBEDTLS_SSL_IN_CONTENT_LEN  768

// <o SL_MBEDTLS_SSL_OUT_CONTENT_LEN> Maximum TLS/DTLS fragment length in bytes (output).
// <i> Default: 768
// <i> The size configured here determines the size of the internal I/O
// <i> buffer used in mbedTLS when sending data.
#define SL_MBEDTLS_SSL_OUT_CONTENT_LEN  768

// <q SL_MBEDTLS_SSL_MAX_FRAGMENT_LENGTH> Enable support for RFC 6066 max_fragment_length extension in SSL.
// <i> Default: 1
// <i> Enable support for RFC 6066 max_fragment_length extension in SSL.
#define SL_MBEDTLS_SSL_MAX_FRAGMENT_LENGTH  1

// <q SL_MBEDTLS_SSL_EXPORT_KEYS> Enable support for exporting key block and master secret.
// <i> Default: 1
// <i> Enable support for exporting key block and master secret.
// <i> This is required for certain users of TLS, e.g. EAP-TLS.
#define SL_MBEDTLS_SSL_EXPORT_KEYS  1

// <q SL_MBEDTLS_KEY_EXCHANGE_PSK_ENABLED> Enable the PSK based ciphersuite modes in SSL / TLS.
// <i> Default: 0
// <i> Enable the PSK based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_PSK_ENABLED  0

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED> Enable the ECDHE-PSK based ciphersuite modes in SSL / TLS.
// <i> Default: 0
// <i> Enable the ECDHE-PSK based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED  0

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED> Enable the ECDHE-ECDSA based ciphersuite modes in SSL / TLS.
// <i> Default: 0
// <i> Enable the ECDHE-ECDSA based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED  0

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED> Enable the ECDHE-RSA based ciphersuite modes in SSL / TLS.
// <i> Default: 0
// <i> Enable the ECDHE-RSA based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED  0

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED> Enable the ECDH-ECDSA based ciphersuite modes in SSL / TLS.
// <i> Default: 0
// <i> Enable the ECDH-ECDSA based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED  0

// <q SL_MBEDTLS_ECP_ENABLE_COMPRESSED_CURVE_PARSING> Enable parsing of the compressed curves.
// <i> Default: 0
// <i> Enable parsing of the compressed curves.
#define SL_MBEDTLS_ECP_ENABLE_COMPRESSED_CURVE_PARSING 0

// <q SL_MBEDTLS_PSA_ASSUME_EXCLUSIVE_BUFFERS> Assume all buffers passed to PSA functions are owned exclusively by the PSA function.
// <i> Default: 0
// <i> This option may be enabled if all buffers passed to any PSA function reside
// <i> in memory that is accessible only to the PSA function during its execution.
// <i> This option MUST be disabled whenever buffer arguments are in memory shared
// <i> with an untrusted party, for example where arguments to PSA calls are passed
// <i> across a trust boundary.
#define SL_MBEDTLS_PSA_ASSUME_EXCLUSIVE_BUFFERS 0

// </h>

// <h> RSA configuration

// <q SL_MBEDTLS_RSA_NO_CRT> Disable use of the Chinese Remainder Theorem for RSA.
// <i> Default: 0
// <i> Disable use of the Chinese Remainder Theorem for RSA private key
// <i> computations.
#define SL_MBEDTLS_RSA_NO_CRT 0

// </h>

// <h> Miscellaneous configuration

// <q SL_MBEDTLS_DRIVERS_ENABLED> Enable Silicon Labs' Mbed TLS- and PSA Crypto drivers.
// <i> Default: 1
// <i> Enable drivers for hardware acceleration (Mbed TLS and PSA Crypto) and
// <i> secure key handling (PSA Crypto).
#define SL_MBEDTLS_DRIVERS_ENABLED 1

// </h>

// <<< end of configuration section >>>

// -----------------------------------------------------------------------------
// Sub-files

#if defined(SLI_MBEDTLS_CONFIG_AUTOGEN_OVERRIDE_FILE)
  #include SLI_MBEDTLS_CONFIG_AUTOGEN_OVERRIDE_FILE
#else
  #include "sli_mbedtls_config_autogen.h"
#endif

#include "sli_mbedtls_omnipresent.h"

#if SL_MBEDTLS_DRIVERS_ENABLED
  #include "sli_mbedtls_acceleration.h"
#endif

#include "sl_mbedtls_device_config.h"

// Include transformation logic to apply CMSIS-config configuration options to
// the correct Mbed TLS / PSA Crypto options.
#include "sli_mbedtls_config_transform_autogen.h"

// Included for backward compatibility reasons.
#include "mbedtls/build_info.h"

#endif // SL_MBEDTLS_CONFIG_H
