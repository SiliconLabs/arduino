/***************************************************************************//**
 * @file
 * @brief Power Manager Private API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_POWER_MANAGER_H
#define SLI_POWER_MANAGER_H

#include "sl_power_manager.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************   HOOK REFERENCES   ****************************
 ******************************************************************************/

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
bool sl_power_manager_sleep_on_isr_exit(void);

// Callback to application after wakeup but before restoring interrupts.
// For internal Silicon Labs use only
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_on_wakeup(void);

// Hook that can be used by the log outputer to suspend transmission of logs
// in case it would require energy mode changes while in the sleep loop.
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_suspend_log_transmission(void);

// Hook that can be used by the log outputer to resume transmission of logs.
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_resume_log_transmission(void);

// Callback to notify possible transition from EM1P to EM2.
// For internal Silicon Labs use only
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_em1p_to_em2_notification(void);

/***************************************************************************//**
 * Mandatory callback that allows to cancel sleeping action.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
bool sl_power_manager_is_ok_to_sleep(void);

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void sli_power_manager_update_hf_clock_settings_preservation_requirement(bool add);

/***************************************************************************//**
 * Adds requirement on the preservation of the High Frequency Clocks settings.
 *
 * @note FOR INTERNAL USE ONLY.
 *
 * @note Must be used together with adding an EM2 requirement.
 ******************************************************************************/
void sli_power_manager_add_hf_clock_settings_preservation_requirement(void);

/***************************************************************************//**
 * Removes requirement on the preservation of the High Frequency Clocks settings.
 *
 * @note FOR INTERNAL USE ONLY.
 *
 * @note Must be used together with removing an EM2 requirement.
 ******************************************************************************/
void sli_power_manager_remove_hf_clock_settings_preservation_requirement(void);

/***************************************************************************//**
 * Informs the power manager module that the high accuracy/high frequency clock
 * is used.
 *
 * @note FOR INTERNAL USE ONLY.
 *
 * @note Must be called by RAIL initialization in case radio clock settings
 *       are not set before the Power Manager initialization.
 ******************************************************************************/
__WEAK void sli_power_manager_set_high_accuracy_hf_clock_as_used(void);

/***************************************************************************//**
 * Gets the wake-up restore process time.
 * If we are not in the context of a deepsleep and therefore don't need to
 * do a restore, the return value is 0.
 *
 *
 * @return   Wake-up restore process time.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sli_power_manager_get_restore_delay(void);

/***************************************************************************//**
 * Initiates the wake-up restore process.
 ******************************************************************************/
void sli_power_manager_initiate_restore(void);

/***************************************************************************//**
 * Performs pre sleep operations.
 *
 * @note Must only be called by the RTOS integration code.
 ******************************************************************************/
void sli_power_manager_pre_sleep(void);

/***************************************************************************//**
 * Fetches current energy mode
 *
 * @return Returns current energy mode
 ******************************************************************************/
sl_power_manager_em_t sli_power_manager_get_current_em(void);

/***************************************************************************//**
 * Update Energy Mode 4 configurations.
 ******************************************************************************/
void sli_power_manager_init_em4(void);

/***************************************************************************//**
 * Enable or disable fast wake-up in EM2 and EM3
 *
 * @note Will also update the wake up time from EM2 to EM0.
 ******************************************************************************/
void sli_power_manager_em23_voltage_scaling_enable_fast_wakeup(bool enable);

/***************************************************************************//**
 * Initializes energy mode transition list.
 ******************************************************************************/
void sli_power_manager_em_transition_event_list_init(void);

/***************************************************************************//**
 * Notify subscribers about energy mode transition.
 *
 * @param  from  Energy mode from which CPU comes from.
 *
 * @param  to    Energy mode to which CPU is going to.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_power_manager_notify_em_transition(sl_power_manager_em_t from,
                                            sl_power_manager_em_t to);

#ifdef __cplusplus
}
#endif

#endif /* SLI_POWER_MANAGER_H */
