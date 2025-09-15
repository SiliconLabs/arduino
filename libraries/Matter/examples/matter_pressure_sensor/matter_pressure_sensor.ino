/*
   Matter pressure sensor example

   The example shows how to create a pressure sensor with the Arduino Matter API.

   The example creates a Matter pressure sensor device and publishes test data to it.
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
#include <MatterPressure.h>

MatterPressure matter_pressure_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_pressure_sensor.begin();

  Serial.println("Matter air pressure sensor");

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
  while (!matter_pressure_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static float current_pressure_hpa = 970.0f;
  static uint32_t last_action = 0;
  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    // Publish the pressure value - you can also use 'matter_pressure_sensor = current_pressure_hpa'
    matter_pressure_sensor.set_measured_value(current_pressure_hpa);
    Serial.printf("Current pressure: %.01f hPa\n", current_pressure_hpa);
    // Increase the humidity value
    current_pressure_hpa += 5.0f;
    if (current_pressure_hpa > 1050.0f) {
      current_pressure_hpa = 970.0f;
    }
  }
}
