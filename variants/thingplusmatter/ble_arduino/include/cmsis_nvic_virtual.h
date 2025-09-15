/***************************************************************************//**
 * @file
 * @brief CMSIS NVIC Virtual Header
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
#ifndef CMSIS_NVIC_VIRTUAL_H
#define CMSIS_NVIC_VIRTUAL_H

#include "sl_interrupt_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

// NVIC backward compatibility layer
#define NVIC_EnableIRQ(irqn)      sl_interrupt_manager_enable_irq(irqn)
#define NVIC_DisableIRQ(irqn)     sl_interrupt_manager_disable_irq(irqn)
#define NVIC_SetPriority          sl_interrupt_manager_set_irq_priority
#define NVIC_GetPriority          sl_interrupt_manager_get_irq_priority
#define NVIC_SystemReset          sl_interrupt_manager_reset_system
#define NVIC_GetPendingIRQ(irqn)    sl_interrupt_manager_is_irq_pending(irqn)
#define NVIC_SetPendingIRQ(irqn)    sl_interrupt_manager_set_irq_pending(irqn)
#define NVIC_ClearPendingIRQ(irqn)  sl_interrupt_manager_clear_irq_pending(irqn)
#define NVIC_GetActive(irqn)        sl_interrupt_manager_get_active_irq(irqn)

// Original NVIC calls
#define NVIC_SetPriorityGrouping    __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping    __NVIC_GetPriorityGrouping
#define NVIC_GetEnableIRQ           __NVIC_GetEnableIRQ

#ifdef __cplusplus
}
#endif

#endif /* SL_CMSIS_NVIC_VIRTUAL_H */
