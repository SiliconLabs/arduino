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
 *   This file defines platform specific alarm methods.
 *
 */

#ifndef _ALARM_H
#define _ALARM_H

#include <stdbool.h>
#include <stdint.h>
#include "openthread/instance.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"

/**
 * This function determines whether the device should sleep after an alarm triggers.
 */
bool efr32AlarmIsReady(void);

#endif // SL_CATALOG_POWER_MANAGER_PRESENT

/**
 * This function initializes the alarm service used by OpenThread.
 *
 */
void efr32AlarmInit(void);

/**
 * This function provides the remaining time (in milliseconds) on an alarm service.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
uint64_t efr32AlarmPendingTime(otInstance *aInstance);

/**
 * This function checks if the alarm service is running.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
bool efr32AlarmIsRunning(otInstance *aInstance);

/**
 * This function performs alarm driver processing.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void efr32AlarmProcess(otInstance *aInstance);

#endif // _ALARM_H
