/*
   Matter illuminance sensor example

   The example shows how to create a illuminance sensor with the Arduino Matter API.

   The example creates a Matter illuminance sensor device and publishes test data to it.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterIlluminance.h>

MatterIlluminance matter_illuminance_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_illuminance_sensor.begin();

  Serial.println("Matter illuminance sensor");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
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
  while (!matter_illuminance_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static float current_illuminance_lux = 0.0f;
  static uint32_t last_action = 0;
  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    // Publish the illuminance value - you can also use 'matter_illuminance_sensor = current_illuminance_lux'
    matter_illuminance_sensor.set_measured_value_lux(current_illuminance_lux);
    Serial.printf("Current light level: %.02f lx\n", current_illuminance_lux);
    // Increase the lux value
    current_illuminance_lux += 15.0f;
    if (current_illuminance_lux > 2000.0f) {
      current_illuminance_lux = 0.0f;
    }
  }
}
