/*
 *  Copyright (c) 2024, The OpenThread Authors.
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
 *   This file defines the APIs required to support multi-channel feature on EFR devices.
 *
 */

#ifdef SL_CATALOG_RAIL_UTIL_IEEE802154_FAST_CHANNEL_SWITCHING_PRESENT
#include "sl_rail_util_ieee802154_fast_channel_switching_config.h"
#endif // SL_CATALOG_RAIL_UTIL_IEEE802154_FAST_CHANNEL_SWITCHING_PRESENT

#if SL_RAIL_UTIL_IEEE802154_FAST_CHANNEL_SWITCHING_ENABLED && !defined(SL_CATALOG_RAIL_MULTIPLEXER_PRESENT)
// When RAIL multiplexer is present, we handle fast channel switching in the multiplexer code
// turn off the feature here since OT lower mac is unaware of the goings on in the zigbee stack
#define FAST_CHANNEL_SWITCHING_SUPPORT 1
#else
#define FAST_CHANNEL_SWITCHING_SUPPORT 0
#endif // SL_RAIL_UTIL_IEEE802154_FAST_CHANNEL_SWITCHING_ENABLED && !defined(SL_CATALOG_RAIL_MULTIPLEXER_PRESENT)

/**
 * Check if the multi-channel feature is enabled.
 *
 * @retval  true    Multi-channel is enabled.
 * @retval  false   Multi-channel is disabled.
 *
 */
bool sl_is_multi_channel_enabled(void);

/**
 * Get the channel switching configuration.
 *
 * @param[out]   channelSwitchingCfg   Pointer to memory to store the switching config into.
 *
 * @retval  OT_ERROR_NONE           Channel switching config was copied succesfully.
 * @retval  OT_ERROR_INVALID_ARGS   Argument passed was invalid.
 *
 */
otError sl_get_channel_switching_cfg(RAIL_IEEE802154_RxChannelSwitchingCfg_t *channelSwitchingCfg);
