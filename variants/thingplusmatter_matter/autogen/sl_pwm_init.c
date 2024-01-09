/***************************************************************************//**
 * @file
 * @brief PWM Driver Instance Initialization
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_pwm.h"

#include "sl_pwm_init_led0_config.h"


#include "em_gpio.h"


sl_pwm_instance_t sl_pwm_led0 = {
  .timer = SL_PWM_LED0_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_LED0_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_LED0_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_LED0_OUTPUT_PIN),
#if defined(SL_PWM_LED0_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_LED0_OUTPUT_LOC),
#endif
};


void sl_pwm_init_instances(void)
{

  sl_pwm_config_t pwm_led0_config = {
    .frequency = SL_PWM_LED0_FREQUENCY,
    .polarity = SL_PWM_LED0_POLARITY,
  };

  sl_pwm_init(&sl_pwm_led0, &pwm_led0_config);

}
