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

#include "arduino_serial_config.h"
extern "C" {
  #include "em_cmu.h"
}

sl_iostream_t* sl_serial_stream_handle = sl_iostream_instance_vcom_info.handle;
sl_iostream_uart_t* sl_serial_instance_handle = sl_iostream_uart_vcom_handle;

void sl_serial_set_baud_rate(uint32_t baudrate)
{
  USART_BaudrateAsyncSet(SL_SERIAL_PERIPHERAL, 0, baudrate, usartOVS16);
}

void sl_serial_init()
{
  sl_iostream_usart_init_vcom();
}

void sl_serial_deinit()
{
  sl_iostream_uart_deinit(sl_serial_instance_handle);

  // Reset the USART to GPIO peripheral routing to enable the pins to function as GPIO
  GPIO->USARTROUTE[SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO].ROUTEEN = 0;
  GPIO->USARTROUTE[SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO].TXROUTE = 0;
  GPIO->USARTROUTE[SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO].RXROUTE = 0;
  GPIO->USARTROUTE[SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO].CTSROUTE = 0;
  GPIO->USARTROUTE[SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO].RTSROUTE = 0;
}

sl_iostream_t* sl_serial1_stream_handle = sl_iostream_instance_mikroe_info.handle;
sl_iostream_uart_t* sl_serial1_instance_handle = sl_iostream_uart_mikroe_handle;

void sl_serial1_set_baud_rate(uint32_t baudrate)
{
  EUSART_BaudrateSet(SL_SERIAL1_PERIPHERAL, 0, baudrate);
}

void sl_serial1_init()
{
  sl_iostream_eusart_init_mikroe();
}

void sl_serial1_deinit()
{
  sl_iostream_uart_deinit(sl_serial1_instance_handle);

  // Reset the USART to GPIO peripheral routing to enable the pins to function as GPIO
  GPIO->EUARTROUTE[SL_IOSTREAM_EUSART_MIKROE_PERIPHERAL_NO].ROUTEEN = 0;
  GPIO->EUARTROUTE[SL_IOSTREAM_EUSART_MIKROE_PERIPHERAL_NO].TXROUTE = 0;
  GPIO->EUARTROUTE[SL_IOSTREAM_EUSART_MIKROE_PERIPHERAL_NO].RXROUTE = 0;
  GPIO->EUARTROUTE[SL_IOSTREAM_EUSART_MIKROE_PERIPHERAL_NO].CTSROUTE = 0;
  GPIO->EUARTROUTE[SL_IOSTREAM_EUSART_MIKROE_PERIPHERAL_NO].RTSROUTE = 0;
}
