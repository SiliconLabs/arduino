/*
   BLE Serial Port Profile (SPP) example

   This example implements the Serial Port Profile (SPP) over BLE.
   It prints the data received on the Serial port to the connected BLE device
   and sends the data received from the BLE device to the Serial port.

   Go to 'https://siliconlabssoftware.github.io/web-bluetooth-spp-application/' to connect your computer
   to the device and send/receive data interactively.

   Find out more on the API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG27 DevKit
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - BGM220 Explorer Kit
   - Ezurio Lyra 24P 20dBm Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */

static const uint8_t advertised_name[] = "SPP Example";
static uint16_t gattdb_session_id;
static uint16_t generic_access_service_handle;
static uint16_t device_name_characteristic_handle;

typedef enum {
  STATE_DISCONNECTED = 0,
  STATE_ADVERTISING,
  STATE_CONNECTED,
  STATE_SPP_MODE
} spp_state_t;
spp_state_t spp_main_state = STATE_DISCONNECTED;

uint16_t spp_local_gatt_service_handle = 0u;
uint16_t spp_local_data_gatt_characteristic_handle = 0u;
uint8_t spp_connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
const uint8_t spp_default_max_packet_size = 20u;
uint8_t spp_max_packet_size = spp_default_max_packet_size;

// Bookkeeping struct for storing amount of sent/received data
typedef struct {
  uint32_t num_pack_sent;
  uint32_t num_bytes_sent;
  uint32_t num_pack_received;
  uint32_t num_bytes_received;
} spp_ts_counters;
spp_ts_counters spp_connection_stat_counter = {};

static void spp_write(uint8_t* data, size_t size);
static void spp_print_stats(spp_ts_counters *p_counters);
static void spp_reset_state();
static void ble_initialize_gatt_db();
static void ble_start_advertising();

// SPP GATT service UUID: 4880c12c-fdcb-4077-8920-a450d7f9b907
const uuid_128 spp_service_uuid = {
  .data = { 0x07, 0xb9, 0xf9, 0xd7, 0x50, 0xa4, 0x20, 0x89, 0x77, 0x40, 0xcb, 0xfd, 0x2c, 0xc1, 0x80, 0x48 }
};

// SPP data GATT characteristic UUID: fec26ec4-6d71-4442-9f81-55bc21d658d6
const uuid_128 spp_data_characteristic_uuid = {
  .data = { 0xd6, 0x58, 0xd6, 0x21, 0xbc, 0x55, 0x81, 0x9f, 0x42, 0x44, 0x71, 0x6d, 0xc4, 0x6e, 0xc2, 0xfe }
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Serial.println("Silicon Labs BLE Serial Port Profile example");
  Serial.println("Go to 'https://siliconlabssoftware.github.io/web-bluetooth-spp-application/' to connect your computer");
}

void loop()
{
  // Handle BLE transmission of the data received from Serial
  static uint8_t data_buf[256];
  static uint16_t data_buf_idx = 0u;
  while (Serial.available()) {
    // Buffer the received data
    char data = Serial.read();
    data_buf[data_buf_idx] = (uint8_t)data;
    data_buf_idx++;
    // If it's the end of a line - or we reach the max allowed packet size - or we would overrun our buffer - send the data out
    if (data == '\n' || data_buf_idx >= spp_max_packet_size || data_buf_idx >= sizeof(data_buf)) {
      spp_write(data_buf, data_buf_idx);
      data_buf_idx = 0u;
    }
  }
}

/**************************************************************************//**
 * Sends data over SPP
 *
 * @param[in] data Pointer to the data to be sent
 * @param[in] size Size of the data to be sent
 *****************************************************************************/
static void spp_write(uint8_t* data, size_t size)
{
  if (spp_main_state != STATE_SPP_MODE) {
    Serial.println("Write error - SPP not connected");
    return;
  }
  sl_status_t sc = sl_bt_gatt_server_send_notification(spp_connection_handle,
                                                       spp_local_data_gatt_characteristic_handle,
                                                       size,
                                                       data);
  if (sc == SL_STATUS_OK) {
    spp_connection_stat_counter.num_pack_sent++;
    spp_connection_stat_counter.num_bytes_sent += size;
  }
}

/**************************************************************************//**
 * Resets the state of the SPP service
 *****************************************************************************/
static void spp_reset_state()
{
  spp_connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
  spp_main_state = STATE_ADVERTISING;
  spp_max_packet_size = spp_default_max_packet_size;
  memset(&spp_connection_stat_counter, 0u, sizeof(spp_ts_counters));
}

/**************************************************************************//**
 * Prints statistics from the provided stat counter
 *
 * @param[in] p_counters Pointer to the stat counter structure
 *****************************************************************************/
static void spp_print_stats(spp_ts_counters *p_counters)
{
  Serial.printf("Bytes sent: %lu (%lu packets)\r\n",
                p_counters->num_bytes_sent,
                p_counters->num_pack_sent);
  Serial.printf("Bytes received: %lu (%lu packets)\r\n",
                p_counters->num_bytes_received,
                p_counters->num_pack_received);
}

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      Serial.begin(115200);
      Serial.println("BLE stack booted");
      // Initialize the application specific GATT DB
      ble_initialize_gatt_db();
      // Start advertising
      ble_start_advertising();
      Serial.println("BLE advertisement started");
      break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("BLE connection opened");
      spp_main_state = STATE_CONNECTED;
      spp_connection_handle = evt->data.evt_connection_opened.connection;
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("---------------------------");
      Serial.println("BLE connection closed");
      spp_print_stats(&spp_connection_stat_counter);
      spp_reset_state();
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
      break;

    case sl_bt_evt_gatt_mtu_exchanged_id:
      // Calculate maximum data per one notification / write-without-response,
      // this depends on the MTU. Up to ATT_MTU-3 bytes can be sent at once.
      spp_max_packet_size = evt->data.evt_gatt_mtu_exchanged.mtu - 3;
      Serial.printf("MTU exchanged, max packet size set to: %d bytes\n", evt->data.evt_gatt_mtu_exchanged.mtu);
      break;

    case sl_bt_evt_gatt_server_characteristic_status_id:
    {
      sl_bt_evt_gatt_server_characteristic_status_t char_status;
      char_status = evt->data.evt_gatt_server_characteristic_status;
      if (char_status.characteristic == spp_local_data_gatt_characteristic_handle) {
        if (char_status.status_flags == sl_bt_gatt_server_client_config) {
          // Characteristic client configuration (CCC) for the 'SPP data' characteristic has been changed
          if (char_status.client_config_flags == sl_bt_gatt_server_notification) {
            spp_main_state = STATE_SPP_MODE;
            Serial.println("Ready for SPP communication");
            Serial.println("---------------------------");
            digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
          } else {
            Serial.println("SPP communication stopped");
            spp_main_state = STATE_CONNECTED;
            digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
          }
        }
      }
    }
    break;

    case sl_bt_evt_gatt_server_attribute_value_id:
    {
      if (evt->data.evt_gatt_server_attribute_value.value.len != 0u) {
        for (uint8_t i = 0u; i < evt->data.evt_gatt_server_attribute_value.value.len; i++) {
          char received_char = (char)evt->data.evt_gatt_server_attribute_value.value.data[i];
          Serial.print(received_char);
        }
        spp_connection_stat_counter.num_pack_received++;
        spp_connection_stat_counter.num_bytes_received += evt->data.evt_gatt_server_attribute_value.value.len;
      }
    }
    break;

    // -------------------------------
    // Default event handler
    default:
      Serial.printf("BLE event: 0x%lx\n", SL_BT_MSG_ID(evt->header));
      break;
  }
}

/**************************************************************************//**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
static void ble_start_advertising()
{
  static uint8_t advertising_set_handle = 0xff;
  static bool init = true;
  sl_status_t sc;

  if (init) {
    // Create an advertising set
    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert_status(sc);

    // Set advertising interval to 100ms
    sc = sl_bt_advertiser_set_timing(
      advertising_set_handle,
      160,   // minimum advertisement interval (milliseconds * 1.6)
      160,   // maximum advertisement interval (milliseconds * 1.6)
      0,     // advertisement duration
      0);    // maximum number of advertisement events
    app_assert_status(sc);

    init = false;
  }

  // Generate data for advertising
  sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
  app_assert_status(sc);

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
  app_assert_status(sc);
}

/**************************************************************************//**
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
static void ble_initialize_gatt_db()
{
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
                                &generic_access_service_handle);
  app_assert_status(sc);

  // Add the Device Name characteristic to the Generic Access service
  // The value of the Device Name characteristic will be advertised
  const sl_bt_uuid_16_t device_name_characteristic_uuid = { .data = { 0x00, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              generic_access_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              device_name_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(advertised_name) - 1,
                                              sizeof(advertised_name) - 1,
                                              advertised_name,
                                              &device_name_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, generic_access_service_handle);
  app_assert_status(sc);

  // Add the SPP service to the GATT DB
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(spp_service_uuid),
                                spp_service_uuid.data,
                                &spp_local_gatt_service_handle);
  app_assert_status(sc);

  // Add the 'SPP data' characteristic to the SPP service
  uint8_t spp_data_char_init_value = 0u;
  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               spp_local_gatt_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE | SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                               0x00,
                                               0x00,
                                               spp_data_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               250,                                  // max length
                                               sizeof(spp_data_char_init_value),     // initial value length
                                               &spp_data_char_init_value,            // initial value
                                               &spp_local_data_gatt_characteristic_handle);
  app_assert_status(sc);

  // Start the SPP service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, spp_local_gatt_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
