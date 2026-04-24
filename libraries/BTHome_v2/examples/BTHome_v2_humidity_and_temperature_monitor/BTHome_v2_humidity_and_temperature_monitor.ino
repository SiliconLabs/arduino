/*
   BTHome v2 humidity and temperature client example

   BTHome homepage: https://bthome.io/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   This example uses the on-board Si7021 sensor of the xG27 DevKit and transmits temperature values.
   This example works only with the Silicon Laboratories xG27 DevKit (brd2602a).
   You need to install an additional Arduino library for Silicon Labs Si7021 relative humidity & temperature sensor
    - Adafruit_Si7021 (https://github.com/adafruit/Adafruit_Si7021)

   Author: Peter Jolankai (Silicon Labs)
 */

#include <Adafruit_Si7021.h>
#include <bthome_v2.h>

Adafruit_Si7021 temp_humidity_sensor;

static const bool encryption = true;
static uint8_t device_name[] = "HumTemp";
static uint8_t device_key[] = "11112222333344445555666677778888";

void measure_sensors();
void print_BT_address();
void print_BTHome_key();

void setup()
{
  Serial.begin(115200);
  // Enable power for the Si7021
  pinMode(PC6, OUTPUT);
  digitalWrite(PC6, HIGH);
  delay(200);

  Serial.println("Silicon Labs BTHome v2 - Humidity and Temperature Monitor");

  // Initialize the Si7021 temperature and humidity sensor
  if ( !temp_humidity_sensor.begin()) {
    Serial.println("Temperature and humidity sensor initialization failed");
    while (1) {
      // Stop code if init failed
    }
  } else {
    Serial.println("Temperature and humidity sensor initialization successful");
  }

  bthome_v2_init(device_name, encryption, device_key, false);
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

    bthome_v2_reset_measurement();
    measure_sensors();
    bthome_v2_build_packet();
    bthome_v2_send_packet();
  }
}

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  bthome_v2_bt_on_event(evt);
}

void measure_sensors()
{
  float humidity =  temp_humidity_sensor.readHumidity();
  float temp = temp_humidity_sensor.readTemperature();
  bthome_v2_add_measurement_float(ID_HUMIDITY_PRECISE, humidity);
  bthome_v2_add_measurement_float(ID_TEMPERATURE, temp);

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C | ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
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
    Serial.print(device_key[i] - 48);
  }
  Serial.println(" ");
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
