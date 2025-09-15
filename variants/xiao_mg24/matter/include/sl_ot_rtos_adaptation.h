/*******************************************************************************
 * @file
 * @brief CMSIS RTOS2 adaptation for running OpenThread in RTOS
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

#ifndef OPENTHREAD_RTOS
#define OPENTHREAD_RTOS

#include <stdbool.h>

typedef enum sl_ot_rtos_event_t
{
    SL_OT_RTOS_EVENT_STACK  = 1 << 0,
    SL_OT_RTOS_EVENT_SERIAL = 1 << 1,
    SL_OT_RTOS_EVENT_APP    = 1 << 2
} sl_ot_rtos_event_t;

/**
 * API to initialize RTOS 'OT Stack' and 'OT Serial' Tasks.
 *
 * Calling this API results in the 'OT Stack' and 'OT Serial' tasks being
 * created. 'OT Serial' task creation is controlled by the config item
 * @p SL_OPENTHREAD_ENABLE_SERIAL_TASK. This is enabled by default.
 *
 */
void sl_ot_rtos_stack_init(void);

/**
 * API to initialize RTOS App task.
 *
 * Calling this API results in app task being created. based on the config item
 * @p SL_OPENTHREAD_ENABLE_APP_TASK.
 *
 * This is enabled by default.
 *
 */
void sl_ot_rtos_app_init(void);

/**
 * Pend an event to a task based on the event received.
 *
 * Based on the event received as input pend one of the tasks for operation
 *
 * @param[in]   event    Type of event received, can take the values indicated in @p sl_ot_rtos_event_t
 *
 */
void sl_ot_rtos_set_pending_event(sl_ot_rtos_event_t event);

/**
 * RTOS tick for application. Will be called anytime app task is running.
 */
void sl_ot_rtos_application_tick(void);

/**
 * Mutex handler, API to obtain access to the stack.
 *
 * A weak implementation is provided, user can implement their own mutex handlers.
 *
 */
void sl_ot_rtos_acquire_stack_mutex(void);

/**
 * Mutex handler, API to relinquish access to the stack.
 *
 * A weak implementation is provided, user can implement their own mutex handlers.
 *
 */
void sl_ot_rtos_release_stack_mutex(void);

/**
 * Mutex handler, API to try to obtain access to the stack.
 *
 * Return true if access granted, false otherwise.
 *
 * A weak implementation is provided, user can implement their own mutex handlers.
 *
 */
bool sl_ot_rtos_try_acquire_stack_mutex(void);

/**
 * Return if the current thread can access the PAL APIs.
 *
 * @retval  True    Current thread can access the PAL APIs.
 * @retval  False   Current thread cannot access the PAL APIs.
 *
 */
bool sl_ot_rtos_task_can_access_pal(void);

#endif
