/*******************************************************************************
 * @file
 * @brief This file defines the radio counters updated by the efr32 PAL
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

#ifndef RADIO_COUNTERS_H_
#define RADIO_COUNTERS_H_

#include <stdint.h>

typedef struct efr32RadioCounters
{
    uint32_t mRailPlatTxTriggered;
    uint32_t mRailPlatRadioReceiveDoneCbCount;
    uint32_t mRailPlatRadioReceiveProcessedCount;
    uint32_t mRailPlatRadioEnergyScanDoneCbCount;
    uint32_t mRailPlatRadioTxDoneCbCount;
    uint32_t mRailTxStarted;
    uint32_t mRailTxStartFailed;
    uint32_t mRailEventAcksReceived;
    uint32_t mRailEventConfigScheduled;
    uint32_t mRailEventConfigUnScheduled;
    uint32_t mRailEventPacketSent;
    uint32_t mRailEventChannelBusy;
    uint32_t mRailEventEnergyScanCompleted;
    uint32_t mRailEventCalNeeded;
    uint32_t mRailEventPacketReceived;
    uint32_t mRailEventNoAck;
    uint32_t mRailEventTxAbort;
    uint32_t mRailEventSchedulerStatusError;
    uint32_t mRailEventsSchedulerStatusTransmitBusy;
    uint32_t mRailEventsSchedulerStatusLastStatus;
    uint32_t mRailEventsEnhAckTxFailed;
    uint32_t mRailEventsScheduledTxTriggeredCount;
    uint32_t mRailEventsScheduledTxStartedCount;
    union
    {
        // Allow for reuse / overloading of fields for various debugging
        uint32_t m32;
        uint16_t m16[2];
        uint8_t  m8[4]; // see defines below for suggested subfield usage
    } mRadioDebugData;
} efr32RadioCounters;

// Offsets for use with Radio Debug Data subfields
enum
{
    RX_DEBUG_COUNTER0 = 0,
    RX_DEBUG_COUNTER1,
    TX_DEBUG_COUNTER0,
    TX_DEBUG_COUNTER1
};
#endif // RADIO_COUNTERS_H_
