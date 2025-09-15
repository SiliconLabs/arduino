/***************************************************************************//**
 * @file
 * @brief Clock Manager HAL APIs.
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

#ifndef SL_CLOCK_MANAGER_HAL_H
#define SL_CLOCK_MANAGER_HAL_H

#include "sl_clock_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Performs Clock Manager runtime initialization.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_runtime_init(void);

/***************************************************************************//**
 * Gets frequency of given oscillator.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_get_oscillator_frequency(sl_oscillator_t oscillator,
                                                           uint32_t        *frequency);

/***************************************************************************//**
 * Gets precision of given oscillator.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_oscillator_precision(sl_oscillator_t oscillator,
                                                           uint16_t        *precision);

/***************************************************************************//**
 * Gets frequency of given clock branch.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_get_clock_branch_frequency(sl_clock_branch_t clock_branch,
                                                             uint32_t          *frequency);

/***************************************************************************//**
 * Gets precision of given clock branch.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_clock_branch_precision(sl_clock_branch_t clock_branch,
                                                             uint16_t          *precision);

/***************************************************************************//**
 * Enables/Disables the bus clock associated with the given module.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_enable_bus_clock(sl_bus_clock_t module,
                                                   bool enable);

/***************************************************************************//**
 * Configures the exported clock feature on CMU to output user selected
 * clock source specified GPIO pin.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_gpio_clock_output(sl_clock_manager_export_clock_source_t export_clock_source,
                                                        sl_clock_manager_export_clock_output_select_t output_select,
                                                        uint16_t divider,
                                                        uint32_t port,
                                                        uint32_t pin);

/***************************************************************************//**
 * Sets the RC oscillator frequency tuning control.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                                uint32_t val);

/***************************************************************************//**
 * Gets the RC oscillator frequency tuning setting.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                                uint32_t *val);

/***************************************************************************//**
 * Sets the HFXO calibration value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_hfxo_calibration(uint32_t val);

/***************************************************************************//**
 * Gets the HFXO calibration value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_hfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * Sets the HFXO CTUNE setting.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_set_ctune(uint32_t ctune);

/***************************************************************************//**
 * Gets the HFXO CTUNE setting.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_get_ctune(uint32_t *ctune);

/***************************************************************************//**
 * Updates the tuning capacitances and calibrate the Core Bias Current.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_calibrate_ctune(uint32_t ctune);

/***************************************************************************//**
 * Sets the LFXO frequency tuning control.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_lfxo_calibration(uint32_t val);

/***************************************************************************//**
 * Gets the LFXO frequency tuning setting.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_lfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * Configures the RCO calibration.
 *****************************************************************************/
sl_status_t sli_clock_manager_hal_configure_rco_calibration(uint32_t cycles,
                                                            sl_clock_manager_clock_calibration_t down_counter_selection,
                                                            sl_clock_manager_clock_calibration_t up_counter_selection,
                                                            bool continuous_calibration);

/***************************************************************************//**
 * Starts the RCO calibration.
 ******************************************************************************/
void sli_clock_manager_hal_start_rco_calibration (void);

/***************************************************************************//**
 * Stops the RCO calibration.
 ******************************************************************************/
void sli_clock_manager_hal_stop_rco_calibration(void);

/***************************************************************************//**
 * Waits for the RCO calibration to finish.
 ******************************************************************************/
void sli_clock_manager_hal_wait_rco_calibration(void);

/***************************************************************************//**
 * Gets calibration count value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_rco_calibration_count(uint32_t *count);

/***************************************************************************//**
 * Sets SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_set_sysclk_source(sl_oscillator_t source);

/***************************************************************************//**
 * Gets SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_get_sysclk_source(sl_oscillator_t *source);

/***************************************************************************//**
 * Waits for USBPLL clock to be ready.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_wait_usbpll(void);

/***************************************************************************//**
 * Sets the external FLASH reference clock.
 *
 * @note This API is not thread-safe and should therefore not be called
 *       across multiple tasks.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_set_ext_flash_clk(sl_oscillator_t oscillator);

/***************************************************************************//**
 * Gets the external FLASH clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sli_clock_manager_hal_get_ext_flash_clk(sl_oscillator_t *oscillator);

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_HAL_H
