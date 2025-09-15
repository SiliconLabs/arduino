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

#ifndef SL_IOSTREAM_INIT_EUSART_INSTANCES_H
#define SL_IOSTREAM_INIT_EUSART_INSTANCES_H

#include "sl_iostream.h"
#include "sl_iostream_uart.h"
#ifdef __cplusplus
extern "C" {
#endif

extern sl_iostream_t *sl_iostream_vcom_handle;
extern sl_iostream_uart_t *sl_iostream_uart_vcom_handle;
extern sl_iostream_instance_info_t sl_iostream_instance_vcom_info;

extern sl_iostream_t *sl_iostream_xg24explorerkit1_handle;
extern sl_iostream_uart_t *sl_iostream_uart_xg24explorerkit1_handle;
extern sl_iostream_instance_info_t sl_iostream_instance_xg24explorerkit1_info;

// Initialize only iostream eusart instance(s)
void sl_iostream_eusart_init_instances(void);

sl_status_t sl_iostream_eusart_init_vcom(void);
sl_status_t sl_iostream_eusart_init_xg24explorerkit1(void);

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)

sl_power_manager_on_isr_exit_t sl_iostream_eusart_vcom_sleep_on_isr_exit(void);

sl_power_manager_on_isr_exit_t sl_iostream_eusart_xg24explorerkit1_sleep_on_isr_exit(void);

#endif

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_INIT_EUSART_INSTANCES_H
