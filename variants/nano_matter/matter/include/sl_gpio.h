/***************************************************************************//**
 * @file
 * @brief General Purpose IO (GPIO) driver API
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_GPIO_H
#define SL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sl_status.h"
#include "sl_device_gpio.h"

#ifndef EM_GPIO_H
#define     gpioPortA  0
#define     gpioPortB  1
#define     gpioPortC  2
#define     gpioPortD  3
#define     gpioPortE  4
#define     gpioPortF  5
#define     gpioPortG  6
#define     gpioPortH  7
#define     gpioPortI  8
#define     gpioPortJ  9
#define     gpioPortK  10
#endif

/* *INDENT-OFF* */
// *****************************************************************************
/// @addtogroup gpio GPIO - General Purpose Input Output
/// @brief General Purpose Input Output driver
///
/// @li @ref gpio_intro
///
///@n @section gpio_intro Introduction
///  This module contains functions to control the GPIO peripheral of Silicon Labs 32-bit MCUs and SoCs.
///  The GPIO driver is used for external and EM4 interrupt configuration, port and pin configuration.
///  as well as manages the interrupt handler.
///
/// @{
// *****************************************************************************
/* *INDENT-ON* */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// GPIO Pin directions.
SL_ENUM(sl_gpio_pin_direction_t) {
  /// Input direction.
  SL_GPIO_PIN_DIRECTION_IN = 0,
  /// Output direction.
  SL_GPIO_PIN_DIRECTION_OUT
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Structure for GPIO port and pin configuration.
 ******************************************************************************/
typedef struct {
  sl_gpio_mode_t mode;
  sl_gpio_pin_direction_t direction;
} sl_gpio_pin_config_t;

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * GPIO interrupt callback function pointer.
 *
 * @param int_no The pin interrupt number to which the callback function is invoked for.
 * @param context Pointer to callback context.
 ******************************************************************************/
typedef void (*sl_gpio_irq_callback_t)(uint8_t int_no, void *context);

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * Initialization of GPIO driver module.
 *
 * @return SL_STATUS_OK if initialization is successful.
 ******************************************************************************/
sl_status_t sl_gpio_init(void);

/***************************************************************************//**
 * Sets the pin direction of GPIO pin.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[in] pin_dir Pin direction of GPIO pin.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin, direction parameters are invalid.
 *         SL_STATUS_INVALID_STATE if GPIO configuration is in lock state.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_direction(const sl_gpio_t *gpio,
                                      sl_gpio_pin_direction_t pin_dir);

/***************************************************************************//**
 * Set the pin mode and set/clear the pin for GPIO pin.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[in] mode The desired pin mode.
 * @param[in] output_value Value to set/clear for pin output on the port.
 *                         Determines the pull-up/pull-down direction of the pin for
 *                         some input mode configurations.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if any of the port, pin, mode parameters are invalid.
 *         SL_STATUS_INVALID_STATE if GPIO configuration is in locked state.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_mode(const sl_gpio_t *gpio,
                                 sl_gpio_mode_t mode,
                                 bool output_value);

/***************************************************************************//**
 * Gets the current configuration selected pin on selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[out] pin_config Pointer to pin configuration such as mode and direction.
 *                        Pointer acts as an output and returns the configuration of
 *                        selected pin on selected port.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if any of the port, pin parameters are invalid.
 *         SL_STATUS_NULL_POINTER if pin_config is passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_config(const sl_gpio_t *gpio,
                                   sl_gpio_pin_config_t *pin_config);

/***************************************************************************//**
 * Sets the selected pin of the selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin parameters are invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * Clears the selected pin of the selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin parameters are invalid.
 ******************************************************************************/
sl_status_t sl_gpio_clear_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * Toggles the state of selected pin on selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin parameters are invalid.
 ******************************************************************************/
sl_status_t sl_gpio_toggle_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * Gets the output state of selected pin on selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[out] pin_value Pointer to return output state of selected pin on selected port
 *                       when configured to output mode.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin parameters are invalid.
 *         SL_STATUS_NULL_POINTER if pin_value passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_output(const sl_gpio_t *gpio,
                                   bool *pin_value);

/***************************************************************************//**
 * Gets the input state of selected pin on selected port.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[out] pin_value Pointer to return input state of selected pin on selected port
 *                       when configured to input mode.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMATER if any of the port, pin parameters are invalid.
 *         SL_STATUS_NULL_POINTER if pin_value passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_input(const sl_gpio_t *gpio,
                                  bool *pin_value);

/***************************************************************************//**
 * Sets the selected pin(s) of selected port.
 *
 * @param[in] port The GPIO port to access.
 * @param[in] pins Bit mask for pins to set.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_port(sl_gpio_port_t port,
                             uint32_t pins);

/***************************************************************************//**
 * Clears the selected pin(s) of selected port.
 *
 * @param[in] port The GPIO Port to access.
 * @param[in] pins Bit mask for bits to clear.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_clear_port(sl_gpio_port_t port,
                               uint32_t pins);

/***************************************************************************//**
 * Gets the output state of pins of selected port.
 *
 * @param[in] gpio The GPIO Port to access.
 * @param[out] port_value Pointer to return output state of pins on selected port.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 *         SL_STATUS_NULL_POINTER if port_value passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_port_output(sl_gpio_port_t port,
                                    uint32_t *port_value);

/***************************************************************************//**
 * Gets the input state of pins of selected port.
 *
 * @param[in] gpio The GPIO Port to access.
 * @param[out] port_value Pointer to return output state of pins on selected port.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 *         SL_STATUS_NULL_POINTER if port_value passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_port_input(sl_gpio_port_t port,
                                   uint32_t *port_value);

/***************************************************************************//**
 * Configures the GPIO pin interrupt.
 *
 * @details By default, this function can be used to register a callback which shall be called upon
 *          interrupt generated for a given pin interrupt number and enables interrupt.
 *          This function configures and enables the external interrupt and performs
 *          callback registration.
 *          It is recommended to use sl_gpio_deconfigure_external_interrupt()
 *          to disable the interrupt and unregister the callback.
 *          see @ref sl_gpio_deconfigure_external_interrupt for more information.
 *          If a valid interrupt number is provided, operation will proceed accordingly.
 *          Otherwise, a valid interrupt number will be generated based on provided port and
 *          pin and used for subsequent operations.
 *
 * @note If the user has a valid interrupt number to provide as input, it can be used.
 *       If the user does not have an interrupt number, they can pass -1 (SL_GPIO_INTERRUPT_UNAVAILABLE)
 *       as value to variable int_no.
 *       The int_no parameter serves even as an output, a pointer to convey the interrupt number
 *       for cases where user lacks an interrupt number.
 * @note the pin number can be selected freely within a group.
 *       Interrupt numbers are divided into 4 groups (int_no / 4) and valid pin
 *       number within the interrupt groups are:
 *       0: pins 0-3   (interrupt number 0-3)
 *       1: pins 4-7   (interrupt number 4-7)
 *       2: pins 8-11  (interrupt number 8-11)
 *       3: pins 12-15 (interrupt number 12-15)
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[in/out] int_no Pointer to interrupt number to trigger.
 *                       Pointer that serves as both an input and an output to return int_no
 *                       when the user lacks an int_no.
 * @param[in] flags Interrupt flags for interrupt configuration.
 *                  Determines the interrupt to get trigger based on rising/falling edge.
 * @param[in] gpio_callback A pointer to gpio callback function.
 * @param[in] context A pointer to the callback context.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if any of the port, pin, flag parameters are invalid.
 *         SL_STATUS_NULL_POINTER if the int_no is passed as NULL.
 *         SL_STATUS_NOT_FOUND if there's no available interrupt number.
 ******************************************************************************/
sl_status_t sl_gpio_configure_external_interrupt(const sl_gpio_t *gpio,
                                                 int32_t *int_no,
                                                 sl_gpio_interrupt_flag_t flags,
                                                 sl_gpio_irq_callback_t gpio_callback,
                                                 void *context);

/***************************************************************************//**
 * Deconfigures the GPIO external pin interrupt.
 *
 * @details This function can be used to deconfigure the external GPIO interrupt.
 *          This function performs callback unregistration, clears and disables the
 *          given interrupt.
 *
 * @note the pin number can be selected freely within a group.
 *       Interrupt numbers are divided into 4 groups (int_no / 4) and valid pin
 *       number within the interrupt groups are:
 *       0: pins 0-3   (interrupt number 0-3)
 *       1: pins 4-7   (interrupt number 4-7)
 *       2: pins 8-11  (interrupt number 8-11)
 *       3: pins 12-15 (interrupt number 12-15)
 *
 * @param[in] int_no Interrupt number to unregister and disable.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if int_no is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_deconfigure_external_interrupt(int32_t int_no);

/***************************************************************************//**
 * Enables one or more GPIO Interrupts.
 *
 * @param[in] int_mask Mask for GPIO Interrupt sources to enable.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_enable_interrupts(uint32_t int_mask);

/***************************************************************************//**
 * Disables one or more GPIO Interrupts.
 *
 * @param[in] int_mask Mask for GPIO Interrupt sources to disable.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_disable_interrupts(uint32_t int_mask);

/***************************************************************************//**
 * Configuration EM4WU pins as external level-sensitive interrupts.
 *
 * @details By default, this function performs callback registration, enables GPIO pin wake-up from EM4,
 *          sets the wake-up polarity, enables GPIO pin retention and enables the EM4 wake-up interrupt.
 *          It is recommended to use sl_gpio_deconfigure_wakeup_em4_interrupt()
 *          to unregister the callback and disable the em4 interrupt as well as GPIO pin wake-up from EM4.
 *          It is recommended to use sl_gpio_set_pin_em4_retention() to enable/disable the GPIO pin retention.
 *          see @ref sl_gpio_deconfigure_wakeup_em4_interrupt() and @ref sl_gpio_set_pin_em4_retention().
 *          If a valid EM4 wake-up interrupt number is provided, operation will proceed accordingly.
 *          Otherwise, a valid EM4 interrupt number will be generated based on provided EM4 configured
 *          port and pin and used for subsequent operations.
 *
 * @note If the user has a valid em4 interrupt number to provide as input, it can be used.
 *       If the user does not have an interrupt number, they can pass -1 (SL_GPIO_INTERRUPT_UNAVAILABLE)
 *       as value to variable em4_int_no.
 *       The em4_int_no parameter serves even as an output, a pointer to convey the em4 interrupt number
 *       for cases where user lacks an em4 interrupt number.
 * @note There are specific ports and pins mapped to an existent EM4WU interrupt
 *       Each EM4WU signal is connected to a fixed pin and port.
 *       Based on chip, EM4 wake up interrupts configured port and pin might vary.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 * @param[in/out] em4_int_no Pointer to interrupt number to trigger.
 *                           Pointer that serves as both an input and an output to return em4_int_no
 *                           when the user lacks an em4_int_no.
 * @param[in] polarity Determines the wakeup polarity.
 *                     true = Active high level-sensitive interrupt.
 *                     false = Active low level-sensitive interrupt.
 * @param[in] gpio_callback A pointer to callback.
 * @param[in] context A pointer to callback context.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if any of the port, pin parameters are invalid.
 *         SL_STATUS_NULL_POINTER if the int_no is passed as NULL.
 *         SL_STATUS_NOT_FOUND if there's no available interrupt number.
 ******************************************************************************/
sl_status_t sl_gpio_configure_wakeup_em4_interrupt(const sl_gpio_t *gpio,
                                                   int32_t *em4_int_no,
                                                   bool polarity,
                                                   sl_gpio_irq_callback_t gpio_callback,
                                                   void *context);

/***************************************************************************//**
 * Utilize this function to deconfigure the EM4 GPIO pin interrupt.
 * It serves to unregister a callback, disable/clear interrupt and clear em4 wakeup source.
 *
 * @details This function performs callback unregistration, clears and disables given em4
 *          interrupt and disables GPIO pin wake-up from EM4.
 *
 * @param[in] em4_int_no EM4 wakeup interrupt number.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if em4_int_no is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_deconfigure_wakeup_em4_interrupt(int32_t em4_int_no);

/***************************************************************************//**
 * Enable EM4 GPIO pin Wake-up bit.
 * Sets the wakeup and polarity of the EM4 wakeup.
 *
 * @param[in] em4_int_mask Mask for setting desired EM4 wake up interrupt to enable.
 *                         Mask contains the bitwise logic OR of which EM4 wake up interrupt to
 *                         enable.
 * @param[in] em4_polarity_mask Mask for setting the wake up polarity for the EM4 wake up interrupt.
 *                              Mask contains the bitwise logic OR of EM4 wake-up interrupt polarity.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_enable_pin_em4_wakeup(uint32_t em4_int_mask,
                                          uint32_t em4_polarity_mask);

/***************************************************************************//**
 * Disabled the GPIO wake up from EM4.
 *
 * @param[in] pinmask Mask for clearing desired EM4 wake up interrupt to disable.
 *                    Mask contains the bitwise logic OR of which EM4 wake up interrupt to
 *                    disable.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_disable_pin_em4_wakeup(uint32_t em4_int_mask);

/***************************************************************************//**
 * Enable/Disable GPIO pin retention of output enable, output value, pull enable, and pull direction in EM4.
 *
 * @param[in] enable true - enables EM4 pin retention.
 *                   false - disables EM4 pin retention.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_em4_retention(bool enable);

/***************************************************************************//**
 * Sets slewrate for selected port.
 *
 * @param[in] port The GPIO port to configure.
 * @param[in] slewrate The slewrate to configure the GPIO port.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_slew_rate(sl_gpio_port_t port,
                                  uint8_t slewrate);

/***************************************************************************//**
 * Gets slewrate for selected port.
 *
 * @param[in] port The GPIO port to get slewrate.
 * @param[out] slewrate Pointer to store the slewrate of selected port.
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_INVALID_PARAMETER if port is invalid.
 *         SL_STATUS_NULL_POINTER if slewrate is passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_get_slew_rate(sl_gpio_port_t port,
                                  uint8_t *slewrate);

/***************************************************************************//**
 * Locks the GPIO Configuration.
 *
 * @note This API locks the functionalities such as sl_gpio_set_pin_mode(),
 *       sl_gpio_configure_external_interrupt() and sl_gpio_configure_wakeup_em4_interrupt().
 *       After locking the GPIO configuration, use sl_gpio_unlock API to unlock
 *       the GPIO configuration to use mentioned functionalities.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_lock(void);

/***************************************************************************//**
 * Unlocks the GPIO Configuration.
 *
 * @note After locking the GPIO configuration it is recommended to unlock the GPIO configuration
 *       using sl_gpio_unlock(). You can determine if the GPIO configuration is locked or unlocked
 *       by using the sl_gpio_is_locked() function.
 *       Before using certain functions like sl_gpio_set_pin_mode(),
 *       sl_gpio_configure_external_interrupt(), and sl_gpio_configure_wakeup_em4_interrupt(),
 *       it's important to check if the GPIO configuration lock is unlocked.
 *
 * @return SL_STATUS_OK if there's no error.
 ******************************************************************************/
sl_status_t sl_gpio_unlock(void);

/***************************************************************************//**
 * Gets current GPIO Lock status.
 *
 * @note This function helps check the current status of GPIO configuration.
 *
 * @param[out] state Pointer to current state of GPIO configuration (lock/unlock).
 *
 * @return SL_STATUS_OK if there's no error.
 *         SL_STATUS_NULL_POINTER if state is passed as null.
 ******************************************************************************/
sl_status_t sl_gpio_is_locked(bool *state);

/** @} (end addtogroup gpio driver) */
#ifdef __cplusplus
}
#endif

#endif /* SL_GPIO_H */
