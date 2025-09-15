/***************************************************************************//**
 * @file
 * @brief Interrupt Manager API internal utility functions.
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

#ifndef SLI_INTERRUPT_MANAGER_H
#define SLI_INTERRUPT_MANAGER_H

#include "sl_interrupt_manager.h"
#include "em_device.h"

#if defined(_SILICON_LABS_32B_SERIES_2)
#include "sl_interrupt_manager_s2_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *   Set a new RAM based interrupt vector table.
 *
 * @details
 *   This function will copy the existing vector table to a RAM area supplied
 *   by the application and set the interrupt controller to use that.
 *
 * @note
 *   The table RAM area must be aligned to a TBD byte boundary.
 *
 * @param[in] table
 *   Base address of new table.
 *
 * @param[in] handler_count
 *   The size of the table, unit is number of interrupt handlers.
 *
 * @return
 *   The prior interrupt vector table address.
 ******************************************************************************/
#if defined(_SILICON_LABS_32B_SERIES_3)                  \
  || (defined(SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM) \
  && (SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM == 1))

sl_interrupt_manager_irq_handler_t *sli_interrupt_manager_set_irq_table(sl_interrupt_manager_irq_handler_t *table,
                                                                        uint32_t handler_count);

#endif

/***************************************************************************//**
 * @brief
 *   Pre-interrupt hook.
 *
 * @details
 *   This function is called before each interrupt service routine
 *   when the interrupt manager hooks feature is enabled.
 *
 * @note
 *   The function is weakly defined, and may be user-defined. By default, the
 *   pre-interrupt hook is empty.
 ******************************************************************************/
void sl_interrupt_manager_irq_enter_hook(void);

/***************************************************************************//**
 * @brief
 *   Register post-interrupt hook.
 *
 * @details
 *   This function is called after each interrupt service routine
 *   when the interrupt manager hooks feature is enabled.
 *
 * @note
 *   The function is weakly defined, and may be user-defined. By default, the
 *   post-interrupt hook is empty.
 ******************************************************************************/
void sl_interrupt_manager_irq_exit_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* SL_INTERRUPT_MANAGER_H */
