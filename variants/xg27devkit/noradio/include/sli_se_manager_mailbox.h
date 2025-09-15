/***************************************************************************//**
 * @file
 * @brief SE Mailbox API
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
#ifndef SLI_SE_MANAGER_MAILBOX_H
#define SLI_SE_MANAGER_MAILBOX_H

#if defined(__linux__)
#define SLI_SE_MAILBOX_HOST_SYSTEM
#else

#include "em_device.h"

#endif // __linux__

#include "sl_common.h"

#if defined(SLI_SE_MAILBOX_HOST_SYSTEM) || defined(SEMAILBOX_PRESENT) || defined(CRYPTOACC_PRESENT)

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************   DEFINES    ***********************************
 ******************************************************************************/

// -----------------------------------------------------------------------------
// SE status codes

/// Response status codes for the Secure Engine
#define SLI_SE_RESPONSE_MASK                    0x000F0000UL
/// Command executed successfully or signature was successfully validated.
#define SLI_SE_RESPONSE_OK                      0x00000000UL

/// Command was not recognized as a valid command, or is not allowed in the
/// current context.
#define SLI_SE_RESPONSE_INVALID_COMMAND         0x00010000UL
/// User did not provide the required credentials to be allowed to execute the
/// command.
#define SLI_SE_RESPONSE_AUTHORIZATION_ERROR     0x00020000UL
/// Signature validation command (e.g. SE_COMMAND_SIGNATURE_VERIFY) failed to
/// verify the given signature as being correct.
#define SLI_SE_RESPONSE_INVALID_SIGNATURE       0x00030000UL
/// A command started in non-secure mode is trying to access secure memory.
#define SLI_SE_RESPONSE_BUS_ERROR               0x00040000UL
/// Internal error
#define SLI_SE_RESPONSE_INTERNAL_ERROR          0x00050000UL
/// An internal error was raised and the command did not execute.
#define SLI_SE_RESPONSE_CRYPTO_ERROR            0x00060000UL
/// One of the passed parameters is deemed invalid (e.g. out of bounds).
#define SLI_SE_RESPONSE_INVALID_PARAMETER       0x00070000UL
/// Failure while checking the host for secure boot
#define SLI_SE_RESPONSE_SECUREBOOT_ERROR        0x00090000UL
/// Failure during selftest
#define SLI_SE_RESPONSE_SELFTEST_ERROR          0x000A0000UL
/// Feature/item not initialized or not present
#define SLI_SE_RESPONSE_NOT_INITIALIZED         0x000B0000UL
/// Abort status code is given when no operation is attempted.
#define SLI_SE_RESPONSE_ABORT                   0x00FF0000UL
#if defined(CRYPTOACC_PRESENT)
/// Root Code Mailbox is invalid.
#define SLI_SE_RESPONSE_MAILBOX_INVALID         0x00FE0000UL
/// Root Code Mailbox is valid
#define SLI_SE_RESPONSE_MAILBOX_VALID           0xE5ECC0DEUL
#endif // CRYPTOACC_PRESENT

// -----------------------------------------------------------------------------
// SE command words
// Commands are grouped based on availability
#define SLI_SE_COMMAND_CHECK_SE_IMAGE           0x43020000UL
#define SLI_SE_COMMAND_APPLY_SE_IMAGE           0x43030000UL
#define SLI_SE_COMMAND_STATUS_SE_IMAGE          0x43040000UL
#define SLI_SE_COMMAND_CHECK_HOST_IMAGE         0x43050001UL
#define SLI_SE_COMMAND_APPLY_HOST_IMAGE         0x43060001UL
#define SLI_SE_COMMAND_STATUS_HOST_IMAGE        0x43070000UL

#define SLI_SE_COMMAND_READ_OTP                 0xFE040000UL

#define SLI_SE_COMMAND_INIT_OTP                 0xFF000001UL
#define SLI_SE_COMMAND_INIT_PUBKEY              0xFF070001UL
#define SLI_SE_COMMAND_READ_PUBKEY              0xFF080001UL

#define SLI_SE_COMMAND_READ_PUBKEY              0xFF080001UL
#define SLI_SE_COMMAND_READ_OTP                 0xFE040000UL

#define SLI_SE_COMMAND_DBG_LOCK_APPLY           0x430C0000UL

// Commands limited to SE devices
#if defined(SEMAILBOX_PRESENT)
  #define SLI_SE_COMMAND_CREATE_KEY               0x02000000UL
  #define SLI_SE_COMMAND_READPUB_KEY              0x02010000UL

  #define SLI_SE_COMMAND_HASH                     0x03000000UL
  #define SLI_SE_COMMAND_HASHUPDATE               0x03010000UL
  #define SLI_SE_COMMAND_HMAC                     0x03020000UL
#if defined(_SILICON_LABS_32B_SERIES_3)
  #define SLI_SE_COMMAND_HMAC_STREAMING_START     0x03040000UL
  #define SLI_SE_COMMAND_HMAC_STREAMING_UPDATE    0x03050000UL
  #define SLI_SE_COMMAND_HMAC_STREAMING_FINISH    0x03060000UL
#endif // _SILICON_LABS_32B_SERIES_3
  #define SLI_SE_COMMAND_HASHFINISH               0x03030000UL

  #define SLI_SE_COMMAND_AES_ENCRYPT              0x04000000UL
  #define SLI_SE_COMMAND_AES_DECRYPT              0x04010000UL
  #define SLI_SE_COMMAND_AES_GCM_ENCRYPT          0x04020000UL
  #define SLI_SE_COMMAND_AES_GCM_DECRYPT          0x04030000UL
  #define SLI_SE_COMMAND_AES_CMAC                 0x04040000UL
  #define SLI_SE_COMMAND_AES_CCM_ENCRYPT          0x04050000UL
  #define SLI_SE_COMMAND_AES_CCM_DECRYPT          0x04060000UL

  #define SLI_SE_COMMAND_SIGNATURE_SIGN           0x06000000UL
  #define SLI_SE_COMMAND_SIGNATURE_VERIFY         0x06010000UL
  #define SLI_SE_COMMAND_EDDSA_SIGN               0x06020000UL
  #define SLI_SE_COMMAND_EDDSA_VERIFY             0x06030000UL

  #define SLI_SE_COMMAND_TRNG_GET_RANDOM          0x07000000UL

  #define SLI_SE_COMMAND_JPAKE_R1_GENERATE        0x0B000000UL
  #define SLI_SE_COMMAND_JPAKE_R1_VERIFY          0x0B000100UL
  #define SLI_SE_COMMAND_JPAKE_R2_GENERATE        0x0B010000UL
  #define SLI_SE_COMMAND_JPAKE_R2_VERIFY          0x0B010100UL
  #define SLI_SE_COMMAND_JPAKE_GEN_SESSIONKEY     0x0B020000UL

  #define SLI_SE_COMMAND_DH                       0x0E000000UL

  #define SLI_SE_COMMAND_STATUS_SE_VERSION        0x43080000UL
  #define SLI_SE_COMMAND_STATUS_OTP_VERSION       0x43080100UL
  #define SLI_SE_COMMAND_WRITE_USER_DATA          0x43090000UL
  #define SLI_SE_COMMAND_ERASE_USER_DATA          0x430A0000UL
  #define SLI_SE_COMMAND_DBG_LOCK_ENABLE_SECURE   0x430D0000UL
  #define SLI_SE_COMMAND_DBG_LOCK_DISABLE_SECURE  0x430E0000UL
  #define SLI_SE_COMMAND_DEVICE_ERASE             0x430F0000UL
  #define SLI_SE_COMMAND_DEVICE_ERASE_DISABLE     0x43100000UL
  #define SLI_SE_COMMAND_DBG_LOCK_STATUS          0x43110000UL
  #define SLI_SE_COMMAND_DBG_SET_RESTRICTIONS     0x43120000UL
  #define SLI_SE_COMMAND_PROTECTED_REGISTER       0x43210000UL
#if defined(_SILICON_LABS_32B_SERIES_3)
  #define SLI_SE_COMMAND_READ_DEVICE_DATA           0x43300000UL
  #define SLI_SE_COMMAND_GET_ROLLBACK_COUNTER       0x43400000UL
#endif
#if defined(SLI_SE_COMMAND_STATUS_READ_RSTCAUSE_AVAILABLE)
// SLI_SE_COMMAND_STATUS_READ_RSTCAUSE is only available on xG21 devices (series-2-config-1)
  #define SLI_SE_COMMAND_STATUS_READ_RSTCAUSE     0x43220000UL
#endif // SLI_SE_COMMAND_STATUS_READ_RSTCAUSE_AVAILABLE
  #define SLI_SE_COMMAND_READ_USER_CERT_SIZE      0x43FA0000UL
  #define SLI_SE_COMMAND_READ_USER_CERT           0x43FB0000UL

  #if defined(_SILICON_LABS_32B_SERIES_3)
    #define SLI_SE_COMMAND_GET_HOST_UPGRADE_FILE_VERSION 0x44000000UL
    #define SLI_SE_COMMAND_SET_HOST_UPGRADE_FILE_VERSION 0x44010000UL
  #endif // _SILICON_LABS_32B_SERIES_3

  #define SLI_SE_COMMAND_ENTER_ACTIVE_MODE        0x45000000UL
  #define SLI_SE_COMMAND_EXIT_ACTIVE_MODE         0x45010000UL

#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
  #define SLI_SE_COMMAND_ATTEST_PSA_IAT           0x0A030000UL
  #define SLI_SE_COMMAND_ATTEST_CONFIG            0x0A040000UL
#endif // _SILICON_LABS_SECURITY_FEATURE_VAULT)

  #define SLI_SE_COMMAND_GET_CHALLENGE            0xFD000001UL
  #define SLI_SE_COMMAND_ROLL_CHALLENGE           0xFD000101UL
  #define SLI_SE_COMMAND_OPEN_DEBUG               0xFD010001UL

  #define SLI_SE_COMMAND_READ_SERIAL              0xFE000000UL
  #define SLI_SE_COMMAND_GET_STATUS               0xFE010000UL
  #define SLI_SE_COMMAND_READ_PUBKEYBOOT          0xFE020001UL
  #define SLI_SE_COMMAND_SET_UPGRADEFLAG_SE       0xFE030000UL
  #define SLI_SE_COMMAND_SET_UPGRADEFLAG_HOST     0xFE030001UL
  #define SLI_SE_COMMAND_READ_TAMPER_RESET_CAUSE  0xFE050000UL
#if defined(_SILICON_LABS_32B_SERIES_3)
  #define SLI_SE_COMMAND_READ_TRACE_FLAGS     0xFE060000UL
#endif
  #define SLI_SE_COMMAND_INIT_PUBKEY_SIGNATURE    0xFF090001UL
  #define SLI_SE_COMMAND_READ_PUBKEY_SIGNATURE    0xFF0A0001UL
  #define SLI_SE_COMMAND_INIT_AES_128_KEY         0xFF0B0001UL
  #if defined(_SILICON_LABS_32B_SERIES_3)
    #define SLI_SE_COMMAND_CONFIGURE_QSPI_REF_CLOCK       0xFF150000UL
    #define SLI_SE_COMMAND_CONFIGURE_QSPI_REGS            0xFF160000UL
    #define SLI_SE_COMMAND_GET_QSPI_FLPLL_CONFIG          0xFF170000UL
    #define SLI_SE_COMMAND_APPLY_CODE_REGION_CONFIG       0xFF500000UL
    #define SLI_SE_COMMAND_CLOSE_CODE_REGION              0xFF510000UL
    #define SLI_SE_COMMAND_ERASE_CODE_REGION              0xFF520000UL
    #define SLI_SE_COMMAND_GET_CODE_REGION_CONFIG         0xFF530000UL
    #define SLI_SE_COMMAND_GET_CODE_REGION_VERSION        0xFF540000UL
    #define SLI_SE_COMMAND_SET_ACTIVE_BANKED_CODE_REGION  0xFF550000UL
    #define SLI_SE_COMMAND_WRITE_CODE_REGION              0xFF560000UL
    #define SLI_SE_COMMAND_ERASE_DATA_REGION              0xFF620000UL
    #define SLI_SE_COMMAND_WRITE_DATA_REGION              0xFF630000UL
    #define SLI_SE_COMMAND_GET_DATA_REGION_LOCATION       0xFF640000UL
  #endif
#endif // SLI_MAILBOX_COMMAND_SUPPORTED

// Commands limited to SE Vault High devices
#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
  #define SLI_SE_COMMAND_WRAP_KEY                 0x01000000UL
  #define SLI_SE_COMMAND_UNWRAP_KEY               0x01020000UL
  #define SLI_SE_COMMAND_DELETE_KEY               0x01050000UL
  #define SLI_SE_COMMAND_TRANSFER_KEY             0x01060000UL

  #define SLI_SE_COMMAND_DERIVE_KEY_PBKDF2_HMAC   0x02020002UL
  #define SLI_SE_COMMAND_DERIVE_KEY_HKDF          0x02020003UL
  #define SLI_SE_COMMAND_DERIVE_KEY_PBKDF2_CMAC   0x02020010UL

  #define SLI_SE_COMMAND_CHACHAPOLY_ENCRYPT       0x0C000000UL
  #define SLI_SE_COMMAND_CHACHAPOLY_DECRYPT       0x0C010000UL
  #define SLI_SE_COMMAND_CHACHA20_ENCRYPT         0x0C020000UL
  #define SLI_SE_COMMAND_CHACHA20_DECRYPT         0x0C030000UL
  #define SLI_SE_COMMAND_POLY1305_KEY_MAC         0x0C040000UL

  #define SLI_SE_COMMAND_DISABLE_TAMPER           0xFD020001UL
#endif // _SILICON_LABS_SECURITY_FEATURE_VAULT

// -----------------------------------------------------------------------------
// SE command options
// Commands are grouped based on availability

/// Secure boot pubkey
#define SLI_SE_KEY_TYPE_BOOT                    0x00000100UL
/// Secure authorization (debug) pubkey
#define SLI_SE_KEY_TYPE_AUTH                    0x00000200UL

// Options limited to SE devices
#if defined(SEMAILBOX_PRESENT)
/// Root pubkey
  #define SLI_SE_KEY_TYPE_ROOT                    0x00000300UL
#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
/// Attestation pubkey
  #define SLI_SE_KEY_TYPE_ATTEST                0x00000400UL
#endif // _SILICON_LABS_SECURITY_FEATURE_VAULT
/// BGL encryption key
  #define SLI_SE_IMMUTABLE_KEY_TYPE_AES_128       0x00000500UL

/// Use MD5 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_MD5          0x00000100UL
/// Use SHA1 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_SHA1         0x00000200UL
/// Use SHA224 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_SHA224       0x00000300UL
/// Use SHA256 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_SHA256       0x00000400UL
#if defined(_SILICON_LABS_32B_SERIES_3)
/// Use SHA1 as hash algorithm for HMAC streaming operation
  #define SLI_SE_COMMAND_OPTION_HMAC_HASH_SHA1      0x00000700UL
/// Use SHA224 as hash algorithm for HMAC streaming operation
  #define SLI_SE_COMMAND_OPTION_HMAC_HASH_SHA224    0x00000800UL
/// Use SHA256 as hash algorithm for HMAC streaming operation
  #define SLI_SE_COMMAND_OPTION_HMAC_HASH_SHA256    0x00000900UL
#endif // _SILICON_LABS_32B_SERIES_3

/// Execute algorithm in ECB mode
  #define SLI_SE_COMMAND_OPTION_MODE_ECB          0x00000100UL
/// Execute algorithm in CBC mode
  #define SLI_SE_COMMAND_OPTION_MODE_CBC          0x00000200UL
/// Execute algorithm in CTR mode
  #define SLI_SE_COMMAND_OPTION_MODE_CTR          0x00000300UL
/// Execute algorithm in CFB mode
  #define SLI_SE_COMMAND_OPTION_MODE_CFB          0x00000400UL

/// Run the whole algorithm, all data present
  #define SLI_SE_COMMAND_OPTION_CONTEXT_WHOLE     0x00000000UL
/// Start the algorithm, but get a context to later add more data
  #define SLI_SE_COMMAND_OPTION_CONTEXT_START     0x00000001UL
/// End the algorithm, get the result
  #define SLI_SE_COMMAND_OPTION_CONTEXT_END       0x00000002UL
/// Add more data input to the algorithm. Need to supply previous context,
/// and get a context back
  #define SLI_SE_COMMAND_OPTION_CONTEXT_ADD       0x00000003UL

/// User data command options
/// Magic paramater for deleting user data
  #define SLI_SE_COMMAND_OPTION_ERASE_UD          0xDE1E7EADUL
  #define SLI_SE_COMMAND_OPTION_WRITE             0x00000100UL
  #define SLI_SE_COMMAND_OPTION_READ              0x00000000UL

  #define SLI_SE_COMMAND_CERT_BATCH               0x00000100UL
  #define SLI_SE_COMMAND_CERT_SE                  0x00000200UL
  #define SLI_SE_COMMAND_CERT_HOST                0x00000300UL

#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
/// Use SHA384 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_SHA384       0x00000500UL
/// Use SHA512 as hash algorithm
  #define SLI_SE_COMMAND_OPTION_HASH_SHA512       0x00000600UL
#if defined(_SILICON_LABS_32B_SERIES_3)
/// Use SHA384 as hash algorithm for HMAC streaming operation
  #define SLI_SE_COMMAND_OPTION_HMAC_HASH_SHA384  0x00000A00UL
/// Use SHA512 as hash algorithm for HMAC streaming operation
  #define SLI_SE_COMMAND_OPTION_HMAC_HASH_SHA512  0x00000B00UL
#endif // _SILICON_LABS_32B_SERIES_3
#endif // _SILICON_LABS_SECURITY_FEATURE_VAULT
#endif // SLI_MAILBOX_COMMAND_SUPPORTED

// -----------------------------------------------------------------------------
// Other defines

/** Maximum amount of parameters supported by the hardware FIFO */
#define SE_FIFO_MAX_PARAMETERS                      13U

/** Stop datatransfer */
#define SLI_SE_DATATRANSFER_STOP                    0x00000001UL
/** Discard datatransfer */
#define SLI_SE_DATATRANSFER_DISCARD                 0x40000000UL
/** Realign datatransfer */
#define SLI_SE_DATATRANSFER_REALIGN                 0x20000000UL
/** Datatransfer Const Address*/
#define SLI_SE_DATATRANSFER_CONSTADDRESS            0x10000000UL
/** Stop Length Mask */
#define SLI_SE_DATATRANSFER_LENGTH_MASK             0x0FFFFFFFUL

/** Maximum amount of parameters for largest command in defined command set */
#ifndef SLI_SE_COMMAND_MAX_PARAMETERS
#define SLI_SE_COMMAND_MAX_PARAMETERS                   4U
#endif

/* Sanity-check defines */
#if SLI_SE_COMMAND_MAX_PARAMETERS > SE_FIFO_MAX_PARAMETERS
#error "Trying to configure more parameters than supported by the hardware"
#endif

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief SE DMA transfer descriptor.

 * Can be linked to each other to provide scatter-gather behavior.
 ******************************************************************************/
typedef struct {
  volatile void* volatile data; /**< Data pointer */
  void* volatile next;          /**< Next descriptor */
  volatile uint32_t length;     /**< Length */
} sli_se_datatransfer_t;

/** Default initialization of data transfer struct */
#define SLI_SE_DATATRANSFER_DEFAULT(address, data_size)                                          \
  {                                                                                              \
    .data = (void*)(address),                        /* Pointer to data block */                 \
    .next = (void*)SLI_SE_DATATRANSFER_STOP,             /* This is the last block by default */ \
    .length = (data_size) | SLI_SE_DATATRANSFER_REALIGN  /* Add size, use realign by default */  \
  }

/***************************************************************************//**
 * @brief          SE mailbox command structure
 *
 * @details
 *   This structure defines the command structure used by the SE mailbox
 ******************************************************************************/
typedef struct {
  uint32_t command;                                   /**< SE Command */
  sli_se_datatransfer_t* data_in;                     /**< Input data */
  sli_se_datatransfer_t* data_out;                    /**< Output data */
  uint32_t parameters[SLI_SE_COMMAND_MAX_PARAMETERS]; /**< Parameters */
  size_t num_parameters;                              /**< Number of parameters */
} sli_se_mailbox_command_t;

/** Default initialization of command struct */
#define SLI_SE_MAILBOX_COMMAND_DEFAULT(command_word)     \
  {                                                      \
    .command = command_word,    /* Given command word */ \
    .data_in = NULL,            /* No data in */         \
    .data_out = NULL,           /* No data out */        \
    .parameters = { 0, 0, 0, 0 }, /* No parameters */    \
    .num_parameters = 0         /* No parameters */      \
  }

/** Possible responses to a command */
typedef uint32_t sli_se_mailbox_response_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Add input data to a mailbox command
 *
 * @details
 *   This function adds a buffer of input data to the given SE command structure
 *   The buffer gets appended by reference at the end of the list of already
 *   added buffers.
 *
 * @note
 *   Note that this function does not copy either the data buffer or the buffer
 *   structure, so make sure to keep the data object in scope until the command
 *   has been executed by the secure element.
 *
 * @param[in]  command
 *   Pointer to an SE Mailbox command structure.
 *
 * @param[in]  data
 *   Pointer to a data transfer structure.
 ******************************************************************************/
void sli_se_mailbox_command_add_input(sli_se_mailbox_command_t *command, sli_se_datatransfer_t *data);

/***************************************************************************//**
 * @brief
 *   Add output data to a mailbox command
 *
 * @details
 *   This function adds a buffer of output data to the given command structure
 *   The buffer gets appended by reference at the end of the list of already
 *   added buffers.
 *
 * @note
 *   Note that this function does not copy either the data buffer or the buffer
 *   structure, so make sure to keep the data object in scope until the command
 *   has been executed by the secure element.
 *
 * @param[in]  command
 *   Pointer to an SE mailbox command structure.
 *
 * @param[in]  data
 *   Pointer to a data transfer structure.
 ******************************************************************************/
void sli_se_mailbox_command_add_output(sli_se_mailbox_command_t *command, sli_se_datatransfer_t *data);

/***************************************************************************//**
 * @brief
 *   Add a parameter to a mailbox command
 *
 * @details
 *   This function adds a parameter word to the passed command.
 *
 * @note
 *   Make sure to not exceed @ref SE_MAX_PARAMETERS.
 *
 * @param[in]  command
 *   Pointer to a filled-out SE command structure.
 * @param[in]  parameter
 *   Parameter to add.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SE_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_se_mailbox_command_add_parameter(sli_se_mailbox_command_t *command, uint32_t parameter);

#if !defined(SLI_SE_MAILBOX_HOST_SYSTEM)
/***************************************************************************//**
 * @brief
 *   Execute the passed command
 *
 * @details
 *   This function starts the execution of the passed command by the secure
 *   element. The RXINT interrupt flag will be set upon completion. Call
 *   @ref sli_se_mailbox_read_response to wait for completion and retrieve the
 *   command's execution status.
 *
 * @param[in]  command
 *   Pointer to a filled-out SE command structure.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SE_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_se_mailbox_execute_command(sli_se_mailbox_command_t *command);
#endif //!defined(SLI_SE_MAILBOX_HOST_SYSTEM)

#if defined(SEMAILBOX_PRESENT)
/***************************************************************************//**
 * @brief
 *   Read the status of the previously executed command.
 *
 * @details
 *   This function waits for any running command to complete before reading the
 *   status of the previously executed command.
 *
 * @note
 *   The command response needs to be read for every executed command, and can
 *   only be read once per executed command (FIFO behavior).
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SE_RESPONSE_OK when the command was executed successfully or a signature
 *   was successfully verified.
 ******************************************************************************/
__STATIC_INLINE sli_se_mailbox_response_t sli_se_mailbox_read_response(void)
{
  while (!(SEMAILBOX_HOST->RX_STATUS & SEMAILBOX_RX_STATUS_RXINT)) {
    // Wait for command completion by polling SE Mailbox RX interrupt flag
  }
  // Return command response
  return (sli_se_mailbox_response_t)(SEMAILBOX_HOST->RX_HEADER & SLI_SE_RESPONSE_MASK);
}

/**
 * \brief          Handle the response of the previously executed command.
 *
 * \details        This function handles the response of the previously
 *                 executed HSE command by calling sli_se_mailbox_read_response
 *                 to read the response value and returns it. For Series-3 this
 *                 function also clears the SEMAILBOX FIFO by reading out the
 *                 unused command handle word.
 *                 This function is called by the ISR of the SEMAILBOX (called
 *                 SEMBRX_IRQHandler ) to clear the SEMBRX_IRQn interrupt signal
 *                 on the SEMAILBOX peripheral side. NOTE: The ISR will also
 *                 need to clear the SEMBRX_IRQn condition in the internal/local
 *                 interrupt controller (NVIC) by calling
 *                 NVIC_ClearPendingIRQ(SEMBRX_IRQn).
 *
 * \return         Value returned by sli_se_mailbox_read_response.
 ******************************************************************************/
__STATIC_INLINE sli_se_mailbox_response_t sli_se_mailbox_handle_response(void)
{
  // Read command response
  sli_se_mailbox_response_t se_mailbox_response = sli_se_mailbox_read_response();

  #if defined(_SILICON_LABS_32B_SERIES_3)
  // Read the command handle word ( not used ) from the SEMAILBOX FIFO
  SEMAILBOX_HOST->FIFO;
  #endif

  // Return command response
  return se_mailbox_response;
}

#elif defined(CRYPTOACC_PRESENT)
sli_se_mailbox_response_t sli_se_mailbox_read_response(void);

/**
 * \brief          Handle the response of the previously executed command.
 *
 * \details        This function handles the response of the previously
 *                 executed VSE command by calling sli_se_mailbox_read_response
 *                 to read the response value and returns it.
 *
 * \return         Value returned by sli_se_mailbox_read_response.
 ******************************************************************************/
__STATIC_INLINE sli_se_mailbox_response_t sli_se_mailbox_handle_response(void)
{
  // Read and return VSE mailbox command response
  return sli_se_mailbox_read_response();
}
#endif // #if defined(SEMAILBOX_PRESENT)

/***************************************************************************//**
 * @brief
 *   Disable one or more SE interrupts.
 *
 * @param[in] flags
 *   SE interrupt sources to disable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the Secure Element module
 *    (SE_CONFIGURATION_(TX/RX)INTEN).
 ******************************************************************************/
__STATIC_INLINE void sli_se_mailbox_disable_interrupt(uint32_t flags)
{
#if defined(SEMAILBOX_PRESENT)
  SEMAILBOX_HOST->CONFIGURATION &= ~flags;
#else
  (void) flags;
#endif
}

/***************************************************************************//**
 * @brief
 *   Enable one or more SE interrupts.
 *
 * @param[in] flags
 *   SE interrupt sources to enable. Use a bitwise logic OR combination of
 *   valid interrupt flags for the Secure Element module
 *   (SEMAILBOX_CONFIGURATION_TXINTEN or SEMAILBOX_CONFIGURATION_RXINTEN).
 ******************************************************************************/
__STATIC_INLINE void sli_se_mailbox_enable_interrupt(uint32_t flags)
{
#if defined(SEMAILBOX_PRESENT)
  SEMAILBOX_HOST->CONFIGURATION |= flags;
#else
  (void) flags;
#endif
}

#if defined(CRYPTOACC_PRESENT)
/***************************************************************************//**
 * @brief
 *   Get current SE version
 *
 * @details
 *   This function returns the current VSE version
 *
 * @param[in]  version
 *   Pointer to location where to copy the version of VSE to.
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SLI_SE_RESPONSE_OK when the command was executed successfully
 *   SLI_SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 *   SLI_SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid
 ******************************************************************************/
sli_se_mailbox_response_t sli_vse_mailbox_get_version(uint32_t *version);

/***************************************************************************//**
 * @brief
 *   Get VSE configuration and status bits
 *
 * @details
 *   This function returns the current VSE configuration and status bits.
 *   The following list explains what the different bits in cfg_status indicate.
 *   A bit value of 1 means enabled, while 0 means disabled:
 *    * [0]: Secure boot
 *    * [1]: Verify secure boot certificate
 *    * [2]: Anti-rollback
 *    * [3]: Narrow page lock
 *    * [4]: Full page lock
 *   The following status bits can be read with VSE versions
 *   higher than 1.2.2.
 *    * [10]: Debug port lock
 *    * [11]: Device erase enabled
 *    * [12]: Secure debug enabled
 *    * [15]: Debug port register state, 1 if the debug port is locked.
 *
 * @param[out]  cfg_status
 *   Pointer to location to copy Configuration Status bits into.
 *
 * @note
 *   This function will check that the mailbox content is valid before
 *   reading the status bits. If the command response has already been read
 *   with a call to @ref sli_vse_mailbox_ack_command(), the validity check will fail, and
 *   the config status bits cannot be read before a reset has occurred.
 *
 * @return
 *   One of the SE_RESPONSE return codes:
 *   SLI_SE_RESPONSE_OK when the command was executed successfully
 *   SLI_SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 *   SLI_SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid
 ******************************************************************************/
sli_se_mailbox_response_t sli_vse_mailbox_get_cfg_status(uint32_t *cfg_status);

/***************************************************************************//**
 * @brief
 *  Get the version number of the OTP from the status field of the output
 *  mailbox
 * @details
 *  This function checks if the OTP version number flag is set in the output
 *  mailbox. If it is, the version number is writen to @ref otp_version pointer
 *  location. If not, it returns error response.
 *
 * @param[out] otp_version
 *  Pointer to location to copy OTP version number into.
 * @return
 *  One of the SE_RESPONSE return codes.
 * @retval SLI_SE_RESPONSE_OK when the command was executed successfully
 ******************************************************************************/
sli_se_mailbox_response_t sli_vse_mailbox_get_otp_version(uint32_t *otp_version);

/***************************************************************************//**
 * @brief
 *   Acknowledge and get status and output data of a completed command.
 *
 * @details
 *   This function acknowledges and gets the status and output data of a
 *   completed mailbox command.
 *   The mailbox command is acknowledged by inverting all bits in the checksum
 *   (XOR with 0xFFFFFFFF).
 *   The output data is copied into the linked list of output buffers pointed
 *   to in the given command data structure.
 *
 * @param[in]  command
 *   Pointer to an SE command structure.
 *
 * @return
 *   One of the SE_RESPONSE return codes.
 * @retval SLI_SE_RESPONSE_OK when the command was executed successfully or a
 *                        signature was successfully verified,
 * @retval SLI_SE_RESPONSE_INVALID_COMMAND when the command ID was not recognized,
 * @retval SE_RESPONSE_AUTHORIZATION_ERROR when the command is not authorized,
 * @retval SE_RESPONSE_INVALID_SIGNATURE when signature verification failed,
 * @retval SE_RESPONSE_BUS_ERROR when a bus error was thrown during the command,
 *                               e.g. because of conflicting Secure/Non-Secure
 *                               memory accesses,
 * @retval SE_RESPONSE_CRYPTO_ERROR on an internal SE failure, or
 * @retval SLI_SE_RESPONSE_INVALID_PARAMETER when an invalid parameter was passed
 * @retval SLI_SE_RESPONSE_MAILBOX_INVALID when mailbox command not done or invalid
 ******************************************************************************/
sli_se_mailbox_response_t sli_vse_mailbox_ack_command(sli_se_mailbox_command_t *command);

/***************************************************************************//**
 * @brief
 *   Read the previously executed command.
 *
 * @details
 *   This function reads the previously executed command.
 *
 * @return
 *   One of the SE command words.
 *   SLI_SE_RESPONSE_MAILBOX_INVALID when the mailbox content is invalid.
 ******************************************************************************/
uint32_t sli_vse_mailbox_read_executed_command(void);

/***************************************************************************//**
 * @brief
 *   Check whether the VSE Output Mailbox is valid.
 *
 * @return True if the VSE Output Mailbox is valid (magic and checksum OK)
 ******************************************************************************/
bool sli_vse_mailbox_is_output_valid(void);

#endif // #if defined(CRYPTOACC_PRESENT)

#ifdef __cplusplus
}
#endif

#endif // #if defined(SLI_SE_MAILBOX_HOST_SYSTEM) || defined(SEMAILBOX_PRESENT) || defined(CRYPTOACC_PRESENT)

#endif /* SLI_SE_MANAGER_MAILBOX_H */
