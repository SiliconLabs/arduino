/***************************************************************************//**
 * @file
 * @brief System API
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

#ifndef _SL_HAL_SYSTEM_H
#define _SL_HAL_SYSTEM_H

#include "em_device.h"
#include "sl_hal_system_generic.h"
#include "sl_enum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup system SYSTEM - System Utils
 * @brief System API
 * @details
 *  This module contains functions to read information such as RAM and Flash size,
 *  device unique ID, chip revision, family, and part number from DEVINFO and
 *  SCB blocks. Functions to configure and read status from FPU are available for
 *  compatible devices.
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// Family identifiers.
SL_ENUM_GENERIC(sl_hal_system_part_family_t, uint32_t) {
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_21 = DEVINFO_PART_FAMILY_MG | (21 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty Gecko Series 2 Config 1 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_21 = DEVINFO_PART_FAMILY_FG | (21 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Flex Gecko Series 2 Config 1 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_BLUE_21 = DEVINFO_PART_FAMILY_BG | (21 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Blue Gecko Series 2 Config 1 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_RCP_21 = DEVINFO_PART_FAMILY_MR | (21 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty RCP Series 2 Config 1 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_22 = DEVINFO_PART_FAMILY_MG | (22 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty Gecko Series 2 Config 2 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_22 = DEVINFO_PART_FAMILY_FG | (22 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Flex Gecko Series 2 Config 2 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_BLUE_22 = DEVINFO_PART_FAMILY_BG | (22 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Blue Gecko Series 2 Config 2 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_EFM32_PEARL_22 = DEVINFO_PART_FAMILY_PG | (22 << _DEVINFO_PART_FAMILYNUM_SHIFT),     ///< EFM32 Pearl Gecko Series 2 Config 2 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_3)
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_23 = DEVINFO_PART_FAMILY_FG | (23 << _DEVINFO_PART_FAMILYNUM_SHIFT),       ///< EFR32 Flex Gecko Series 2 Config 3 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_ZEN_23 = DEVINFO_PART_FAMILY_ZG | (23 << _DEVINFO_PART_FAMILYNUM_SHIFT),        ///< EFR32 Zen Gecko Series 2 Config 3 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_EFM32_PEARL_23 = DEVINFO_PART_FAMILY_PG | (23 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFM32 Pearl Gecko Series 2 Config 3 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_SIDEWALK_23 = DEVINFO_PART_FAMILY_SG | (23 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Side Walk Gecko Series 2 Config 3 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_4)
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_24 = DEVINFO_PART_FAMILY_MG | (24 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty Gecko Series 2 Config 4 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_24 = DEVINFO_PART_FAMILY_FG | (24 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Flex Gecko Series 2 Config 4 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_BLUE_24 = DEVINFO_PART_FAMILY_BG | (24 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Blue Gecko Series 2 Config 4 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_5)
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_25 = DEVINFO_PART_FAMILY_FG | (25 << _DEVINFO_PART_FAMILYNUM_SHIFT),       ///< EFR32 Flex Gecko Series 2 Config 5 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_6)
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_26 = DEVINFO_PART_FAMILY_MG | (26 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty Gecko Series 2 Config 6 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_BLUE_26 = DEVINFO_PART_FAMILY_BG | (26 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Blue Gecko Series 2 Config 6 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_EFM32_PEARL_26 = DEVINFO_PART_FAMILY_PG | (26 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFM32 Pearl Gecko Series 2 Config 6 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_7)
  SL_HAL_SYSTEM_PART_FAMILY_MIGHTY_27 = DEVINFO_PART_FAMILY_MG | (27 << _DEVINFO_PART_FAMILYNUM_SHIFT), ///< EFR32 Mighty Gecko Series 2 Config 7 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_BLUE_27 = DEVINFO_PART_FAMILY_BG | (27 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Blue Gecko Series 2 Config 7 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_8)
  SL_HAL_SYSTEM_PART_FAMILY_FLEX_28 = DEVINFO_PART_FAMILY_FG | (28 << _DEVINFO_PART_FAMILYNUM_SHIFT),  ///< EFR32 Flex Gecko Series 2 Config 8 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_ZEN_28 = DEVINFO_PART_FAMILY_ZG | (28 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Zen Gecko Series 2 Config 8 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_SIDEWALK_28 = DEVINFO_PART_FAMILY_SG | (28 << _DEVINFO_PART_FAMILYNUM_SHIFT),   ///< EFR32 Side Walk Gecko Series 2 Config 8 Value Device Family
  SL_HAL_SYSTEM_PART_FAMILY_EFM32_PEARL_28 = DEVINFO_PART_FAMILY_PG | (28 << _DEVINFO_PART_FAMILYNUM_SHIFT),  ///< EFM32 Pearl Gecko Series 2 Config 8 Value Device Family
#endif
#if defined(_SILICON_LABS_32B_SERIES_3_CONFIG_301)
  SL_HAL_SYSTEM_PART_FAMILY_BLUETOOTH_301 = DEVINFO_PART0_PROTOCOL_BLUETOOTH        \
                                            | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                            | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 Bluetooth Config 1 Value Device Family (BG)
  SL_HAL_SYSTEM_PART_FAMILY_PROPRIETARY_301 = DEVINFO_PART0_PROTOCOL_PROPRIETARY      \
                                              | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                              | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 Proprietary Config 1 Value Device Family (FG)
  SL_HAL_SYSTEM_PART_FAMILY_FIFTEENPFOUR_301 = DEVINFO_PART0_PROTOCOL_FIFTEENPFOUR     \
                                               | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                               | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 15.4 Config 1 Value Device Family (MG)
  SL_HAL_SYSTEM_PART_FAMILY_PEARL_301 = DEVINFO_PART0_PROTOCOL_PEARL            \
                                        | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                        | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 Pearl Config 1 Value Device Family (PG)
  SL_HAL_SYSTEM_PART_FAMILY_WIFI_301 = DEVINFO_PART0_PROTOCOL_WIFI             \
                                       | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                       | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 Wifi Config 1 Value Device Family (WG)
  SL_HAL_SYSTEM_PART_FAMILY_ZWAVE_301 = DEVINFO_PART0_PROTOCOL_ZWAVE            \
                                        | (0x33 << _DEVINFO_PART0_SERIES_SHIFT) \
                                        | (0x30 << _DEVINFO_PART0_DIECODE0_SHIFT), ///< SI Series 3 Zwave Config 1 Value Device Family (ZG)
#endif
  SL_HAL_SYSTEM_PART_FAMILY_UNKNOWN = 0xFF  ///< Unknown Device Family. Family ID is missing on unprogrammed parts.
};

/***************************************************************************//**
 * @brief
 *   Get the MCU family identifier.
 *
 * @return
 *   Family identifier of MCU.
 *
 * @note
 *   This function retrieves family ID by reading the chip's device info
 *   structure in flash memory. Users can retrieve family ID directly
 *   by reading DEVINFO->PART item and decode with mask and shift
 *   \#defines defined in \<part_family\>_devinfo.h (refer to code
 *   below for details).
 ******************************************************************************/
sl_hal_system_part_family_t sl_hal_system_get_family(void);

/** @} (end addtogroup system) */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SL_HAL_SYSTEM_H */
