/*
   Matter water heater example

   The example shows how to create a water heater device with the Arduino Matter API.

   The example creates a Matter water heater device and prints any user setting changes to the Serial terminal.
   The builtin LED is also toggled based on the current system mode.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Ludovic BOUE
 */
#include <Matter.h>
#include <MatterWaterHeater.h>

MatterWaterHeater matter_water_heater;

const float COLD_WATER_TEMP = 20.0f; // Assumed incoming cold water temperature in Celsius

// Estimates the tank's remaining hot water level from the current and target temperatures,
// assuming incoming cold water enters the tank at COLD_WATER_TEMP.
void update_tank_percentage();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Matter.set_product_name("Matter water heater");
  Matter.begin();
  matter_water_heater.begin();

  Serial.println("Matter water heater");

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
  while (!matter_water_heater.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  // Describe the physical tank - a single immersion element heating a 100 liter tank
  matter_water_heater.set_heater_types(MatterWaterHeater::heater_type_t::IMMERSION_ELEMENT_1);
  matter_water_heater.set_tank_volume(100);

  // Set the measured water temperature to a fixed value
  matter_water_heater.set_local_temperature(45.0f);

  // Set the initial target water temperature
  matter_water_heater.set_heating_setpoint(55.0f);

  update_tank_percentage();
}

void loop()
{
  // Print the current setpoint if it changes
  static int16_t setpoint_prev = 0;
  int16_t setpoint = matter_water_heater.get_heating_setpoint_raw();
  if (setpoint_prev != setpoint) {
    Serial.printf("Water heater setpoint: %.01f C\n", matter_water_heater.get_heating_setpoint());
    setpoint_prev = setpoint;
    update_tank_percentage();
  }

  // Print the current mode if it changes
  // Toggle the LED on/off based on whether we're heating or not
  static MatterWaterHeater::water_heater_mode_t mode_prev = MatterWaterHeater::water_heater_mode_t::OFF;
  MatterWaterHeater::water_heater_mode_t mode = matter_water_heater.get_system_mode();
  if (mode_prev != mode) {
    if (mode == MatterWaterHeater::water_heater_mode_t::OFF) {
      Serial.println("Water heater mode: OFF");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
    if (mode == MatterWaterHeater::water_heater_mode_t::HEAT) {
      Serial.println("Water heater mode: HEAT");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    }
    mode_prev = mode;
  }

  // Print the current boost state if it changes
  static MatterWaterHeater::boost_state_t boost_prev = MatterWaterHeater::boost_state_t::INACTIVE;
  MatterWaterHeater::boost_state_t boost = matter_water_heater.get_boost_state();
  if (boost_prev != boost) {
    Serial.printf("Water heater boost state: %s\n", boost == MatterWaterHeater::boost_state_t::ACTIVE ? "ACTIVE" : "INACTIVE");
    boost_prev = boost;
  }
}

// Estimates the tank's remaining hot water level from the current and target temperatures,
// assuming incoming cold water enters the tank at COLD_WATER_TEMP.
void update_tank_percentage()
{
  float current_temperature = matter_water_heater.get_local_temperature();
  float target_temperature = matter_water_heater.get_heating_setpoint();

  int tank_percentage = 0;
  if (target_temperature != COLD_WATER_TEMP) {
    tank_percentage = (int)(((current_temperature - COLD_WATER_TEMP) / (target_temperature - COLD_WATER_TEMP)) * 100.0f);
  }
  if (tank_percentage < 0) {
    tank_percentage = 0;
  }
  if (tank_percentage > 100) {
    tank_percentage = 100;
  }
  matter_water_heater.set_tank_percentage(tank_percentage);
}
