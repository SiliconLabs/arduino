/*
   BLE HID (Human Interface Device) keyboard example

   The example creates a simple HID keyboard that inputs a predefined text over BLE when a button is pressed.

   On startup, the sketch will start a BLE advertisement with the configured name, and then
   it will accept any incoming connection. When a device is connected and enables notification for the
   Report characteristic, the device will send the predefined text whenever button 0 on-board is pressed.
   You can test this functionality by using a smartphone or laptop. For example, connect your laptop to this HID keyboard,
   Open the text editor app, then press the button 0 on the board, the predefined text will be shown on the text editor.
   On disconnection the LED turns off and the BLE advertisement restarts.

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

   Note: on boards which don't have an on-board button an external button should be added to a GPIO
   pin. In this example the external button is connected to the pin A0.

   Author: Hai Nguyen (Silicon Labs)
 */

#define KEY_ARRAY_SIZE         25
#define MODIFIER_INDEX         0
#define DATA_INDEX             2
#define CAPSLOCK_KEY_OFF       0x00
#define CAPSLOCK_KEY_ON        0x02

#define HID_KB_DEVICE_NAME     "Silabs HID keyboard"

#define BTN_PRESSED            0
#define BTN_RELEASED           1

#ifndef BTN_BUILTIN
#define BTN_BUILTIN            PA0
#endif

static uint16_t gattdb_hid_report_char_handle;

static uint8_t input_report[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
volatile static uint8_t input_key;
volatile static uint8_t input_key_counter = 0;

volatile static const uint8_t reduced_key_array[] = {
  0x04,   /* a */
  0x05,   /* b */
  0x06,   /* c */
  0x07,   /* d */
  0x08,   /* e */
  0x09,   /* f */
  0x0a,   /* g */
  0x0b,   /* h */
  0x0c,   /* i */
  0x0d,   /* j */
  0x0e,   /* k */
  0x0f,   /* l */
  0x10,   /* m */
  0x11,   /* n */
  0x12,   /* o */
  0x13,   /* p */
  0x14,   /* q */
  0x15,   /* r */
  0x16,   /* s */
  0x17,   /* t */
  0x18,   /* u */
  0x19,   /* v */
  0x1a,   /* w */
  0x1b,   /* x */
  0x1c,   /* y */
  0x1d,   /* z */
};

static void ble_initialize_gatt_db();
static void ble_start_advertising();
static void on_button_change();

void setup()
{
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, on_button_change, CHANGE);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
}

void loop()
{
  ;
}

/**************************************************************************//**
 * Button change callback
 * A callback called in interrupt context whenever the button changes its state
 *****************************************************************************/
static void on_button_change()
{
  if (digitalRead(BTN_BUILTIN) == BTN_PRESSED) {
    input_key = reduced_key_array[input_key_counter];
  } else {
    input_key_counter = (KEY_ARRAY_SIZE == input_key_counter) ? 0 : (input_key_counter + 1);
    input_key = 0;
  }
  // Send an external signal to the BLE event handler loop to transmit the key press
  sl_bt_external_signal(1);
}

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  static bool notification_enabled = false;
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      Serial.begin(115200);
      Serial.println("Silicon Labs BLE HID Keyboard example");
      Serial.println("BLE stack booted");
      // Initialize the application specific GATT DB
      ble_initialize_gatt_db();
      // Configure security requirements and I/O capabilities
      sc = sl_bt_sm_configure(0, sl_bt_sm_io_capability_noinputnooutput);
      app_assert_status(sc);
      // Allow new bondings
      sc = sl_bt_sm_set_bondable_mode(1);
      app_assert_status(sc);
      // Start advertising
      ble_start_advertising();
      Serial.println("BLE advertisement started");
      break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("BLE connection opened");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
      sc = sl_bt_sm_increase_security(evt->data.evt_connection_opened.connection);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("BLE connection closed");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
      notification_enabled = false;
      // Restart advertising
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
      break;

    // -------------------------------
    // This event indicates that the bonding procedure is successfully completed
    case sl_bt_evt_sm_bonded_id:
      Serial.println("Bonding successful");
      break;

    // -------------------------------
    // This event is triggered if the pairing or bonding procedure fails
    case sl_bt_evt_sm_bonding_failed_id:
      Serial.println("Bonding failed");
      // Previous bond is broken, delete it and close connection, host must retry at least once
      sc = sl_bt_sm_delete_bondings();
      app_assert_status(sc);
      sc = sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event is received when a GATT characteristic status changes
    case sl_bt_evt_gatt_server_characteristic_status_id:
      if (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_hid_report_char_handle) {
        // client characteristic configuration changed by remote GATT client
        if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config) {
          if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == sl_bt_gatt_server_notification) {
            notification_enabled = true;
          } else {
            notification_enabled = false;
          }
        }
      }
      break;

    // -------------------------------
    // This event indicates that an external signal have been received
    case sl_bt_evt_system_external_signal_id:
      if (notification_enabled) {
        memset(input_report, 0, sizeof(input_report));

        input_report[MODIFIER_INDEX] = CAPSLOCK_KEY_OFF;
        input_report[DATA_INDEX] = input_key;

        sc = sl_bt_gatt_server_notify_all(gattdb_hid_report_char_handle,
                                          sizeof(input_report),
                                          input_report);
        app_assert_status(sc);
        if (input_key > 0) {
          Serial.printf("Key report sent; key='%c'", (input_key + 0x5D));
          Serial.println();
        }
      }
      break;
    // -------------------------------
    // Default event handler
    default:
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
  uint16_t gattdb_session_id;
  uint16_t service;
  uint16_t characteristic;
  uint16_t descriptor;

  // Create a new GATT database
  sc = sl_bt_gattdb_new_session(&gattdb_session_id);
  app_assert_status(sc);

  // Generic Access service
  uint8_t ga_uuid[] = { 0x00, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                2,
                                ga_uuid,
                                &service);
  app_assert_status(sc);

  // Device Name characteristic
  sl_bt_uuid_16_t device_name_uuid = {
    .data = { 0x00, 0x2A }
  };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              device_name_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              strlen(HID_KB_DEVICE_NAME),
                                              strlen(HID_KB_DEVICE_NAME),
                                              (uint8_t *)HID_KB_DEVICE_NAME,
                                              &characteristic);
  app_assert_status(sc);

  // Appearance characteristic
  sl_bt_uuid_16_t apperance_uuid = {
    .data = { 0x01, 0x2A }
  };
  const uint8_t appearance_value[] = { 0, 0 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              apperance_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              2,
                                              sizeof(appearance_value),
                                              appearance_value,
                                              &characteristic);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // Battery Service
  uint8_t battery_service_uuid[] = { 0x0F, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                0,
                                2,
                                battery_service_uuid,
                                &service);
  app_assert_status(sc);

  // Battery Level characteristic
  sl_bt_uuid_16_t battery_level_uuid = {
    .data = { 0x19, 0x2A }
  };
  const uint8_t battery_level_value[1] = { 100 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              battery_level_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              1,
                                              sizeof(battery_level_value),
                                              battery_level_value,
                                              &characteristic);
  app_assert_status(sc);

  // Characteristic Presentation Format descriptor
  sl_bt_uuid_16_t characteristic_presentation_format_uuid = {
    .data = { 0x04, 0x29 }
  };
  const uint8_t characteristic_presentation_format_value[] = { 0, 0, 0, 0, 0, 0, 0 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          0,
                                          characteristic_presentation_format_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          7,
                                          sizeof(characteristic_presentation_format_value),
                                          characteristic_presentation_format_value,
                                          &descriptor);
  app_assert_status(sc);

  // Start the Battery service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // HID service
  uint8_t hid_service_uuid[] = { 0x12, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                2,
                                hid_service_uuid,
                                &service);
  app_assert_status(sc);

  // Protocol Mode characteristic
  sl_bt_uuid_16_t protocol_mode_characteristic_uuid = {
    .data = { 0x4E, 0x2A }
  };
  const uint8_t protocol_mode_init_value[] = { 0x01 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ
                                              | SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE,
                                              0x00,
                                              0x00,
                                              protocol_mode_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              1,
                                              sizeof(protocol_mode_init_value),
                                              protocol_mode_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // Report characteristic
  sl_bt_uuid_16_t report_characteristic_uuid = {
    .data = { 0x4D, 0x2A }
  };
  const uint8_t report_init_value[] = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ
                                              | SL_BT_GATTDB_CHARACTERISTIC_WRITE
                                              | SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                              0,
                                              0,
                                              report_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              8,
                                              sizeof(report_init_value),
                                              report_init_value,
                                              &gattdb_hid_report_char_handle);
  app_assert_status(sc);

  // Report Reference descriptor
  sl_bt_uuid_16_t report_reference_descriptor_uuid = {
    .data = { 0x08, 0x29 }
  };
  const uint8_t report_reference_value[] = { 0x00, 0x01 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          gattdb_hid_report_char_handle,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          SL_BT_GATTDB_ENCRYPTED_READ,
                                          report_reference_descriptor_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          2,
                                          sizeof(report_reference_value),
                                          report_reference_value,
                                          &descriptor);
  app_assert_status(sc);

  // Report Map characteristic
  sl_bt_uuid_16_t report_map_characteristic_uuid = {
    .data = { 0x4B, 0x2A }
  };

  const uint8_t report_map_init_value[] = { 0x05, 0x01, // Usage Page (Generic Desktop)
                                            0x09, 0x06, // Usage (Keyboard)
                                            0xa1, 0x01, // Collection (Application)
                                            0x05, 0x07, // Usage Page (Keyboard/Keypad)
                                            0x19, 0xe0, // Usage Minimum (224)
                                            0x29, 0xe7, // Usage Maximum (231)
                                            0x15, 0x00, // Logical Minimum (0)
                                            0x25, 0x01, // Logical Maximum (1)
                                            0x75, 0x01, // Report Size (1)
                                            0x95, 0x08, // Report Count (8)
                                            0x81, 0x02, // Input Modifier byte
                                            0x95, 0x01, // Report Count (1)
                                            0x75, 0x08, // Report Size (8)
                                            0x81, 0x01, // Input Reserved byte
                                            0x95, 0x06, // Report Count (6)
                                            0x75, 0x08, // Report Size (8)
                                            0x15, 0x00, // Logical Minimum (0)
                                            0x25, 0x65, // Logical Maximum (101)
                                            0x05, 0x07, // Usage Page (Keyboard/Keypad)
                                            0x19, 0x00, // Usage minimum (0)
                                            0x29, 0x65, // Usage maximum (101)
                                            0x81, 0x00, // Input Data byte
                                            0xc0 };     // End Collection
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              SL_BT_GATTDB_ENCRYPTED_READ,
                                              0x00,
                                              report_map_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(report_map_init_value),
                                              sizeof(report_map_init_value),
                                              report_map_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // External Report Reference descriptor
  sl_bt_uuid_16_t external_report_reference_descriptor_uuid = {
    .data = { 0x07, 0x29 }
  };
  const uint8_t external_report_reference_value[] = { 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          SL_BT_GATTDB_ENCRYPTED_READ,
                                          external_report_reference_descriptor_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          2,
                                          sizeof(external_report_reference_value),
                                          external_report_reference_value,
                                          &descriptor);
  app_assert_status(sc);

  // HID Information characteristic
  sl_bt_uuid_16_t hid_info_characteristic_uuid = {
    .data = { 0x4A, 0x2A }
  };
  const uint8_t hid_info_init_value[] = { 0x01, 0x11, 0x00, 0x02 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              hid_info_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              4,
                                              sizeof(hid_info_init_value),
                                              hid_info_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // HID control characteristic
  sl_bt_uuid_16_t hid_control_characteristic_uuid = {
    .data = { 0x4C, 0x2A }
  };

  const uint8_t hid_control_init_value[] = { 0x00 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE,
                                              0x00,
                                              0x00,
                                              hid_control_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              1,
                                              sizeof(hid_control_init_value),
                                              hid_control_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // Start the HID Keyboard service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
