/*******************************************************************************
 * @file
 * @brief This file includes the Radio Coex initializers
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

#ifndef RADIO_COEX_H_
#define RADIO_COEX_H_

#include <assert.h>
#include <openthread-core-config.h>
#include <openthread-system.h>
#include <openthread/link.h>
#include <openthread/platform/alarm-micro.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/diag.h>
#include <openthread/platform/radio.h>
#include <openthread/platform/time.h>

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#ifndef SL_CATALOG_OT_SIMULATION_PRESENT

#ifdef SL_CATALOG_RAIL_UTIL_COEX_PRESENT
#include "coexistence-802154.h"
#include "coexistence-ot.h"
#include "coexistence.h"
#endif // SL_CATALOG_RAIL_UTIL_COEX_PRESENT

#ifdef SL_CATALOG_RAIL_UTIL_ANT_DIV_PRESENT
#include "sl_rail_util_ant_div.h"
#endif // SL_CATALOG_RAIL_UTIL_ANT_DIV_PRESENT

#ifdef SL_CATALOG_RAIL_UTIL_IEEE802154_PHY_SELECT_PRESENT
#include "sl_rail_util_ieee802154_phy_select.h"
#endif // SL_CATALOG_RAIL_UTIL_IEEE802154_PHY_SELECT

#else // SL_CATALOG_OT_SIMULATION_PRESENT

#include "rail_util_simulation.h"

#endif // SL_CATALOG_OT_SIMULATION_PRESENT

#include "common/code_utils.hpp"
#include "utils/code_utils.h"

typedef enum
{
    SL_OT_COEX_EVENT_REQUEST_COUNT = 0,
    SL_OT_COEX_EVENT_GRANT_IMMEDIATE_COUNT,
    SL_OT_COEX_EVENT_GRANT_WAIT_COUNT,
    SL_OT_COEX_EVENT_GRANT_WAIT_ACTIVATED_COUNT,
    SL_OT_COEX_EVENT_GRANT_WAIT_TIMEOUT_COUNT,
    SL_OT_COEX_EVENT_GRANT_DEACTIVATED_DURING_REQUEST_COUNT,
    SL_OT_COEX_EVENT_DELAYED_GRANT_COUNT,
    SL_OT_COEX_EVENT_AVG_REQUEST_TO_GRANT_TIME
} sl_coex_events;

typedef struct sl_ot_coex_counter
{
    otRadioCoexMetrics metrics;
    uint64_t           timestamp;
    uint64_t           totalTxReqToGrantDuration;
    uint64_t           totalRxReqToGrantDuration;
} sl_ot_coex_counter_t;

/**
 * Reset the radio coex counters.
 */
void sli_radio_coex_reset(void);

#endif // RADIO_COEX_H_
