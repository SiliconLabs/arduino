/*
   Copyright (c) 2015 Arduino LLC.  All right reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <cmath>
#include <inttypes.h>
#include "Stream.h"
#include "RingBuffer.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "arduino_serial_config.h"

#define HARDSER_PARITY_EVEN (0x1ul)
#define HARDSER_PARITY_ODD  (0x2ul)
#define HARDSER_PARITY_NONE (0x3ul)
#define HARDSER_PARITY_MASK (0xFul)

#define HARDSER_STOP_BIT_1    (0x10ul)
#define HARDSER_STOP_BIT_1_5  (0x20ul)
#define HARDSER_STOP_BIT_2    (0x30ul)
#define HARDSER_STOP_BIT_MASK (0xF0ul)

#define HARDSER_DATA_5    (0x100ul)
#define HARDSER_DATA_6    (0x200ul)
#define HARDSER_DATA_7    (0x300ul)
#define HARDSER_DATA_8    (0x400ul)
#define HARDSER_DATA_MASK (0xF00ul)

#define SERIAL_5N1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_5)
#define SERIAL_6N1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_6)
#define SERIAL_7N1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_7)
#define SERIAL_8N1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_NONE | HARDSER_DATA_8)
#define SERIAL_5N2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_5)
#define SERIAL_6N2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_6)
#define SERIAL_7N2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_7)
#define SERIAL_8N2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_NONE | HARDSER_DATA_8)
#define SERIAL_5E1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_5)
#define SERIAL_6E1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_6)
#define SERIAL_7E1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_7)
#define SERIAL_8E1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_EVEN | HARDSER_DATA_8)
#define SERIAL_5E2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_5)
#define SERIAL_6E2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_6)
#define SERIAL_7E2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_7)
#define SERIAL_8E2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_EVEN | HARDSER_DATA_8)
#define SERIAL_5O1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_5)
#define SERIAL_6O1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_6)
#define SERIAL_7O1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_7)
#define SERIAL_8O1  (HARDSER_STOP_BIT_1 | HARDSER_PARITY_ODD  | HARDSER_DATA_8)
#define SERIAL_5O2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_5)
#define SERIAL_6O2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_6)
#define SERIAL_7O2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_7)
#define SERIAL_8O2  (HARDSER_STOP_BIT_2 | HARDSER_PARITY_ODD  | HARDSER_DATA_8)

class HardwareSerial : public Stream
{
public:
  HardwareSerial(sl_iostream_t* stream,
                 sl_iostream_uart_t* instance,
                 void(*baud_rate_set_fn)(uint32_t baudrate),
                 void(*init_fn)(void),
                 void(*deinit_fn)(void),
                 void(*serial_event_fn)(void));
  virtual void begin(unsigned long);
  virtual void begin(unsigned long baudrate, uint16_t config);
  virtual void end();
  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual void flush(void);
  virtual size_t write(uint8_t data);
  virtual size_t write(const uint8_t* data, size_t size);
  using Print::write;   // pull in write(str) from Print
  virtual operator bool();
  void task();
  void handleSerialEvent();
  void printf(const char* fmt, ...);
private:
  static const uint8_t printf_buffer_size = 128u;

  RingBuffer<128> rx_buf;

  SemaphoreHandle_t serial_mutex;
  StaticSemaphore_t serial_mutex_buf;

  void (*baud_rate_set_fn)(uint32_t baudrate);
  void (*init_fn)(void);
  void (*deinit_fn)(void);
  void (*serial_event_fn)(void);

  sl_iostream_t* stream_handle;
  sl_iostream_uart_t* instance_handle;

  bool initialized;
};

void serialEvent(void);

#if (NUM_HW_SERIAL > 1)
void serialEvent1(void);
#endif // #if (NUM_HW_SERIAL > 1)

#endif // HardwareSerial_h
