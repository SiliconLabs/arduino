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

#include "Arduino.h"

#ifndef __ARDUINO_SERIAL_H
#define __ARDUINO_SERIAL_H

#include <cmath>
#include <inttypes.h>
#include "api/RingBuffer.h"
#include "Arduino.h"
#include "api/HardwareSerial.h"
#include "api/Stream.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "arduino_serial_config.h"

namespace arduino {
class UARTClass : public HardwareSerial
{
public:
  UARTClass(sl_iostream_t* stream,
            sl_iostream_uart_t* instance,
            void(*baud_rate_set_fn)(uint32_t baudrate),
            void(*init_fn)(void),
            void(*deinit_fn)(void),
            void(*serial_event_fn)(void));
  void begin(unsigned long);
  void begin(unsigned long baudrate, uint16_t config);
  void end();
  int available(void);
  int peek(void);
  int read(void);
  void flush(void);
  size_t write(uint8_t data);
  size_t write(const uint8_t* data, size_t size);
  using Print::write;   // pull in write(str) from Print
  operator bool();
  void task();
  void handleSerialEvent();
  void printf(const char* fmt, ...);
  void suspend();
  void resume();
private:
  static const uint8_t printf_buffer_size = 128u;

  RingBufferN<128> rx_buf;

  SemaphoreHandle_t serial_mutex;
  StaticSemaphore_t serial_mutex_buf;

  void (*baud_rate_set_fn)(uint32_t baudrate);
  void (*init_fn)(void);
  void (*deinit_fn)(void);
  void (*serial_event_fn)(void);

  sl_iostream_t* stream_handle;
  sl_iostream_uart_t* instance_handle;

  bool initialized;
  unsigned long baudrate;
  bool suspended;
};
} // namespace arduino

void serialEvent(void);
#if (NUM_HW_SERIAL > 1)
void serialEvent1(void);
#endif // (NUM_HW_SERIAL > 1)

extern arduino::UARTClass Serial;
#if (NUM_HW_SERIAL > 1)
extern arduino::UARTClass Serial1;
#endif // (NUM_HW_SERIAL > 1)

#endif // __ARDUINO_SERIAL_H
