/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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



#include <sl_common.h>
#include "sl_bluetooth.h"
#include "sl_assert.h"
#include "sl_bt_stack_init.h"
#include "sl_component_catalog.h"
#if !defined(SL_CATALOG_KERNEL_PRESENT)
/**
 * Override @ref PendSV_Handler for the Link Layer task when Bluetooth runs
 * in baremetal mode. The override must not exist when Bluetooth runs in an RTOS
 * where the link layer task runs in a thread.
 */
#include <em_device.h>
void PendSV_Handler()
{
  sl_bt_priority_handle();
}
#endif

/**
 * Internal stack function to start the Bluetooth stack.
 *
 * @return SL_STATUS_OK if the stack was successfully started
 */
extern sl_status_t sli_bt_system_start_bluetooth();

void sl_bt_init(void)
{
#if !defined(SL_CATALOG_KERNEL_PRESENT)
  NVIC_ClearPendingIRQ(PendSV_IRQn);
  NVIC_EnableIRQ(PendSV_IRQn);
#endif

  // Stack initialization could fail, e.g., due to out of memory.
  // The failure could not be returned to user as the system initialization
  // does not return an error code. Use the EFM_ASSERT to catch the failure,
  // which requires either DEBUG_EFM or DEBUG_EFM_USER is defined.
  sl_status_t err = sl_bt_stack_init();
  EFM_ASSERT(err == SL_STATUS_OK);

  // When neither Bluetooth on-demand start feature nor an RTOS is present, the
  // Bluetooth stack is always started already at init-time.
#if !defined(SL_CATALOG_BLUETOOTH_ON_DEMAND_START_PRESENT) && !defined(SL_CATALOG_KERNEL_PRESENT)
  err = sli_bt_system_start_bluetooth();
  EFM_ASSERT(err == SL_STATUS_OK);
#endif
}

SL_WEAK void sl_bt_on_event(sl_bt_msg_t* evt)
{
  (void)(evt);
}

void sl_bt_process_event(sl_bt_msg_t *evt)
{
  sl_bt_on_event(evt);
}

#if !defined(SL_CATALOG_KERNEL_PRESENT)
// When running in an RTOS, the stack events are processed in a dedicated
// event processing task, and these functions are not used at all.

SL_WEAK bool sl_bt_can_process_event(uint32_t len)
{
  (void)(len);
  return true;
}

void sl_bt_step(void)
{
  sl_bt_msg_t evt;

  sl_bt_run();
  uint32_t event_len = sl_bt_event_pending_len();
  // For preventing from data loss, the event will be kept in the stack's queue
  // if application cannot process it at the moment.
  if ((event_len == 0) || (!sl_bt_can_process_event(event_len))) {
    return;
  }

  // Pop (non-blocking) a Bluetooth stack event from event queue.
  sl_status_t status = sl_bt_pop_event(&evt);
  if(status != SL_STATUS_OK){
    return;
  }
  sl_bt_process_event(&evt);
}
#endif // !defined(SL_CATALOG_KERNEL_PRESENT)
