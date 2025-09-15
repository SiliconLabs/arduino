/***************************************************************************//**
 * @file
 * @brief Core API
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

#ifndef SL_CORE_H
#define SL_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup sl_core Core
 *
 * @section sl_core_intro Introduction
 *
 * The core abstraction API provides high-level, device agnostic, control of
 * core peripherals, most notably the ability to execute code in sections with
 * varying levels of interrupt masking.
 *
 * This module provides support for two types of critical sections, each
 * with different interrupt masking capabilities.
 *
 * @li <b>CRITICAL section</b>: Inside a critical section, all interrupts are
 *     masked (except for core exception handlers).
 * @li <b>ATOMIC section</b>: Inside an atomic section, interrupts with a
 *     priority less than the configurable @ref SL_CORE_BASE_PRIORITY_LEVEL
 *     value will be masked.
 *
 * @section sl_core_conf Compile-time Configuration
 *
 * The following #define is used to configure sl_core:
 * @code{.c}
 * // Enables debug methods to measure the time spent in critical sections.
 * #define SL_CORE_DEBUG_INTERRUPTS_MASKED_TIMING   0
 * @endcode
 *
 * @section sl_core_macro_api Macro API
 *
 * The core abstraction API has macros to facilitate executing code in
 * ATOMIC and CRITICAL sections.
 *
 * @ref CORE_DECLARE_IRQ_STATE, @ref CORE_ENTER_ATOMIC() and
 * @ref CORE_EXIT_ATOMIC() can be used together to implement an ATOMIC section.
 * @code{.c}
 * {
 *   CORE_DECLARE_IRQ_STATE;           // Storage for saving IRQ state prior to
 *                                     // atomic section entry.
 *
 *   CORE_ENTER_ATOMIC();              // Enter atomic section.
 *
 *   ...
 *   ... your code goes here ...
 *   ...
 *
 *   CORE_EXIT_ATOMIC();               // Exit atomic section, IRQ state is restored.
 * }
 * @endcode
 *
 * @ref CORE_ATOMIC_SECTION(yourcode) is aconcatenation of all three of the
 * macros above.
 * @code{.c}
 * {
 *   CORE_ATOMIC_SECTION(
 *     ...
 *     ... your code goes here ...
 *     ...
 *   )
 * }
 * @endcode
 *
 * The following macros implement CRITICAL sections in a similar fashion as
 * described above for ATOMIC sections:
 * <li>@ref CORE_DECLARE_IRQ_STATE</li>
 * <li>@ref CORE_ENTER_CRITICAL()</li>
 * <li>@ref CORE_EXIT_CRITICAL()</li>
 * <li>@ref CORE_CRITICAL_SECTION(yourcode)</li>
 *
 * @section sl_core_reimplementation API Reimplementation
 *
 * Most of the functions in the API are implemented as weak functions. This means
 * that it is easy to reimplement when special needs arise. Shown below is a
 * reimplementation of CRITICAL sections suitable if FreeRTOS OS is used:
 * @code{.c}
 * CORE_irqState_t CORE_EnterCritical(void)
 * {
 *   vPortEnterCritical();
 *   return 0;
 * }
 *
 * void CORE_ExitCritical(CORE_irqState_t irqState)
 * {
 *   (void)irqState;
 *   vPortExitCritical();
 * }
 * @endcode
 * Also note that CORE_Enter/ExitCritical() are not implemented as inline
 * functions. As a result, reimplementations will be possible even when original
 * implementations are inside a linked library.
 *
 * Some RTOSes must be notified on interrupt handler entry and exit. Macros
 * @ref CORE_INTERRUPT_ENTRY() and @ref CORE_INTERRUPT_EXIT() are suitable
 * placeholders for inserting such code. Insert these macros in all your
 * interrupt handlers and then override the default macro implementations.
 * This is an example if uC/OS is used:
 * @code{.c}
 * // In emlib_config.h:
 *
 * #define CORE_INTERRUPT_ENTRY()   OSIntEnter()
 * #define CORE_INTERRUPT_EXIT()    OSIntExit()
 * @endcode
 *
 * @section sl_core_max_timing Maximum Interrupt Disabled Time
 *
 * The maximum time spent (in cycles) in critical and atomic sections can be
 * measured for performance and interrupt latency analysis.
 * To enable the timings, use the SL_CORE_ENABLE_INTERRUPT_DISABLED_TIMING
 * configuration option. When enabled, the functions
 * @ref CORE_get_max_time_critical_section() and
 * @ref CORE_get_max_time_atomic_section()
 * can be used to get the max timings since startup.
 *
 * @section sl_core_porting Porting from em_int
 *
 * Existing code using INT_Enable() and INT_Disable() must be ported to the
 * sl_core API. While em_int used, a global counter to store the interrupt state,
 * sl_core uses a local variable. Any usage of INT_Disable(), therefore, needs to
 * be replaced with a declaration of the interrupt state variable before entering
 * the critical section.
 *
 * Since the state variable is in local scope, the critical section exit
 * needs to occur within the scope of the variable. If multiple nested critical
 * sections are used, each needs to have its own state variable in its own scope.
 *
 * In many cases, completely disabling all interrupts using CRITICAL sections
 * might be more heavy-handed than needed. When porting, consider whether
 * an ATOMIC section can be used to only disable a subset of the interrupts.
 *
 * Replacing em_int calls with sl_core function calls:
 * @code{.c}
 * void func(void)
 * {
 *   // INT_Disable();
 *   CORE_DECLARE_IRQ_STATE;
 *   CORE_ENTER_ATOMIC();
 *     .
 *     .
 *     .
 *   // INT_Enable();
 *   CORE_EXIT_ATOMIC();
 * }
 * @endcode
 * @{
 ******************************************************************************/

/*******************************************************************************
 *****************************   DEFINES   *************************************
 ******************************************************************************/

#if !defined(CORE_ATOMIC_BASE_PRIORITY_LEVEL)
/** The interrupt priority level disabled within ATOMIC regions. Interrupts
 *  with priority level equal to or lower than this definition will be disabled
 *  within ATOMIC regions. */
#define CORE_ATOMIC_BASE_PRIORITY_LEVEL 3
#else
#ifndef SL_SUPPRESS_DEPRECATION_WARNINGS_SDK_2024_6
#warning "The CORE_ATOMIC_BASE_PRIORITY_LEVEL configuration is DEPRECATED. In \
  later releases, the base priority of atomic sections will be hardcoded to 3 \
  and will no longer be configurable. Please consider updating the priorities \
  of interrupts in your application to account for this new hardcoded value."
#endif
#endif

/*******************************************************************************
 ************************   MACRO API   ***************************************
 ******************************************************************************/

/// Allocate storage for PRIMASK or BASEPRI value for use by
/// CORE_ENTER/EXIT_ATOMIC() and CORE_ENTER/EXIT_CRITICAL() macros.
#define CORE_DECLARE_IRQ_STATE      CORE_irqState_t irqState

/// CRITICAL style interrupt disable.
#define CORE_CRITICAL_IRQ_DISABLE() CORE_CriticalDisableIrq()

/// CRITICAL style interrupt enable.
#define CORE_CRITICAL_IRQ_ENABLE()  CORE_CriticalEnableIrq()

/// Convenience macro for implementing a CRITICAL section.
#define CORE_CRITICAL_SECTION(yourcode) \
  {                                     \
    CORE_DECLARE_IRQ_STATE;             \
    CORE_ENTER_CRITICAL();              \
    {                                   \
      yourcode                          \
    }                                   \
    CORE_EXIT_CRITICAL();               \
  }

/// Enter CRITICAL section. Assumes that a @ref CORE_DECLARE_IRQ_STATE exist in
/// scope.
#define CORE_ENTER_CRITICAL() irqState = CORE_EnterCritical()

/// Exit CRITICAL section. Assumes that a @ref CORE_DECLARE_IRQ_STATE exist in
/// scope.
#define CORE_EXIT_CRITICAL()  CORE_ExitCritical(irqState)

/// CRITICAL style yield.
#define CORE_YIELD_CRITICAL() CORE_YieldCritical()

/// ATOMIC style interrupt disable.
#define CORE_ATOMIC_IRQ_DISABLE() CORE_AtomicDisableIrq()

/// ATOMIC style interrupt enable.
#define CORE_ATOMIC_IRQ_ENABLE()  CORE_AtomicEnableIrq()

/// Convenience macro for implementing an ATOMIC section.
#define CORE_ATOMIC_SECTION(yourcode) \
  {                                   \
    CORE_DECLARE_IRQ_STATE;           \
    CORE_ENTER_ATOMIC();              \
    {                                 \
      yourcode                        \
    }                                 \
    CORE_EXIT_ATOMIC();               \
  }

/// Enter ATOMIC section. Assumes that a @ref CORE_DECLARE_IRQ_STATE exist in
/// scope.
#define CORE_ENTER_ATOMIC()   irqState = CORE_EnterAtomic()

/// Exit ATOMIC section. Assumes that a @ref CORE_DECLARE_IRQ_STATE exist in
/// scope.
#define CORE_EXIT_ATOMIC()    CORE_ExitAtomic(irqState)

/// ATOMIC style yield.
#define CORE_YIELD_ATOMIC()   CORE_YieldAtomic()

/// Check if IRQ is disabled.
#define CORE_IRQ_DISABLED()   CORE_IrqIsDisabled()

/// Check if inside an IRQ handler.
#define CORE_IN_IRQ_CONTEXT() CORE_InIrqContext()

// Reset System.
#define CORE_RESET_SYSTEM()   CORE_ResetSystem()

/*******************************************************************************
 *************************   TYPEDEFS   ****************************************
 ******************************************************************************/

/// Storage for PRIMASK or BASEPRI value.
typedef uint32_t CORE_irqState_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 *
 *   Disable all interrupts by setting PRIMASK.
 *   (Fault exception handlers will still be enabled).
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_CriticalDisableIrq(void);

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 *
 *   Enable interrupts by clearing PRIMASK.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_CriticalEnableIrq(void);

/***************************************************************************//**
 * @brief
 *   Exit a CRITICAL section.
 *
 * @param[in] irqState
 *   The interrupt priority blocking level to restore to PRIMASK when exiting
 *   the CRITICAL section. This value is usually the one returned by a prior
 *   call to @ref CORE_EnterCritical().
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_ExitCritical(CORE_irqState_t irqState);

/***************************************************************************//**
 * @brief
 *   Brief interrupt enable/disable sequence to allow handling of
 *   pending interrupts.
 *
 * @note
 *   Usually used within a CRITICAL section.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_YieldCritical(void);

/***************************************************************************//**
 * @brief
 *   Enter a CRITICAL section.
 *
 *   When a CRITICAL section is entered, all interrupts (except fault handlers)
 *   are disabled.
 *
 * @return
 *   The value of PRIMASK register prior to the CRITICAL section entry.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
CORE_irqState_t CORE_EnterCritical(void);

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 *
 *   Disable interrupts with a priority lower or equal to
 *   @ref CORE_ATOMIC_BASE_PRIORITY_LEVEL. Sets core BASEPRI register
 *   to CORE_ATOMIC_BASE_PRIORITY_LEVEL.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_CriticalDisableIrq().
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void  CORE_AtomicDisableIrq(void);

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 *
 *   Enable interrupts by setting core BASEPRI register to 0.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_BASEPRI and PRIMASK
 *   is set (CPU is inside a CRITICAL section), interrupts will still be
 *   disabled after calling this function.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_CriticalEnableIrq().
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void  CORE_AtomicEnableIrq(void);

/***************************************************************************//**
 * @brief
 *   Exit an ATOMIC section.
 *
 * @param[in] irqState
 *   The interrupt priority blocking level to restore to BASEPRI when exiting
 *   the ATOMIC section. This value is usually the one returned by a prior
 *   call to @ref CORE_EnterAtomic().
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is set to @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_ExitCritical().
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void  CORE_ExitAtomic(CORE_irqState_t irqState);

/***************************************************************************//**
 * @brief
 *   Brief interrupt enable/disable sequence to allow handling of
 *   pending interrupts.
 *
 * @note
 *   Usually used within an ATOMIC section.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_YieldCritical().
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void  CORE_YieldAtomic(void);

/***************************************************************************//**
 * @brief
 *   Enter an ATOMIC section.
 *
 *   When an ATOMIC section is entered, interrupts with priority lower or equal
 *   to @ref CORE_ATOMIC_BASE_PRIORITY_LEVEL are disabled.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_EnterCritical().
 *
 * @return
 *   The value of BASEPRI register prior to ATOMIC section entry.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
CORE_irqState_t CORE_EnterAtomic(void);

/***************************************************************************//**
 * @brief
 *   Check whether the current CPU operation mode is handler mode.
 *
 * @return
 *   True if the CPU is in handler mode (currently executing an interrupt handler).
 *   @n False if the CPU is in thread mode.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
bool  CORE_InIrqContext(void);

/***************************************************************************//**
 * @brief
 *   Check if interrupts are disabled.
 *
 * @return
 *   True if interrupts are disabled.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
bool  CORE_IrqIsDisabled(void);

/***************************************************************************//**
 * @brief
 *   Returns the max time spent in critical section.
 *
 * @return
 *   The max time spent in critical section.
 *
 * @note SL_CORE_DEBUG_INTERRUPTS_MASKED_TIMING must be enabled.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t CORE_get_max_time_critical_section(void);

/***************************************************************************//**
 * @brief
 *   Returns the max time spent in atomic section.
 *
 * @return
 *   The max time spent in atomic section.
 *
 * @note SL_CORE_DEBUG_INTERRUPTS_MASKED_TIMING must be enabled.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t CORE_get_max_time_atomic_section(void);

/***************************************************************************//**
 * @brief
 *   Clears the max time spent in atomic section.
 *
 * @note SL_CORE_DEBUG_INTERRUPTS_MASKED_TIMING must be enabled.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_clear_max_time_critical_section(void);

/***************************************************************************//**
 * @brief
 *   Clears the max time spent in atomic section.
 *
 * @note SL_CORE_DEBUG_INTERRUPTS_MASKED_TIMING must be enabled.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CORE, SL_CODE_CLASS_TIME_CRITICAL)
void CORE_clear_max_time_atomic_section(void);

/***************************************************************************//**
 * @brief
 *   Reset chip routine.
 ******************************************************************************/
void CORE_ResetSystem(void);

/** @} (end addtogroup sl_core) */

#ifdef __cplusplus
}
#endif

#endif /* SL_CORE_H */
