/*
   BTHome v2 internal (CPU) temperature sensor example

   BTHome homepage: https://bthome.io/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   This example uses the internal die temperature sensor of the board and transmits temperature values.
   Temperature values are sent to the BTHome Server.
   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG27 DevKit
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - BGM220 Explorer Kit

   Author: Peter Jolankai (Silicon Labs)
 */

#include <bthome_v2.h>

const bool encryption = true;
static float temperature = 0.0f;
static uint8_t dev_name[] = "Temp";
static uint8_t dev_key[] = "11112222333344445555666677778888";

void print_BT_address();
void print_BTHome_key();

void setup()
{
  Serial.begin(115200);
  Serial.println("Silicon Labs BTHome v2 - Internal (CPU) Temperature Monitor");
  bthome_v2_init(dev_name, encryption, dev_key, false);
  bthome_v2_add_measurement_float(ID_TEMPERATURE, (float)temperature);
  bthome_v2_send_packet();
  print_BT_address();
  if (encryption) {
    print_BTHome_key();
  }
}

void loop()
{
  const uint32_t MEASUREMENT_INTERVAL_MS = 5000u;
  static uint32_t previousMillis = 0u;
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {
    previousMillis = currentMillis;

    temperature = getCPUTemp();
    bthome_v2_reset_measurement();
    bthome_v2_add_measurement_float(ID_TEMPERATURE, temperature);
    bthome_v2_build_packet();

    Serial.print("CPU temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  bthome_v2_bt_on_event(evt);
}

void print_BT_address()
{
  bd_addr address;
  uint8_t address_type;

  sl_bt_system_get_identity_address(&address, &address_type);
  Serial.print("Bluetooth ");
  Serial.print(address_type ? "static random" : "public device");
  Serial.print(" address: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X",
                address.addr[5], address.addr[4], address.addr[3],
                address.addr[2], address.addr[1], address.addr[0]);
  Serial.println();
}

void print_BTHome_key()
{
  Serial.print("BTHome key: ");
  for (uint8_t i = 0; i < 32; i++) {
    Serial.print(dev_key[i] - 48);
  }
  Serial.println(" ");
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
