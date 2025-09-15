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

#include "SPI.h"
#include "arduino_spi_config.h"

using namespace arduino;

SilabsSPI::SilabsSPI(SPIDRV_Handle_t sl_spidrv_handle, SPIDRV_Init_t* sl_spidrv_config, SPIDRV_Callback_t dma_transfer_finished_callback) :
  initialized(false)
{
  this->sl_spidrv_handle = sl_spidrv_handle;
  this->sl_spidrv_config = sl_spidrv_config;
  this->dma_transfer_finished_callback = dma_transfer_finished_callback;

  this->spi_transfer_mutex = xSemaphoreCreateMutexStatic(&this->spi_transfer_mutex_buf);
  configASSERT(this->spi_transfer_mutex);
  this->spi_busy_mutex = xSemaphoreCreateMutexStatic(&this->spi_busy_mutex_buf);
  configASSERT(this->spi_busy_mutex);
}

void SilabsSPI::begin()
{
  if (this->initialized) {
    return;
  }
  sl_status_t sc = SPIDRV_Init(this->sl_spidrv_handle, this->sl_spidrv_config);
  if (sc == SL_STATUS_OK) {
    this->initialized = true;
  }
}

void SilabsSPI::beginTransaction(SPISettings settings)
{
  xSemaphoreTake(this->spi_busy_mutex, portMAX_DELAY);
  // Don't do anything if the settings don't change
  if (this->settings.getClockFreq() == settings.getClockFreq()
      && this->settings.getBitOrder() == settings.getBitOrder()
      && this->settings.getDataMode() == settings.getDataMode()) {
    return;
  }
  // Deinit the peripheral
  SPIDRV_DeInit(this->sl_spidrv_handle);
  // Apply the new settings
  this->sl_spidrv_config->bitRate = settings.getClockFreq();
  setBitOrder(settings.getBitOrder());
  setDataMode(settings.getDataMode());
  // Reinit the peripheral
  SPIDRV_Init(this->sl_spidrv_handle, this->sl_spidrv_config);
}

// Uses direct blocking transfers with the USART/EUSART driver
uint8_t SilabsSPI::transfer(uint8_t data)
{
  uint8_t rx_byte = 0u;
  xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
  rx_byte = sl_spi_direct_transfer((void*)this->sl_spidrv_config->port, data);
  xSemaphoreGive(this->spi_transfer_mutex);
  return rx_byte;
}

// Uses direct blocking transfers with the USART/EUSART driver
uint16_t SilabsSPI::transfer16(uint16_t data)
{
  uint16_t rx_bytes = 0u;
  uint8_t rx_data[2];
  uint8_t tx_data[2];
  tx_data[0] = (uint8_t)(data >> 8);
  tx_data[1] = (uint8_t)data;
  rx_data[0] = this->transfer(tx_data[0]);
  rx_data[1] = this->transfer(tx_data[1]);
  rx_bytes = ((uint16_t)rx_data[0] << 8) + rx_data[1];
  return rx_bytes;
}

// Uses DMA and waits for the transaction to complete, has a large overhead for small amounts of data
void SilabsSPI::transfer(void* tx_buf, size_t count, bool block)
{
  if (block) {
    this->_transfer_block(tx_buf, count);
  } else {
    this->_transfer_nonblock(tx_buf, count);
  }
}

void SilabsSPI::transfer(void *buf, size_t count)
{
  if (!buf || count == 0u) {
    return;
  }
  // Transfer bytes from 'buf' while replacing them with the received bytes
  this->transfer(buf, buf, count, true);
}

void SilabsSPI::_transfer_block(void* tx_buf, size_t count)
{
  size_t bytes_transferred = 0;
  // Go while we still have bytes to transfer
  while (bytes_transferred < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_transferred, count);
    // Start the data transfer with DMA
    SPIDRV_MTransmitB(sl_spidrv_handle, (uint8_t*)tx_buf + bytes_transferred, current_transfer_size);
    // Add the transferred amount to the total transferred bytes
    bytes_transferred += current_transfer_size;
  }
}

void SilabsSPI::_transfer_nonblock(void* tx_buf, size_t count)
{
  size_t bytes_transferred = 0;

  // Take the transfer mutex
  xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
  // Go while we still have bytes to transfer
  while (bytes_transferred < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_transferred, count);
    // Start the data transfer with DMA
    SPIDRV_MTransmit(sl_spidrv_handle, (uint8_t*)tx_buf + bytes_transferred, current_transfer_size, this->dma_transfer_finished_callback);
    // Try to take the mutex again - current task will be blocked here until the transfer finishes
    // The dma_transfer_finished_callback will give the mutex back and the next chunk transfer will start then
    xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
    // Add the transferred amount to the total transferred bytes
    bytes_transferred += current_transfer_size;
  }
  // Give back the transfer mutex
  xSemaphoreGive(this->spi_transfer_mutex);
}

void SilabsSPI::transfer(void* tx_buf, void* rx_buf, size_t count, bool block)
{
  if (block) {
    this->_transfer_block(tx_buf, rx_buf, count);
  } else {
    this->_transfer_nonblock(tx_buf, rx_buf, count);
  }
}

void SilabsSPI::_transfer_block(void* tx_buf, void* rx_buf, size_t count)
{
  size_t bytes_transferred = 0;
  // Go while we still have bytes to transfer
  while (bytes_transferred < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_transferred, count);
    // Transfer the data
    SPIDRV_MTransferB(this->sl_spidrv_handle, tx_buf, rx_buf, current_transfer_size);
    // Add the transferred amount to the total transferred bytes
    bytes_transferred += current_transfer_size;
  }
}

void SilabsSPI::_transfer_nonblock(void* tx_buf, void* rx_buf, size_t count)
{
  size_t bytes_transferred = 0;

  // Take the transfer mutex
  xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
  // Go while we still have bytes to transfer
  while (bytes_transferred < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_transferred, count);
    // Transfer the data
    SPIDRV_MTransfer(this->sl_spidrv_handle, tx_buf, rx_buf, current_transfer_size, this->dma_transfer_finished_callback);
    // Try to take the mutex again - current task will be blocked here until the transfer finishes
    // The dma_transfer_finished_callback will give the mutex back and the next chunk transfer will start then
    xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
    // Add the transferred amount to the total transferred bytes
    bytes_transferred += current_transfer_size;
  }
  // Give back the transfer mutex
  xSemaphoreGive(this->spi_transfer_mutex);
}

uint8_t SilabsSPI::receive()
{
  return this->transfer(0xff);
}

uint16_t SilabsSPI::receive16()
{
  return this->transfer16(0xffff);
}

void SilabsSPI::receive(void* rx_buf, size_t count, bool block)
{
  if (block) {
    this->_receive_block(rx_buf, count);
  } else {
    this->_receive_nonblock(rx_buf, count);
  }
}

void SilabsSPI::_receive_block(void* rx_buf, size_t count)
{
  size_t bytes_received = 0;
  // Go while we still have bytes to receive
  while (bytes_received < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_received, count);
    // Receive the data
    SPIDRV_MReceiveB(this->sl_spidrv_handle, rx_buf, current_transfer_size);
    // Add the transferred amount to the total transferred bytes
    bytes_received += current_transfer_size;
  }
}

void SilabsSPI::_receive_nonblock(void* rx_buf, size_t count)
{
  size_t bytes_received = 0;

  // Take the transfer mutex
  xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
  // Go while we still have bytes to receive
  while (bytes_received < count) {
    size_t current_transfer_size = get_next_dma_transfer_size(bytes_received, count);
    // Receive the data
    SPIDRV_MReceive(this->sl_spidrv_handle, rx_buf, current_transfer_size, this->dma_transfer_finished_callback);
    // Try to take the mutex again - current task will be blocked here until the transfer finishes
    // The dma_transfer_finished_callback will give the mutex back and the next chunk transfer will start then
    xSemaphoreTake(this->spi_transfer_mutex, portMAX_DELAY);
    // Add the transferred amount to the total transferred bytes
    bytes_received += current_transfer_size;
  }
  // Give back the transfer mutex
  xSemaphoreGive(this->spi_transfer_mutex);
}

void SilabsSPI::endTransaction(void)
{
  xSemaphoreGive(this->spi_busy_mutex);
}

void SilabsSPI::end(void)
{
  this->endTransaction();
  SPIDRV_DeInit(this->sl_spidrv_handle);
  this->initialized = false;
}

void SilabsSPI::setBitOrder(uint8_t bitOrder)
{
  if (bitOrder == LSBFIRST) {
    this->sl_spidrv_config->bitOrder = spidrvBitOrderLsbFirst;
  } else {
    this->sl_spidrv_config->bitOrder = spidrvBitOrderMsbFirst;
  }
}

void SilabsSPI::setDataMode(uint8_t dataMode)
{
  switch (dataMode) {
    case SPI_MODE0:
      this->sl_spidrv_config->clockMode = spidrvClockMode0;
      break;
    case SPI_MODE1:
      this->sl_spidrv_config->clockMode = spidrvClockMode1;
      break;
    case SPI_MODE2:
      this->sl_spidrv_config->clockMode = spidrvClockMode2;
      break;
    case SPI_MODE3:
      this->sl_spidrv_config->clockMode = spidrvClockMode3;
      break;
    default:
      this->sl_spidrv_config->clockMode = spidrvClockMode0;
      break;
  }
}

void SilabsSPI::setClockDivider(uint8_t clockDiv)
{
  // This call is very AVR specific and not used
  // Clock speed can only be set via SPISettings in beginTransaction
  (void)clockDiv;
}

void SilabsSPI::attachInterrupt(void)
{
  ;
}

void SilabsSPI::detachInterrupt(void)
{
  ;
}

uint32_t SilabsSPI::getCurrentBusSpeed()
{
  uint32_t bitrate = 0;
  SPIDRV_GetBitrate(sl_spidrv_handle, &bitrate);
  return bitrate;
}

size_t SilabsSPI::get_next_dma_transfer_size(size_t transferred, size_t total)
{
  size_t remaining_bytes = total - transferred;
  // If the remaining bytes are larger than the max transfer size
  if (remaining_bytes >= this->DMA_MAX_TRANSFER_SIZE) {
    // Go with the max transfer size
    return this->DMA_MAX_TRANSFER_SIZE;
  } else {
    // If the remaining bytes fit into the MAX_TRANSER_SIZE go with the amount of remaining bytes
    return remaining_bytes;
  }
}

void SilabsSPI::dma_transfer_finished_cb(struct SPIDRV_HandleData *handle, Ecode_t transferStatus, int itemsTransferred)
{
  (void)handle;
  (void)transferStatus;
  (void)itemsTransferred;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(this->spi_transfer_mutex, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SilabsSPI::usingInterrupt(int interruptNumber)
{
  (void)interruptNumber;
}

void SilabsSPI::notUsingInterrupt(int interruptNumber)
{
  (void)interruptNumber;
}

// Called from ISR when the DMA finishes the current task
void spi0_dma_transfer_finished_callback(struct SPIDRV_HandleData *handle, Ecode_t transferStatus, int itemsTransferred)
{
  SPI.dma_transfer_finished_cb(handle, transferStatus, itemsTransferred);
}

arduino::SilabsSPI SPI(SL_SPIDRV_PERIPHERAL_HANDLE, &sl_spidrv_config, spi0_dma_transfer_finished_callback);

#if (NUM_HW_SPI > 1)
void spi1_dma_transfer_finished_callback(struct SPIDRV_HandleData *handle, Ecode_t transferStatus, int itemsTransferred)
{
  SPI1.dma_transfer_finished_cb(handle, transferStatus, itemsTransferred);
}

arduino::SilabsSPI SPI1(SL_SPIDRV1_PERIPHERAL_HANDLE, &sl_spidrv_config_spi1, spi1_dma_transfer_finished_callback);
#endif // (NUM_HW_SPI > 1)
