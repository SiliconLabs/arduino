/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
