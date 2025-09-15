/***************************************************************************//**
 * @file
 * @brief Silicon Labs Secure Engine Manager internal API.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef SE_MANAGER_INTERNAL_H
#define SE_MANAGER_INTERNAL_H

#include "sli_se_manager_features.h"

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED) || defined(SLI_VSE_MAILBOX_COMMAND_SUPPORTED)

#include "sl_status.h"
#include "sli_se_manager_mailbox.h"
#include "sl_se_manager.h"
#include "sl_se_manager_key_handling.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Defines

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED)
// Due to a problem with the countermeasures applied to
// accelerated point multiplication over elliptic curves,
// it is possible that random errors are encountered (this
// is extremely unilkely for truly random keys).
// As a workaround for this, the affected commands will
// retry the operation in order to reduce the probability
// that the error code was returned incorrectly. This helps
// lower the error probability further when using purposely
// small or large scalars, for example during testing.
#define SLI_SE_MAX_POINT_MULT_RETRIES   3U
#endif

// -------------------------------
// Function-like macros

/***************************************************************************//**
 * @brief
 *   Helper macro to init/reset the SE command struct of an SE command context
 *
 * @param[in] cmd_ctx
 *   Pointer to SE context containing the command to initialize/reset
 *
 * @param[out] command_word
 *   Command word to set in the SE command.
 *
 ******************************************************************************/
#define sli_se_command_init(cmd_ctx, command_word) \
  cmd_ctx->command.command = command_word;         \
  cmd_ctx->command.data_in = NULL;                 \
  cmd_ctx->command.data_out = NULL;                \
  cmd_ctx->command.num_parameters = 0;

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED)
/***************************************************************************//**
 * @brief
 *   Helper macros to add key parameters and input/output blocks to SE commands
 *
 * @param[in] cmd_ctx
 *   Pointer to SE context
 * @param[in] key
 *   Pointer to sl_se_key_descriptor_t structure
 * @param[out] status
 *   SL_Status_T
 *
 ******************************************************************************/

// Add keyspec to command for given key
#define sli_add_key_parameters(cmd_ctx, key, status) {                \
    uint32_t keyspec;                                                 \
    (status) = sli_se_key_to_keyspec((key), &keyspec);                \
    if ((status) != SL_STATUS_OK) {                                   \
      return (status);                                                \
    }                                                                 \
    sli_se_mailbox_command_add_parameter(&cmd_ctx->command, keyspec); \
}

// Add key metadata buffers to command for given key
#define sli_add_key_metadata(cmd_ctx, key, status)        \
  /* Auth data */                                         \
  sli_se_datatransfer_t auth_buffer;                      \
  (status) = sli_se_get_auth_buffer((key), &auth_buffer); \
  if ((status) != SL_STATUS_OK) {                         \
    return (status);                                      \
  }                                                       \
  sli_se_mailbox_command_add_input(&cmd_ctx->command, &auth_buffer);

// Add key metadata buffers with custom auth buffer to command for given key
#define sli_add_key_metadata_custom(cmd_ctx, auth_data_buf, key, status) \
  /* Auth data */                                                        \
  sli_se_datatransfer_t auth_data_buf;                                   \
  (status) = sli_se_get_auth_buffer((key), &auth_data_buf);              \
  if ((status) != SL_STATUS_OK) {                                        \
    return (status);                                                     \
  }                                                                      \
  sli_se_mailbox_command_add_input(&cmd_ctx->command, &auth_data_buf);

// Add key input buffer to given command
#define sli_add_key_input(cmd_ctx, key, status)                     \
  sli_se_datatransfer_t key_input_buffer;                           \
  (status) = sli_se_get_key_input_output((key), &key_input_buffer); \
  if ((status) != SL_STATUS_OK) {                                   \
    return (status);                                                \
  }                                                                 \
  sli_se_mailbox_command_add_input(&cmd_ctx->command, &key_input_buffer);

// Add Key output buffer to given command
#define sli_add_key_output(cmd_ctx, key, status)                     \
  sli_se_datatransfer_t key_output_buffer;                           \
  (status) = sli_se_get_key_input_output((key), &key_output_buffer); \
  if ((status) != SL_STATUS_OK) {                                    \
    return (status);                                                 \
  }                                                                  \
  sli_se_mailbox_command_add_output(&cmd_ctx->command, &key_output_buffer);
#endif // SLI_MAILBOX_COMMAND_SUPPORTED

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

sl_status_t sli_se_to_sl_status(sli_se_mailbox_response_t res);

/***************************************************************************//**
 * @brief
 *   Take the SE lock in order to synchronize multiple threads calling into
 *   the SE Manager API concurrently.
 *
 * @return
 *   SL_STATUS_OK when successful, or else error code.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SE_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_se_lock_acquire(void);

/***************************************************************************//**
 * @brief
 *   Give the SE lock in order to synchronize multiple threads calling into
 *   the SE Manager API concurrently.
 *
 * @return
 *   SL_STATUS_OK when successful, or else error code.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SE_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_se_lock_release(void);

/***************************************************************************//**
 * @brief
 *   Execute and wait for mailbox command to complete.
 *
 * @param[in] cmd_ctx
 *   Pointer to an SE command context object.
 *
 * @return
 *   Status code, @ref sl_status.h.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SE_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_se_execute_and_wait(sl_se_command_context_t *cmd_ctx);

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED)
// Key handling helper functions
sl_status_t sli_key_get_storage_size(const sl_se_key_descriptor_t* key,
                                     uint32_t *storage_size);
sl_status_t sli_key_get_size(const sl_se_key_descriptor_t* key, uint32_t* size);
sl_status_t sli_key_check_equivalent(const sl_se_key_descriptor_t* key_1,
                                     const sl_se_key_descriptor_t* key_2,
                                     bool check_key_flag,
                                     bool public_export);

sl_status_t sli_se_key_to_keyspec(const sl_se_key_descriptor_t* key,
                                  uint32_t* keyspec);
sl_status_t sli_se_keyspec_to_key(const uint32_t keyspec,
                                  sl_se_key_descriptor_t* key);
sl_status_t sli_se_get_auth_buffer(const sl_se_key_descriptor_t* key,
                                   sli_se_datatransfer_t* auth_buffer);
sl_status_t sli_se_get_key_input_output(const sl_se_key_descriptor_t* key,
                                        sli_se_datatransfer_t* buffer);
#endif // SLI_MAILBOX_COMMAND_SUPPORTED

#ifdef __cplusplus
}
#endif

#endif /* defined(SLI_MAILBOX_COMMAND_SUPPORTED) || defined(SLI_VSE_MAILBOX_COMMAND_SUPPORTED) */

#endif /* SE_MANAGER_INTERNAL_H */
