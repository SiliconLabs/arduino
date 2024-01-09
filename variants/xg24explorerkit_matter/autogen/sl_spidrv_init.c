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


#include "sl_spidrv_eusart_mikroe_config.h"
#include "sl_spidrv_eusart_xg24explorerkit1_config.h"

SPIDRV_HandleData_t sl_spidrv_eusart_mikroe_handle_data;
SPIDRV_Handle_t sl_spidrv_eusart_mikroe_handle = &sl_spidrv_eusart_mikroe_handle_data;

SPIDRV_HandleData_t sl_spidrv_eusart_xg24explorerkit1_handle_data;
SPIDRV_Handle_t sl_spidrv_eusart_xg24explorerkit1_handle = &sl_spidrv_eusart_xg24explorerkit1_handle_data;

SPIDRV_Init_t sl_spidrv_eusart_init_mikroe = {
  .port = SL_SPIDRV_EUSART_MIKROE_PERIPHERAL,
  .portTx = SL_SPIDRV_EUSART_MIKROE_TX_PORT,
  .portRx = SL_SPIDRV_EUSART_MIKROE_RX_PORT,
  .portClk = SL_SPIDRV_EUSART_MIKROE_SCLK_PORT,
#if defined(SL_SPIDRV_EUSART_MIKROE_CS_PORT)
  .portCs = SL_SPIDRV_EUSART_MIKROE_CS_PORT,
#endif
  .pinTx = SL_SPIDRV_EUSART_MIKROE_TX_PIN,
  .pinRx = SL_SPIDRV_EUSART_MIKROE_RX_PIN,
  .pinClk = SL_SPIDRV_EUSART_MIKROE_SCLK_PIN,
#if defined(SL_SPIDRV_EUSART_MIKROE_CS_PIN)
  .pinCs = SL_SPIDRV_EUSART_MIKROE_CS_PIN,
#endif
  .bitRate = SL_SPIDRV_EUSART_MIKROE_BITRATE,
  .frameLength = SL_SPIDRV_EUSART_MIKROE_FRAME_LENGTH,
  .dummyTxValue = 0,
  .type = SL_SPIDRV_EUSART_MIKROE_TYPE,
  .bitOrder = SL_SPIDRV_EUSART_MIKROE_BIT_ORDER,
  .clockMode = SL_SPIDRV_EUSART_MIKROE_CLOCK_MODE,
  .csControl = SL_SPIDRV_EUSART_MIKROE_CS_CONTROL,
  .slaveStartMode = SL_SPIDRV_EUSART_MIKROE_SLAVE_START_MODE,
};

SPIDRV_Init_t sl_spidrv_eusart_init_xg24explorerkit1 = {
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

void sl_spidrv_init_instances(void) {
#if !defined(SL_SPIDRV_EUSART_MIKROE_CS_PIN)
  EFM_ASSERT(sl_spidrv_eusart_init_mikroe.csControl == spidrvCsControlApplication);
#endif 
#if !defined(SL_SPIDRV_EUSART_XG24EXPLORERKIT1_CS_PIN)
  EFM_ASSERT(sl_spidrv_eusart_init_xg24explorerkit1.csControl == spidrvCsControlApplication);
#endif 
  SPIDRV_Init(sl_spidrv_eusart_mikroe_handle, &sl_spidrv_eusart_init_mikroe);
  SPIDRV_Init(sl_spidrv_eusart_xg24explorerkit1_handle, &sl_spidrv_eusart_init_xg24explorerkit1);
}
