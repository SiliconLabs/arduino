/***************************************************************************//**
 * @file
 * @brief SLEEPTIMER hardware abstraction layer definition.
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

#ifndef SL_SLEEPTIMER_HAL_H
#define SL_SLEEPTIMER_HAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "em_device.h"
#include "sli_sleeptimer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Hardware Abstraction Layer of the sleep timer init.
 ******************************************************************************/
void sleeptimer_hal_init_timer(void);

/*******************************************************************************
 * Hardware Abstraction Layer to get the current timer count.
 *
 * @return Value in ticks of the timer counter.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sleeptimer_hal_get_counter(void);

/*******************************************************************************
 * Hardware Abstraction Layer to get a timer comparator value.
 *
 * @return Value in ticks of the timer comparator.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sleeptimer_hal_get_compare(void);

/*******************************************************************************
 * Hardware Abstraction Layer to set a timer comparator value.
 *
 * @param value Number of ticks to set.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_set_compare(uint32_t value);

/*******************************************************************************
 * Hardware Abstraction Layer to set a comparator value to trigger a
 * peripheral request signal to initialize.
 *
 * @param value Number of ticks to set.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_set_compare_prs_hfxo_startup(int32_t value);

/*******************************************************************************
 * Hardware Abstraction Layer to get the timer frequency.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sleeptimer_hal_get_timer_frequency(void);

/*******************************************************************************
 * Hardware Abstraction Layer to enable timer interrupts.
 *
 * @param local_flag Internal interrupt flag.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_enable_int(uint8_t local_flag);

/*******************************************************************************
 * Hardware Abstraction Layer to disable timer interrupts.
 *
 * @param local_flag Internal interrupt flag.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_disable_int(uint8_t local_flag);

/*******************************************************************************
 * Hardware Abstraction Layer to set timer interrupts.
 *
 * @param local_flag Internal interrupt flag.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_set_int(uint8_t local_flag);

/*******************************************************************************
 * Hardware Abstraction Layer to get the sleeptimer's clock accuracy.
 *
 * @return Clock accuracy in PPM.
 ******************************************************************************/
uint16_t sleeptimer_hal_get_clock_accuracy(void);

/*******************************************************************************
 * Hardware Abstraction Layer to get the capture channel value.
 *
 * @note Not supported by all peripherals Sleeptimer can use.
 *
 * @return Capture value.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sleeptimer_hal_get_capture(void);

/*******************************************************************************
 * Hardware Abstraction Layer to reset PRS signal triggered by the associated
 * peripheral.
 *
 * @note Not supported by all peripherals Sleeptimer can use.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_reset_prs_signal(void);

/*******************************************************************************
 * Hardware Abstraction Layer to disable PRS compare and capture channel.
 *
 * @note Not supported by all peripherals Sleeptimer can use.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sleeptimer_hal_disable_prs_compare_and_capture_channel(void);

/*******************************************************************************
 * Process the timer interrupt.
 *
 * @param flags Internal interrupt flag.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void process_timer_irq(uint8_t local_flag);

/***************************************************************************//**
 * @brief
 *   Convert prescaler divider to a logarithmic value. It only works for even
 *   numbers equal to 2^n.
 *
 * @param[in] presc
 *   Prescaler value used to set the frequency divider. The divider is equal to
 *   ('presc' + 1). If a divider value is passed for 'presc', 'presc' will be
 *   equal to (divider - 1).
 *
 * @return
 *   Logarithm base 2 (binary) value, i.e. exponent as used by fixed
 *   2^n prescalers.
 ******************************************************************************/
__STATIC_INLINE uint32_t sleeptimer_hal_presc_to_log2(uint32_t presc)
{
  uint32_t log2;

  // Integer prescalers take argument less than 32768.
  EFM_ASSERT(presc < 32768U);

  // Count leading zeroes and "reverse" result. Consider divider value to get
  // exponent n from 2^n, so ('presc' +1).
  log2 = 31UL - __CLZ(presc + (uint32_t) 1);

  // Check that prescaler is a 2^n number.
  EFM_ASSERT(presc == (SL_Log2ToDiv(log2) - 1U));

  return log2;
}

#ifdef __cplusplus
}
#endif

#endif /* SL_SLEEPTIMER_HAL_H */
