/***************************************************************************//**
 * @file
 * @brief The MFM specific header file for the RAIL library.
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

#ifndef __RAIL_MFM_H__
#define __RAIL_MFM_H__

#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup MFM Multi-Level Frequency Modulation
/// @ingroup Protocol_Specific
/// @brief MFM configuration routines
///
/// @note This feature is only supported on EFR32xG23 devices.
///
/// This feature can be used to directly control the TX interpolation filter
/// input to allow for a more flexible frequency modulation scheme than the
/// standard MODEM. When doing this, the MFM buffer is treated as an array
/// of 8-bit signed data used as normalized frequency deviation to the SYNTH
/// frequency to directly control the interpolation filter input.
/// No support for frame handling, coding, nor shaping is supported.
/// Only compatible with FSK modulations.
///
/// The functions in this group configure RAIL Multi-Level Frequency Modulation (MFM)
/// hardware acceleration features.
///
/// To configure MFM functionality, the application must first set up
/// a RAIL instance with \ref RAIL_Init() and other setup functions.
/// Before enabling MFM, a ping-pong buffer (called buffer0 and buffer1
/// below) must be configured via \ref RAIL_SetMfmPingPongFifo() and
/// populated with the initial buffer content.
/// MFM is enabled by setting \ref RAIL_TxDataSource_t::TX_MFM_DATA using
/// \ref RAIL_ConfigData() and is activated when transmit is started by
/// \ref RAIL_StartTx(). Once transmitting the data in the ping-pong buffers,
/// RAIL will manage them so it looks like a continuous transmission to the
/// receiver. Every time one of the ping-pong buffers has been transmitted,
/// \ref RAIL_EVENT_MFM_TX_BUFFER_DONE is triggered so the application can
/// update the data in that buffer without the need to start/stop the
/// transmission. \ref RAIL_EVENT_MFM_TX_BUFFER_DONE can be enable with \ref
/// RAIL_ConfigEvents().
/// Use \ref RAIL_StopTx() to finish transmitting.
/// @code{.c}
/// #define MFM_RAW_BUF_WORDS 128
/// extern RAIL_Handle_t railHandle;
/// uint8_t txCount = 0;
/// uint32_t mfmPingPongBuffers[2][MFM_RAW_BUF_WORDS];
///
/// typedef struct mfmConfigApp {
///   RAIL_MFM_PingPongBufferConfig_t buffer;
///   RAIL_StateTiming_t timings;
///   RAIL_DataConfig_t dataConfig;
/// } mfmConfigApp_t;
///
/// static mfmConfigApp_t mfmConfig = {
///   .buffer = {
///     .pBuffer0 = (&mfmPingPongBuffers[0]),
///     .pBuffer1 = (&mfmPingPongBuffers[1]),
///     .bufferSizeWords = MFM_RAW_BUF_WORDS,
///   },
///   .timings = {
///     .idleToTx = 100,
///     .idleToRx = 0,
///     .rxToTx = 0,
///     .txToRx = 0,
///     .rxSearchTimeout = 0,
///     .txToRxSearchTimeout = 0
///   },
///   .dataConfig = {
///     .txSource = TX_MFM_DATA,
///     .rxSource = RX_PACKET_DATA,
///     .txMethod = PACKET_MODE,
///     .rxMethod = PACKET_MODE,
///   },
/// };
///
/// // Main RAIL events handler callback
/// static void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events)
/// {
///   // Increment TX counter
///   if (events & RAIL_EVENT_MFM_BUF_DONE) {
///       txCount++;
///       return;
///     }
///   }
/// }
///
/// void mfmInit(void)
/// {
///   // initialize MFM
///   uint32_t idx;
///   uint32_t *pDst0 = mfmConfig.buffer.pBuffer0;
///   uint32_t *pDst1 = mfmConfig.buffer.pBuffer1;
///   for (idx = 0; idx < (mfmConfig.buffer.bufferSizeWords / 4); idx++) {
///     pDst0[4 * idx + 0] = 0x755A3100;
///     pDst1[4 * idx + 0] = 0x755A3100;
///     pDst0[4 * idx + 1] = 0x315A757F;
///     pDst1[4 * idx + 1] = 0x315A757F;
///     pDst0[4 * idx + 2] = 0x8BA6CF00;
///     pDst1[4 * idx + 2] = 0x8BA6CF00;
///     pDst0[4 * idx + 3] = 0xCFA68B81;
///     pDst1[4 * idx + 3] = 0xCFA68B81;
///   }
///
///   RAIL_Status_t status;
///   status = RAIL_SetMfmPingPongFifo(railHandle, &mfmConfig.buffer);
///   assert(status == RAIL_STATUS_NO_ERROR);
///
///   status = RAIL_SetStateTiming(railHandle, &mfmConfig.timings);
///   assert(status == RAIL_STATUS_NO_ERROR);
///
///   mfmConfig.dataConfig.txSource = TX_MFM_DATA;
///   status = RAIL_ConfigData(railHandle, &mfmConfig.dataConfig);
///   assert(status == RAIL_STATUS_NO_ERROR);
///
///   // start transmitting
///   status = RAIL_StartTx(railHandle, 0, 0, NULL);
///   assert(status == RAIL_STATUS_NO_ERROR);
/// }
///
/// void mfmDeInit(void)
/// {
///   RAIL_Status_t status;
///   status = RAIL_StopTx(railHandle, RAIL_STOP_MODES_ALL);
///   assert(status == RAIL_STATUS_NO_ERROR);
///
///   mfmConfig.dataConfig.txSource = TX_PACKET_DATA;
///   status = RAIL_ConfigData(railHandle, &mfmConfig.dataConfig);
///   assert(status == RAIL_STATUS_NO_ERROR);
/// }
/// @endcode
///
/// @{

/**
 * @struct RAIL_MFM_PingPongBufferConfig_t
 * @brief A configuration structure for MFM Ping-pong buffer in RAIL.
 */
typedef struct RAIL_MFM_PingPongBufferConfig {
  /** Pointer to buffer 0. Must be 32-bit aligned. */
  uint32_t *pBuffer0;
  /** Pointer to buffer 1. Must be 32-bit aligned. */
  uint32_t *pBuffer1;
  /** Size of each buffer in 32-bit words. */
  uint32_t bufferSizeWords;
} RAIL_MFM_PingPongBufferConfig_t;

/**
 * Set MFM ping-pong buffer.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] config A non-NULL pointer to the MFM ping-pong buffer configuration structure.
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_SetMfmPingPongFifo(RAIL_Handle_t railHandle,
                                      const RAIL_MFM_PingPongBufferConfig_t *config);

/** @} */ // end of MFM

#ifdef __cplusplus
}
#endif

#endif // __RAIL_MFM_H__
