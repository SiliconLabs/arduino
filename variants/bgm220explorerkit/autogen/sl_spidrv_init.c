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

#include "spidrv.h"
#include "sl_spidrv_instances.h"
#include "sl_assert.h"


#include "sl_spidrv_usart_mikroe_config.h"

SPIDRV_HandleData_t sl_spidrv_usart_mikroe_handle_data;
SPIDRV_Handle_t sl_spidrv_usart_mikroe_handle = &sl_spidrv_usart_mikroe_handle_data;

SPIDRV_Init_t sl_spidrv_usart_init_mikroe = {
  .port = SL_SPIDRV_USART_MIKROE_PERIPHERAL,
#if defined(_USART_ROUTELOC0_MASK)
  .portLocationTx = SL_SPIDRV_USART_MIKROE_TX_LOC,
  .portLocationRx = SL_SPIDRV_USART_MIKROE_RX_LOC,
  .portLocationClk = SL_SPIDRV_USART_MIKROE_CLK_LOC,
#if defined(SL_SPIDRV_USART_MIKROE_CS_LOC)
  .portLocationCs = SL_SPIDRV_USART_MIKROE_CS_LOC,
#endif
#elif defined(_GPIO_USART_ROUTEEN_MASK)
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
#else
  .portLocation = SL_SPIDRV_USART_MIKROE_ROUTE_LOC,
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

void sl_spidrv_init_instances(void) {
#if !defined(SL_SPIDRV_USART_MIKROE_CS_PIN)
  EFM_ASSERT(sl_spidrv_usart_init_mikroe.csControl == spidrvCsControlApplication);
#endif
  SPIDRV_Init(sl_spidrv_usart_mikroe_handle, &sl_spidrv_usart_init_mikroe);
}
