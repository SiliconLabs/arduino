/*
   BLE blinky example

   The example allows a remote BLE device to control the onboard LED and get state updates from the onboard button.

   On startup the sketch will start a BLE advertisement with the configured name, then
   it will accept any incoming connection. The device offers a GATT service and characteristic
   for controlling the onboard LED and reporting the state of an onboard button. The demo will work
   on boards without a built-in button however the button state reporting feature will be disabled.
   With the Simplicity Connect app you can test this functionality by going to the "Demo" tab and selecting "Blinky".

   Find out more on the API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   Get the Simplicity Connect app:
    - https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo
    - https://apps.apple.com/us/app/efr-connect-ble-mobile-app/id1030932759

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

bool btn_notification_enabled = false;
volatile bool btn_state_changed = false;
volatile uint8_t btn_state = LOW;
static void btn_state_change_callback();
static void send_button_state_notification();
static void set_led_on(bool state);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  set_led_on(false);
  Serial.begin(115200);
  Serial.println("Silicon Labs BLE blinky example");

  // If the board has a built-in button configure it for usage
  #ifdef BTN_BUILTIN
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, btn_state_change_callback, CHANGE);
  #else // BTN_BUILTIN
  // Avoid warning for unused function on boards without a button
  (void)btn_state_change_callback;
  #endif // BTN_BUILTIN
}

void loop()
{
  if (btn_state_changed) {
    btn_state_changed = false;
    send_button_state_notification();
  }
}

static void ble_initialize_gatt_db();
static void ble_start_advertising();

static const uint8_t advertised_name[] = "Blinky Example";
static uint16_t gattdb_session_id;
static uint16_t generic_access_service_handle;
static uint16_t name_characteristic_handle;
static uint16_t blinky_service_handle;
static uint16_t led_control_characteristic_handle;
static uint16_t btn_report_characteristic_handle;

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
    {
      Serial.begin(115200);
      Serial.println("BLE stack booted");

      // Initialize the application specific GATT table
      ble_initialize_gatt_db();

      // Start advertising
      ble_start_advertising();
      Serial.println("BLE advertisement started");
    }
    break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("BLE connection opened");
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("BLE connection closed");
      // Restart the advertisement
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
      break;

    // -------------------------------
    // This event indicates that the value of an attribute in the local GATT
    // database was changed by a remote GATT client
    case sl_bt_evt_gatt_server_attribute_value_id:
      // Check if the changed characteristic is the LED control
      if (led_control_characteristic_handle == evt->data.evt_gatt_server_attribute_value.attribute) {
        Serial.println("LED control characteristic data received");
        // Check the length of the received data
        if (evt->data.evt_gatt_server_attribute_value.value.len == 0) {
          break;
        }
        // Get the received byte
        uint8_t received_data = evt->data.evt_gatt_server_attribute_value.value.data[0];
        // Turn the LED on/off according to the received data
        if (received_data == 0x00) {
          set_led_on(false);
          Serial.println("LED off");
        } else if (received_data == 0x01) {
          set_led_on(true);
          Serial.println("LED on");
        }
      }
      break;

    // -------------------------------
    // This event is received when a GATT characteristic status changes
    case sl_bt_evt_gatt_server_characteristic_status_id:
      // If the 'Button report' characteristic has been changed
      if (evt->data.evt_gatt_server_characteristic_status.characteristic == btn_report_characteristic_handle) {
        // The client just enabled the notification - send notification of the current button state
        if (evt->data.evt_gatt_server_characteristic_status.client_config_flags & sl_bt_gatt_notification) {
          Serial.println("Button state change notification enabled");
          btn_notification_enabled = true;
          btn_state_change_callback();
        } else {
          Serial.println("Button state change notification disabled");
          btn_notification_enabled = false;
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
 * Called on button state change - stores the current button state and
 * sets a flag that a button state change occurred.
 * If the board doesn't have a button the function does nothing.
 *****************************************************************************/
static void btn_state_change_callback()
{
  // If the board has a built-in button
  #ifdef BTN_BUILTIN
  // The real button state is inverted - most boards have an active low button configuration
  btn_state = !digitalRead(BTN_BUILTIN);
  btn_state_changed = true;
  #endif // BTN_BUILTIN
}

/**************************************************************************//**
 * Sends a BLE notification the the client if notifications are enabled and
 * the board has a built-in button.
 *****************************************************************************/
static void send_button_state_notification()
{
  if (!btn_notification_enabled) {
    return;
  }
  sl_status_t sc = sl_bt_gatt_server_notify_all(btn_report_characteristic_handle,
                                                sizeof(btn_state),
                                                (const uint8_t*)&btn_state);
  if (sc == SL_STATUS_OK) {
    Serial.print("Notification sent, button state: ");
    Serial.println(btn_state);
  }
}

/**************************************************************************//**
 * Sets the built-in LED to the desired state accounting for the inverted LED
 * logic on select boards.
 *****************************************************************************/
static void set_led_on(bool state)
{
  if (state) {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  } else {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
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
                                              &name_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, generic_access_service_handle);
  app_assert_status(sc);

  // Add the Blinky service to the GATT DB
  // UUID: de8a5aac-a99b-c315-0c80-60d4cbb51224
  const uuid_128 blinky_service_uuid = {
    .data = { 0x24, 0x12, 0xb5, 0xcb, 0xd4, 0x60, 0x80, 0x0c, 0x15, 0xc3, 0x9b, 0xa9, 0xac, 0x5a, 0x8a, 0xde }
  };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(blinky_service_uuid),
                                blinky_service_uuid.data,
                                &blinky_service_handle);
  app_assert_status(sc);

  // Add the 'LED Control' characteristic to the Blinky service
  // UUID: 5b026510-4088-c297-46d8-be6c736a087a
  const uuid_128 led_control_characteristic_uuid = {
    .data = { 0x7a, 0x08, 0x6a, 0x73, 0x6c, 0xbe, 0xd8, 0x46, 0x97, 0xc2, 0x88, 0x40, 0x10, 0x65, 0x02, 0x5b }
  };
  uint8_t led_char_init_value = 0;
  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               blinky_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE,
                                               0x00,
                                               0x00,
                                               led_control_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               1,                               // max length
                                               sizeof(led_char_init_value),     // initial value length
                                               &led_char_init_value,            // initial value
                                               &led_control_characteristic_handle);

  // Add the 'Button report' characteristic to the Blinky service
  // UUID: 61a885a4-41c3-60d0-9a53-6d652a70d29c
  const uuid_128 btn_report_characteristic_uuid = {
    .data = { 0x9c, 0xd2, 0x70, 0x2a, 0x65, 0x6d, 0x53, 0x9a, 0xd0, 0x60, 0xc3, 0x41, 0xa4, 0x85, 0xa8, 0x61 }
  };
  uint8_t btn_char_init_value = 0;
  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               blinky_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                               0x00,
                                               0x00,
                                               btn_report_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               1,                               // max length
                                               sizeof(btn_char_init_value),     // initial value length
                                               &btn_char_init_value,            // initial value
                                               &btn_report_characteristic_handle);

  // Start the Blinky service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, blinky_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
