/***************************************************************************//**
 * @file
 * @brief SPIDRV_EUSART Config
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_SPIDRV_EUSART_NANOMATTER1_CONFIG_H
#define SL_SPIDRV_EUSART_NANOMATTER1_CONFIG_H

#include "spidrv.h"

// <<< Use Configuration Wizard in Context Menu >>>
// <h> SPIDRV settings

// <o SL_SPIDRV_EUSART_NANOMATTER1_BITRATE> SPI bitrate
// <i> Default: 1000000
#define SL_SPIDRV_EUSART_NANOMATTER1_BITRATE           1000000

// <o SL_SPIDRV_EUSART_NANOMATTER1_FRAME_LENGTH> SPI frame length <7-16>
// <i> Default: 8
#define SL_SPIDRV_EUSART_NANOMATTER1_FRAME_LENGTH      8

// <o SL_SPIDRV_EUSART_NANOMATTER1_TYPE> SPI mode
// <spidrvMaster=> Master
// <spidrvSlave=> Slave
#define SL_SPIDRV_EUSART_NANOMATTER1_TYPE              spidrvMaster

// <o SL_SPIDRV_EUSART_NANOMATTER1_BIT_ORDER> Bit order on the SPI bus
// <spidrvBitOrderLsbFirst=> LSB transmitted first
// <spidrvBitOrderMsbFirst=> MSB transmitted first
#define SL_SPIDRV_EUSART_NANOMATTER1_BIT_ORDER         spidrvBitOrderMsbFirst

// <o SL_SPIDRV_EUSART_NANOMATTER1_CLOCK_MODE> SPI clock mode
// <spidrvClockMode0=> SPI mode 0: CLKPOL=0, CLKPHA=0
// <spidrvClockMode1=> SPI mode 1: CLKPOL=0, CLKPHA=1
// <spidrvClockMode2=> SPI mode 2: CLKPOL=1, CLKPHA=0
// <spidrvClockMode3=> SPI mode 3: CLKPOL=1, CLKPHA=1
#define SL_SPIDRV_EUSART_NANOMATTER1_CLOCK_MODE        spidrvClockMode0

// <o SL_SPIDRV_EUSART_NANOMATTER1_CS_CONTROL> SPI master chip select (CS) control scheme.
// <spidrvCsControlAuto=> CS controlled by the SPI driver
// <spidrvCsControlApplication=> CS controlled by the application
#define SL_SPIDRV_EUSART_NANOMATTER1_CS_CONTROL        spidrvCsControlApplication

// <o SL_SPIDRV_EUSART_NANOMATTER1_SLAVE_START_MODE> SPI slave transfer start scheme
// <spidrvSlaveStartImmediate=> Transfer starts immediately
// <spidrvSlaveStartDelayed=> Transfer starts when the bus is idle (CS deasserted)
// <i> Only applies if instance type is spidrvSlave
#define SL_SPIDRV_EUSART_NANOMATTER1_SLAVE_START_MODE  spidrvSlaveStartImmediate
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <eusart signal=TX,RX,SCLK,(CS)> SL_SPIDRV_EUSART_NANOMATTER1
// $[EUSART_SL_SPIDRV_EUSART_NANOMATTER1]
#define SL_SPIDRV_EUSART_NANOMATTER1_PERIPHERAL    EUSART0
#define SL_SPIDRV_EUSART_NANOMATTER1_PERIPHERAL_NO 0

// EUSART0 TX on PA4
#define SL_SPIDRV_EUSART_NANOMATTER1_TX_PORT       gpioPortA
#define SL_SPIDRV_EUSART_NANOMATTER1_TX_PIN        4

// EUSART0 RX on PA5
#define SL_SPIDRV_EUSART_NANOMATTER1_RX_PORT       gpioPortA
#define SL_SPIDRV_EUSART_NANOMATTER1_RX_PIN        5

// EUSART0 SCLK on PA3
#define SL_SPIDRV_EUSART_NANOMATTER1_SCLK_PORT     gpioPortA
#define SL_SPIDRV_EUSART_NANOMATTER1_SCLK_PIN      3

// [EUSART_SL_SPIDRV_EUSART_NANOMATTER1]$
// <<< sl:end pin_tool >>>

#endif // SL_SPIDRV_EUSART_NANOMATTER1_CONFIG_HEUSART_
