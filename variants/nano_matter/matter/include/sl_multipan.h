/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief
 *   Helper functions for Silicon Labs Multipan implementation.
 */

#ifndef SL_MULTIPAN_H_
#define SL_MULTIPAN_H_

#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
#include <openthread/platform/multipan.h>
#endif
#include "utils/code_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
#define RADIO_INTERFACE_COUNT (OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_NUM + 1)
#else
#define RADIO_INTERFACE_COUNT 1
extern otInstance *sInstance;
#endif

#define INVALID_INTERFACE_INDEX (0xFF)

typedef enum
{
    EFR32_IID_BCAST   = 0,
    EFR32_IID_1       = 1,
    EFR32_IID_2       = 2,
    EFR32_IID_3       = 3,
    EFR32_IID_INVALID = 0xFF
} efr32Iid_t;

/*
 * RAIL accepts 3 pan indices 0, 1 or 2. But valid IIDs are 1, 2 and 3 (0 is reserved for bcast).
 * This API validates the passed IID and converts it into usable PanIndex.
 */

static inline uint8_t efr32GetPanIndexFromIid(uint8_t iid)
{
    uint8_t panIndex = 0;

#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
    otEXPECT_ACTION(((iid < RADIO_INTERFACE_COUNT) && (iid != 0)), panIndex = INVALID_INTERFACE_INDEX);
    panIndex = iid - 1;
exit:
#else
    panIndex = iid;
#endif

    return panIndex;
}

static inline otInstance *efr32GetInstanceFromIid(efr32Iid_t aIid)
{
#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
    return otPlatMultipanIidToInstance((uint8_t)aIid);
#else
    OT_UNUSED_VARIABLE(aIid);
    return sInstance;
#endif
}

static inline uint8_t efr32GetIidFromInstance(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
    return otPlatMultipanInstanceToIid(aInstance);
#else
    return 0;
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SL_MULTIPAN_H_
