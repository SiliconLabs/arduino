/***************************************************************************//**
 * @file
 * @brief Core interrupt handling API (Device Specific)
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef EM_CORE_H
#define EM_CORE_H

#include "em_device.h"
#include "em_core_generic.h"
#include "sl_common.h"

/***************************************************************************//**
 * @addtogroup core
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** Number of words in a NVIC mask set. */
#define CORE_NVIC_REG_WORDS   ((EXT_IRQ_COUNT + 31) / 32)

/** Number of entries in a default interrupt vector table. */
#define CORE_DEFAULT_VECTOR_TABLE_ENTRIES   (EXT_IRQ_COUNT + 16)

/** Highest priority for core interrupt. */
#define CORE_INTERRUPT_HIGHEST_PRIORITY 0

/** Default priority for core interrupt. */
#define CORE_INTERRUPT_DEFAULT_PRIORITY 5

/** Lowest priority for core interrupt. */
#define CORE_INTERRUPT_LOWEST_PRIORITY 7

// Compile time sanity check.
#if (CORE_NVIC_REG_WORDS > 3)
#error "em_core: Unexpected NVIC external interrupt count."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ************************   MACRO API   ***************************************
 ******************************************************************************/

//
//  NVIC mask section macro API.
//

/** Allocate storage for NVIC interrupt masks for use by
 *  CORE_ENTER/EXIT_NVIC() macros. */
#define CORE_DECLARE_NVIC_STATE       CORE_nvicMask_t nvicState

/** Allocate storage for NVIC interrupt masks.
 *  @param[in] x
 *    The storage variable name to use.*/
#define CORE_DECLARE_NVIC_MASK(x)     CORE_nvicMask_t x

/** Allocate storage for and zero initialize NVIC interrupt mask.
 *  @param[in] x
 *    The storage variable name to use.*/
#define CORE_DECLARE_NVIC_ZEROMASK(x) CORE_nvicMask_t x = { { 0 } }

/** NVIC mask style interrupt disable.
 *  @param[in] mask
 *    Mask specifying which NVIC interrupts to disable. */
#define CORE_NVIC_DISABLE(mask)   CORE_NvicDisableMask(mask)

/** NVIC mask style interrupt enable.
 *  @param[in] mask
 *    Mask specifying which NVIC interrupts to enable. */
#define CORE_NVIC_ENABLE(mask)    CORE_NvicEnableMask(mask)

/** Convenience macro for implementing a NVIC mask section.
 *  @param[in] mask
 *    Mask specifying which NVIC interrupts to disable within the section.
 *  @param[in] yourcode
 *    The code for the section. */
#define CORE_NVIC_SECTION(mask, yourcode) \
  {                                       \
    CORE_DECLARE_NVIC_STATE;              \
    CORE_ENTER_NVIC(mask);                \
    {                                     \
      yourcode                            \
    }                                     \
    CORE_EXIT_NVIC();                     \
  }

/** Enter NVIC mask section. Assumes that a @ref CORE_DECLARE_NVIC_STATE exist
 *  in scope.
 *  @param[in] disable
 *    Mask specifying which NVIC interrupts to disable within the section. */
#define CORE_ENTER_NVIC(disable)  CORE_EnterNvicMask(&nvicState, disable)

/** Exit NVIC mask section. Assumes that a @ref CORE_DECLARE_NVIC_STATE exist
 *  in scope. */
#define CORE_EXIT_NVIC()          CORE_NvicEnableMask(&nvicState)

/** NVIC maks style yield.
 * @param[in] enable
 *   Mask specifying which NVIC interrupts to briefly enable. */
#define CORE_YIELD_NVIC(enable)   CORE_YieldNvicMask(enable)

/*******************************************************************************
 *************************   TYPEDEFS   ****************************************
 ******************************************************************************/

/** Storage for NVIC interrupt masks. */
typedef struct {
  uint32_t a[CORE_NVIC_REG_WORDS];    /*!< Array of NVIC mask words. */
} CORE_nvicMask_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

bool  CORE_IrqIsBlocked(IRQn_Type irqN) SL_DEPRECATED_API_SDK_2024_6;

void  CORE_GetNvicEnabledMask(CORE_nvicMask_t *mask) SL_DEPRECATED_API_SDK_2024_6;
bool  CORE_GetNvicMaskDisableState(const CORE_nvicMask_t *mask) SL_DEPRECATED_API_SDK_2024_6;

void  CORE_EnterNvicMask(CORE_nvicMask_t *nvicState,
                         const CORE_nvicMask_t *disable) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_NvicDisableMask(const CORE_nvicMask_t *disable) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_NvicEnableMask(const CORE_nvicMask_t *enable) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_YieldNvicMask(const CORE_nvicMask_t *enable) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_NvicMaskSetIRQ(IRQn_Type irqN, CORE_nvicMask_t *mask) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_NvicMaskClearIRQ(IRQn_Type irqN, CORE_nvicMask_t *mask) SL_DEPRECATED_API_SDK_2024_6;
bool  CORE_NvicIRQDisabled(IRQn_Type irqN) SL_DEPRECATED_API_SDK_2024_6;

void *CORE_GetNvicRamTableHandler(IRQn_Type irqN) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_SetNvicRamTableHandler(IRQn_Type irqN, void *handler) SL_DEPRECATED_API_SDK_2024_6;
void  CORE_InitNvicVectorTable(uint32_t *sourceTable,
                               uint32_t sourceSize,
                               uint32_t *targetTable,
                               uint32_t targetSize,
                               void *defaultHandler,
                               bool overwriteActive);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup core) */

#endif /* EM_CORE_H */
