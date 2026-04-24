/*
   Matter power source and temperature sensor example

   The example shows how to create a temperature sensor with a simple power source
   using the Arduino Matter API.

   The example creates a Matter temperature sensor and a Matter simple power source device.
   The temperature sensor publishes the current CPU temperature and the simple power source publishes
   the same value as battery percent.
   This example uses the MatterSimplePowerSource class, which only exposes battery percentage and charge level.
   You can alternatively use the MatterPowerSource class to expose all battery related attributes
   (see the matter_power_source example).

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
#include <MatterPowerSource.h>
using ChargeLevel = MatterSimplePowerSource::charge_level_t;

MatterTemperature matter_temp_sensor;
MatterSimplePowerSource matter_power_source;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_temp_sensor.begin();
  matter_power_source.begin();

  Serial.println("Matter power source and temperature sensor");

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
  while (!matter_temp_sensor.is_online() || !matter_power_source.is_online()) {
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop()
{
  float current_cpu_temp = getCPUTemp();
  uint8_t current_battery_percent = 0u;

  // Set the battery percent to the same value as the temperature
  if (current_cpu_temp >= 100.0f) {
    current_battery_percent = 100u;
  } else if (current_cpu_temp > 0.0f) {
    current_battery_percent = (uint8_t)current_cpu_temp;
  }

  // Set the battery charge level based on the battery percent
  if (current_battery_percent < 5u) {
    matter_power_source.set_battery_charge_level(ChargeLevel::CHARGE_LEVEL_CRITICAL);
  } else if (current_battery_percent < 20u) {
    matter_power_source.set_battery_charge_level(ChargeLevel::CHARGE_LEVEL_WARNING);
  } else {
    matter_power_source.set_battery_charge_level(ChargeLevel::CHARGE_LEVEL_OK);
  }

  // Publish the current CPU temperature and battery percent values
  matter_temp_sensor.set_measured_value_celsius(current_cpu_temp);
  matter_power_source.set_battery_percent(current_battery_percent);

  Serial.printf("Current CPU temperature: %.02f C\n", current_cpu_temp);
  Serial.printf("Current battery percent: %u%%\n", current_battery_percent);
  delay(2000);
}
