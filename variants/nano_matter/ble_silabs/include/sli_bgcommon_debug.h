/***************************************************************************//**
 * @brief Debugging helpers used internally by the Bluetooth stack
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_BGCOMMON_DEBUG_H
#define SLI_BGCOMMON_DEBUG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup sli_bgcommon_debug
 * @{
 *
 * @brief Debugging helpers used internally by the Bluetooth stack
 *
 * The functions provided by this file are used by the Bluetooth stack to access
 * device-specific registers that the device-agnostic Bluetooth libraries cannot
 * take a direct dependency to.
 *
 ******************************************************************************/

/**
 * @brief Enable the MCU cycle counter
 */
void sli_bgcommon_debug_init_cycle_count(void);

/**
 * @brief Get the current MCU cycle count
 */
uint32_t sli_bgcommon_debug_get_cycle_count(void);

/** @} end sli_bgcommon_debug */

#ifdef __cplusplus
}
#endif

#endif // SLI_BGCOMMON_DEBUG_H
