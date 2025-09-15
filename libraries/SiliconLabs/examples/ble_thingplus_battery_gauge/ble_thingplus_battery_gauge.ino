/*
   SparkFun ThingPlus Matter battery fuel gauge with BLE

   The example shows the usage of the Silicon Labs BLE API (BGAPI) in the Arduino ecosystem
   by presenting the onboard MAX17048's battery percentage measurements through BLE.

   On startup the sketch will start a BLE advertisement with the configured name, then
   it will accept any incoming connection. When a device is connected the built-in LED
   will be illuminated and the advertisement stops.
   The device will offer the standard BLE Battery service through which the actual
   battery state of the device can be read.
   On disconnection the LED turns off and the BLE advertisement restarts.

   Find out more on the API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   You can scan for the device and initiate connections with the Simplicity Connect app:
    - https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo
    - https://apps.apple.com/us/app/efr-connect-ble-mobile-app/id1030932759

   Compatible boards:
   - SparkFun Thing Plus MGM240P

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

static const uint8_t advertised_name[] = "Silicon Labs Arduino";
static uint16_t battery_service_handle;
static uint16_t battery_level_characteristic_handle;

SFE_MAX1704X lipo(MAX1704X_MAX17048);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Serial.println("SparkFun ThingPlus Matter battery fuel gauge BLE example");

  Wire.begin();
  if (!lipo.begin()) {
    Serial.println(F("MAX17048 not detected, please check wiring"));
    while (1) ;
  }
  uint8_t id = lipo.getID();
  Serial.print("Chip ID: 0x");
  Serial.println(id, HEX);
  lipo.quickStart();
  lipo.setThreshold(20);
}

void loop()
{
  static uint8_t battery_percent_prev = 0u;
  uint8_t battery_percent = (uint8_t)lipo.getSOC();
  // If the current battery percentage is different from the previous
  if (battery_percent != battery_percent_prev) {
    // Update the local GATT with the current battery percentage
    sl_bt_gatt_server_write_attribute_value(battery_level_characteristic_handle, 0, sizeof(battery_percent), &battery_percent);
    // Print the current percentage to serial
    Serial.print("Battery: ");
    Serial.print(battery_percent);
    Serial.println("%");
    battery_percent_prev = battery_percent;
  }
  delay(1000);
}

static void ble_initialize_gatt_db();
static void ble_start_advertising();

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
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("BLE connection closed");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
      // Restart advertising
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
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
  static uint16_t gattdb_session_id;
  static uint16_t generic_access_service_handle;
  static uint16_t generic_access_characteristic_handle;

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
                                              &generic_access_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, generic_access_service_handle);
  app_assert_status(sc);

  // Battery Service
  uint8_t battery_service_uuid[] = { 0x0F, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                0,
                                2,
                                battery_service_uuid,
                                &battery_service_handle);
  app_assert_status(sc);

  // Battery Level characteristic
  sl_bt_uuid_16_t battery_level_characteristic_uuid = { .data = { 0x19, 0x2A } };
  const uint8_t battery_level_value[1] = { 50 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              battery_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              battery_level_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              1,
                                              sizeof(battery_level_value),
                                              battery_level_value,
                                              &battery_level_characteristic_handle);
  app_assert_status(sc);

  // Start the Battery service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, battery_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
