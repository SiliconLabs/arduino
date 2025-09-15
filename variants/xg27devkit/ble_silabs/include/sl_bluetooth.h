/***************************************************************************//**
 * @file
 * @brief Bluetooth initialization and event processing
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdbool.h>
#include "sl_power_manager.h"
#include "sl_bluetooth_config.h"
#include "sl_bt_api.h"
#define SL_BT_COMPONENT_ADVERTISERS 0

#define SL_BT_COMPONENT_CONNECTIONS 0

// Initialize Bluetooth core functionality
void sl_bt_init(void);

// Polls bluetooth stack for an event and processes it
void sl_bt_step(void);

/**
 * Tell if the application can process a new Bluetooth event in its current
 * state, for example, based on resource availability status.
 * If true is returned by this function, sl_bt_process_event will be called
 * for event processing. Otherwise, the event will be kept in stack's event
 * queue until the application can process it.
 *
 * @note Default implementation of this function returns true.
 * Application can override it for own business logic.
 *
 * @param len Data length of the event
 * @return ture if event can be processed; false otherwise
 */
bool sl_bt_can_process_event(uint32_t len);

// Processes a single bluetooth event
void sl_bt_process_event(sl_bt_msg_t *evt);

void sl_bt_on_event(sl_bt_msg_t* evt);

// Power Manager related functions
bool sli_bt_is_ok_to_sleep(void);
sl_power_manager_on_isr_exit_t sli_bt_sleep_on_isr_exit(void);

#endif // BLUETOOTH_H
