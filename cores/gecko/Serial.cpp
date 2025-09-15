/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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

#include "Serial.h"

#include <cstdarg>
#include "em_usart.h"
#include "sl_iostream.h"

using namespace arduino;

UARTClass::UARTClass(sl_iostream_t* stream,
                     sl_iostream_uart_t* instance,
                     void(*baud_rate_set_fn)(uint32_t baudrate),
                     void(*init_fn)(void),
                     void(*deinit_fn)(void),
                     void(*serial_event_fn)(void)) :
  serial_mutex(nullptr),
  initialized(false),
  baudrate(115200),
  suspended(false)
{
  this->serial_mutex = xSemaphoreCreateMutexStatic(&this->serial_mutex_buf);
  configASSERT(this->serial_mutex);
  this->baud_rate_set_fn = baud_rate_set_fn;
  this->init_fn = init_fn;
  this->deinit_fn = deinit_fn;
  this->stream_handle = stream;
  this->instance_handle = instance;
  this->serial_event_fn = serial_event_fn;
}

void UARTClass::begin(unsigned long baudrate)
{
  if (this->initialized) {
    return;
  }
  this->init_fn();
  this->baud_rate_set_fn(baudrate);
  this->initialized = true;
  this->baudrate = baudrate;
}

void UARTClass::begin(unsigned long baudrate, uint16_t config)
{
  (void)config;
  this->begin(baudrate);
}

void UARTClass::end()
{
  if (!this->initialized) {
    return;
  }
  this->deinit_fn();
  this->initialized = false;
}

int UARTClass::available(void)
{
  task();
  return rx_buf.available();
}

int UARTClass::peek(void)
{
  return rx_buf.peek();
}

int UARTClass::read(void)
{
  task();
  return rx_buf.read_char();
}

void UARTClass::flush(void)
{
  ;
}

size_t UARTClass::write(uint8_t data)
{
  return this->write(&data, 1);
}

size_t UARTClass::write(const uint8_t* data, size_t size)
{
  if (!this->initialized) {
    return 0;
  }
  sl_iostream_write(this->stream_handle, data, size);
  return size;
}

void UARTClass::printf(const char *fmt, ...)
{
  char message[this->printf_buffer_size];
  va_list args;
  va_start(args, fmt);
  vsnprintf(message, sizeof(message), fmt, args);
  va_end(args);
  this->write((uint8_t*)message, strlen(message));
}

void UARTClass::suspend()
{
  if (!this->initialized) {
    return;
  }
  // The small delay is here to allow the last bytes to be transmitted
  delay(1);
  this->end();
  this->suspended = true;
}

void UARTClass::resume()
{
  if (!this->suspended) {
    return;
  }
  this->begin(this->baudrate);
  this->suspended = false;
}

UARTClass::operator bool()
{
  return true;
}

void UARTClass::task()
{
  if (!this->initialized) {
    return;
  }
  xSemaphoreTake(this->serial_mutex, portMAX_DELAY);

  uint8_t buf[64];
  size_t bytes_read = 0;
  // Set the iostream read API to non-blocking mode
  sl_iostream_uart_set_read_block(this->instance_handle, false);
  sl_iostream_read(this->stream_handle, buf, sizeof(buf), &bytes_read);
  if (bytes_read > 0) {
    for (uint8_t i = 0; i < bytes_read; i++) {
      rx_buf.store_char(buf[i]);
    }
  }

  xSemaphoreGive(this->serial_mutex);
}

void UARTClass::handleSerialEvent()
{
  if (this->available()) {
    this->serial_event_fn();
  }
}

__attribute__((weak)) void serialEvent(void)
{
  ;
}

arduino::UARTClass Serial(sl_serial_stream_handle,
                          sl_serial_instance_handle,
                          sl_serial_set_baud_rate,
                          sl_serial_init,
                          sl_serial_deinit,
                          serialEvent);

#if (NUM_HW_SERIAL > 1)
__attribute__((weak)) void serialEvent1(void)
{
  ;
}

arduino::UARTClass Serial1(sl_serial1_stream_handle,
                           sl_serial1_instance_handle,
                           sl_serial1_set_baud_rate,
                           sl_serial1_init,
                           sl_serial1_deinit,
                           serialEvent1);
#endif // #if (NUM_HW_SERIAL > 1)
