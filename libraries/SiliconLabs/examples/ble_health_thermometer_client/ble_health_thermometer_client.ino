/*
   BLE health thermometer client example

   The example connects to another board running the 'BLE Health Thermometer' example and reads the temperature through BLE

   On startup the sketch will start a scanning for the other board running the 'ble_health_thermometer' example and
   advertising as "Thermometer Example". Once the other board is found, it establishes a connection,
   discovers it's services and characteristics, then subscribes to the temperature measurements.
   After the subscription the example starts receiving the temperature data from the other board periodically,
   and prints it to Serial.

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
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
}

void loop()
{
}

// Connection states
enum conn_state_t {
  ST_BOOT,
  ST_SCAN,
  ST_CONNECT,
  ST_SERVICE_DISCOVER,
  ST_CHAR_DISCOVER,
  ST_REQUEST_INDICATION,
  ST_RECEIVE_DATA
};

// IEEE 11073 float structure
typedef struct {
  uint8_t mantissa_l;
  uint8_t mantissa_m;
  int8_t mantissa_h;
  int8_t exponent;
} IEEE_11073_float;

static bool find_complete_local_name_in_advertisement(sl_bt_evt_scanner_legacy_advertisement_report_t *response);
static float translate_IEEE_11073_temperature_to_float(IEEE_11073_float const *IEEE_11073_value);

const uint8_t thermometer_service_uuid[] = { 0x09, 0x18 };
const sl_bt_uuid_16_t temp_measurement_characteristic_uuid = { .data = { 0x1C, 0x2A } };
const uint8_t advertised_name[] = "Thermometer Example";

uint32_t thermometer_service_handle = __UINT32_MAX__;
uint16_t temp_measurement_char_handle = __UINT16_MAX__;
conn_state_t connection_state = ST_BOOT;

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // This event is received when the BLE device has successfully booted
    case sl_bt_evt_system_boot_id:
      // Print a welcome message
      Serial.println();
      Serial.println("Silicon Labs BLE health thermometer client example");
      Serial.println("BLE stack booted");
      // Start scanning for other BLE devices
      sc = sl_bt_scanner_set_parameters(sl_bt_scanner_scan_mode_active, 16, 16);
      app_assert_status(sc);
      sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                               sl_bt_scanner_discover_generic);
      app_assert_status(sc);
      Serial.println("Started scanning...");
      connection_state = ST_SCAN;
      break;

    // This event is received when we scan the advertisement of another BLE device
    case sl_bt_evt_scanner_legacy_advertisement_report_id:
      Serial.println("BLE scan report received");
      // If we find the other devices's name
      if (find_complete_local_name_in_advertisement(&(evt->data.evt_scanner_legacy_advertisement_report))) {
        Serial.println("Target device found");
        // Stop scanning
        sc = sl_bt_scanner_stop();
        app_assert_status(sc);
        // Connect to the device
        sc = sl_bt_connection_open(evt->data.evt_scanner_legacy_advertisement_report.address,
                                   evt->data.evt_scanner_legacy_advertisement_report.address_type,
                                   sl_bt_gap_phy_1m,
                                   NULL);
        app_assert_status(sc);
        connection_state = ST_CONNECT;
      }
      break;

    // This event is received when a BLE connection has been opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("Connection opened");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
      // Discover Health Thermometer service on the connected device
      sc = sl_bt_gatt_discover_primary_services_by_uuid(evt->data.evt_connection_opened.connection,
                                                        sizeof(thermometer_service_uuid),
                                                        thermometer_service_uuid);
      app_assert_status(sc);
      connection_state = ST_SERVICE_DISCOVER;
      break;

    // This event is received when a BLE connection has been closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("Connection closed");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
      // Restart scanning
      sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                               sl_bt_scanner_discover_generic);
      app_assert_status(sc);
      Serial.println("Restarted scanning...");
      connection_state = ST_SCAN;
      break;

    // This event is generated when a new service is discovered
    case sl_bt_evt_gatt_service_id:
      Serial.println("GATT service discovered");
      // Store the handle of the discovered Thermometer Service
      thermometer_service_handle = evt->data.evt_gatt_service.service;
      break;

    // This event is generated when a new characteristic is discovered
    case sl_bt_evt_gatt_characteristic_id:
      Serial.println("GATT charactersitic discovered");
      // Store the handle of the discovered Temperature Measurement characteristic
      temp_measurement_char_handle = evt->data.evt_gatt_characteristic.characteristic;
      break;

    // This event is received when a GATT procedure completes
    case sl_bt_evt_gatt_procedure_completed_id:
      Serial.println("GATT procedure completed");

      if (connection_state == ST_SERVICE_DISCOVER) {
        Serial.println("GATT service discovery finished");
        // Discover thermometer characteristic on the connected device
        sc = sl_bt_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,
                                                         thermometer_service_handle,
                                                         sizeof(temp_measurement_characteristic_uuid.data),
                                                         temp_measurement_characteristic_uuid.data);
        app_assert_status(sc);
        connection_state = ST_CHAR_DISCOVER;
        break;
      }

      if (connection_state == ST_CHAR_DISCOVER) {
        Serial.println("GATT characteristic discovery finished");
        // Enable temperature measurement indications
        sc = sl_bt_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
                                                        temp_measurement_char_handle,
                                                        sl_bt_gatt_indication);
        app_assert_status(sc);
        connection_state = ST_REQUEST_INDICATION;
        break;
      }

      if (connection_state == ST_REQUEST_INDICATION) {
        Serial.println("Temperature measurement indications enabled");
        connection_state = ST_RECEIVE_DATA;
      }
      break;

    // This event is received when a characteristic value was received (like an indication)
    case sl_bt_evt_gatt_characteristic_value_id:
    {
      Serial.println("GATT data received");
      // Get the received data from the event
      uint8_t* char_value = &(evt->data.evt_gatt_characteristic_value.value.data[0]);
      // Convert it back to float
      float temperature = translate_IEEE_11073_temperature_to_float((IEEE_11073_float *)(char_value + 1));
      // Print to Serial
      Serial.print("Received temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      sc = sl_bt_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
      app_assert_status(sc);
    }
    break;

    // Default event handler
    default:
      Serial.print("BLE event: 0x");
      Serial.println(SL_BT_MSG_ID(evt->header), HEX);
      break;
  }
}

/**************************************************************************//**
 * Finds a configured name in BLE advertisements
 *
 * @param[in] response BLE response event received from scanning
 *
 * @return true if found, false otherwise
 *****************************************************************************/
static bool find_complete_local_name_in_advertisement(sl_bt_evt_scanner_legacy_advertisement_report_t *response)
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
      if (memcmp(response->data.data + i + 2, advertised_name, strlen((const char*)advertised_name)) == 0) {
        found = true;
        break;
      }
    }
    // Jump to next advertisement record
    i = i + advertisement_length + 1;
  }
  return found;
}

/**************************************************************************//**
 * Translates a IEEE-11073 temperature value to float
 *
 * @param[in] IEEE_11073_value the IEEE 11073 float value to convert
 *
 * @return the converted value in float, NAN on failure
 *****************************************************************************/
static float translate_IEEE_11073_temperature_to_float(IEEE_11073_float const *IEEE_11073_value)
{
  int32_t mantissa = 0;
  uint8_t mantissa_l;
  uint8_t mantissa_m;
  int8_t mantissa_h;
  int8_t exponent;

  // Wrong Argument: NULL pointer is passed
  if (!IEEE_11073_value) {
    return NAN;
  }

  // Caching Fields
  mantissa_l = IEEE_11073_value->mantissa_l;
  mantissa_m = IEEE_11073_value->mantissa_m;
  mantissa_h = IEEE_11073_value->mantissa_h;
  exponent =  IEEE_11073_value->exponent;

  // IEEE-11073 Standard NaN Value Passed
  if ((mantissa_l == 0xFF) && (mantissa_m == 0xFF) && (mantissa_h == 0x7F) && (exponent == 0x00)) {
    return NAN;
  }

  // Converting a 24bit Signed Value to a 32bit Signed Value
  mantissa |= mantissa_h;
  mantissa <<= 8;
  mantissa |= mantissa_m;
  mantissa <<= 8;
  mantissa |= mantissa_l;
  mantissa <<= 8;
  mantissa >>= 8;

  return ((float)mantissa) * pow(10.0f, (float)exponent);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
