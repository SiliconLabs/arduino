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

#ifndef SL_BTCTRL_RTOS_CONFIG_H
#define SL_BTCTRL_RTOS_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Priority Configuration for Bluetooth RTOS Tasks

// <o SL_BTCTRL_RTOS_LINK_LAYER_TASK_PRIORITY> Bluetooth link layer task priority
// <i> Default: 52 (CMSIS-RTOS2 osPriorityRealtime4)
// <i> Define the priority of the Bluetooth link layer task. This must be a valid
// <i> priority value from CMSIS-RTOS2 osPriority_t definition.
#define SL_BTCTRL_RTOS_LINK_LAYER_TASK_PRIORITY     (52)

// <o SL_BTCTRL_RTOS_LINK_LAYER_TASK_STACK_SIZE> Bluetooth link layer task stack size in bytes
// <i> Default: 1000
// <i> Define the stack size of the Bluetooth link layer task. The value is in bytes
// <i> and will be word aligned when it is applied at the task creation.
#ifdef _SILICON_LABS_32B_SERIES_3
#define SL_BTCTRL_RTOS_LINK_LAYER_TASK_STACK_SIZE   (1312)
#else
#define SL_BTCTRL_RTOS_LINK_LAYER_TASK_STACK_SIZE   (1000)
#endif // _SILICON_LABS_32B_SERIES_3

// </h> End Priority Configuration for Bluetooth RTOS Tasks

// <<< end of configuration section >>>

#endif // SL_BT_RTOS_CONFIG_H
