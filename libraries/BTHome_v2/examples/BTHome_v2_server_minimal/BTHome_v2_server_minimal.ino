/*
   BTHome v2 minimal server example

   This example is a minimal implementation of a BTHome server device.
   Receives BTHome data from client devices.
   BTHome homepage: https://bthome.io/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   How to use the example:
   1) You need two BTHome compatible boards to test the application. One client board, and another server board.

   2) Flash the client board with a BTHome client example e.g. BTHome_v2_internal_temperature_sensor,
   and connect the client board to a serial monitor. The client example will print its
   Bluetooth address to the Serial monitor and the BTHome key at startup (press reset button on the board).

   3) Copy the client board's Bluetooth address and BTHome key - and update the mac_address_string and key_string in this sketch.
   Build and flash to the BTHome server board. Open the Serial monitor for the server board.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG27 DevKit
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - BGM220 Explorer Kit

   Author: Peter Jolankai (Silicon Labs)
   Special thanks to Tamas Jozsi and Nghia Bui
 */

#include <bthome_v2_server.h>

// Update with the Bluetooth address and BTHome key of your client device
char mac_address_string[] = "00112233FFEE";
char key_string[] = "11112222333344445555666677778888";

uint8_t mac_address[6] = {};
uint8_t key[16] = {};

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Silicon Labs BTHome v2 - minimal server example");

  char mac_address_string_to_compare[] = "00112233FFEE";
  if (memcmp(mac_address_string, mac_address_string_to_compare, 12) == 0) {
    Serial.println("Please update 'mac_address_string' and 'key_string' following the instructions at the top of the sketch!");
  }

  // Convert the Bluetooth address string to a byte array format
  for (uint8_t i = 0; i < 6; i++) {
    char octet[3];
    memset(octet, '\0', sizeof(octet));
    memcpy(octet, (uint8_t *)&mac_address_string[2 * i], 2);
    mac_address[i] = (uint8_t)strtol(octet, NULL, 16);
  }

  // Convert the key string to a 16 byte length format
  for (uint8_t i = 0; i < 16; i++) {
    char octet[3];
    memset(octet, '\0', sizeof(octet));
    memcpy(octet, (uint8_t *)&key_string[2 * i], 2);
    key[i] = (uint8_t)strtol(octet, NULL, 16);
  }

  bthome_v2_server_key_register(mac_address, key);
  interested_add(mac_address);
}

void loop()
{
  const uint32_t MEASUREMENT_INTERVAL_MS = 3000u;
  static uint32_t previousMillis = 0u;
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {
    previousMillis = currentMillis;

    bthome_v2_server_sensor_data_t object[5];
    uint8_t object_count = 0u;

    bthome_v2_server_sensor_data_read(mac_address, object, 5, &object_count, NULL);

    for (uint8_t j = 0; j < object_count; j++) {
      Serial.print("Object ID: ");
      Serial.print(object[j].object_id, HEX);
      Serial.print(" | Measurement data: ");
      Serial.println((float)object[j].data / object[j].factor);
    }
    Serial.println();
  }
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  bthome_v2_server_on_event(evt);

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready
    // Do not call any stack command before receiving this boot event
    case sl_bt_evt_system_boot_id:
      // start scanning for BTHome devices
      bthome_v2_server_start_scan_network();
      break;

    default:
      break;
  }
}

void bthome_v2_server_found_device_callback(uint8_t *mac, uint8_t *payload, uint8_t payload_length)
{
  bool encrypted;
  bool key_available;
  (void)payload;
  (void)payload_length;

  Serial.println("");
  Serial.print("BTHome client device found; Bluetooth address: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();

  bthome_v2_server_check_device(mac, &encrypted, &key_available);
  Serial.print("Encryption: ");
  Serial.print(encrypted ? "Yes" : "No");
  Serial.print(" | Encryption key available: ");
  Serial.println(key_available ? "Yes" : "No");
  Serial.println();
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
