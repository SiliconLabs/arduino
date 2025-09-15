/***************************************************************************//**
 * @file
 * @brief This file contains the type definitions for EFR32xG2x chip-specific
 *   aspects of RAIL.
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

#ifdef  SLI_LIBRARY_BUILD

// This file should not be included when doing SLI_LIBRARY_BUILDs

#else//!SLI_LIBRARY_BUILD

#ifndef __RAIL_CHIP_SPECIFIC_H_
#if !defined(__RAIL_TYPES_H__) && !defined(DOXYGEN_SHOULD_SKIP_THIS)
#warning rail_chip_specific.h should only be included by rail_types.h
#include "rail_types.h" // Force rail_chip_specific.h only within rail_types.h
#else // __RAIL_TYPES_H__
/// Include guard
#define __RAIL_CHIP_SPECIFIC_H_
#include "sl_status.h"
#include "rail_features.h"
#ifdef RAIL_INTERNAL_BUILD
#include "rail_chip_specific_internal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * General Structures
 *****************************************************************************/
/**
 * @addtogroup General_EFR32XG2X EFR32xG2x
 * @ingroup General
 * @{
 * @brief Types specific to the EFR32xG2x for general configuration.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_EFR32XG21_STATE_BUFFER_BYTES
 * @brief The EFR32xG21 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG21_STATE_BUFFER_BYTES 592

/**
 * @def RAIL_EFR32XG22_STATE_BUFFER_BYTES
 * @brief The EFR32xG22 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG22_STATE_BUFFER_BYTES 608

/**
 * @def RAIL_EFR32XG23_STATE_BUFFER_BYTES
 * @brief The EFR32xG23 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG23_STATE_BUFFER_BYTES 616

/**
 * @def RAIL_EFR32XG24_STATE_BUFFER_BYTES
 * @brief The EFR32xG24 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG24_STATE_BUFFER_BYTES 632

/**
 * @def RAIL_EFR32XG25_STATE_BUFFER_BYTES
 * @brief The EFR32xG25 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG25_STATE_BUFFER_BYTES 632

/**
 * @def RAIL_EFR32XG26_STATE_BUFFER_BYTES
 * @brief The EFR32xG26 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG26_STATE_BUFFER_BYTES 632

/**
 * @def RAIL_EFR32XG27_STATE_BUFFER_BYTES
 * @brief The EFR32xG27 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG27_STATE_BUFFER_BYTES 608

/**
 * @def RAIL_EFR32XG28_STATE_BUFFER_BYTES
 * @brief The EFR32xG28 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG28_STATE_BUFFER_BYTES 624

/**
 * @def RAIL_EFR32XG29_STATE_BUFFER_BYTES
 * @brief The EFR32XG29 series size needed for
 *   \ref RAIL_StateBufferEntry_t::bufferBytes.
 */
#define RAIL_EFR32XG29_STATE_BUFFER_BYTES 608

#ifndef RAIL_STATE_BUFFER_BYTES
/**
 * @def RAIL_STATE_BUFFER_BYTES
 * @brief The size needed for \ref RAIL_StateBufferEntry_t::bufferBytes
 *   on this platform for this radio. This compile-time size may be slightly
 *   larger than what \ref RAIL_GetStateBufferSize() determines at run-time.
 */
#if (_SILICON_LABS_32B_SERIES_2_CONFIG == 1)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG21_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 2)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG22_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 3)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG23_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 4)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG24_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 5)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG25_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 6)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG26_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 7)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG27_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 8)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG28_STATE_BUFFER_BYTES
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 9)
#define RAIL_STATE_BUFFER_BYTES RAIL_EFR32XG29_STATE_BUFFER_BYTES
#else
#define RAIL_STATE_BUFFER_BYTES 0 // Sate Doxygen
#error "Unsupported platform!"
#endif
#endif //#ifndef RAIL_STATE_BUFFER_BYTES

#endif//DOXYGEN_SHOULD_SKIP_THIS

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * @def RAIL_SEQ_IMAGE_1
 * @brief A macro for the first sequencer image.
 */
#define RAIL_SEQ_IMAGE_1 1

/**
 * @def RAIL_SEQ_IMAGE_2
 * @brief A macro for the second sequencer image.
 */
#define RAIL_SEQ_IMAGE_2 2

#ifndef RAIL_INTERNAL_BUILD
#if ((_SILICON_LABS_32B_SERIES_2_CONFIG == 4) || (_SILICON_LABS_32B_SERIES_2_CONFIG == 6))
/**
 * @def RAIL_SEQ_IMAGE_PA_10_DBM
 * @brief A chip-specific macro for the sequencer image used on EFR32xG24 and EFR32xG26 OPNs
 *   with a 10 dBm PA.
 */
#define RAIL_SEQ_IMAGE_PA_10_DBM RAIL_SEQ_IMAGE_1

/**
 * @def RAIL_SEQ_IMAGE_PA_20_DBM
 * @brief A chip-specific macro for the sequencer image used on EFR32xG24 and EFR32xG26 OPNs
 *   with a 20 dBm PA.
 */
#define RAIL_SEQ_IMAGE_PA_20_DBM RAIL_SEQ_IMAGE_2

/**
 * @def RAIL_SEQ_IMAGE_COUNT
 * @brief A macro for the total number of sequencer images supported on the
 *   platform.
 */
#define RAIL_SEQ_IMAGE_COUNT 2

#else //((_SILICON_LABS_32B_SERIES_2_CONFIG != 4) && (_SILICON_LABS_32B_SERIES_2_CONFIG != 6))

/**
 * @def RAIL_SEQ_IMAGE_DEFAULT
 * @brief A chip-specific macro for the default sequencer image on platforms
 *   that support only one sequencer image.
 */
#define RAIL_SEQ_IMAGE_DEFAULT RAIL_SEQ_IMAGE_1

/**
 * @def RAIL_SEQ_IMAGE_COUNT
 * @brief A macro for the total number of sequencer images supported on the
 *   platform.
 */
#define RAIL_SEQ_IMAGE_COUNT 1
#endif //((_SILICON_LABS_32B_SERIES_2_CONFIG == 4) || (_SILICON_LABS_32B_SERIES_2_CONFIG == 6))
#endif //RAIL_INTERNAL_BUILD

/**
 * @struct RAIL_RffpllConfig_t
 * @brief Stores information relevant to the Radio-Friendly Frequency
 *   Phase-Locked Loop (RFFPLL) for the PHY configuration currently loaded in
 *   memory.
 */
typedef struct {
  /** Divider X (Modem Clock), Divider Y (M33 System Clock), and Divider N (Feedback) values. */
  uint32_t dividers;
  /** Radio clock frequency in Hz. */
  uint32_t radioFreqHz;
  /** System clock frequency in Hz. */
  uint32_t sysclkFreqHz;
} RAIL_RffpllConfig_t;

/**
 * @def RAIL_RFFPLL_DIVIDERX_MASK
 * @brief Bit mask for RFFPLL DIVX in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERX_MASK   0x000000FFUL

/**
 * @def RAIL_RFFPLL_DIVIDERX_SHIFT
 * @brief Shift value for RFFPLL DIVX in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERX_SHIFT  0

/**
 * @def RAIL_RFFPLL_DIVIDERY_MASK
 * @brief Bit mask for RFFPLL DIVY in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERY_MASK   0x0000FF00UL

/**
 * @def RAIL_RFFPLL_DIVIDERY_SHIFT
 * @brief Shift value for RFFPLL DIVY in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERY_SHIFT  8

/**
 * @def RAIL_RFFPLL_DIVIDERN_MASK
 * @brief Bit mask for RFFPLL DIVN in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERN_MASK   0x00FF0000UL

/**
 * @def RAIL_RFFPLL_DIVIDERN_SHIFT
 * @brief Shift value for RFFPLL DIVN in \ref RAIL_RffpllConfig_t::dividers.
 */
#define RAIL_RFFPLL_DIVIDERN_SHIFT  16

/**
 * @typedef RAIL_TimerTick_t
 * @brief Internal RAIL hardware timer tick that drives the RAIL timebase.
 *   A tick is roughly 0.5 microseconds but it wraps somewhat before
 *   0xFFFFFFFF giving a time range of about 17 minutes.
 *
 * @note \ref RAIL_TimerTicksToUs() can be used to convert the delta between
 *   two \ref RAIL_TimerTick_t values to microseconds.
 */
typedef uint32_t RAIL_TimerTick_t;

/**
 * @def RAIL_GetTimerTick(timerTickType)
 * @brief The RAIL hardware timer ticks value.
 *
 * @note timerTickType is added for compatibility reasons and is ignored here;
 *   this gets the equivalent of \ref RAIL_TIMER_TICK_DEFAULT.
 */
#define RAIL_GetTimerTick(timerTickType) (*RAIL_TimerTick)

/**
 * A global pointer to the memory address of the 32-bit
 * \ref RAIL_TimerTick_t internal RAIL hardware timer that drives
 * the RAIL timebase.
 * Equivalent to \ref RAIL_TimerTick_t for its granularity and range.
 */
extern const volatile uint32_t *RAIL_TimerTick;

/**
 * A global pointer to the memory address of the internal RAIL hardware timer
 * that captures the latest RX packet reception time.
 * See \ref RAIL_TimerTick_t for its granularity and range.
 *
 * @note This would not include the RX chain delay, so may not exactly
 *   correspond to the \ref RAIL_Time_t packet timestamp available within
 *   \ref RAIL_RxPacketDetails_t::timeReceived which reflects the actual
 *   on-air time that the packet finished.
 */
extern const volatile uint32_t *RAIL_RxPacketTimestamp;

/**
 * Get elapsed time, in microseconds, between two \ref RAIL_TimerTick_t ticks.
 *
 * @param[in] startTick Tick recorded at the start of the operation.
 * @param[in] endTick Tick recorded at the end of the operation.
 * @return The elapsed time, in microseconds, between two timer ticks.
 */
RAIL_Time_t RAIL_TimerTicksToUs(RAIL_TimerTick_t startTick,
                                RAIL_TimerTick_t endTick);

/**
 * Get \ref RAIL_TimerTick_t tick corresponding to a \ref RAIL_Time_t time.
 *
 * @param[in] microseconds Time in microseconds.
 * @return The \ref RAIL_TimerTick_t tick corresponding to the
 *   \ref RAIL_Time_t time.
 */
RAIL_TimerTick_t RAIL_UsToTimerTicks(RAIL_Time_t microseconds);

#endif//DOXYGEN_SHOULD_SKIP_THIS

/** @} */ // end of group General_EFR32XG2X

/******************************************************************************
 * Multiprotocol
 *****************************************************************************/
/**
 * @addtogroup Multiprotocol_EFR32XG2X EFR32xG2x
 * @ingroup Multiprotocol
 * @{
 * @brief EFR32xG2x-specific multiprotocol support defines.
 */

/**
 * @def TRANSITION_TIME_US
 * @brief Time it takes to take care of protocol switching.
 */
#if _SILICON_LABS_32B_SERIES_2_CONFIG > 1
// XG22 + devices
#define TRANSITION_TIME_US 510
#else
// XG21
#define TRANSITION_TIME_US 500
#endif

/** @} */ // end of group Multiprotocol_EFR32XG2X

/******************************************************************************
 * Calibration
 *****************************************************************************/
/**
 * @addtogroup Calibration_EFR32XG2X EFR32xG2x
 * @ingroup Calibration
 * @{
 * @brief EFR32xG2x-specific Calibrations.
 */

/**
 * @def RAIL_RF_PATHS_2P4GIG
 * @brief Indicates the number of 2.4 GHz RF Paths suppported.
 */
#ifndef RAIL_RF_PATHS_2P4GIG
#if ((_SILICON_LABS_32B_SERIES_2_CONFIG == 1) || (_SILICON_LABS_32B_SERIES_2_CONFIG == 4) || (_SILICON_LABS_32B_SERIES_2_CONFIG == 6))
#define RAIL_RF_PATHS_2P4GIG 2
#elif ((_SILICON_LABS_32B_SERIES_2_CONFIG == 2) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 7)   \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 9)   \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 8))
#define RAIL_RF_PATHS_2P4GIG 1
#else
#define RAIL_RF_PATHS_2P4GIG 0
#endif
#endif//RAIL_RF_PATHS_2P4GHZ

/**
 * @def RAIL_RF_PATHS_SUBGIG
 * @brief Indicates the number of Sub-GHz RF Paths supported.
 */
#ifndef RAIL_RF_PATHS_SUBGHZ
#if _SILICON_LABS_32B_SERIES_2_CONFIG == 3
#define RAIL_RF_PATHS_SUBGIG 2
#elif  _SILICON_LABS_32B_SERIES_2_CONFIG == 5
#define RAIL_RF_PATHS_SUBGIG 2
#elif _SILICON_LABS_32B_SERIES_2_CONFIG == 8
#define RAIL_RF_PATHS_SUBGIG 1
#else
#define RAIL_RF_PATHS_SUBGIG 0
#endif
#endif//RAIL_RF_PATHS_SUBGHZ

/**
 * @def RAIL_RF_PATHS
 * @brief Indicates the number of RF Paths supported.
 */
#define RAIL_RF_PATHS (RAIL_RF_PATHS_SUBGIG + RAIL_RF_PATHS_2P4GIG)

#if     (RAIL_RF_PATHS > RAIL_MAX_RF_PATHS)
#error "Update rail_types.h RAIL_MAX_RF_PATHS"
#endif

/**
 * @def RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS
 * @brief Indicates this version of RAIL supports IR calibration on multiple RF paths
 * Needed for backwards compatibility.
 */
#if RAIL_RF_PATHS > 1
#define RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS 1
#else
#ifdef  DOXYGEN_SHOULD_SKIP_THIS // Leave undefined except for doxygen
#define RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS 0
#endif //DOXYGEN_SHOULD_SKIP_THIS
#endif //RAIL_RF_PATHS

/**
 * @struct RAIL_ChannelConfigEntryAttr
 * @brief A channel configuration entry attribute structure. Items listed
 *  are designed to be altered and updated during run-time.
 */
struct RAIL_ChannelConfigEntryAttr {
  /** IR calibration attributes specific to each channel configuration entry. */
 #if     RAIL_SUPPORTS_OFDM_PA
  RAIL_IrCalValues_t calValues;
 #else//!RAIL_SUPPORTS_OFDM_PA
  RAIL_RxIrCalValues_t calValues;
 #endif//RAIL_SUPPORTS_OFDM_PA
};

/** @} */ // end of group Calibration_EFR32XG2X

/******************************************************************************
 * Transmit
 *****************************************************************************/
/**
 * @addtogroup PA_EFR32XG2X EFR32xG2x
 * @ingroup PA
 * @{
 * @brief Types specific to the EFR32xG2x for dealing with the on-chip PAs.
 */

#ifndef RAIL_TX_POWER_LEVEL_2P4_HP_MAX
#if _SILICON_LABS_32B_SERIES_2_CONFIG == 1
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (180U)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MIN     (1U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_MP_MAX     (90U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_MP_MIN     (1U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MAX     (64U)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MIN     (0U)
#elif ((_SILICON_LABS_32B_SERIES_2_CONFIG == 4) || (_SILICON_LABS_32B_SERIES_2_CONFIG == 6))
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 *  EFR32xG24: capable of 20dBm max output power has max powerlevel:180
 *  EFR32xG24: capable of 10dBm max output power has max powerlevel:90
 *  EFR32xG26: capable of 20dBm max output power has max powerlevel:180
 *  EFR32xG26: capable of 10dBm max output power has max powerlevel:90
 */
#if defined (_SILICON_LABS_EFR32_2G4HZ_HP_PA_PRESENT) \
  && (_SILICON_LABS_EFR32_2G4HZ_HP_PA_MAX_OUTPUT_DBM > 10)
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (180U)
#else
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (90U)
#endif
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MIN     (0U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MAX     (15U)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MIN     (0U)
#elif ((_SILICON_LABS_32B_SERIES_2_CONFIG == 2) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 7)   \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 9))
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (127U)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MIN     (0U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MAX     (15U)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MIN     (0U)
#elif _SILICON_LABS_32B_SERIES_2_CONFIG == 8
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (240)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MIN     (1U)
#else //EFR32xG23
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MAX     (240)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_HP_MIN     (1U)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_MP_MAX     (RAIL_TX_POWER_LEVEL_2P4_HP_MAX)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_MP_MIN     (RAIL_TX_POWER_LEVEL_2P4_HP_MIN)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MAX     (RAIL_TX_POWER_LEVEL_2P4_HP_MAX)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LP_MIN     (RAIL_TX_POWER_LEVEL_2P4_HP_MIN)
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LLP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LLP_MAX     (RAIL_TX_POWER_LEVEL_2P4_HP_MAX)
/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_2P4GIG_LLP mode.
 */
#define RAIL_TX_POWER_LEVEL_2P4_LLP_MIN     (RAIL_TX_POWER_LEVEL_2P4_HP_MIN)
#endif //_SILICON_LABS_32B_SERIES_2_CONFIG
#endif //RAIL_TX_POWER_LEVEL_2P4_HP_MAX

#if RAIL_SUPPORTS_SUBGHZ_BAND
/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when using
 * a Sub-GHz PA mode.
 */
#ifndef RAIL_SUBGIG_MAX
#if _SILICON_LABS_32B_SERIES_2_CONFIG == 3 || _SILICON_LABS_32B_SERIES_2_CONFIG == 8
#define RAIL_SUBGIG_MAX 240U
#elif _SILICON_LABS_32B_SERIES_2_CONFIG == 5
#define RAIL_SUBGIG_MAX 79U
#else
#define RAIL_SUBGIG_MAX 0U
#endif
#endif//RAIL_SUBGIG_MAX

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when using
 * a Sub-GHz PA mode.
 */
#define RAIL_SUBGIG_MIN 1U

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_HP_MAX (RAIL_SUBGIG_MAX)

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_HP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_HP_MIN (RAIL_SUBGIG_MIN)

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MP_MAX (RAIL_SUBGIG_MAX)

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_MP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MP_MIN (RAIL_SUBGIG_MIN)

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_LP_MAX (RAIL_SUBGIG_MAX)

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_LP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_LP_MIN (RAIL_SUBGIG_MIN)

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_LLP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_LLP_MAX (RAIL_SUBGIG_MAX)

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_SUBGIG_LLP mode.
 */
#define RAIL_TX_POWER_LEVEL_SUBGIG_LLP_MIN (RAIL_SUBGIG_MIN)
#endif //RAIL_SUPPORTS_SUBGHZ_BAND

#if RAIL_SUPPORTS_OFDM_PA
#if _SILICON_LABS_32B_SERIES_2_CONFIG == 5
#define RAIL_OFDM_PA_MAX      204U
#define RAIL_OFDM_PA_MULT     5U
#define RAIL_OFDM_PA_MIN      0U
#endif

/**
 * The maximum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE mode.
 */
#define RAIL_TX_POWER_LEVEL_OFDM_PA_MAX (RAIL_OFDM_PA_MAX)

/**
 * The minimum valid value for the \ref RAIL_TxPowerLevel_t when in \ref
 * RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE mode.
 */
#define RAIL_TX_POWER_LEVEL_OFDM_PA_MIN (RAIL_OFDM_PA_MIN)
#endif //RAIL_SUPPORTS_OFDM_PA

/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_HP_MAX      RAIL_TX_POWER_LEVEL_2P4_HP_MAX
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_HP_MIN      RAIL_TX_POWER_LEVEL_2P4_HP_MIN
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_MP_MAX      RAIL_TX_POWER_LEVEL_2P4_MP_MAX
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_MP_MIN      RAIL_TX_POWER_LEVEL_2P4_MP_MIN
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_LP_MAX      RAIL_TX_POWER_LEVEL_2P4_LP_MAX
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_LP_MIN      RAIL_TX_POWER_LEVEL_2P4_LP_MIN
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MAX  RAIL_TX_POWER_LEVEL_SUBGIG_HP_MAX
/** Backwards compatability define */
#define RAIL_TX_POWER_LEVEL_SUBGIG_MIN  RAIL_TX_POWER_LEVEL_SUBGIG_HP_MIN

/**
 * The number of PA's on this chip (including Virtual PAs).
 */
#ifndef RAIL_NUM_PA
#if ((_SILICON_LABS_32B_SERIES_2_CONFIG == 2) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 4) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 6) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 7) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 9))
#define RAIL_NUM_PA (2U)
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 3)
#define RAIL_NUM_PA (4U)
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 8)
#define RAIL_NUM_PA (5U)
#elif (_SILICON_LABS_32B_SERIES_2_CONFIG == 5)
#define RAIL_NUM_PA (4U)
#else
#define RAIL_NUM_PA (3U)
#endif
#endif //#ifndef RAIL_NUM_PA

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
// Only those supported per-platform are defined, for use with #ifdef in
// apps or librail code.
#if     RAIL_SUPPORTS_2P4GHZ_BAND
#define RAIL_TX_POWER_MODE_2P4GIG_HP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4GIG_HP)
#define RAIL_TX_POWER_MODE_2P4_HP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4_HP)
#if     _SILICON_LABS_32B_SERIES_2_CONFIG == 1
#define RAIL_TX_POWER_MODE_2P4GIG_MP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4GIG_MP)
#define RAIL_TX_POWER_MODE_2P4_MP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4_MP)
#endif//_SILICON_LABS_32B_SERIES_2_CONFIG == 1
#if     ((_SILICON_LABS_32B_SERIES_2_CONFIG != 3) && (_SILICON_LABS_32B_SERIES_2_CONFIG != 8))
#define RAIL_TX_POWER_MODE_2P4GIG_LP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4GIG_LP)
#define RAIL_TX_POWER_MODE_2P4_LP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4_LP)
#endif//((_SILICON_LABS_32B_SERIES_2_CONFIG != 3) && (_SILICON_LABS_32B_SERIES_2_CONFIG != 8))
#define RAIL_TX_POWER_MODE_2P4GIG_HIGHEST ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4GIG_HIGHEST)
#define RAIL_TX_POWER_MODE_2P4_HIGHEST ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_2P4_HIGHEST)
#endif//RAIL_SUPPORTS_2P4GHZ_BAND

#if     RAIL_SUPPORTS_SUBGHZ_BAND
#if     RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE
#define RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE)
#else//!RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE
#define RAIL_TX_POWER_MODE_SUBGIG_HP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_HP)
#define RAIL_TX_POWER_MODE_SUBGIG ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG)
#define RAIL_TX_POWER_MODE_SUBGIG_MP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_MP)
#define RAIL_TX_POWER_MODE_SUBGIG_LP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_LP)
#define RAIL_TX_POWER_MODE_SUBGIG_LLP ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_LLP)
#endif//RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE
#define RAIL_TX_POWER_MODE_SUBGIG_HIGHEST ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_SUBGIG_HIGHEST)
#endif//RAIL_SUPPORTS_SUBGHZ_BAND
#if     RAIL_SUPPORTS_OFDM_PA
#if     RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE
#define RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE)
#define RAIL_TX_POWER_MODE_OFDM_PA  ((RAIL_TxPowerMode_t) RAIL_TX_POWER_MODE_OFDM_PA)
#endif//RAIL_SUPPORTS_DBM_POWERSETTING_MAPPING_TABLE
#endif//RAIL_SUPPORTS_OFDM_PA
#endif//DOXYGEN_SHOULD_SKIP_THIS

/** Convenience macro for any mapping table mode. */
#define RAIL_POWER_MODE_IS_ANY_DBM_POWERSETTING_MAPPING_TABLE(x) \
  (((x) == RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE)        \
   || ((x) == RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE))

/** Convenience macro to check if the power mode supports raw setting. */
#define RAIL_POWER_MODE_SUPPORTS_RAW_SETTING(x)           \
  (((x) != RAIL_TX_POWER_MODE_OFDM_PA_POWERSETTING_TABLE) \
   && ((x) != RAIL_TX_POWER_MODE_SUBGIG_POWERSETTING_TABLE))
/** @} */ // end of group PA_EFR32XG2X

/******************************************************************************
 * RX Channel Hopping
 *****************************************************************************/
/**
 * @addtogroup Rx_Channel_Hopping_EFR32XG2X EFR32xG2x
 * @ingroup Rx_Channel_Hopping
 * @{
 * @brief EFR32xG2x-specific RX channel hopping.
 */

#if _SILICON_LABS_32B_SERIES_2_CONFIG == 8
/// The static amount of memory needed per channel for channel hopping, measured
/// in 32 bit words, regardless of the size of radio configuration structures.
#define RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL (65U)
#else
/// The static amount of memory needed per channel for channel hopping, measured
/// in 32 bit words, regardless of the size of radio configuration structures.
#define RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL (56U)
#endif

#if     (RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL \
         > RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL_WORST_CASE)
#error "Update rail_types.h RAIL_CHANNEL_HOPPING_BUFFER_SIZE_PER_CHANNEL_WORST_CASE"
#endif

/** @} */  // end of group Rx_Channel_Hopping_EFR32XG2X

/******************************************************************************
 * Sleep Structures
 *****************************************************************************/
/**
 * @addtogroup Sleep_EFR32XG2X EFR32xG2x
 * @ingroup Sleep
 * @{
 * @brief EFR32xG2x-specific Sleeping.
 */

/// Default PRS channel to use when configuring sleep
#define RAIL_TIMER_SYNC_PRS_CHANNEL_DEFAULT  (7U)

#if ((_SILICON_LABS_32B_SERIES_2_CONFIG == 2) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 7) \
  || (_SILICON_LABS_32B_SERIES_2_CONFIG == 9))
/// Default RTCC channel to use when configuring sleep
#define RAIL_TIMER_SYNC_RTCC_CHANNEL_DEFAULT (1U)
#else
/// Default RTCC channel to use when configuring sleep
#define RAIL_TIMER_SYNC_RTCC_CHANNEL_DEFAULT (0U)
#endif

/** @} */ // end of group Sleep_EFR32XG2X

/******************************************************************************
 * State Transitions
 *****************************************************************************/
/**
 * @addtogroup State_Transitions_EFR32XG2X EFR32xG2x
 * @ingroup State_Transitions
 * @{
 * @brief EFR32xG2x-specific State Transitions.
 */

/**
 * @def RAIL_MINIMUM_TRANSITION_US
 * @brief The minimum value for a consistent RAIL transition
 * @note Transitions may need to be slower than this when using longer
 *   \ref RAIL_TxPowerConfig_t::rampTime values
 */
#define RAIL_MINIMUM_TRANSITION_US (100U)

/**
 * @def RAIL_MAXIMUM_TRANSITION_US
 * @brief The maximum value for a consistent RAIL transition
 */
#define RAIL_MAXIMUM_TRANSITION_US (1000000U)

/**
 * Internal Radio State type mapping for EFR32 chips.
 */
typedef RAIL_RadioStateEfr32_t RAIL_RacRadioState_t;

/** @} */ // end of group State_Transitions_EFR32XG2X

#ifdef __cplusplus
}
#endif

#endif //__RAIL_TYPES_H__

#endif //__RAIL_CHIP_SPECIFIC_H_

#endif //SLI_LIBRARY_BUILD
