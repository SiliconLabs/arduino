/*
   Matter sleep mode temperature sensor example

   The example shows how to create a sleep mode temperature sensor with the Arduino Matter API.

   The example creates a Matter temperature sensor device and publishes the current CPU temperature through it.
   It goes to sleep mode for 10 minutes.
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
#include <MatterTemperature.h>
#include "ArduinoLowPower.h"

MatterTemperature matter_temp_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_temp_sensor.begin();

  Serial.println("Matter temperature sensor");

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
  while (!matter_temp_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  // Delay to operate after sleep mode exit
  delay(1000);
  float current_cpu_temp = getCPUTemp();
  matter_temp_sensor.set_measured_value_celsius(current_cpu_temp);
  Serial.printf("Current CPU temperature: %.02f C\n", current_cpu_temp);
  delay(2000);
  // Going to sleep mode for 10 minutes (600 s = 600.000 ms)
  LowPower.sleep(600000);
}
