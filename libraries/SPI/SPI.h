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

#include "Arduino.h"
#include "api/HardwareSPI.h"

#ifndef SPI_H
#define SPI_H

#include <cmath>
#include <inttypes.h>
#include <cstddef>
#include "spidrv.h"
#include "FreeRTOS.h"
#include "semphr.h"

namespace arduino {
class SilabsSPI : public SPIClass
{
public:
  SilabsSPI(SPIDRV_Handle_t sl_spidrv_handle, SPIDRV_Init_t* sl_spidrv_config, SPIDRV_Callback_t dma_transfer_finished_callback);

  virtual uint8_t transfer(uint8_t data);
  virtual uint16_t transfer16(uint16_t data);
  virtual void transfer(void *buf, size_t count);

  // Transaction Functions
  virtual void usingInterrupt(int interruptNumber);
  virtual void notUsingInterrupt(int interruptNumber);
  virtual void beginTransaction(SPISettings settings);
  virtual void endTransaction(void);

  // SPI Configuration methods
  virtual void attachInterrupt();
  virtual void detachInterrupt();

  virtual void begin();
  virtual void end();

  /***************************************************************************//**
   * Transfers the provided amount of bytes on the SPI bus.
   * Uses DMA - not recommended for small transactions.
   *
   * @param[in] tx_buf Pointer to the data to be transferred
   * @param[in] count Size of the data to be transferred
   * @param[in] block Sets whether the transfer should busy-wait for completion
   *                  or the task should yield while the transfer is ongoing
   ******************************************************************************/
  void transfer(void* tx_buf, size_t count, bool block);

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

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setBitOrder(uint8_t bitOrder);

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setDataMode(uint8_t dataMode);

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings
  void setClockDivider(uint8_t clockDiv);

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
  SPISettings settings = SPISettings(1000000, LSBFIRST, SPI_MODE0);

  SPIDRV_Handle_t sl_spidrv_handle;
  SPIDRV_Init_t* sl_spidrv_config;
  SPIDRV_Callback_t dma_transfer_finished_callback;
  SemaphoreHandle_t spi_transfer_mutex;
  StaticSemaphore_t spi_transfer_mutex_buf;
  SemaphoreHandle_t spi_busy_mutex;
  StaticSemaphore_t spi_busy_mutex_buf;
};
} // namespace arduino

extern arduino::SilabsSPI SPI;

#if (NUM_HW_SPI > 1)
extern arduino::SilabsSPI SPI1;
#endif // (NUM_HW_SPI > 1)

#endif // SPI_H
