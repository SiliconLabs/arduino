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

// This file is an adapter between the different USART/EUSART instances and the Arduino Core driver
#ifndef ARDUINO_SERIAL_CONFIG_H
#define ARDUINO_SERIAL_CONFIG_H

#include "sl_iostream_eusart_vcom_config.h"
#include "sl_iostream_eusart_xg24explorerkit1_config.h"
extern "C" {
  #include "em_usart.h"
  #include "em_eusart.h"
  #include "sl_iostream_handles.h"
  #include "sl_iostream_uart.h"
}

#define SL_SERIAL_PERIPHERAL SL_IOSTREAM_EUSART_VCOM_PERIPHERAL

extern sl_iostream_t* sl_serial_stream_handle;
extern sl_iostream_uart_t* sl_serial_instance_handle;
void sl_serial_set_baud_rate(uint32_t baudrate);
void sl_serial_init();
void sl_serial_deinit();

#define SL_SERIAL1_PERIPHERAL SL_IOSTREAM_EUSART_XG24EXPLORERKIT1_PERIPHERAL

extern sl_iostream_t* sl_serial1_stream_handle;
extern sl_iostream_uart_t* sl_serial1_instance_handle;
void sl_serial1_set_baud_rate(uint32_t baudrate);
void sl_serial1_init();
void sl_serial1_deinit();

#endif // ARDUINO_SERIAL_CONFIG_H
