/*
   Matter CO (carbon monoxide) sensor example

   The example shows how to create a CO sensor with the Arduino Matter API.

   The example creates a Matter CO sensor device and publishes CO concentration information through it.
   The device has to be commissioned to a Matter hub first.

   SAFETY DISCLAIMER
   ----------------
   This code and the associated library functions are provided
   for demonstration/evaluation purposes only.
   Do NOT use this code, the library, the referenced device, or any outputs from it
   in life-support, safety-critical, or emergency systems where failure
   could cause injury, death, or environmental/asset damage.
   ALWAYS rely on certified, properly maintained and calibrated instruments
   and approved safety controls. This example does not implement the redundancy,
   diagnostics, or certifications required for such use.
   Use only at your own risk.
   The authors of this software and library do not take any responsibility for
   any kind of injury or damage.
   ----------------

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <Matter.h>
#include <MatterConcentrationMeasurement.h>

MatterCO matter_co_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_co_sensor.begin();

  Serial.println("Matter CO sensor");

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
  while (!matter_co_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static float current_co = 0.0f;
  static uint32_t last_action = 0;
  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    // Publish the CO concentration value - you can also use 'matter_co_sensor.set_measured_value(current_co)'
    matter_co_sensor = current_co;
    Serial.printf("Current CO concentration: %.0f ppb\n", current_co);
    // Increase the CO concentration value
    current_co += 15.0f;
    if (current_co > 10000.0f) {
      current_co = 0.0f;
    }
  }
}
