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

#ifndef WIRE_H
#define WIRE_H
// For the API description refer to: https://www.arduino.cc/reference/en/language/functions/communication/wire/

#include <cmath>
#include <cstddef>
#include <cstring>
#include "em_gpio.h"
#include "sl_i2cspm.h"
#include "arduino_i2c_config.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "RingBuffer.h"

class TwoWire {
public:
  /***************************************************************************//**
   * Constructor for TwoWire
   ******************************************************************************/
  TwoWire();

  /***************************************************************************//**
   * Starts the I2C peripheral in leader mode
   ******************************************************************************/
  void begin();

  /***************************************************************************//**
   * Starts the I2C peripheral in follower mode
   *
   * @param[in] follower_mode_address I2C follower address to join the bus with
   ******************************************************************************/
  void begin(uint8_t follower_mode_address);

  /***************************************************************************//**
   * Deinitializes the I2C peripheral
   ******************************************************************************/
  void end();

  /***************************************************************************//**
   * Requests bytes from an I2C follower
   * Discards any unread data from the receive buffer before reading.
   * (leader mode only)
   *
   * @param[in] address The address of the I2C follower
   * @param[in] number_of_bytes The number of bytes requested from the follower
   *
   * @return Returns the number of bytes received
   ******************************************************************************/
  uint8_t requestFrom(uint8_t address, uint8_t number_of_bytes);

  /***************************************************************************//**
   * Requests bytes from an I2C follower
   * Discards any unread data from the receive buffer before reading.
   * (leader mode only)
   *
   * @param[in] address The address of the I2C follower
   * @param[in] number_of_bytes The number of bytes requested from the follower
   * @param[in] stop Indicates whether the I2C communication should be ended
   *                 after the reception
   *
   * @return Returns the number of bytes received
   ******************************************************************************/
  uint8_t requestFrom(uint8_t address, uint8_t number_of_bytes, uint8_t stop);

  /***************************************************************************//**
   * Starts an I2C transmission with a follower device
   * (leader mode only)
   *
   * @param[in] address The address of the I2C follower
   ******************************************************************************/
  void beginTransmission(uint16_t follower_address);

  /***************************************************************************//**
   * Ends the I2C transmission with a follower device
   * (leader mode only)
   *
   * @param[in] stop Indicates whether an I2C STOP condition should be sent
   *                 ending the session with the follower device
   *
   * @return Returns a WireStatus indicating the result of the operation
   ******************************************************************************/
  uint8_t endTransmission(bool stop = true);

  /***************************************************************************//**
   * Sends the provided byte over the I2C bus
   * (leader/follower mode)
   *
   * @param[in] value Byte of data to be sent over the I2C bus
   *
   * @return Returns the number of bytes sent
   ******************************************************************************/
  size_t write(uint8_t value);

  /***************************************************************************//**
   * Sends the provided bytes over the I2C bus
   * (leader mode only)
   *
   * @param[in] data Pointer to the data to be sent
   * @param[in] size Size of the data to be sent
   *
   * @return Returns the number of bytes sent
   ******************************************************************************/
  size_t write(const uint8_t* data, size_t size);

  /***************************************************************************//**
   * Gets the number of bytes received from the leader/follower device
   * (leader/follower mode)
   *
   * @return Returns the number of bytes received from the follower device
   ******************************************************************************/
  int available();

  /***************************************************************************//**
   * Returns the next byte from the receive buffer if available
   * (leader/follower mode)
   *
   * @return Returns the next received byte, -1 on failure
   ******************************************************************************/
  int read();

  /***************************************************************************//**
   * Sets the bus clock speed
   * (leader/follower mode)
   *
   * @param[in] clock The requested bus clock speed in hertz
   ******************************************************************************/
  void setClock(const uint32_t clock);

  /***************************************************************************//**
   * Sets the function which should be called on reception
   * (leader/follower mode)
   *
   * @param[in] user_onreceive_cb Pointer to the receive callback function
   ******************************************************************************/
  void onReceive(void (*user_onreceive_cb)(int));

  /***************************************************************************//**
   * Sets the function which should be called on data request from the leader
   * (follower mode only)
   *
   * @param[in] user_onrequest_cb Pointer to the request callback function
   ******************************************************************************/
  void onRequest(void (*user_onrequest_cb)(void));

  /***************************************************************************//**
   * Sets the timeout and whether a reset should occur on timeout
   * Note: setting the timeout amount has no effect, only the timeout behaviour
   * (leader/follower mode)
   *
   * @param[in] timeout The requested timeout amount in microseconds
   * @param[in] reset_on_timeout Indicates whether a communication reset
   *                             should be performed on timeout
   ******************************************************************************/
  void setWireTimeout(int timeout, bool reset_on_timeout);

  /***************************************************************************//**
   * Clears the communication timeout flag
   * (leader/follower mode)
   ******************************************************************************/
  void clearWireTimeoutFlag();

  /***************************************************************************//**
   * returns the communication timeout flag
   * (leader/follower mode)
   *
   * @return Returns the communication timeout flag
   ******************************************************************************/
  bool getWireTimeoutFlag();

  /***************************************************************************//**
   * Interrupt handler for the I2C peripheral
   * Meant to be called by the I2C ISR and not externally by users.
   * (follower mode only)
   ******************************************************************************/
  void _wire_irq_handler();

  enum WireStatus {
    SUCCESS = 0,
    DATA_TOO_LONG,
    NACK_ADDRESS,
    NACK_DATA,
    OTHER_ERROR,
    TIMEOUT
  };

private:
  enum wire_role_t {
    NOT_INITIALIZED,
    LEADER,
    FOLLOWER
  };
  wire_role_t role;

  /***************************************************************************//**
   * Sends a command and reads the resulting byte(s) over the I2C bus
   *
   * @param[in] cmd The command to be sent
   * @param[in] cmdLen The length of the command in bytes
   * @param[out] result The received bytes sent by the I2C follower
   * @param[in] resultLen The number of the bytes requested from the I2C follower
   * @param[in] i2c_address The address of the I2C follower to communicate with
   *
   * @return Returns zero on OK, non-zero otherwise
   ******************************************************************************/
  uint32_t i2c_leader_read(uint8_t *cmd, size_t cmdLen, uint8_t *result, size_t resultLen, uint16_t i2c_address);

  /***************************************************************************//**
   * Sends a command and data to the follower device over the I2C bus
   *
   * @param[in] cmd The command to be sent
   * @param[in] cmdLen The length of the command in bytes
   * @param[in] data The data byte(s) to be sent to the I2C follower
   * @param[in] dataLen The number of the bytes to be sent to the I2C follower
   * @param[in] i2c_address The address of the I2C follower to communicate with
   *
   * @return Returns zero on OK, non-zero otherwise
   ******************************************************************************/
  uint32_t i2c_leader_write(uint8_t *cmd, size_t cmdLen, uint8_t *data, size_t dataLen, uint16_t i2c_address);

  bool timeout_flag;
  bool reset_on_timeout;

  static const uint32_t tx_buffer_size = 64u;
  static const uint32_t rx_buffer_size = 64u;
  uint8_t tx_buffer[tx_buffer_size];
  uint8_t rx_buffer[rx_buffer_size];

  uint16_t follower_address;
  bool transmission_in_progress;

  uint32_t tx_buf_write_idx;
  uint32_t rx_buf_read_idx;
  uint32_t rx_buf_available;

  uint8_t follower_mode_address;
  bool follower_transaction_in_progress;
  RingBuffer<64> follower_mode_rx_buffer;

  void (*user_onreceive_cb)(int);
  void (*user_onrequest_cb)(void);

  SemaphoreHandle_t wire_mutex;
  StaticSemaphore_t wire_mutex_buf;
};

#endif // WIRE_H
