/**************************************************************************//**
 * @file
 * @brief Device Manager API Definition
 ******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories, Inc. www.silabs.com</b>
 ******************************************************************************
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
 *****************************************************************************/
#ifndef SL_DEVICE_PERIPHERAL_TYPES_H
#define SL_DEVICE_PERIPHERAL_TYPES_H

#include "sl_device_clock.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup device_peripheral Device Abstraction Peripheral
 *
 * @{
 ******************************************************************************/

// ----------------------------------------------------------------------------
// TYPEDEFS

/// Define peripheral structure.
typedef struct sl_peripheral {
  uint32_t base;                 ///< Peripheral base address.
  sl_clock_branch_t clk_branch;  ///< Peripheral clock branch.
  sl_bus_clock_t bus_clock;      ///< Peripheral bus clock.
} sl_peripheral_val_t;

/// Define peripheral typedef.
typedef const sl_peripheral_val_t* sl_peripheral_t;

/** @} (end addtogroup device_peripheral) */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_PERIPHERAL_TYPES_H
