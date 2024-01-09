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

#include "Wire.h"

TwoWire::TwoWire() :
  role(wire_role_t::NOT_INITIALIZED),
  timeout_flag(false),
  follower_address(0u),
  transmission_in_progress(false),
  tx_buf_write_idx(0u),
  rx_buf_read_idx(0u),
  rx_buf_available(0u),
  follower_mode_address(0u),
  follower_transaction_in_progress(false),
  user_onreceive_cb(nullptr),
  user_onrequest_cb(nullptr),
  wire_mutex(nullptr)
{
  memset(this->rx_buffer, 0x00, sizeof(this->rx_buffer));
  memset(this->tx_buffer, 0x00, sizeof(this->tx_buffer));
  this->wire_mutex = xSemaphoreCreateMutexStatic(&this->wire_mutex_buf);
  configASSERT(this->wire_mutex);
}

void TwoWire::begin()
{
  if (this->role != wire_role_t::NOT_INITIALIZED) {
    return;
  }
  this->role = wire_role_t::LEADER;
  I2CSPM_Init(arduino_i2c_config);
}

void TwoWire::begin(uint8_t follower_mode_address)
{
  if (this->role != wire_role_t::NOT_INITIALIZED) {
    return;
  }
  this->role = wire_role_t::FOLLOWER;
  this->follower_mode_address = follower_mode_address;

  // Use default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  // Configure I2C peripheral to be addressable as a follower
  i2cInit.master = false;

  // Enable GPIO pins
  GPIO_PinModeSet(SL_I2CSPM_SCL_PORT, SL_I2CSPM_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(SL_I2CSPM_SDA_PORT, SL_I2CSPM_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);

  // Route I2C pins to GPIO
  GPIO->I2CROUTE[SL_I2CSPM_PERIPHERAL_NUM].SDAROUTE = (GPIO->I2CROUTE[SL_I2CSPM_PERIPHERAL_NUM].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
                                                      | (SL_I2CSPM_SDA_PORT << _GPIO_I2C_SDAROUTE_PORT_SHIFT
                                                         | (SL_I2CSPM_SDA_PIN << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[SL_I2CSPM_PERIPHERAL_NUM].SCLROUTE = (GPIO->I2CROUTE[SL_I2CSPM_PERIPHERAL_NUM].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
                                                      | (SL_I2CSPM_SCL_PORT << _GPIO_I2C_SCLROUTE_PORT_SHIFT
                                                         | (SL_I2CSPM_SCL_PIN << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[SL_I2CSPM_PERIPHERAL_NUM].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;

  // Initialize the I2C peripheral
  I2C_Init(SL_I2CSPM_PERIPHERAL, &i2cInit);

  // Set follower address
  I2C_SlaveAddressSet(SL_I2CSPM_PERIPHERAL, this->follower_mode_address);

  // Configure interrupts
  I2C_IntClear(SL_I2CSPM_PERIPHERAL, _I2C_IF_MASK);
  I2C_IntEnable(SL_I2CSPM_PERIPHERAL, I2C_IEN_ADDR | I2C_IEN_RXDATAV | I2C_IEN_ACK | I2C_IEN_SSTOP | I2C_IEN_BUSERR | I2C_IEN_ARBLOST);

  #if defined(I2C0)
  if (SL_I2CSPM_PERIPHERAL == I2C0) {
    NVIC_EnableIRQ(I2C0_IRQn);
  }
  #endif
  #if defined(I2C1)
  if (SL_I2CSPM_PERIPHERAL == I2C1) {
    NVIC_EnableIRQ(I2C1_IRQn);
  }
  #endif
  #if defined(I2C2)
  if (SL_I2CSPM_PERIPHERAL == I2C2) {
    NVIC_EnableIRQ(I2C2_IRQn);
  }
  #endif
}

void TwoWire::end()
{
  this->role = wire_role_t::NOT_INITIALIZED;
  this->timeout_flag = false;
  this->follower_address = 0u;
  this->transmission_in_progress = false;
  this->tx_buf_write_idx = 0u;
  this->rx_buf_read_idx = 0u;
  this->rx_buf_available = 0u;
  memset(this->rx_buffer, 0x00, sizeof(this->rx_buffer));
  memset(this->tx_buffer, 0x00, sizeof(this->tx_buffer));

  this->follower_transaction_in_progress = false;
  this->follower_mode_rx_buffer.clear();

  I2C_Reset(SL_I2CSPM_PERIPHERAL);

  // Reset the I2C to GPIO peripheral routing to enable the pins to function as GPIO
  #if defined(I2C0)
  if (SL_I2CSPM_PERIPHERAL == I2C0) {
    GPIO->I2CROUTE[0].ROUTEEN = 0;
    GPIO->I2CROUTE[0].SCLROUTE = 0;
    GPIO->I2CROUTE[0].SDAROUTE = 0;
  }
  #endif
  #if defined(I2C1)
  if (SL_I2CSPM_PERIPHERAL == I2C1) {
    GPIO->I2CROUTE[1].ROUTEEN = 0;
    GPIO->I2CROUTE[1].SCLROUTE = 0;
    GPIO->I2CROUTE[1].SDAROUTE = 0;
  }
  #endif
  #if defined(I2C2)
  if (SL_I2CSPM_PERIPHERAL == I2C2) {
    GPIO->I2CROUTE[2].ROUTEEN = 0;
    GPIO->I2CROUTE[2].SCLROUTE = 0;
    GPIO->I2CROUTE[2].SDAROUTE = 0;
  }
  #endif

  #if defined(I2C0)
  if (SL_I2CSPM_PERIPHERAL == I2C0) {
    NVIC_DisableIRQ(I2C0_IRQn);
  }
  #endif
  #if defined(I2C1)
  if (SL_I2CSPM_PERIPHERAL == I2C1) {
    NVIC_DisableIRQ(I2C1_IRQn);
  }
  #endif
  #if defined(I2C2)
  if (SL_I2CSPM_PERIPHERAL == I2C2) {
    NVIC_DisableIRQ(I2C2_IRQn);
  }
  #endif
}

uint8_t TwoWire::requestFrom(uint8_t follower_address, uint8_t number_of_bytes)
{
  if (this->role == wire_role_t::NOT_INITIALIZED || this->role == wire_role_t::FOLLOWER) {
    return 0;
  }

  // Check for overflow when requesting more bytes than the buffer size
  if (number_of_bytes > this->rx_buffer_size) {
    this->tx_buf_write_idx = 0u;
    this->rx_buf_read_idx = 0u;
    this->rx_buf_available = 0u;
    return 0;
  }

  // Send out the Tx buffer and get the incoming bytes
  uint32_t ret = this->i2c_leader_read(this->tx_buffer, this->tx_buf_write_idx, this->rx_buffer, number_of_bytes, follower_address);

  this->tx_buf_write_idx = 0u;
  this->rx_buf_read_idx = 0u;
  this->rx_buf_available = 0u;

  // If the I2C operation was successful
  if (ret == 0) {
    this->rx_buf_available = number_of_bytes;
    if (this->user_onreceive_cb) {
      this->user_onreceive_cb(this->rx_buf_available);
    }
    return this->rx_buf_available;
  }

  // End the transmission if configured and it has run to a timeout
  if (this->reset_on_timeout && this->timeout_flag && ret != 0) {
    this->endTransmission(true);
  }

  return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t number_of_bytes, uint8_t stop)
{
  if (this->role == wire_role_t::NOT_INITIALIZED || this->role == wire_role_t::FOLLOWER) {
    return 0;
  }

  uint8_t result = this->requestFrom(address, number_of_bytes);
  this->endTransmission(stop);
  return result;
}

void TwoWire::beginTransmission(uint16_t follower_address)
{
  if (this->role == wire_role_t::NOT_INITIALIZED || this->role == wire_role_t::FOLLOWER) {
    return;
  }

  xSemaphoreTake(this->wire_mutex, portMAX_DELAY);
  this->follower_address = follower_address;
  transmission_in_progress = true;
}

uint8_t TwoWire::endTransmission(bool stop)
{
  if (this->role == wire_role_t::NOT_INITIALIZED || this->role == wire_role_t::FOLLOWER) {
    return 0;
  }

  if (stop) {
    uint32_t ret = 0u;
    // If we have data in the Tx buffer - send it out without waiting for incoming bytes
    if (this->tx_buf_write_idx > 0) {
      ret = this->i2c_leader_write(this->tx_buffer, this->tx_buf_write_idx, NULL, 0, this->follower_address);
    }

    this->tx_buf_write_idx = 0u;
    this->follower_address = 0;
    this->transmission_in_progress = false;

    if (ret != 0) {
      xSemaphoreGive(this->wire_mutex);
      return WireStatus::OTHER_ERROR;
    }
  }
  xSemaphoreGive(this->wire_mutex);
  return 0;
}

size_t TwoWire::write(uint8_t value)
{
  if (this->role == wire_role_t::NOT_INITIALIZED) {
    return -1;
  }

  if (this->role == wire_role_t::FOLLOWER) {
    if (this->follower_transaction_in_progress) {
      SL_I2CSPM_PERIPHERAL->TXDATA = value;
      SL_I2CSPM_PERIPHERAL->CMD = I2C_CMD_ACK;
      return 1;
    } else {
      return -1;
    }
  }

  if (!this->transmission_in_progress) {
    return -1;
  }

  // Check for overflow
  if (this->tx_buf_write_idx >= this->tx_buffer_size) {
    this->tx_buf_write_idx = 0u;
    return 0;
  }

  this->tx_buffer[this->tx_buf_write_idx] = value;
  this->tx_buf_write_idx++;
  return 1;
}

size_t TwoWire::write(const uint8_t* data, size_t size)
{
  if (this->role == wire_role_t::NOT_INITIALIZED || this->role == wire_role_t::FOLLOWER) {
    return -1;
  }

  if (!data || size == 0) {
    return 0;
  }
  size_t bytes_written = 0;
  for (size_t i = 0; i < size; i++) {
    size_t res = this->write(data[i]);
    if (res == 0) {
      return bytes_written;
    }
    bytes_written += res;
  }
  return bytes_written;
}

int TwoWire::available()
{
  if (this->role == wire_role_t::FOLLOWER) {
    return this->follower_mode_rx_buffer.available();
  }

  if (this->role == wire_role_t::LEADER) {
    return this->rx_buf_available - this->rx_buf_read_idx;
  }

  return 0;
}

int TwoWire::read()
{
  if (this->role == wire_role_t::NOT_INITIALIZED) {
    return -1;
  }

  if (this->role == wire_role_t::FOLLOWER) {
    if (this->follower_mode_rx_buffer.available()) {
      return this->follower_mode_rx_buffer.read_char();
    } else {
      return -1;
    }
  }

  if (this->rx_buf_available == 0 || this->rx_buf_read_idx == this->rx_buf_available) {
    return -1;
  }

  uint8_t data = this->rx_buffer[this->rx_buf_read_idx];
  this->rx_buf_read_idx++;

  return data;
}

void TwoWire::setClock(const uint32_t clock)
{
  if (this->role == wire_role_t::NOT_INITIALIZED) {
    return;
  }
  I2C_BusFreqSet(arduino_i2c_config->port, 0, clock, i2cClockHLRStandard);
}

void TwoWire::onReceive(void (*user_onreceive_cb)(int))
{
  this->user_onreceive_cb = user_onreceive_cb;
}

void TwoWire::onRequest(void (*user_onrequest_cb)(void))
{
  this->user_onrequest_cb = user_onrequest_cb;
}

void TwoWire::setWireTimeout(int timeout, bool reset_on_timeout)
{
  // The I2CSPM driver doesn't allow us to configure the timeout value
  (void)timeout;
  this->reset_on_timeout = reset_on_timeout;
}

void TwoWire::clearWireTimeoutFlag()
{
  this->timeout_flag = false;
}

bool TwoWire::getWireTimeoutFlag()
{
  return this->timeout_flag;
}

uint32_t TwoWire::i2c_leader_read(uint8_t *cmd, size_t cmdLen, uint8_t *result, size_t resultLen, uint16_t i2c_address)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = i2c_address << 1;

  if (cmdLen > 0) {
    seq.flags = I2C_FLAG_WRITE_READ;

    seq.buf[0].data = cmd;
    seq.buf[0].len  = cmdLen;
    seq.buf[1].data = result;
    seq.buf[1].len  = resultLen;
  } else {
    seq.flags = I2C_FLAG_READ;

    seq.buf[0].data = result;
    seq.buf[0].len  = resultLen;
  }

  ret = I2CSPM_Transfer(SL_I2CSPM_PERIPHERAL, &seq);

  if (ret == i2cTransferNack) {
    return ret;
  } else if (ret != i2cTransferDone) {
    this->timeout_flag = true;
    return ret;
  }

  return ret;
}

uint32_t TwoWire::i2c_leader_write(uint8_t *cmd, size_t cmdLen, uint8_t *data, size_t dataLen, uint16_t i2c_address)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr = i2c_address << 1;
  seq.buf[0].data = cmd;
  seq.buf[0].len = cmdLen;

  if (dataLen > 0) {
    seq.flags = I2C_FLAG_WRITE_WRITE;
    seq.buf[1].data = data;
    seq.buf[1].len = dataLen;
  } else {
    seq.flags = I2C_FLAG_WRITE;
  }

  ret = I2CSPM_Transfer(SL_I2CSPM_PERIPHERAL, &seq);

  if (ret == i2cTransferNack) {
    return ret;
  } else if (ret != i2cTransferDone) {
    this->timeout_flag = true;
    return ret;
  }

  return ret;
}

void TwoWire::_wire_irq_handler()
{
  if (this->role != wire_role_t::FOLLOWER) {
    return;
  }

  uint32_t i2c_int_flags = SL_I2CSPM_PERIPHERAL->IF;
  uint32_t rx_data;

  // If some sort of fault occurred - abort the current transaction and return
  if (i2c_int_flags & (I2C_IF_BUSERR | I2C_IF_ARBLOST)) {
    this->follower_transaction_in_progress = false;
    I2C_IntClear(SL_I2CSPM_PERIPHERAL, I2C_IF_BUSERR | I2C_IF_ARBLOST);
    return;
  }

  // 'Address Match' indicating that a leader device started a transaction with us
  if (i2c_int_flags & I2C_IF_ADDR) {
    this->follower_transaction_in_progress = true;
    rx_data = SL_I2CSPM_PERIPHERAL->RXDATA;
    // Leader read (read bit set)
    if (rx_data & 0x1) {
      if (this->user_onrequest_cb) {
        this->user_onrequest_cb();
      }
    } else {
      // Leader write
      SL_I2CSPM_PERIPHERAL->CMD = I2C_CMD_ACK;
    }
    I2C_IntClear(SL_I2CSPM_PERIPHERAL, I2C_IF_ADDR | I2C_IF_RXDATAV);
  } else if (i2c_int_flags & I2C_IF_RXDATAV) {
    // Leader writes data
    rx_data = SL_I2CSPM_PERIPHERAL->RXDATA;
    if (!this->follower_mode_rx_buffer.isFull()) {
      this->follower_mode_rx_buffer.store_char(rx_data);
      SL_I2CSPM_PERIPHERAL->CMD = I2C_CMD_ACK;
      if (this->user_onreceive_cb) {
        this->user_onreceive_cb(this->follower_mode_rx_buffer.available());
      }
    } else {
      SL_I2CSPM_PERIPHERAL->CMD = I2C_CMD_NACK;
    }
    I2C_IntClear(SL_I2CSPM_PERIPHERAL, I2C_IF_RXDATAV);
  }

  // Leader requests data
  if (i2c_int_flags & I2C_IF_ACK) {
    if (this->user_onrequest_cb) {
      this->user_onrequest_cb();
    }
    I2C_IntClear(SL_I2CSPM_PERIPHERAL, I2C_IF_ACK);
  }

  // End of transaction
  if (i2c_int_flags & I2C_IF_SSTOP) {
    this->follower_transaction_in_progress = false;
    I2C_IntClear(SL_I2CSPM_PERIPHERAL, I2C_IF_SSTOP);
  }
}

TwoWire Wire;

void I2C0_IRQHandler(void)
{
  Wire._wire_irq_handler();
}

void I2C1_IRQHandler(void)
{
  Wire._wire_irq_handler();
}

void I2C2_IRQHandler(void)
{
  Wire._wire_irq_handler();
}
