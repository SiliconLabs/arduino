/*
   Matter power source example

   The example shows how to create a Matter power source device with the Arduino Matter API.

   The example creates a Matter power source device and publishes test battery data through it.
   This example uses the MatterPowerSource class, which exposes all battery related attributes.
   You can alternatively use the MatterSimplePowerSource class to only expose basic data like
   battery percentage and charge level (see the matter_power_source_and_sensor example).

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
#include <MatterPowerSource.h>
using ChargeLevel = MatterSimplePowerSource::charge_level_t;

MatterPowerSource matter_power_source;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_power_source.begin();

  Serial.println("Matter power source");

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
  while (!matter_power_source.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  matter_power_source.set_description("Battery");
  matter_power_source.set_status(MatterPowerSource::ACTIVE);
  matter_power_source.set_order(0u);
  matter_power_source.set_bat_present(true);
}

void loop()
{
  static uint8_t current_percent_remaining = 100u;
  static uint32_t last_action = 0;

  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    bool battery_replacement_needed = false;
    ChargeLevel current_charge_level = ChargeLevel::CHARGE_LEVEL_OK;

    // Set the voltage and time remaining to plausible test values based on the battery percentage
    uint32_t current_battery_voltage = 3000u + ((uint32_t)current_percent_remaining * 12u);
    uint32_t current_battery_time_remaining = (uint32_t)current_percent_remaining * 120u;

    // Set the charge level values based on the battery percentage
    if (current_percent_remaining > 50u) {
      current_charge_level = ChargeLevel::CHARGE_LEVEL_OK;
    } else if (current_percent_remaining > 20u) {
      current_charge_level = ChargeLevel::CHARGE_LEVEL_WARNING;
    } else {
      current_charge_level = ChargeLevel::CHARGE_LEVEL_CRITICAL;
    }

    // Set the battery replacement needed flag
    if (current_percent_remaining <= 20u) {
      battery_replacement_needed = true;
    }

    // Publish the battery data
    matter_power_source.set_bat_charge_level(current_charge_level);
    matter_power_source.set_bat_percent_remaining(current_percent_remaining);
    matter_power_source.set_bat_voltage(current_battery_voltage);
    matter_power_source.set_bat_time_remaining(current_battery_time_remaining);
    matter_power_source.set_bat_replacement_needed(battery_replacement_needed);

    Serial.printf("Battery percent remaining: %u%%\n", current_percent_remaining);
    Serial.printf("Battery voltage: %lu mV\n", current_battery_voltage);
    Serial.printf("Battery replacement needed: %s\n", battery_replacement_needed ? "yes" : "no");

    if (current_percent_remaining > 0u) {
      current_percent_remaining -= 5u;
    } else {
      current_percent_remaining = 100u;
    }
  }
}
