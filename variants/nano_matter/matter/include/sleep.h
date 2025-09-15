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
 *   This file includes the initializers for supporting OpenThread with
 * power manager.
 *
 */

#ifndef SLEEP_H_
#define SLEEP_H_

#include <stdbool.h>

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

/**
 * This function initializes the sleep interface
 * and starts the platform in an active state.
 *
 */
void sl_ot_sleep_init(void);

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
/**
 * This function notifies the platform to refresh sleep requirements.
 *
 */
void sl_ot_sleep_update(void);

/**
 * This function notifies power manager whether OpenThread will
 * prevent the system from sleeping when sleep is requested.
 *
 * This function is only used for bare metal applications.
 *
 * @retval true     The OpenThread power manager module will not prevent app from sleeping.
 *         false    The OpenThread power manager module will prevent app from sleeping.
 *
 */
bool sl_ot_is_ok_to_sleep(void);

/**
 * This function notifies power manager whether OpenThread will
 * prevent the system from sleeping when an ISR that interrupt sleep exits.
 *
 * This function is only used for bare metal applications.
 *
 * @retval SL_POWER_MANAGER_IGNORE  The OpenThread power manager module will not influence
 *                                  power manager when deciding to sleep after ISR exit
 *         SL_POWER_MANAGER_WAKEUP  The OpenThread power manager module will prevent
 *                                  power manager from entering sleep after ISR exit.
 */
sl_power_manager_on_isr_exit_t sl_ot_sleep_on_isr_exit(void);
#endif

#endif // SLEEP_H_
