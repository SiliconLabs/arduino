/***************************************************************************//**
 * @file
 * @brief NVM3 page handling functions
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

#ifndef NVM3_PAGE_H
#define NVM3_PAGE_H

#include "nvm3_hal.h"
#include "nvm3_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

#define NVM3_PAGE_COUNTER_SIZE          27U         // 27 bits
#define NVM3_PAGE_COUNTER_MASK          ((1U << NVM3_PAGE_COUNTER_SIZE) - 1U)
#define NVM3_PAGE_BCCB_SIZE             5U          //  5 bits
#define NVM3_PAGE_BCCB_MASK             ((1U << NVM3_PAGE_BCCB_SIZE) - 1U)

#define NVM3_PAGE_HEADER_WORDS          5           // The number of words in the page header
#define NVM3_PAGE_HEADER_SIZE           (sizeof(nvm3_PageHdr_t))
#define NVM3_PAGE_HEADER_WSIZE          (sizeof(nvm3_PageHdr_t) / sizeof(uint32_t))
#define NVM3_PAGE_H1_OFFSET             (0 * sizeof(uint32_t))
#define NVM3_PAGE_H2_OFFSET             (1 * sizeof(uint32_t))
#define NVM3_PAGE_H3_OFFSET             (2 * sizeof(uint32_t))
#define NVM3_PAGE_H4_OFFSET             (3 * sizeof(uint32_t))
#define NVM3_PAGE_H5_OFFSET             (4 * sizeof(uint32_t))

#define NVM3_ERASE_COUNT_INVALID        0xFFFFFFFFU
#define NVM3_PAGE_INDEX_INVALID         0xFFFFU

typedef struct nvm3_PageHdr {
  uint32_t    data[NVM3_PAGE_HEADER_WORDS];
} nvm3_PageHdr_t;

/// @endcond

typedef enum {
  nvm3_PageStateGood,
  nvm3_PageStateGoodEip,
  nvm3_PageStateBad,
  nvm3_PageStateInvalidErased,
  nvm3_PageStateInvalidUnknown,
} nvm3_PageState_t;

#if defined(NVM3_SECURITY)
sl_status_t nvm3_pageHeaderWrite(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr, uint32_t eraseCnt, nvm3_HalInfo_t *halInfo, nvm3_SecurityType_t secType);
#else
sl_status_t nvm3_pageHeaderWrite(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr, uint32_t eraseCnt, nvm3_HalInfo_t *halInfo);
#endif
void nvm3_pageSetBad(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr);
sl_status_t nvm3_pageSetEip(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr);
uint32_t nvm3_pageGetEraseCnt(nvm3_PageHdr_t *pageHdr);
nvm3_PageState_t nvm3_pageGetState(nvm3_PageHdr_t *pageHdr);
bool nvm3_pageStateIsGood(nvm3_PageState_t pageState);
bool nvm3_pageStateIsInvalid(nvm3_PageState_t pageState);
nvm3_ObjPtr_t nvm3_pageGetFirstObj(nvm3_HalPtr_t pageAdr);
#if defined(NVM3_SECURITY)
nvm3_SecurityType_t nvm3_pageGetSecType(nvm3_PageHdr_t *pageHdr);
sl_status_t nvm3_pageErase(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr, uint32_t eraseCnt, nvm3_HalInfo_t *halInfo, nvm3_SecurityType_t secType);
#else
sl_status_t nvm3_pageErase(const nvm3_HalHandle_t *hal, nvm3_HalPtr_t pageAdr, uint32_t eraseCnt, nvm3_HalInfo_t *halInfo);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NVM3_PAGE_H */
