/*
   Matter thermostat example

   The example shows how to create a thermostat device with the Arduino Matter API.

   The example creates a Matter thermostat device and print any user setting changes to the Serial terminal.
   The builtin LED is also toggled based on the current system mode.
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
#include <MatterThermostat.h>

MatterThermostat matter_thermostat;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Matter.begin();
  matter_thermostat.begin();

  Serial.println("Matter thermostat");

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
  while (!matter_thermostat.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  // Set the local temperature to a fixed value
  matter_thermostat.set_local_temperature(21.5f);

  // Set the initial setpoint value
  matter_thermostat.set_heating_setpoint(23.0f);
}

void loop()
{
  // Print the current setpoint if it changes
  static int16_t setpoint_prev = 0;
  int16_t setpoint = matter_thermostat.get_heating_setpoint_raw();
  if (setpoint_prev != setpoint) {
    Serial.printf("Thermostat setpoint: %.01f C\n", matter_thermostat.get_heating_setpoint());
    setpoint_prev = setpoint;
  }

  // Print the current mode if it changes
  // Toggle the LED on/off based on whether we're heating or not
  static MatterThermostat::thermostat_mode_t mode_prev = MatterThermostat::thermostat_mode_t::OFF;
  MatterThermostat::thermostat_mode_t mode = matter_thermostat.get_system_mode();
  if (mode_prev != mode) {
    if (mode == MatterThermostat::thermostat_mode_t::OFF) {
      Serial.println("Thermostat mode: OFF");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
    if (mode == MatterThermostat::thermostat_mode_t::HEAT) {
      Serial.println("Thermostat mode: HEAT");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    }
    mode_prev = mode;
  }
}
