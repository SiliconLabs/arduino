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

#ifndef EZBLE_H
#define EZBLE_H

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This library is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif

#include "Arduino.h"
extern "C" {
  #include "sl_bluetooth.h"
}
#include "api/RingBuffer.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define EZBLE_ENABLE_DEBUG_LOGGING 0

// ezBLE service
// UUID: de8a5aac-a99b-c315-0c80-60d4cbb5beef
const uuid_128 ezble_service_uuid = {
  .data = { 0xef, 0xbe, 0xb5, 0xcb, 0xd4, 0x60, 0x80, 0x0c, 0x15, 0xc3, 0x9b, 0xa9, 0xac, 0x5a, 0x8a, 0xde }
};

// ezBLE data characteristic
// UUID: 5b026510-4088-c297-46d8-be6c7367beef
const uuid_128 ezble_data_characteristic_uuid = {
  .data = { 0xef, 0xbe, 0x67, 0x73, 0x6c, 0xbe, 0xd8, 0x46, 0x97, 0xc2, 0x88, 0x40, 0x10, 0x65, 0x02, 0x5b }
};

class ezBLEclass : public Stream {
public:
  enum ezble_role_t {
    UNKNOWN,
    SERVER,
    SERVER_MULTICONN,
    CLIENT
  };

  ezBLEclass();
  void begin(ezble_role_t role, const char* ble_name = "ezBLE");
  void beginServer(const char* ble_name = "ezBLE");
  void beginClient(const char* ble_name = "ezBLE");

  void end();
  bool connected();
  const char* get_ble_name();
  void handle_ble_event(sl_bt_msg_t* evt);
  void printf(const char* fmt, ...);
  void onReceive(void (*user_onreceive_callback)(int));
  void onConnect(void (*user_onconnect_callback)());
  void onDisconnect(void (*user_ondisconnect_callback)());

  virtual size_t write(uint8_t data);
  virtual size_t write(const uint8_t* data, size_t size);
  virtual int read();
  virtual int available();
  virtual int peek();

private:
  enum ezble_state_t {
    ST_NOT_STARTED = 0,
    ST_ERROR,
    ST_BOOT,
    ST_DISCONNECTED,
    ST_DISCOVER_SERVICES,
    ST_DISCOVER_CHARACTERISTICS,
    ST_READY,
    ST_BUSY
  };

  void set_ble_name(const char* ble_name);

  void init_gattdb();
  void init_advertising();
  void start_advertising();
  void stop_advertising();
  void init_scanning();
  void start_scanning();
  void stop_scanning();

  void handle_boot_event();
  void handle_conn_open(sl_bt_msg_t* evt);
  void handle_conn_close();
  void process_scan_reports(sl_bt_msg_t* evt);
  void handle_gatt_service_discovery(sl_bt_msg_t* evt);
  void handle_gatt_characteristic_discovery(sl_bt_msg_t* evt);
  void handle_gatt_procedure_complete(sl_bt_msg_t* evt);
  void handle_gatt_data_receive(sl_bt_msg_t* evt);

  void discover_ezble_service();
  void discover_ezble_characteristics();

  bool find_complete_local_name_in_advertisement(sl_bt_evt_scanner_legacy_advertisement_report_t* response, const char* name);
  void call_user_onReceive(int bytes);
  void call_user_onConnect();
  void call_user_onDisconnect();
  size_t transfer_outgoing_data();
  void set_state(ezble_state_t state_new);
  void ezble_log(const char* fmt, ...);

  bool gattdb_initialized;
  uint8_t connection_handle;
  uint8_t advertising_set_handle;
  uint16_t local_generic_access_gatt_service_handle;
  uint16_t local_name_gatt_characteristic_handle;
  uint16_t local_ezble_gatt_service_handle;
  uint16_t local_ezble_data_gatt_characteristic_handle;
  uint32_t remote_ezble_gatt_service_handle;
  uint16_t remote_ezble_data_gatt_characteristic_handle;

  bool ble_stack_booted;
  ezble_role_t role;
  const char* ble_name;
  ezble_state_t state;
  void (*user_onreceive_callback)(int);
  void (*user_onconnect_callback)(void);
  void (*user_ondisconnect_callback)(void);

  static const uint8_t printf_buffer_size = 64u;
  static const uint16_t max_ble_transfer_size = 250u;
  static const size_t data_buffer_size = 512u;

  RingBufferN<data_buffer_size> rx_buf;
  RingBufferN<data_buffer_size> tx_buf;

  SemaphoreHandle_t rx_buf_mutex;
  StaticSemaphore_t rx_buf_mutex_buf;
  SemaphoreHandle_t tx_buf_mutex;
  StaticSemaphore_t tx_buf_mutex_buf;
};

extern ezBLEclass ezBLE;

#endif // EZBLE_H
