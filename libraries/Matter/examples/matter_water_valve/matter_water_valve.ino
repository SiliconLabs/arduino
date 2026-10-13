/*
   Matter water valve example

   The example shows how to create a water valve device with the Arduino Matter API.

   The example creates a Matter water valve device that can be opened (optionally for a set
   duration) or closed from a Matter hub, and toggles the builtin LED based on its current state.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Ludovic BOUÉ
 */
#include <Matter.h>
#include <MatterWaterValve.h>

MatterWaterValve matter_water_valve;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Matter.set_product_name("Matter water valve");
  Matter.begin();
  matter_water_valve.begin();

  Serial.println("Matter water valve");

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
  while (!matter_water_valve.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  // Run down the countdown of a timed open operation, log the remaining time, and close the valve once it elapses
  static unsigned long last_tick_ms = 0;
  if ((matter_water_valve.get_remaining_duration() > 0) && (millis() - last_tick_ms >= 1000)) {
    last_tick_ms = millis();
    uint32_t remaining_duration = matter_water_valve.get_remaining_duration() - 1;
    matter_water_valve.set_remaining_duration(remaining_duration);
    Serial.printf("Water valve remaining duration: %lu s\n", (unsigned long)remaining_duration);
  }

  // Log the default open duration whenever it's changed remotely, e.g. from a Matter hub
  static uint32_t default_open_duration_prev = matter_water_valve.get_default_open_duration();
  uint32_t default_open_duration = matter_water_valve.get_default_open_duration();
  if (default_open_duration_prev != default_open_duration) {
    Serial.printf("Water valve default open duration changed: %lu s\n", (unsigned long)default_open_duration);
    default_open_duration_prev = default_open_duration;
  }

  // Toggle the LED on/off based on the current valve state
  static MatterWaterValve::valve_state_t state_prev = MatterWaterValve::valve_state_t::VALVE_STATE_CLOSED;
  MatterWaterValve::valve_state_t state = matter_water_valve.get_current_state();
  if (state_prev != state) {
    if (state == MatterWaterValve::valve_state_t::VALVE_STATE_OPEN) {
      Serial.println("Water valve state: OPEN");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    } else {
      Serial.println("Water valve state: CLOSED");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
    state_prev = state;
  }
}
