/***************************************************************************//**
 * @file
 * @brief DMADRV API definition.
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

#ifndef __SILICON_LABS_DMADRV_H__
#define __SILICON_LABS_DMADRV_H__

#include "em_device.h"

#include "ecode.h"

#include "dmadrv_signals.h"

#if defined(LDMA_PRESENT) && (LDMA_COUNT == 1)
#if (_SILICON_LABS_32B_SERIES > 2)
#define EMDRV_DMADRV_LDMA_S3
#else
#define EMDRV_DMADRV_DMA_PRESENT
#define EMDRV_DMADRV_LDMA
#endif
#else
#error "No valid DMA engine defined."
#endif

#include "dmadrv_config.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup dmadrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup dmadrv_error_codes Error Codes
 * @{
 ******************************************************************************/

#define ECODE_EMDRV_DMADRV_OK                  (ECODE_OK)                               ///< A successful return value.
#define ECODE_EMDRV_DMADRV_PARAM_ERROR         (ECODE_EMDRV_DMADRV_BASE | 0x00000001)   ///< An illegal input parameter.
#define ECODE_EMDRV_DMADRV_NOT_INITIALIZED     (ECODE_EMDRV_DMADRV_BASE | 0x00000002)   ///< DMA is not initialized.
#define ECODE_EMDRV_DMADRV_ALREADY_INITIALIZED (ECODE_EMDRV_DMADRV_BASE | 0x00000003)   ///< DMA has already been initialized.
#define ECODE_EMDRV_DMADRV_CHANNELS_EXHAUSTED  (ECODE_EMDRV_DMADRV_BASE | 0x00000004)   ///< No DMA channels available.
#define ECODE_EMDRV_DMADRV_IN_USE              (ECODE_EMDRV_DMADRV_BASE | 0x00000005)   ///< DMA is in use.
#define ECODE_EMDRV_DMADRV_ALREADY_FREED       (ECODE_EMDRV_DMADRV_BASE | 0x00000006)   ///< A DMA channel was free.
#define ECODE_EMDRV_DMADRV_CH_NOT_ALLOCATED    (ECODE_EMDRV_DMADRV_BASE | 0x00000007)   ///< A channel is not reserved.

/** @} (end addtogroup error codes) */
/***************************************************************************//**
 * @brief
 *  DMADRV transfer completion callback function.
 *
 * @details
 *  The callback function is called when a transfer is complete.
 *
 * @param[in] channel
 *  The DMA channel number.
 *
 * @param[in] sequenceNo
 *  The number of times the callback was called. Useful on long chains of
 *  linked transfers or on endless ping-pong type transfers.
 *
 * @param[in] userParam
 *  Optional user parameter supplied on DMA invocation.
 *
 * @return
 *   When doing ping-pong transfers, return true to continue or false to
 *   stop transfers.
 ******************************************************************************/
typedef bool (*DMADRV_Callback_t)(unsigned int channel,
                                  unsigned int sequenceNo,
                                  void *userParam);

Ecode_t DMADRV_AllocateChannel(unsigned int *channelId,
                               void         *capabilities);
Ecode_t DMADRV_DeInit(void);
Ecode_t DMADRV_FreeChannel(unsigned int channelId);
Ecode_t DMADRV_Init(void);

Ecode_t DMADRV_MemoryPeripheral(unsigned int              channelId,
                                DMADRV_PeripheralSignal_t peripheralSignal,
                                void                      *dst,
                                void                      *src,
                                bool                      srcInc,
                                int                       len,
                                DMADRV_DataSize_t         size,
                                DMADRV_Callback_t         callback,
                                void                      *cbUserParam);
Ecode_t DMADRV_PeripheralMemory(unsigned int              channelId,
                                DMADRV_PeripheralSignal_t peripheralSignal,
                                void                      *dst,
                                void                      *src,
                                bool                      dstInc,
                                int                       len,
                                DMADRV_DataSize_t         size,
                                DMADRV_Callback_t         callback,
                                void                      *cbUserParam);
Ecode_t DMADRV_MemoryPeripheralPingPong(unsigned int              channelId,
                                        DMADRV_PeripheralSignal_t peripheralSignal,
                                        void                      *dst,
                                        void                      *src0,
                                        void                      *src1,
                                        bool                      srcInc,
                                        int                       len,
                                        DMADRV_DataSize_t         size,
                                        DMADRV_Callback_t         callback,
                                        void                      *cbUserParam);
Ecode_t DMADRV_PeripheralMemoryPingPong(unsigned int              channelId,
                                        DMADRV_PeripheralSignal_t peripheralSignal,
                                        void                      *dst0,
                                        void                      *dst1,
                                        void                      *src,
                                        bool                      dstInc,
                                        int                       len,
                                        DMADRV_DataSize_t         size,
                                        DMADRV_Callback_t         callback,
                                        void                      *cbUserParam);

#if defined(EMDRV_DMADRV_LDMA)
Ecode_t DMADRV_LdmaStartTransfer(int                channelId,
                                 LDMA_TransferCfg_t *transfer,
                                 LDMA_Descriptor_t  *descriptor,
                                 DMADRV_Callback_t  callback,
                                 void               *cbUserParam);
#elif defined(EMDRV_DMADRV_LDMA_S3)
Ecode_t DMADRV_LdmaStartTransfer(int                            channelId,
                                 sl_hal_ldma_transfer_config_t  *transfer,
                                 sl_hal_ldma_descriptor_t       *descriptor,
                                 DMADRV_Callback_t              callback,
                                 void                           *cbUserParam);
#endif

Ecode_t DMADRV_PauseTransfer(unsigned int channelId);
Ecode_t DMADRV_ResumeTransfer(unsigned int channelId);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DMADRV, SL_CODE_CLASS_TIME_CRITICAL)
Ecode_t DMADRV_StopTransfer(unsigned int channelId);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DMADRV, SL_CODE_CLASS_TIME_CRITICAL)
Ecode_t DMADRV_TransferActive(unsigned int channelId,
                              bool         *active);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DMADRV, SL_CODE_CLASS_TIME_CRITICAL)
Ecode_t DMADRV_TransferCompletePending(unsigned int channelId,
                                       bool         *pending);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DMADRV, SL_CODE_CLASS_TIME_CRITICAL)
Ecode_t DMADRV_TransferDone(unsigned int channelId,
                            bool         *done);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DMADRV, SL_CODE_CLASS_TIME_CRITICAL)
Ecode_t DMADRV_TransferRemainingCount(unsigned int channelId,
                                      int          *remaining);

/** @} (end addtogroup dmadrv) */

#ifdef __cplusplus
}
#endif

#endif /* __SILICON_LABS_DMADRV_H__ */
