/***************************************************************************//**
 * @file
 * @brief RAM and peripheral bit-field set, clear, read and write API.
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

#ifndef SL_HAL_BUS_H
#define SL_HAL_BUS_H

#include "sl_assert.h"
#include "sl_core.h"
#include "em_device.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup bus BUS - Bitfield Read/Write
 * @brief BUS register and RAM bit-field read/write API
 * @details
 *  API to perform field set/clear/write/read access to RAM and peripheral's registers.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Perform a single-bit write operation on a 32-bit word in RAM.
 *
 * @param[in] addr An address of a 32-bit word in RAM.
 *
 * @param[in] bit A bit position to write, 0-31.
 *
 * @param[in] val A value to set bit to, 0 or 1.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_ram_write_bit(volatile uint32_t *addr,
                                              uint32_t bit,
                                              uint32_t val)
{
  uint32_t tmp = *addr;

  /* Make sure val is not more than 1 because only one bit needs to be set. */
  *addr = (tmp & ~(1UL << bit)) | ((val & 1UL) << bit);
}

/***************************************************************************//**
 * @brief
 *   Perform a single-bit read operation on a 32-bit word in RAM.
 *
 * @param[in] addr RAM address.
 *
 * @param[in] bit A bit position to read, 0-31.
 *
 * @return
 *     The requested bit shifted to bit position 0 in the return value.
 ******************************************************************************/
__STATIC_INLINE unsigned int sl_hal_bus_ram_read_bit(volatile const uint32_t *addr,
                                                     uint32_t bit)
{
  return ((*addr) >> bit) & 1UL;
}

/***************************************************************************//**
 * @brief
 *   Perform a single-bit atomic write operation on a peripheral register.
 *
 * @details
 *   This function uses built-in hardware 4K-aliased addressing that allows to
 *   perform an atomic read-modify-write operation on a single register bit.
 *   See the reference manual for more details about alias addressing.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] bit A bit position to write, 0-31.
 *
 * @param[in] val A value to set bit to, 0 or 1.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_COMMON, SL_CODE_CLASS_TIME_CRITICAL)
__STATIC_INLINE void sl_hal_bus_reg_write_bit(volatile uint32_t *addr,
                                              uint32_t bit,
                                              uint32_t val)
{
  EFM_ASSERT(bit < 32U);

#if defined(PER_REG_BLOCK_SET_OFFSET) && defined(PER_REG_BLOCK_CLR_OFFSET)
  uint32_t aliasAddr;
  if (val != 0U) {
    aliasAddr = (uint32_t)addr + PER_REG_BLOCK_SET_OFFSET;
  } else {
    aliasAddr = (uint32_t)addr + PER_REG_BLOCK_CLR_OFFSET;
  }
  *(volatile uint32_t *)aliasAddr = 1UL << bit;
#else
  uint32_t tmp = *addr;

  // Make sure val is not more than 1 because only one bit needs to be set.
  *addr = (tmp & ~(1 << bit)) | ((val & 1) << bit);
#endif
}

/***************************************************************************//**
 * @brief
 *   Perform a single-bit atomic read operation on a peripheral register.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] bit A bit position to read, 0-31.
 *
 * @return
 *     The requested bit shifted to bit position 0 in the return value.
 ******************************************************************************/
__STATIC_INLINE unsigned int sl_hal_bus_reg_read_bit(volatile const uint32_t *addr,
                                                     uint32_t bit)
{
  return ((*addr) >> bit) & 1UL;
}

/***************************************************************************//**
 * @brief
 *   Perform an atomic masked set operation on a peripheral register address.
 *
 * @details
 *   A peripheral register masked set provides a set operation of a bit-mask
 *   in a peripheral register. All 1s in the mask are set to 1 in the register.
 *   All 0s in the mask are not changed in the register.
 *   RAMs and special peripherals are not supported.
 *
 * @note
 *   This function uses built-in hardware 4K-aliased addressing that allows to
 *   perform an atomic read-modify-write operation.
 *   See the reference manual for more details about alias addressing.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] mask A mask to set.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_set_mask(volatile uint32_t *addr,
                                             uint32_t mask)
{
#if defined(PER_REG_BLOCK_SET_OFFSET)
  uint32_t aliasAddr = (uint32_t)addr + PER_REG_BLOCK_SET_OFFSET;
  *(volatile uint32_t *)aliasAddr = mask;
#else
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  *addr |= mask;
  CORE_EXIT_CRITICAL();
#endif
}

/***************************************************************************//**
 * @brief
 *   Perform an atomic masked clear operation on the peripheral register address.
 *
 * @details
 *   A peripheral register masked clear provides a clear operation of a bit-mask
 *   in a peripheral register. All 1s in the mask are set to 0 in the register.
 *   All 0s in the mask are not changed in the register.
 *   RAMs and special peripherals are not supported.
 *
 * @note
 *   This function uses built-in hardware 4K-aliased addressing that allows to
 *   perform an atomic read-modify-write operation.
 *   See the reference manual for more details about alias addressing.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] mask A mask to clear.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_clear_mask(volatile uint32_t *addr,
                                               uint32_t mask)
{
#if defined(PER_REG_BLOCK_CLR_OFFSET)
  uint32_t aliasAddr = (uint32_t)addr + PER_REG_BLOCK_CLR_OFFSET;
  *(volatile uint32_t *)aliasAddr = mask;
#else
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  *addr &= ~mask;
  CORE_EXIT_CRITICAL();
#endif
}

/***************************************************************************//**
 * @brief
 *   Perform peripheral register masked write.
 *
 * @details
 *   This function first reads the peripheral register and updates only bits
 *   that are set in the mask with content of val. Typically, the mask is a
 *   bit-field in the register and the value val is within the mask.
 *
 * @note
 *   The read-modify-write operation is executed in a critical section to
 *   guarantee atomicity. Note that atomicity can only be guaranteed if register
 *   is modified only by the core, and not by other peripherals (like DMA).
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] mask A peripheral register mask.
 *
 * @param[in] val  A peripheral register value. The value must be shifted to the
                  correct bit position in the register corresponding to the field
                  defined by the mask parameter. The register value must be
                  contained in the field defined by the mask parameter. The
                  register value is masked to prevent involuntary spillage.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_write_mask(volatile uint32_t *addr,
                                               uint32_t mask,
                                               uint32_t val)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  *addr = (*addr & ~mask) | (val & mask);
  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *   Perform a peripheral register masked read.
 *
 * @details
 *   Read an unshifted and masked value from a peripheral register.
 *
 * @note
 *   This operation is not hardware accelerated.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] mask A peripheral register mask.
 *
 * @return
 *   An unshifted and masked register value.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_bus_reg_read_mask(volatile const uint32_t *addr,
                                                  uint32_t mask)
{
  return *addr & mask;
}

/** @} (end addtogroup bus) */

#ifdef __cplusplus
}
#endif

#endif /* SL_HAL_BUS_H */
