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

extern "C" {
  #include "em_usart.h"
  #include "em_eusart.h"
}
#include "sl_spidrv_instances.h"
#include "sl_spidrv_usart_mikroe_config.h"
#include "sl_spidrv_eusart_xg24explorerkit1_config.h"

#ifndef ARDUINO_SPI_CONFIG_H
#define ARDUINO_SPI_CONFIG_H

#define SL_SPIDRV_PERIPHERAL SL_SPIDRV_USART_MIKROE_PERIPHERAL

extern SPIDRV_Init_t sl_spidrv_config;
#define SL_SPIDRV_PERIPHERAL_HANDLE sl_spidrv_usart_mikroe_handle

// Not used - just here to silence the compiler warnings
#define SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PORT       gpioPortB
#define SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PIN        2

extern SPIDRV_Init_t sl_spidrv_config_spi1;
#define SL_SPIDRV1_PERIPHERAL_HANDLE sl_spidrv_eusart_xg24explorerkit1_handle

uint8_t sl_spi_direct_transfer(void* spi_peripheral, uint8_t data);

#endif // ARDUINO_SPI_CONFIG_H
