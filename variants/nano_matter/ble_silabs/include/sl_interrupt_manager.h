/***************************************************************************//**
 * @file
 * @brief Interrupt Management API to enable and configure interrupts.
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

#ifndef SL_INTERRUPT_MANAGER_H
#define SL_INTERRUPT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_core.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup interrupt_manager Interrupt Manager
 * @brief Interrupt management service can be used for general interrupt management.
 * The source files for Interrupt Manager platform software module are present under
 * platform/services/interrupt_manager.
 * @details
 * ## Overview
 * The Interrupt Manager is a service that offers interupt management functions and configurations
 * for setting the interrupt vector in RAM, managing the core reset initiation function and
 * doing general interrupt management operations.
 *
 * ## Configuration Options
 *
 * Some properties of the Interrupt Manager are compile-time configurable. These
 * properties are set in the sl_interrupt_manager_s2_config.h file.
 * These are the available configuration parameters with default values defined.
 * @code
 *
 * // <q SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM> Put the interrupt vector table in RAM.
 * // <i> Set to 1 to put the vector table in RAM.
 * // <i> Default: 0
 * #define SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM  0
 * @endcode
 *
 * @note The SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM configuration is only available
 * on series 2. Enabling the S2_INTERRUPTS_IN_RAM configuration will tell the Interrupt Manager
 * to copy the interrupt vector table from ROM to RAM and select it as the interrupt table.
 * On newer series this feature is always enabled.
 *
 * ## The API
 *
 * This section contains brief descriptions of the functions in the API. For more
 * information on input and output parameters and return values,
 * click on the hyperlinked function names.
 *
 * @ref sl_interrupt_manager_disable_interrupts and @ref sl_interrupt_manager_enable_interrupts()
 * are used to prevent interrupts from executing during a certain timelapse.
 *
 * @ref sl_interrupt_manager_is_irq_disabled, @ref sl_interrupt_manager_is_irq_blocked
 * are used to know the status of an interrupt, either if it's disabled or blocked by one of the
 * following reasons: priority masking, disabling or an active interrupt of higher priority
 * is executing.
 *
 * @ref sl_interrupt_manager_is_irq_pending, @ref sl_interrupt_manager_set_irq_pending and
 * @ref sl_interrupt_manager_clear_irq_pending
 * are used for control and query of external interrupt source pending status.
 *
 * @ref sl_interrupt_manager_get_irq_priority and @ref sl_interrupt_manager_set_irq_priority
 * are used to get or set the priority for a specific interrupt.
 *
 * ## Priority Stratification
 * With the Interrupt Manager service and more generally in the Simplicity SDK, there are multiple distinct
 * levels of priority stratification.
 *
 * Each of these has their own characteristics and purposes.
 * For example, the higher priority group is considered to not be able to call kernel, power manager
 * or protocol stacks functions. They will only be impacted by critical sections (general interrupt
 * disable) but will be above atomic base interrupt priority level for execution. The higher level
 * is considered to be between 0 and 2 and the base interrupt priority level is 3.
 *
 * In the normal priority group you will find most application interrupts and such interrupts will be
 * the ones that will make calls to more features such as kernel, power manager and protocol stacks API.
 * It is this way because they are less deterministic than the "higher priority interrupts".
 *
 * <table>
 * <caption id="interrupt_priority_strat">Priority stratification inside SDK</caption>
 * <tr><th>Priority<th>Purpose
 * <tr><td>0 - 2 (Highest)<td>
 * <ul>
 *     <li>No Kernel calls
 *     <li>No Power Manager calls
 *      <li>Not maskable by atomic sections
 * </ul>
 * <tr><td>3 - 7 (Normal)<td>
 * <ul>
 *     <li>kernel calls
 *     <li>power manager
 *      <li>protocol stacks API
 * </ul>
 * <tr><td>7 (Lowest)<td>
 * <ul>
 *     <li>PendSV level of priority
 * </ul>
 * </table>
 * @{
 ******************************************************************************/

/// @brief sl_interrupt_manager interrupt handler function.
typedef void(*sl_interrupt_manager_irq_handler_t)(void);

/***************************************************************************//**
 * @brief
 *   Initialize interrupt controller hardware and initialise vector table
 *   in RAM.
 *
 * @note
 *   The interrupt manager init function will perform the initialization only
 *   once even if it's called multiple times.
 ******************************************************************************/
void sl_interrupt_manager_init(void);

/***************************************************************************//**
 * @brief
 *   Reset the cpu core.
 ******************************************************************************/
void sl_interrupt_manager_reset_system(void);

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 ******************************************************************************/
void sl_interrupt_manager_disable_interrupts(void);

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 ******************************************************************************/
void sl_interrupt_manager_enable_interrupts(void);

/***************************************************************************//**
 * @brief
 *   Disable interrupt for an interrupt source.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 ******************************************************************************/
void sl_interrupt_manager_disable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Enable interrupt for an interrupt source.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 ******************************************************************************/
void sl_interrupt_manager_enable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Check if an interrupt is disabled.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   True if the interrupt is disabled.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_disabled(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Check if a specific interrupt is blocked.
 *
 * @note
 *   The function return true if the IRQ is disabled.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   True if the interrupt is disabled or blocked.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_blocked(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Get Pending Interrupt
 *
 * @note
 *   Read the pending status of a specified interrupt and returns it status.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   false  Interrupt status is not pending.
 *   true   Interrupt status is pending.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_pending(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Set interrupt status to pending.
 *
 * @note
 *   Sets an interrupt pending status to true.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 ******************************************************************************/
void sl_interrupt_manager_set_irq_pending(int32_t irqn);

/***************************************************************************//**
*  @brief
*    Clear Pending Interrupt
*
*  @details
*   Clear an interrupt pending status
*
* @param[in] irqn
*   The interrupt number of the interrupt source.
*
*  @note
*   irqn must not be negative.
*******************************************************************************/
void sl_interrupt_manager_clear_irq_pending(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Set the interrupt handler of an interrupt source.
 *
 * @note
 *   This function depends on a RAM based interrupt vector table, i.e.
 *   SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM must be true. Or the device
 *   must be Series 3.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @param[in] handler
 *   The new interrupt handler for the interrupt source.
 *
 * @return
 *   The prior interrupt handler for the interrupt source.
 ******************************************************************************/
sl_status_t sl_interrupt_manager_set_irq_handler(int32_t irqn,
                                                 sl_interrupt_manager_irq_handler_t handler);

/***************************************************************************//**
 * @brief
 *   Get the interrupt preemption priority of an interrupt source.
 *
 * @note
 *   The number of priority levels is platform dependent.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   The interrupt priority for the interrupt source.
 *   Value 0 denotes the highest priority.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_irq_priority(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Set the interrupt preemption priority of an interrupt source.
 *
 * @note
 *   The number of priority levels is platform dependent.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @param[in] priority
 *   The new interrupt priority for the interrupt source.
 *   Value 0 denotes the highest priority.
 ******************************************************************************/
void sl_interrupt_manager_set_irq_priority(int32_t irqn, uint32_t priority);

/***************************************************************************//**
 * @brief
 *   Increase the interrupt preemption priority of an interrupt source.
 *   relative to the default priority.
 *
 * @details
 *   This function is useful to be architecture agnostic with priority values.
 *
 *   Usage:
 *   new_prio = sl_interrupt_manager_increase_irq_priority_from_default(IRQn, 1);
 *
 *   This will increase the priority of IRQn by 1.
 *
 * @param[in] irqn
 *   The irq to change the priority.
 *
 * @param[in] diff
 *   The relative difference.
 ******************************************************************************/
void sl_interrupt_manager_increase_irq_priority_from_default(int32_t irqn, uint32_t diff);

/***************************************************************************//**
 * @brief
 *   Decrease the interrupt preemption priority of an interrupt source
 *   relative to the default priority.
 *
 * @details
 *   This function is useful to be architecture agnostic with priority values.
 *
 *   Usage:
 *   new_prio = sl_interrupt_manager_decrease_irq_priority_from_default(IRQn, 1);
 *
 *   This will decrease the priority of IRQn by 1.
 *
 * @param[in] irqn
 *   The irq to change the priority.
 *
 * @param[in] diff
 *   The relative difference.
 ******************************************************************************/
void sl_interrupt_manager_decrease_irq_priority_from_default(int32_t irqn, uint32_t diff);

/***************************************************************************//**
 * @brief
 *   Get the default interrupt preemption priority value.
 *
 * @return
 *   The default priority.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_default_priority(void);

/***************************************************************************//**
 * @brief
 *   Get the highest interrupt preemption priority value.
 *
 * @return
 *   The highest priority value.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_highest_priority(void);

/***************************************************************************//**
 * @brief
 *   Get the lowest interrupt preemption priority value.
 *
 * @return
 *   The lowest priority value.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_lowest_priority(void);

/***************************************************************************//**
 * @brief
 *   Get the interrupt active status.
 *
 * @param[in] irqn
 *   The interrupt number of the interrupt source.
 *
 * @return
 *   The interrupt active status.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_active_irq(int32_t irqn);

/***************************************************************************//**
 * @brief
 *   Get the current ISR table.
 *
 * @details
 *   Depending on the configuration of the Interrupt Manager, this table of
 *   ISRs may be in RAM or in FLASH, and each ISR may or may not be wrapped by
 *   enter/exit hooks.
 *
 * @return
 *   The current ISR table.
 ******************************************************************************/
sl_interrupt_manager_irq_handler_t* sl_interrupt_manager_get_isr_table(void);

/** @} (end addtogroup interrupt_manager) */

#ifdef __cplusplus
}
#endif

#endif /* SL_INTERRUPT_MANAGER_H */
