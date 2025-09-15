/***************************************************************************//**
 * @file
 * @brief Clock Manager APIs.
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

#ifndef SL_CLOCK_MANAGER_H
#define SL_CLOCK_MANAGER_H

#include <stdbool.h>
#include <stdlib.h>
#include "sl_status.h"
#include "sl_enum.h"
#include "sl_device_clock.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup clock_manager Clock Manager
 *
 * @details
 * ## Overview
 *
 *  Clock Manager is a platform-level software module that manages
 *  the device's oscillators and clock tree.
 *  The Clock Manager module is split into two main parts: The Initialization part
 *  and the Runtime part. The runtime part has its component
 *  \a clock_manager_runtime and can be used independently from the initialization
 *  part. The \a clock_manager component includes both the initialization part and the
 *  runtime part and it should be the component added to your project slcp file.
 *
 *  ## Initialization
 *  The initialization part includes the configuration files
 *  \a sl_clock_manager_oscillator_config.h and \a sl_clock_manager_tree_config.h.
 *  As their name indicates, those C header files are used to configure the different
 *  device oscillators and the device clock tree. Those header files use the CMSIS
 *  Configuration Wizard Annotations and are specific to each device.
 *  The API function sl_clock_manager_init() is used to initialize the Clock Manager
 *  module based on the configuration values specified in the two configuration files.
 *  This function must be called early during your initialization sequence.
 *  If the SL System component (@ref system) is used by your application, the
 *  sl_clock_manager_init() call will be added automatically to your initialization
 *  sequence.
 *
 *  ### Oscillators Configuration
 *  Oscillators' configurations are all grouped in the \a sl_clock_manager_oscillator_config.h
 *  file. Crystal-based oscillators, HFXO and LFXO, have an enable/disable configuration to
 *  indicate if the required crystal is present or not. In the absence of the required
 *  crystal, the configuration must be disabled and the associated oscillator will
 *  not be initialized.
 *
 *  The HFXO configuration also provides the configuration for the Crystal Sharing
 *  feature when supported by the device. This feature allows to use the dedicated
 *  HFCLKOUT pin to output a sinusoidal clock that can be used as the HFXO
 *  input for another EFR device. In the configuration, you need to specify if your
 *  device is the leader or the follower. The leader will be the one outputting the
 *  clock and the follower, the one receiving the clock signal. In the leader configuration,
 *  the GPIO pin is used to receive the request from the follower. You can refer to your
 *  device datasheet to know the available location for the HFXO BUFOUT_REQ pin.
 *  In the follower mode, the pin configuration can be used to send an HFXO request
 *  signal to the leader. The "High Frequency Clock Ouput" section of your device
 *  reference manual also provides more details about this feature.
 *
 *  The first HFRCO module, whose output clock is called HFRCODPLL, can be connected to
 *  the DPLL module to have a better precision clock. When the DPLL is enabled through
 *  the configuration define \a SL_CLOCK_MANAGER_HFRCO_DPLL_EN, the DPLL settings
 *  take precedence over the HFRCO band configuration.
 *
 *  ### Clock Tree Configuration
 *  The device clock tree configurations are all grouped in the
 *  \a sl_clock_manager_tree_config.h file. Refer to your device's reference manual for
 *  the clock tree diagram and see which peripherals are connected to which clock branches.
 *  In the configuration file, each clock branch can be independently configured.
 *  However, to facilitate the clock setup for users, two additional configurations
 *  were added: \a SL_CLOCK_MANAGER_DEFAULT_HF_CLOCK_SOURCE and
 *  \a SL_CLOCK_MANAGER_DEFAULT_LF_CLOCK_SOURCE. Those configurations allow the selection
 *  of the default high-frequency and low-frequency oscillators to be used inside the clock
 *  tree. Every clock branch that can benefit from those default selections will use them
 *  by default.
 *  On certain devices, the @ref power_manager module offers an Execution Modes feature
 *  with the \a SL_POWER_MANAGER_EXECUTION_MODES_FEATURE_EN configuration. When this feature
 *  is enabled, the \a SL_CLOCK_MANAGER_SYSCLK_SOURCE configuration could be overriden by the
 *  Execution Modes feature. Refer to the description of \a SL_CLOCK_MANAGER_SYSCLK_SOURCE in
 *  \a sl_clock_manager_tree_config.h file to know if this is the case.
 *
 *  Some peripherals have an internal clock divider. Those are not handled by the
 *  Clock Manager configuration. The peripheral driver will usually expose the divider
 *  configuration when such a divider is present inside the peripheral module.
 *
 *  The Clock tree configuration is available at compile-time only. The Clock Manager
 *  module does not offer API functions to manipulate the clock tree at runtime.
 *
 *  The more oscillators are used by different clock branches the more power
 *  consumption you will have. To limit your power consumption, you can try to limit the
 *  number of oscillators used. So for example, only use one High-frequency oscillator
 *  and one Low-frequency oscillator across the clock tree. However, if the
 *  application is radio-based, the HFXO oscillator is mandatory for the Radio
 *  clock branch and if the Radio clock branch is connected to the SYSCLK branch,
 *  this will limit you to use HFXO for SYSCLK as well. In this specific case, SYSCLK
 *  could also be configured to use HFRCO with DPLL, but the chosen frequency must be two
 *  times the HFXO frequency so that the Radio module can retrieve the HF crystal frequency
 *  with its divider. This will also come with an increase in power consumption since both
 *  HFXO and HFRCO oscillators will be used. Refer to your device reference manual to know
 *  if your Radio clock is connected to the SYSCLK clock branch or not.
 *
 *  @note The Clock Manager Initialization is incompatible with the \a device_init_clocks
 *        (@ref device_init_clocks), \a device_init_hfxo (@ref device_init_hfxo),
 *        \a device_init_hfrco (@ref device_init_hfrco) \a device_init_dpll (@ref device_init_dpll),
 *        \a device_init_rffpll (@ref device_init_rffpll), \a device_init_usbpll (@ref device_init_usbpll),
 *        \a device_init_lfxo (@ref device_init_lfxo) and \a device_init_lfrco (@ref device_init_lfrco)
 *        components.
 *        This does not mean that the \a device_init component (@ref device_init) is incompatible with the
 *        \a clock_manager component. The \a device_init component can pull other initialization
 *        modules like EMU and DCDC that are not related to clocks. Therefore, both
 *        \a device_init and \a clock_manager should be present in your project file. SLC will
 *        take care of pulling only the sub \a device_init_xxx components that are needed.
 *
 *  The runtime part, which is associated with the \a clock_manager_runtime component,
 *  has also an initialization function of its own, sl_clock_manager_runtime_init().
 *  This function must also be part of the initialization sequence. If the SL System
 *  component (@ref system) is used by your application, the
 *  sl_clock_manager_runtime_init() call will be added automatically to your
 *  initialization sequence.
 *
 *  ## Functionalities
 *  The Runtime part includes functionalities related to oscillators, clock tree
 *  and the CMU hardware module features. The main functionalities are:
 *    - Retrieving the frequency or precision of an oscillator or clock branch
 *    - Enabling/Disabling modules' bus clock
 *    - Retrieving or setting calibration values for oscillators
 *    - Exporting clocks to GPIO
 *    - Starting an RCO Calibration process based on a reference clock source
 *
 *  ### Retrieve the frequency or precision of an oscillator or clock branch
 *  API functions sl_clock_manager_get_oscillator_frequency() and
 *  sl_clock_manager_get_oscillator_precision() allow retrieving respectively
 *  the frequency and precision of a given oscillator. Similar functions
 *  exist for clock branches: sl_clock_manager_get_clock_branch_frequency() and
 *  sl_clock_manager_get_clock_branch_precision().
 *
 *  To retrieve the frequency or precision of a specific peripheral, you will
 *  first need to retrieve to which clock branch this peripheral is connected.
 *  To do so, the Device Manager and its @ref device_peripheral
 *  can be used. The below code example shows how to retrieve the clock branch
 *  of the TIMER0 peripheral.
 *
 *  @code{.c}
 *  #include "sl_clock_manager.h"
 *  #include "sl_device_peripheral.h"
 *
 *  sl_status_t status;
 *  uint32_t freq;
 *  sl_clock_branch_t clock_branch;
 *
 *  clock_branch = sl_device_peripheral_get_clock_branch(SL_PERIPHERAL_TIMER0);
 *  status = sl_clock_manager_get_clock_branch_frequency(clock_branch, &freq);
 *  @endcode
 *
 * ### Enable/Disable modules' bus clock
 * Before accessing a peripheral's register interface, its bus clock must be enabled,
 * or else a bus fault exception will be triggered. API functions
 * sl_clock_manager_enable_bus_clock() and sl_clock_manager_disable_bus_clock()
 * allow to perform such operations.
 *
 * Note that the peripheral clock will automatically be enabled when a peripheral
 * is enabled with the clock on-demand feature.
 *
 * ### Oscillator Calibration
 * The Clock Manager initialization, if present, will calibrate the different
 * oscillators during the initialization sequence, but sometimes calibration
 * values must be updated during runtime in certain conditions, for example, if
 * the device temperature changes too much. This is considered an advanced
 * functionality and users must be careful as to when to use this functionality.
 *
 * API functions sl_clock_manager_set_rc_oscillator_calibration() and
 * sl_clock_manager_get_rc_oscillator_calibration() allow to set or get the
 * CAL register of HFRCO and LFRCO oscillators. Not all devices have an LFRCO
 * module with a CAL register. Some LFRCO modules will have a high-precision
 * configuration allowing to use the HFXO to auto-calibrate the LFRCO. Refer
 * to your device reference manual to retrieve oscillator specifications.
 *
 * API functions sl_clock_manager_set_hfxo_calibration() and
 * sl_clock_manager_get_hfxo_calibration() allow to set or get the \a COREBIASANA
 * inside the HFXO \a XTALCTRL register. The HFXO module has a Core Bias Optimization
 * stage at the end of the oscillator startup sequence that allows to further
 * optimize current consumption. This optimization will automatically set the
 * \a COREBIASANA bitfield when finished. Upon reset, this optimization will run
 * the first time HFXO is started and afterwards, the \a XTALCTRL->SKIPCOREBIASOPT
 * bit will automatically be set so that next time HFXO is started during the
 * application lifetime, the optimization stage will be skipped. This optimization
 * stage takes a while to run, in the order of hundreds of milliseconds, therefore
 * we don't want it to run each time HFXO is started.
 * With the function sl_clock_manager_set_hfxo_calibration() it is possible to
 * manually set the \a COREBIASANA bitfield and set the \a SKIPCOREBIASOPT bit.
 * This function will usually be used in the context of an EM4 wake-up where to
 * save on the initialization sequence time, we want to skip the Core Bias Optimization
 * stage and manually set the value that would have previously been retrieved with
 * sl_clock_manager_get_hfxo_calibration() and saved in an EM4 retained memory.
 * In this context, sl_clock_manager_set_hfxo_calibration() will need to be called
 * early in the initialization sequence, before the usual clock initialization
 * function.
 *
 * slx_clock_manager_hfxo_set_ctune(), slx_clock_manager_hfxo_get_ctune() and
 * slx_clock_manager_hfxo_calibrate_ctune() functions allow to manipulate the
 * HFXO tuning capacitances. Changing the CTUNE value while HFXO is running
 * can result in significant clock glitches for one clock period. Therefore,
 * those functions should be used with caution. The difference between the
 * slx_clock_manager_hfxo_set_ctune() and slx_clock_manager_hfxo_calibrate_ctune()
 * functions is that the calibration one will also start and wait for the HFXO
 * Core Bias Optimization stage to complete.
 *
 * API functions sl_clock_manager_set_lfxo_calibration() and
 * sl_clock_manager_get_lfxo_calibration() allow to set and get the LFXO CTUNE
 * value.
 *
 * ### Export clocks to GPIO
 * The CMU module offers the functionality to export a given clock source to a
 * GPIO pin. Refer to function sl_clock_manager_set_gpio_clock_output() for more
 * details and the #sl_clock_manager_export_clock_source_t enum for a list of
 * acceptable clock sources. Note that there is a specific clock branch named
 * EXPCLK that is usually connected to the SYSCLK and offers an additional divider.
 *
 * ### RCO Calibration
 * The CMU module also offers RCO Calibration hardware support. This can be
 * used to calibrate at runtime HFRCO and LFRCO oscillators using a high-precision
 * reference clock. Refer to your device reference manual for more
 * details about this functionality. API function
 * sl_clock_manager_configure_rco_calibration() can be used to configure the
 * calibration process. Then sl_clock_manager_start_rco_calibration() and
 * sl_clock_manager_stop_rco_calibration() can be called to start/stop the
 * process. sl_clock_manager_wait_rco_calibration() function can be called to
 * actively wait for the process to finish. And finally,
 * sl_clock_manager_get_rco_calibration_count() can be called to retrieve the
 * calibration process result.
 *
 * @{
 ******************************************************************************/

/// Export clock source.
/// This is to be used with the sl_clock_manager_set_gpio_clock_output() API function.
SL_ENUM(sl_clock_manager_export_clock_source_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_DISABLED,   ///< Export Clock Source Disabled
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_FSRCO,      ///< Export Clock Source FSRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFXO,       ///< Export Clock Source HFXO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCODPLL,  ///< Export Clock Source HFRCODPLL
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCOEM23,  ///< Export Clock Source HFRCOEM23
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFEXPCLK,   ///< Export Clock Source HFEXPCLK
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFXO,       ///< Export Clock Source LFXO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_PLFRCO,     ///< Export Clock Source PLFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFRCO,      ///< Export Clock Source LFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_ULFRCO,     ///< Export Clock Source ULFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HCLK,       ///< Export Clock Source HCLK
};

/// Export clock output selection.
/// This is to be used with the sl_clock_manager_set_gpio_clock_output() API function.
SL_ENUM(sl_clock_manager_export_clock_output_select_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_0 = 0,  ///< Export Clock Output #0
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_1,      ///< Export Clock Output #1
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_2,      ///< Export Clock Output #2
};

/// Clocks available for Calibration.
/// This is to be used with the sl_clock_manager_configure_rco_calibration() API function.
SL_ENUM(sl_clock_manager_clock_calibration_t) {
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HCLK,       ///< Clock Calibration HCLK
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_PRS,        ///< Clock Calibration PRS
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFXO,       ///< Clock Calibration HFXO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFXO,       ///< Clock Calibration LFXO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCODPLL,  ///< Clock Calibration HFRCODPLL
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCOEM23,  ///< Clock Calibration HFRCOEM23
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_FSRCO,      ///< Clock Calibration FSRCO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFRCO,      ///< Clock Calibration LFRCO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_ULFRCO      ///< Clock Calibration ULFRCO
};

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * Performs Clock Manager runtime initialization.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_runtime_init(void);

/***************************************************************************//**
 * Gets frequency of given oscillator.
 *
 * @param[in] oscillator  Oscillator
 *
 * @param[out] frequency  Oscillator's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sl_clock_manager_get_oscillator_frequency(sl_oscillator_t oscillator,
                                                      uint32_t *frequency);

/***************************************************************************//**
 * Gets precision of given oscillator.
 *
 * @param[in] oscillator  Oscillator
 *
 * @param[out] precision  Oscillator's precision in PPM
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_oscillator_precision(sl_oscillator_t oscillator,
                                                      uint16_t *precision);

/***************************************************************************//**
 * Gets frequency of given clock branch.
 *
 * @param[in] clock_branch  Clock Branch
 *
 * @param[out] frequency    Clock Branch's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sl_clock_manager_get_clock_branch_frequency(sl_clock_branch_t clock_branch,
                                                        uint32_t *frequency);

/***************************************************************************//**
 * Gets precision of given clock branch.
 *
 * @param[in] clock_branch  Clock Branch
 *
 * @param[out] precision    Clock Branch's precision in PPM
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_clock_branch_precision(sl_clock_branch_t clock_branch,
                                                        uint16_t *precision);

/***************************************************************************//**
 * Enables the given module's bus clock.
 *
 * @param[in] module_bus_clock  module's bus clock to enable.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note modules' bus clocks are defined in the
 *       @ref device_clock in the Bus Clock Defines section.
 ******************************************************************************/
sl_status_t sl_clock_manager_enable_bus_clock(sl_bus_clock_t module_bus_clock);

/***************************************************************************//**
 * Disables the given module's bus clock.
 *
 * @param[in] module_bus_clock  module's bus clock to disable.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note modules' bus clocks are defined in the
 *       @ref device_clock in the Bus Clock Defines section.
 ******************************************************************************/
sl_status_t sl_clock_manager_disable_bus_clock(sl_bus_clock_t module_bus_clock);

/***************************************************************************//**
 * Configures one clock export output with specified clock source.
 *
 * @param[in] export_clock_source One of the exportable clock source.
 *
 * @param[in] output_select       Selected export clock output channel.
 *
 * @param[in] hfexp_divider       HFEXP clock divider (1 to 32).
 *                                Note: This parameter only affects the EXPCLK
 *                                      branch frequency.
 *
 * @param[in] port                GPIO port to output exported clock.
 *
 * @param[in] pin                 GPIO pin number to output exported clock.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_gpio_clock_output(sl_clock_manager_export_clock_source_t export_clock_source,
                                                   sl_clock_manager_export_clock_output_select_t output_select,
                                                   uint16_t hfexp_divider,
                                                   uint32_t port,
                                                   uint32_t pin);

/***************************************************************************//**
 * Sets the RC oscillator frequency tuning control.
 *
 * @param[in] oscillator  RC Oscillator to set tuning value for.
 *
 * @param[in] val  The RC oscillator frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note RC Oscillator tuning is done during production, and the tuning value is
 *       loaded after a reset by the Clock Manager initialization code.
 *       Changing the tuning value from the calibrated value is for more advanced
 *       use. Certain oscillators also have build-in tuning optimization.
 *
 * @note Supported RC oscillators include:
 *        - SL_OSCILLATOR_HFRCODPLL
 *        - SL_OSCILLATOR_HFRCOEM23
 *        - SL_OSCILLATOR_LFRCO
 ******************************************************************************/
sl_status_t sl_clock_manager_set_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t val);

/***************************************************************************//**
 * Gets the RC oscillator frequency tuning setting.
 *
 * @param[in] oscillator  An RC oscillator to get tuning value for.
 *
 * @param[out] val The RC oscillator frequency tuning setting in use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note Supported RC oscillators include:
 *       - SL_OSCILLATOR_HFRCODPLL
 *       - SL_OSCILLATOR_HFRCOEM23
 *       - SL_OSCILLATOR_LFRCO
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t *val);

/***************************************************************************//**
 * Sets the HFXO calibration value.
 *
 * @param[in] val
 *   The HFXO calibration setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_hfxo_calibration(uint32_t val);

/***************************************************************************//**
 * Gets the HFXO calibration value.
 *
 * @param[out] val  The current HFXO calibration value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_hfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * Sets the HFXO's CTUNE.
 *
 * @param[in] ctune  The HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Sets the XI value to the given ctune value and sets the XO value based
 *        on that same value, but with an offset that is hardware dependent.
 *        Updating CTune while the crystal oscillator is running can
 *        result in significant clock glitches for one XO clock period.
 *        Should be used with caution.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_set_ctune(uint32_t ctune);

/***************************************************************************//**
 * Gets the HFXO's CTUNE.
 *
 * @param[out] ctune  The returned HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function only returns the CTUNE XI value.
 *        The XO value follows the XI value with a fixed delta that is
 *        hardware dependent.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_get_ctune(uint32_t *ctune);

/***************************************************************************//**
 * Updates the tuning capacitances and calibrate the Core Bias Current.
 *
 * @param[in] ctune  The HFXO's CTUNE value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  Calibrating the CTUNE is time consuming and will cause glitches on the
 *        HFXO's clock. Care and caution should be taken when using this API.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_calibrate_ctune(uint32_t ctune);

/***************************************************************************//**
 * Sets the LFXO frequency tuning control.
 *
 * @param[in] val  The LFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_lfxo_calibration(uint32_t val);

/***************************************************************************//**
 * Gets the LFXO frequency tuning setting.
 *
 * @param[out] val  The LFXO frequency tuning setting to use.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_lfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * Configures the RCO calibration.
 *
 * @param[in] cycles  Number of cycles to run calibration. Increasing this
 *                    number increases precision, but the calibration will
 *                    take more time.
 *
 * @param[in] down_counter_selection
 *   The clock which will be counted down cycles.
 *
 * @param[in] up_counter_selection
 *   The number of cycles generated by this clock will be counted and
 *   added up, the result can be given with
 *   sl_clock_manager_get_rco_calibration_count().
 *
 * @param[in] continuous_calibration
 *    Flag when true configures continuous calibration.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note RCO calibration related functions are not thread-safe and should
 *       therefore not be called across multiple tasks.
 ******************************************************************************/
sl_status_t sl_clock_manager_configure_rco_calibration(uint32_t cycles,
                                                       sl_clock_manager_clock_calibration_t down_counter_selection,
                                                       sl_clock_manager_clock_calibration_t up_counter_selection,
                                                       bool continuous_calibration);

/***************************************************************************//**
 * Starts the RCO calibration.
 *
 * @note RCO calibration related functions are not thread-safe and should
 *       therefore not be called across multiple tasks.
 ******************************************************************************/
void sl_clock_manager_start_rco_calibration(void);

/***************************************************************************//**
 * Stops the RCO calibration.
 *
 * @note RCO calibration related functions are not thread-safe and should
 *       therefore not be called across multiple tasks.
 ******************************************************************************/
void sl_clock_manager_stop_rco_calibration(void);

/***************************************************************************//**
 * Waits for the RCO calibration to finish.
 *
 * @note RCO calibration related functions are not thread-safe and should
 *       therefore not be called across multiple tasks.
 ******************************************************************************/
void sl_clock_manager_wait_rco_calibration(void);

/***************************************************************************//**
 * Gets calibration count value, returns the value of the up counter.
 *
 * @param[out] count  Calibration count value.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note RCO calibration related functions are not thread-safe and should
 *       therefore not be called across multiple tasks.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rco_calibration_count(uint32_t *count);

/***************************************************************************//**
 * Waits for USBPLL clock to be ready.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_wait_usbpll(void);

/***************************************************************************//**
 * When this callback function is called, it means that HFXO failed twice in
 * a row to start with normal configurations. This may mean that there is a
 * bad crystal. When getting this callback, HFXO is running but its properties
 * (frequency, precision) are not guaranteed. This should be considered as an
 * error situation.
 *
 * @note This callback will be called only when the
 *       SL_CLOCK_MANAGER_HFXO_SLEEPY_CRYSTAL_SUPPORT config is enabled
 ******************************************************************************/
void sl_clock_manager_hfxo_notify_consecutive_failed_startups(void);

/***************************************************************************//**
 * Sets the external FLASH reference clock.
 *
 * @param[in] oscillator  Oscillator used to clock the external FLASH.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note This API is not thread-safe and should therefore not be called
 *       across multiple tasks.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sl_clock_manager_set_ext_flash_clk(sl_oscillator_t oscillator);

/***************************************************************************//**
 * Gets the external FLASH clock source.
 *
 * @param[out] oscillator Oscillator used to clock the external FLASH.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
sl_status_t sl_clock_manager_get_ext_flash_clk(sl_oscillator_t *oscillator);

/** @} (end addtogroup clock_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_H
