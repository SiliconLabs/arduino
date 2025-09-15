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

#include "arduino_spi_config.h"

SPIDRV_Init_t sl_spidrv_config = {
  .port = SL_SPIDRV_USART_MIKROE_PERIPHERAL,
  .portTx = SL_SPIDRV_USART_MIKROE_TX_PORT,
  .portRx = SL_SPIDRV_USART_MIKROE_RX_PORT,
  .portClk = SL_SPIDRV_USART_MIKROE_CLK_PORT,
#if defined(SL_SPIDRV_USART_MIKROE_CS_PORT)
  .portCs = SL_SPIDRV_USART_MIKROE_CS_PORT,
#endif
  .pinTx = SL_SPIDRV_USART_MIKROE_TX_PIN,
  .pinRx = SL_SPIDRV_USART_MIKROE_RX_PIN,
  .pinClk = SL_SPIDRV_USART_MIKROE_CLK_PIN,
#if defined(SL_SPIDRV_USART_MIKROE_CS_PIN)
  .pinCs = SL_SPIDRV_USART_MIKROE_CS_PIN,
#endif
  .bitRate = SL_SPIDRV_USART_MIKROE_BITRATE,
  .frameLength = SL_SPIDRV_USART_MIKROE_FRAME_LENGTH,
  .dummyTxValue = 0,
  .type = SL_SPIDRV_USART_MIKROE_TYPE,
  .bitOrder = SL_SPIDRV_USART_MIKROE_BIT_ORDER,
  .clockMode = SL_SPIDRV_USART_MIKROE_CLOCK_MODE,
  .csControl = SL_SPIDRV_USART_MIKROE_CS_CONTROL,
  .slaveStartMode = SL_SPIDRV_USART_MIKROE_SLAVE_START_MODE,
};

SPIDRV_Init_t sl_spidrv_config_spi1 = {
  .port = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_PERIPHERAL,
  .portTx = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_TX_PORT,
  .portRx = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_RX_PORT,
  .portClk = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_SCLK_PORT,
#if defined(SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PORT)
  .portCs = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PORT,
#endif
  .pinTx = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_TX_PIN,
  .pinRx = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_RX_PIN,
  .pinClk = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_SCLK_PIN,
#if defined(SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PIN)
  .pinCs = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PIN,
#endif
  .bitRate = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_BITRATE,
  .frameLength = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_FRAME_LENGTH,
  .dummyTxValue = 0,
  .type = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_TYPE,
  .bitOrder = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_BIT_ORDER,
  .clockMode = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CLOCK_MODE,
  .csControl = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_CONTROL,
  .slaveStartMode = SL_SPIDRV_EUSART_XG24EXPLORERKIT1_SLAVE_START_MODE,
};

uint8_t sl_spi_direct_transfer(void* spi_peripheral, uint8_t data)
{
  if ((USART_TypeDef*)spi_peripheral == USART0) {
    return USART_SpiTransfer((USART_TypeDef*)spi_peripheral, data);
  } else if ((EUSART_TypeDef*)spi_peripheral == EUSART0 || (EUSART_TypeDef*)spi_peripheral == EUSART1) {
    return EUSART_Spi_TxRx((EUSART_TypeDef*)spi_peripheral, (uint16_t)data);
  }
  return 0;
}
