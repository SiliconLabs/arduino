/***************************************************************************//**
 * @file
 * @brief API defining access to SYSCFG registers
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

#ifndef SL_HAL_SYSCFG_H
#define SL_HAL_SYSCFG_H

#include <stdint.h>

#if defined(SL_TRUSTZONE_NONSECURE)
#include "sli_tz_service_syscfg.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup syscfg SYSTEM CONFIGURATION - System Configurations
 * @brief Syscfg API
 * @details
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/*******************************************************************************
 ******************************** TZ SERVICES **********************************
 ******************************************************************************/

/*******************************************************************************
 * @brief
 *   Reads CHIPREV register.
 ******************************************************************************/
uint32_t sl_hal_syscfg_read_chip_rev(void);

/*******************************************************************************
 * @brief
 *   Set SYSTICEXTCLKEN bit in CFGSYSTIC to one.
 ******************************************************************************/
void sl_hal_syscfg_set_systicextclken_cfgsystic(void);

/*******************************************************************************
 * @brief
 *   Clear SYSTICEXTCLKEN bit in CFGSYSTIC to zero.
 ******************************************************************************/
void sl_hal_syscfg_clear_systicextclken_cfgsystic(void);

#ifdef __cplusplus
}
#endif
#endif // SL_HAL_SYSCFG_H
