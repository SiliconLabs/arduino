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

#include "ezBLE.h"

ezBLEclass::ezBLEclass() :
  gattdb_initialized(false),
  connection_handle(UINT8_MAX),
  advertising_set_handle(UINT8_MAX),
  local_generic_access_gatt_service_handle(UINT16_MAX),
  local_name_gatt_characteristic_handle(UINT16_MAX),
  local_ezble_gatt_service_handle(UINT16_MAX),
  local_ezble_data_gatt_characteristic_handle(UINT16_MAX),
  remote_ezble_gatt_service_handle(UINT32_MAX),
  remote_ezble_data_gatt_characteristic_handle(UINT16_MAX),
  ble_stack_booted(false),
  state(ezble_state_t::ST_NOT_STARTED),
  user_onreceive_callback(nullptr),
  user_onconnect_callback(nullptr),
  user_ondisconnect_callback(nullptr)
{
  this->rx_buf_mutex = xSemaphoreCreateMutexStatic(&this->rx_buf_mutex_buf);
  configASSERT(this->rx_buf_mutex);
  this->tx_buf_mutex = xSemaphoreCreateMutexStatic(&this->tx_buf_mutex_buf);
  configASSERT(this->tx_buf_mutex);
}

void ezBLEclass::begin(ezble_role_t role, const char* ble_name)
{
  if (this->state != ST_NOT_STARTED) {
    return;
  }

  this->role = role;
  this->set_ble_name(ble_name);
  this->init_gattdb();
  this->ezble_log("ezBLE started; role='%u'", this->role);
  // If the BLE stack has already booted, we can move on to discovery,
  // otherwise wait for the boot event
  if (this->ble_stack_booted) {
    if (this->role == SERVER) {
      this->init_advertising();
      this->start_advertising();
    }
    if (this->role == CLIENT) {
      this->init_scanning();
      this->start_scanning();
    }
    this->set_state(ST_DISCONNECTED);
  } else {
    this->set_state(ST_BOOT);
  }
}

void ezBLEclass::beginServer(const char* ble_name)
{
  this->begin(ezble_role_t::SERVER, ble_name);
}

void ezBLEclass::beginClient(const char* ble_name)
{
  this->begin(ezble_role_t::CLIENT, ble_name);
}

void ezBLEclass::end()
{
  xSemaphoreTake(this->tx_buf_mutex, portMAX_DELAY);
  xSemaphoreTake(this->rx_buf_mutex, portMAX_DELAY);

  this->stop_advertising();
  this->stop_scanning();

  // Close the connection if it's open
  if (this->state == ST_DISCOVER_SERVICES
      || this->state == ST_DISCOVER_CHARACTERISTICS
      || this->state == ST_READY
      || this->state == ST_BUSY) {
    sl_status_t sc = sl_bt_connection_close(this->connection_handle);
    if (sc != SL_STATUS_OK) {
      xSemaphoreGive(this->tx_buf_mutex);
      xSemaphoreGive(this->rx_buf_mutex);
      this->ezble_log("Could not close connection");
      return;
    }
  }
  // Clear buffers
  this->rx_buf.clear();
  this->tx_buf.clear();
  // Reset handles
  this->connection_handle = UINT8_MAX;
  this->remote_ezble_gatt_service_handle = UINT32_MAX;
  this->remote_ezble_data_gatt_characteristic_handle = UINT16_MAX;
  // Reset state and callback
  this->set_state(ST_NOT_STARTED);
  this->user_onreceive_callback = nullptr;
  this->user_onconnect_callback = nullptr;
  this->user_ondisconnect_callback = nullptr;
  this->ezble_log("ezBLE ended");

  xSemaphoreGive(this->tx_buf_mutex);
  xSemaphoreGive(this->rx_buf_mutex);
}

bool ezBLEclass::connected()
{
  // If the state is READY or BUSY then we're connected to the other device and able to communicate
  if (this->state == ezble_state_t::ST_READY || this->state == ezble_state_t::ST_BUSY) {
    return true;
  }
  return false;
}

const char* ezBLEclass::get_ble_name()
{
  return this->ble_name;
}

void ezBLEclass::set_ble_name(const char* ble_name)
{
  this->ble_name = ble_name;

  if (!this->gattdb_initialized) {
    return;
  }
  // If the GATT DB is already initialized we have to set the new name in the DB
  sl_status_t sc = sl_bt_gatt_server_write_attribute_value(this->local_name_gatt_characteristic_handle,
                                                           0u,
                                                           strlen(ble_name),
                                                           (const uint8_t*)ble_name);
  if (sc != SL_STATUS_OK) {
    this->ezble_log("Setting new advertised name in GATT DB failed!");
  }
}

void ezBLEclass::set_state(ezble_state_t state_new)
{
  this->ezble_log("State changed: %u -> %u", this->state, state_new);
  this->state = state_new;
}

size_t ezBLEclass::write(uint8_t data)
{
  return this->write(&data, 1);
}

size_t ezBLEclass::write(const uint8_t* data, size_t size)
{
  // If the Tx buffer is full
  if (tx_buf.isFull()) {
    this->ezble_log("Tx buffer overflow!");
    // Try to transmit the buffered data to free up space
    (void)this->transfer_outgoing_data();
    // Return with an error
    return -1;
  }

  xSemaphoreTake(this->tx_buf_mutex, portMAX_DELAY);
  for (size_t i = 0; i < size; i++) {
    this->tx_buf.store_char(data[i]);
    // If the buffer is gets full while writing
    // transfer any buffered data, then return with an error
    if (tx_buf.isFull()) {
      xSemaphoreGive(this->tx_buf_mutex);
      this->ezble_log("Tx buffer overflow!");
      (void)this->transfer_outgoing_data();
      return -1;
    }
  }
  xSemaphoreGive(this->tx_buf_mutex);

  return this->transfer_outgoing_data();
}

void ezBLEclass::printf(const char* fmt, ...)
{
  char message[this->printf_buffer_size];
  va_list args;
  va_start(args, fmt);
  vsnprintf(message, sizeof(message), fmt, args);
  va_end(args);
  this->write((uint8_t*)message, strlen(message));
}

void ezBLEclass::onReceive(void (*user_onreceive_callback)(int))
{
  if (!user_onreceive_callback) {
    return;
  }
  this->user_onreceive_callback = user_onreceive_callback;
}

void ezBLEclass::onConnect(void (*user_onconnect_callback)())
{
  if (!user_onconnect_callback) {
    return;
  }
  this->user_onconnect_callback = user_onconnect_callback;
}

void ezBLEclass::onDisconnect(void (*user_ondisconnect_callback)())
{
  if (!user_ondisconnect_callback) {
    return;
  }
  this->user_ondisconnect_callback = user_ondisconnect_callback;
}

void ezBLEclass::call_user_onReceive(int bytes)
{
  if (!this->user_onreceive_callback) {
    return;
  }
  this->user_onreceive_callback(bytes);
}

void ezBLEclass::call_user_onConnect()
{
  if (!this->user_onconnect_callback) {
    return;
  }
  this->user_onconnect_callback();
}

void ezBLEclass::call_user_onDisconnect()
{
  if (!this->user_ondisconnect_callback) {
    return;
  }
  this->user_ondisconnect_callback();
}

size_t ezBLEclass::transfer_outgoing_data()
{
  if (this->state != ezble_state_t::ST_READY) {
    return -1;
  }

  if (!this->tx_buf.available()) {
    return -2;
  }

  // Get all the data from the Tx buffer
  xSemaphoreTake(this->tx_buf_mutex, portMAX_DELAY);
  size_t local_buf_idx = 0;
  // Create a buffer as large as the max BLE transfer size
  uint8_t local_buf[this->max_ble_transfer_size];
  // Copy as much data as we have/can into the buffer
  while (this->tx_buf.available() && local_buf_idx < this->max_ble_transfer_size) {
    local_buf[local_buf_idx] = this->tx_buf.read_char();
    local_buf_idx++;
  }
  xSemaphoreGive(this->tx_buf_mutex);

  sl_status_t sc = sl_bt_gatt_write_characteristic_value(this->connection_handle,
                                                         this->remote_ezble_data_gatt_characteristic_handle,
                                                         local_buf_idx,
                                                         local_buf);
  if (sc != SL_STATUS_OK) {
    this->ezble_log("GATT write failed");
    return -3;
  }
  this->set_state(ST_BUSY);

  this->ezble_log("Sent %u bytes", local_buf_idx);
  return local_buf_idx;
}

int ezBLEclass::read(void)
{
  xSemaphoreTake(this->rx_buf_mutex, portMAX_DELAY);
  int data = this->rx_buf.read_char();
  xSemaphoreGive(this->rx_buf_mutex);
  return data;
}

int ezBLEclass::available()
{
  return this->rx_buf.available();
}

int ezBLEclass::peek()
{
  return rx_buf.peek();
}

void ezBLEclass::init_advertising()
{
  sl_status_t sc;
  // Create an advertising set
  if (this->advertising_set_handle == UINT8_MAX) {
    sc = sl_bt_advertiser_create_set(&this->advertising_set_handle);
    app_assert_status(sc);
  }

  // Set advertising interval to 100ms
  sc = sl_bt_advertiser_set_timing(
    this->advertising_set_handle,
    160,     // minimum advertisement interval (milliseconds * 1.6)
    160,     // maximum advertisement interval (milliseconds * 1.6)
    0,       // advertisement duration
    0);      // maximum number of advertisement events
  app_assert_status(sc);
}

void ezBLEclass::start_advertising()
{
  sl_status_t sc;
  // Generate data for advertising
  sc = sl_bt_legacy_advertiser_generate_data(this->advertising_set_handle, sl_bt_advertiser_general_discoverable);
  app_assert_status(sc);

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(this->advertising_set_handle, sl_bt_advertiser_connectable_scannable);
  app_assert_status(sc);

  this->ezble_log("Started advertising as '%s'", this->ble_name);
}

void ezBLEclass::stop_advertising()
{
  sl_status_t sc = sl_bt_advertiser_stop(this->advertising_set_handle);
  app_assert_status(sc);
}

void ezBLEclass::init_scanning()
{
  sl_status_t sc = sl_bt_scanner_set_parameters(sl_bt_scanner_scan_mode_active, 16, 16);
  app_assert_status(sc);
}

void ezBLEclass::start_scanning()
{
  sl_status_t sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                                       sl_bt_scanner_discover_generic);
  app_assert_status(sc);
}

void ezBLEclass::stop_scanning()
{
  sl_status_t sc = sl_bt_scanner_stop();
  app_assert_status(sc);
}

void ezBLEclass::handle_boot_event()
{
  this->ble_stack_booted = true;
  if (this->state == ezble_state_t::ST_BOOT) {
    this->set_state(ST_DISCONNECTED);
    if (this->role == SERVER) {
      this->init_advertising();
      this->start_advertising();
    }
    if (this->role == CLIENT) {
      this->init_scanning();
      this->start_scanning();
    }
  }
}

void ezBLEclass::handle_conn_open(sl_bt_msg_t* evt)
{
  this->connection_handle = evt->data.evt_connection_opened.connection;
  this->set_state(ST_DISCOVER_SERVICES);
  this->discover_ezble_service();
}

void ezBLEclass::handle_conn_close()
{
  // If the connection close was requested (like calling end()) we shouldn't do anything
  if (this->state == ST_NOT_STARTED) {
    return;
  }
  this->set_state(ST_DISCONNECTED);
  this->connection_handle = UINT8_MAX;
  this->remote_ezble_gatt_service_handle = UINT32_MAX;
  this->remote_ezble_data_gatt_characteristic_handle = UINT16_MAX;
  this->call_user_onDisconnect();
  if (this->role == SERVER) {
    this->start_advertising();
  }
  if (this->role == CLIENT) {
    this->start_scanning();
  }
}

void ezBLEclass::process_scan_reports(sl_bt_msg_t* evt)
{
  if (this->role != CLIENT) {
    return;
  }

  if (find_complete_local_name_in_advertisement(&(evt->data.evt_scanner_legacy_advertisement_report), this->ble_name)) {
    this->ezble_log("Target device found");
    this->stop_scanning();
    sl_status_t sc = sl_bt_connection_open(evt->data.evt_scanner_legacy_advertisement_report.address,
                                           evt->data.evt_scanner_legacy_advertisement_report.address_type,
                                           sl_bt_gap_phy_1m,
                                           NULL);
    app_assert_status(sc);
  }
}

void ezBLEclass::handle_gatt_service_discovery(sl_bt_msg_t* evt)
{
  if (this->state == ezble_state_t::ST_DISCOVER_SERVICES) {
    this->remote_ezble_gatt_service_handle = evt->data.evt_gatt_service.service;
  }
}

void ezBLEclass::handle_gatt_characteristic_discovery(sl_bt_msg_t* evt)
{
  if (this->state == ezble_state_t::ST_DISCOVER_CHARACTERISTICS) {
    this->remote_ezble_data_gatt_characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
  }
}

void ezBLEclass::handle_gatt_procedure_complete(sl_bt_msg_t* evt)
{
  (void)evt;
  // Service discovery finishes
  if (this->state == ezble_state_t::ST_DISCOVER_SERVICES && this->remote_ezble_gatt_service_handle != UINT32_MAX) {
    this->discover_ezble_characteristics();
    this->set_state(ST_DISCOVER_CHARACTERISTICS);
  }

  // Characteristic discovery finishes
  if (this->state == ezble_state_t::ST_DISCOVER_CHARACTERISTICS && this->remote_ezble_data_gatt_characteristic_handle != UINT16_MAX) {
    this->set_state(ST_READY);
    this->call_user_onConnect();
    this->ezble_log("Entered READY state");
  }

  // GATT write finishes
  if (this->state == ezble_state_t::ST_BUSY) {
    this->set_state(ST_READY);
    if (this->tx_buf.available()) {
      this->transfer_outgoing_data();
    }
  }
}

void ezBLEclass::handle_gatt_data_receive(sl_bt_msg_t* evt)
{
  if (this->local_ezble_data_gatt_characteristic_handle != evt->data.evt_gatt_server_attribute_value.attribute) {
    return;
  }
  uint8_t data_len = evt->data.evt_gatt_server_attribute_value.value.len;
  uint8_t* data = evt->data.evt_gatt_server_attribute_value.value.data;
  this->ezble_log("Received %u bytes", data_len);

  if (this->rx_buf.isFull()) {
    // Overflow, Rx buffer is full, cannot store any additional data
    this->ezble_log("Rx buffer overflow!");
    return;
  }

  xSemaphoreTake(this->rx_buf_mutex, portMAX_DELAY);

  uint8_t buffered_bytes = 0u;
  for (uint8_t i = 0; i < data_len; i++) {
    this->rx_buf.store_char(data[i]);
    buffered_bytes++;
    if (this->rx_buf.isFull()) {
      // Overflow, Rx buffer is full, cannot store any additional data
      xSemaphoreGive(this->rx_buf_mutex);
      this->ezble_log("Rx buffer overflow!");
      this->call_user_onReceive(buffered_bytes);
      return;
    }
  }

  xSemaphoreGive(this->rx_buf_mutex);
  this->call_user_onReceive(buffered_bytes);
}

void ezBLEclass::discover_ezble_service()
{
  sl_status_t sc = sl_bt_gatt_discover_primary_services_by_uuid(this->connection_handle,
                                                                sizeof(ezble_service_uuid.data),
                                                                ezble_service_uuid.data);
  app_assert_status(sc);
}

void ezBLEclass::discover_ezble_characteristics()
{
  sl_status_t sc = sl_bt_gatt_discover_characteristics_by_uuid(this->connection_handle,
                                                               this->remote_ezble_gatt_service_handle,
                                                               sizeof(ezble_data_characteristic_uuid.data),
                                                               ezble_data_characteristic_uuid.data);
  app_assert_status(sc);
}

void ezBLEclass::init_gattdb()
{
  if (this->gattdb_initialized) {
    return;
  }
  static uint16_t gattdb_session_id;
  sl_status_t sc;
  // Create a new GATT database
  sc = sl_bt_gattdb_new_session(&gattdb_session_id);
  app_assert_status(sc);

  // Add the Generic Access service to the GATT DB
  const uint8_t generic_access_service_uuid[] = { 0x00, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(generic_access_service_uuid),
                                generic_access_service_uuid,
                                &this->local_generic_access_gatt_service_handle);
  app_assert_status(sc);

  // Add the Device Name characteristic to the Generic Access service
  // The value of the Device Name characteristic will be advertised
  const sl_bt_uuid_16_t device_name_characteristic_uuid = { .data = { 0x00, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              this->local_generic_access_gatt_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              device_name_characteristic_uuid,
                                              sl_bt_gattdb_variable_length_value,
                                              128u,
                                              strlen(this->ble_name),
                                              (const uint8_t*)this->ble_name,
                                              &this->local_name_gatt_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, this->local_generic_access_gatt_service_handle);
  app_assert_status(sc);

  // Add the ezBLE service to the GATT DB
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(ezble_service_uuid),
                                ezble_service_uuid.data,
                                &this->local_ezble_gatt_service_handle);
  app_assert_status(sc);

  // Add the 'ezBLE data' characteristic to the ezBLE service
  uint8_t ezble_data_char_init_value = 0;
  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               this->local_ezble_gatt_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE,
                                               0x00,
                                               0x00,
                                               ezble_data_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               this->max_ble_transfer_size,            // max length
                                               sizeof(ezble_data_char_init_value),     // initial value length
                                               &ezble_data_char_init_value,            // initial value
                                               &this->local_ezble_data_gatt_characteristic_handle);
  app_assert_status(sc);

  // Start the ezBLE service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, this->local_ezble_gatt_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);

  this->gattdb_initialized = true;
}

bool ezBLEclass::find_complete_local_name_in_advertisement(sl_bt_evt_scanner_legacy_advertisement_report_t* response, const char* name)
{
  int i = 0;
  bool found = false;

  // Go through the response data
  while (i < (response->data.len - 1)) {
    uint8_t advertisement_length = response->data.data[i];
    uint8_t advertisement_type = response->data.data[i + 1];

    // Type 0x09 = Complete Local Name, 0x08 Shortened Name
    // If the field type matches the Complete Local Name
    if (advertisement_type == 0x09) {
      // Check if device name matches
      if (memcmp(response->data.data + i + 2, name, strlen(name)) == 0) {
        found = true;
        break;
      }
    }
    // Jump to next advertisement record
    i = i + advertisement_length + 1;
  }
  return found;
}

void ezBLEclass::handle_ble_event(sl_bt_msg_t* evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      this->ezble_log("BLE stack booted");
      this->handle_boot_event();
      break;

    case sl_bt_evt_connection_opened_id:
      this->ezble_log("BLE connection opened");
      this->handle_conn_open(evt);
      break;

    case sl_bt_evt_connection_closed_id:
      this->ezble_log("BLE connection closed");
      this->handle_conn_close();
      break;

    case sl_bt_evt_scanner_legacy_advertisement_report_id:
      this->ezble_log("BLE scan report received");
      this->process_scan_reports(evt);
      break;

    case sl_bt_evt_gatt_service_id:
      this->ezble_log("GATT service discovered");
      this->handle_gatt_service_discovery(evt);
      break;

    case sl_bt_evt_gatt_characteristic_id:
      this->ezble_log("GATT charactersitic discovered");
      this->handle_gatt_characteristic_discovery(evt);
      break;

    case sl_bt_evt_gatt_procedure_completed_id:
      this->ezble_log("GATT procedure completed");
      this->handle_gatt_procedure_complete(evt);
      break;

    case sl_bt_evt_gatt_server_attribute_value_id:
      this->ezble_log("GATT data received");
      this->handle_gatt_data_receive(evt);
      break;

    default:
      this->ezble_log("BLE event: 0x%x", SL_BT_MSG_ID(evt->header));
      break;
  }
}

void ezBLEclass::ezble_log(const char* fmt, ...)
{
  #if (EZBLE_ENABLE_DEBUG_LOGGING) == 1

  char message[this->printf_buffer_size];
  va_list args;
  va_start(args, fmt);
  vsnprintf(message, sizeof(message), fmt, args);
  va_end(args);
  Serial.print("[ezBLE] ");
  Serial.println(message);

  #else // EZBLE_ENABLE_DEBUG_LOGGING

  (void)fmt;

  #endif // EZBLE_ENABLE_DEBUG_LOGGING
}

ezBLEclass ezBLE;

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t* evt)
{
  // Pass all the stack events to ezBLE
  ezBLE.handle_ble_event(evt);
}
