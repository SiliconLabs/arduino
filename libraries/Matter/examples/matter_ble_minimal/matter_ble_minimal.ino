/*
   Matter BLE minimal example

   The example shows how to use the Silicon Labs Bluetooth Low Energy stack and the Matter library simultaneously.

   The example lets users control the onboard LED through Matter while also serving BLE connections.
   The device has to be commissioned to a Matter hub first.
   You can test the BLE functionality using the "Scan" tab in the Simplicity Connect app.

   Find out more on the Silicon Labs BLE API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   Get the Simplicity Connect app:
    - https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo
    - https://apps.apple.com/us/app/efr-connect-ble-mobile-app/id1030932759

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterBLE.h>
#include <MatterLightbulb.h>

static void ble_initialize_gatt_db();
static void ble_start_advertising();

static const uint8_t ble_advertised_name[] = "Arduino Matter BLE";
static uint16_t ble_gattdb_session_id;
static uint16_t ble_generic_access_service_handle;
static uint16_t ble_name_characteristic_handle;
static uint8_t ble_connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
static bool matter_commissioning = false;

MatterLightbulb matter_bulb;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_bulb.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.println("Matter BLE minimal example");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
    matter_commissioning = true;
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_bulb.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  // Wait until the commissioning BLE device disconnects before interacting with the BLE stack
  if (matter_commissioning) {
    while (ble_connection_handle != SL_BT_INVALID_CONNECTION_HANDLE) {
      yield();
    }
  }
  matter_commissioning = false;
  ble_initialize_gatt_db();
  ble_start_advertising();
  Serial.println("BLE advertisement started");
}

void loop()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Bulb ON");
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Bulb OFF");
  }
}

void matter_ble_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      Serial.begin(115200);
      Serial.println("BLE stack booted");
      break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      ble_connection_handle = evt->data.evt_connection_opened.connection;
      Serial.print("BLE connection opened; conn_handle=");
      Serial.println(ble_connection_handle);
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.print("BLE connection closed; conn_handle=");
      Serial.println(ble_connection_handle);
      ble_connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
      if (!matter_commissioning) {
        ble_start_advertising();
        Serial.println("BLE advertisement restarted");
      }
      break;

    // -------------------------------
    // Default event handler
    default:
      Serial.print("BLE event: 0x");
      Serial.println(SL_BT_MSG_ID(evt->header), HEX);
      break;
  }
}

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

  // Add the Complete Local Name to the advertisement data
  uint8_t adv_name_packet[sizeof(ble_advertised_name) + 1];
  adv_name_packet[0] = sizeof(ble_advertised_name);
  adv_name_packet[1] = 0x09;
  memcpy(adv_name_packet + 2, ble_advertised_name, sizeof(ble_advertised_name) - 1);
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0x09, sizeof(adv_name_packet), adv_name_packet);
  app_assert_status(sc);

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
  app_assert_status(sc);
}

static void ble_initialize_gatt_db()
{
  sl_status_t sc;

  // Disable the static GATT services used by the Matter stack
  // Use 'sl_bt_gatt_server_set_capabilities(static_matter_pairing_service, 0);' to keep the Matter pairing service
  sc = sl_bt_gatt_server_set_capabilities(static_service_gatt_none, 0);
  app_assert_status(sc);

  // Create a new GATT database
  sc = sl_bt_gattdb_new_session(&ble_gattdb_session_id);
  app_assert_status(sc);

  // Add the Generic Access service to the GATT DB
  const uint8_t generic_access_service_uuid[] = { 0x00, 0x18 };
  sc = sl_bt_gattdb_add_service(ble_gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(generic_access_service_uuid),
                                generic_access_service_uuid,
                                &ble_generic_access_service_handle);
  app_assert_status(sc);

  // Add the Device Name characteristic to the Generic Access service
  // The value of the Device Name characteristic will be advertised
  const sl_bt_uuid_16_t device_name_characteristic_uuid = { .data = { 0x00, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(ble_gattdb_session_id,
                                              ble_generic_access_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              device_name_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(ble_advertised_name) - 1,
                                              sizeof(ble_advertised_name) - 1,
                                              ble_advertised_name,
                                              &ble_name_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(ble_gattdb_session_id, ble_generic_access_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(ble_gattdb_session_id);
  app_assert_status(sc);
}
