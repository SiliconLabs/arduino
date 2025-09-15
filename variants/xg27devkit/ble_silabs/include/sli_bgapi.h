/***************************************************************************//**
 * @brief Silicon Labs internal API provided by the BGAPI protocol
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef SLI_BGAPI_H
#define SLI_BGAPI_H

#include "sl_status.h"
#include "sl_bgapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DOXYGEN_INCLUDE_INTERNAL */

/***************************************************************************//**
 * @addtogroup sli_bgapi BGAPI Protocol internal API
 * @{
 *
 * @brief Silicon Labs internal API to the BGAPI Protocol
 *
 * The BGAPI Protocol provides some services that are needed internally by
 * Silicon Labs components. The APIs provided by this file must not be used by
 * application code.
 */

// -----------------------------------------------------------------------------
// Silicon Labs internal types for the BGAPI protocol

/**
 * @brief Type definition for the command handler delegate function.
 *
 * The command handler delegate is used by the BGAPI protocol to handle a
 * command. The default delegate @ref sli_bgapi_cmd_handler_delegate executes
 * the BGAPI command handler function as a direct function call. The RTOS
 * adaptation uses @ref sli_bgapi_set_cmd_handler_delegate to set a special
 * delegate that handles the required inter-process communication (IPC) when an
 * RTOS is used.
 *
 * @param[in] header The BGAPI command header of the command to process
 * @param[in] handler The handler function to process the BGAPI command
 * @param[in] payload The BGAPI command payload data of the command to process
 */
typedef void sli_bgapi_cmd_handler_delegate_t (uint32_t header,
                                               sl_bgapi_handler handler,
                                               const void* payload);

#define SLI_BGAPI_BUFFER_SIZE (SL_BGAPI_MSG_HEADER_LEN + SL_BGAPI_MAX_PAYLOAD_SIZE)

/**
 * @brief The default command handler delegate function.
 */
sli_bgapi_cmd_handler_delegate_t sli_bgapi_cmd_handler_delegate;

// -----------------------------------------------------------------------------
// Silicon Labs internal functions to control the BGAPI protocol

/**
 * @brief Set the BGAPI command handler delegate.
 *
 * @param[in] cmd_handler_delegate Pointer to the command handler delegate function
 */
void sli_bgapi_set_cmd_handler_delegate(sli_bgapi_cmd_handler_delegate_t *cmd_handler_delegate);

/**
 * @brief Pop an event out of the BGAPI event queue of a specific device.
 *
 * @param[in] dev_type The BGAPI device to pop events from
 * @param[in] event_buffer_size The size of the supplied event buffer
 * @param[out] event_buffer The event buffer to receive the popped event
 *
 * @return SL_STATUS_OK if successful, otherwise an error code
 */
sl_status_t sli_bgapi_pop_event(uint8_t dev_type,
                                size_t event_buffer_size,
                                void *event_buffer);

/**
 * @brief Set the BGAPI command/response buffer.
 *
 * @param[in] buffer The buffer to use for BGAPI command/response data
 * @return The previous buffer
 */
void *sli_bgapi_set_buffer(void *buffer);

/** @} end sli_bgapi */

/** @endcond */ // DOXYGEN_INCLUDE_INTERNAL

#ifdef __cplusplus
}
#endif

#endif // SLI_BGAPI_H
