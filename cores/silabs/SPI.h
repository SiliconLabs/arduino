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

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED
// For the API description refer to: https://www.arduino.cc/reference/en/language/functions/communication/spi/

#include <cmath>
#include <inttypes.h>
#include <cstddef>
#include "spidrv.h"
#include "FreeRTOS.h"
#include "semphr.h"

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

class SPISettings {
public:
  SPISettings() : _clock(1000000), _bitOrder(LSBFIRST), _dataMode(SPI_MODE0)
  {
  }
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode)
  {
  }
  uint32_t _clock;
  uint8_t  _bitOrder;
  uint8_t  _dataMode;
};

class SPIClass {
public:
  /***************************************************************************//**
   * Constructor for SPIClass
   ******************************************************************************/
  SPIClass(SPIDRV_Handle_t sl_spidrv_handle, SPIDRV_Init_t* sl_spidrv_config, SPIDRV_Callback_t dma_transfer_finished_callback);

  /***************************************************************************//**
   * Enables the SPI bus
   ******************************************************************************/
  void begin();

  /***************************************************************************//**
   * Starts an SPI transaction with the provided settings
   *
   * @param[in] settings An SPISettings instance containing the clock, bit order
   *                     and data mode to be used
   ******************************************************************************/
  void beginTransaction(SPISettings settings);

  /***************************************************************************//**
   * Transfers the provided byte while simultaneously receives one byte
   *
   * @param[in] data The byte of data to be transferred
   *
   * @return the byte of data received on the SPI bus
   ******************************************************************************/
  uint8_t transfer(uint8_t data);

  /***************************************************************************//**
   * Transfers the provided two bytes while simultaneously receiving two bytes
   *
   * @param[in] data The two bytes of data to be transferred
   *
   * @return the two bytes of data received on the SPI bus
   ******************************************************************************/
  uint16_t transfer16(uint16_t data);

  /***************************************************************************//**
   * Transfers the provided amount of bytes on the SPI bus.
   * Uses DMA - not recommended for small transactions.
   *
   * @param[in] tx_buf Pointer to the data to be transferred
   * @param[in] count Size of the data to be transferred
   * @param[in] block Sets whether the transfer should busy-wait for completion
   *                  or the task should yield while the transfer is ongoing
   ******************************************************************************/
  void transfer(void* tx_buf, size_t count, bool block = false);

  /***************************************************************************//**
   * Transfers the provided amount of bytes while simultaneously receiving
   * the same amount of bytes.
   * Uses DMA - not recommended for small transactions.
   * Silabs specific, non-standard Arduino call.
   *
   * @param[in] tx_buf Pointer to the data to be transferred
   * @param[out] rx_buf Pointer to the array to store the received data
   * @param[in] count Size of the data to be transferred/received
   * @param[in] block Sets whether the transfer should busy-wait for completion
   *                  or the task should yield while the transfer is ongoing
   ******************************************************************************/
  void transfer(void* tx_buf, void* rx_buf, size_t count, bool block = false);

  /***************************************************************************//**
   * Receives a byte on the SPI bus.
   * Silabs specific, non-standard Arduino call.
   *
   * @return the byte of data received on the SPI bus
   ******************************************************************************/
  uint8_t receive();

  /***************************************************************************//**
   * Receives two bytes on the SPI bus.
   * Silabs specific, non-standard Arduino call.
   *
   * @return the two bytes of data received on the SPI bus
   ******************************************************************************/
  uint16_t receive16();

  /***************************************************************************//**
   * Receives the provided amount of bytes on the SPI bus.
   * Uses DMA - not recommended for small transactions.
   * Silabs specific, non-standard Arduino call.
   *
   * @param[out] rx_buf Pointer to the array to store the received data
   * @param[in] count Number of bytes to be received
   * @param[in] block Sets whether the transfer should busy-wait for completion
   *                  or the task should yield while the transfer is ongoing
   ******************************************************************************/
  void receive(void* rx_buf, size_t count, bool block = false);

  /***************************************************************************//**
   * Ends the currently ongoing SPI transaction
   ******************************************************************************/
  void endTransaction(void);

  /***************************************************************************//**
   * Disables the SPI bus
   ******************************************************************************/
  void end();

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setBitOrder(uint8_t bitOrder);

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setDataMode(uint8_t dataMode);

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setClockDivider(uint8_t clockDiv);

  // These undocumented functions should not be used
  // Left here for API compatibility
  void attachInterrupt();
  void detachInterrupt();

  /***************************************************************************//**
   * Returns the actual clock speed of the SPI bus which can be different
   * than what the user requested. Silabs specific API.
   *
   * @return the current real clock speed of the SPI bus
   ******************************************************************************/
  uint32_t getCurrentBusSpeed();

  /***************************************************************************//**
   * Callback function - called from outside when a DMA transfer finishes
   *
   * @param[in] handle The SPIDRV device handle used to start the transfer
   * @param[in] transferStatus The number of bytes actually transferred
   * @param[in] itemsTransferred The number of bytes transferred
   ******************************************************************************/
  void dma_transfer_finished_cb(struct SPIDRV_HandleData *handle, Ecode_t transferStatus, int itemsTransferred);

private:
  void _transfer_block(void* tx_buf, size_t count);
  void _transfer_nonblock(void* tx_buf, size_t count);

  void _transfer_block(void* tx_buf, void* rx_buf, size_t count);
  void _transfer_nonblock(void* tx_buf, void* rx_buf, size_t count);

  void _receive_block(void* rx_buf, size_t count);
  void _receive_nonblock(void* rx_buf, size_t count);

  static const int DMA_MAX_TRANSFER_SIZE = 2048;
  size_t get_next_dma_transfer_size(size_t transferred, size_t total);

  bool initialized;
  SPISettings settings;

  SPIDRV_Handle_t sl_spidrv_handle;
  SPIDRV_Init_t* sl_spidrv_config;
  SPIDRV_Callback_t dma_transfer_finished_callback;
  SemaphoreHandle_t spi_transfer_mutex;
  StaticSemaphore_t spi_transfer_mutex_buf;
  SemaphoreHandle_t spi_busy_mutex;
  StaticSemaphore_t spi_busy_mutex_buf;
};

#endif
