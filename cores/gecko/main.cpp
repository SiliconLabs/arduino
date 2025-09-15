/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
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

#include "Arduino.h"

#ifdef ARDUINO_MATTER
#include <MatterConfig.h>
#endif

void arduino_task(void *p_arg);
inline static void handle_serial_events();
static const uint32_t arduino_task_stack_size = ARDUINO_MAIN_TASK_STACK_SIZE;
static const uint32_t arduino_task_priority = 1u;
static StackType_t arduino_task_stack[arduino_task_stack_size] = { 0 };
static StaticTask_t arduino_task_buffer;
static TaskHandle_t arduino_task_handle;
static bool system_init_finished = false;
static uint32_t system_reset_cause = 0u;

int main()
{
  // Save the reset cause
  system_reset_cause = RMU_ResetCauseGet();
  // The Matter SDK also gets the reset cause and clears it after, so we need to avoid clearing it here on Matter
  #ifndef ARDUINO_MATTER
  RMU_ResetCauseClear();
  #endif

  sl_system_init();
  init_arduino_variant();
  system_init_finished = true;

  escape_hatch();
  gpio_interrupt_handler_init();

  arduino_task_handle = xTaskCreateStatic(arduino_task,
                                          "arduino_task",
                                          arduino_task_stack_size,
                                          NULL,
                                          arduino_task_priority,
                                          arduino_task_stack,
                                          &arduino_task_buffer);
  app_assert(NULL != arduino_task_handle, "Arduino task creation failed");

  #ifdef ARDUINO_MATTER
  // Initialize the Matter stack which also starts the FreeRTOS kernel
  SilabsMatterConfig::AppInit();
  #else
  sl_system_kernel_start();
  #endif

  return 0;
}

void arduino_task(void *p_arg)
{
  (void)p_arg;
  setup();
  while (1) {
    loop();
    handle_serial_events();
    taskYIELD();
  }
}

inline static void handle_serial_events()
{
  Serial.task();
  Serial.handleSerialEvent();

  #if (NUM_HW_SERIAL > 1)
  Serial1.task();
  Serial1.handleSerialEvent();
  #endif // #if (NUM_HW_SERIAL > 1)
}

bool get_system_init_finished()
{
  return system_init_finished;
}

uint32_t get_system_reset_cause()
{
  return system_reset_cause;
}

SL_WEAK void escape_hatch()
{
  ;
}
