/*
   Matter lightbulb with identification support example

   The example shows how to create a simple on/off lightbulb with support for identification using the Arduino Matter API.

   The example lets users control the onboard LED through Matter.
   The lightbulb will also respond to identification requests by blinking the LED.
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
#include <MatterLightbulb.h>

MatterLightbulb matter_bulb;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_bulb.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.println("Matter lightbulb with identification");

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
  while (!matter_bulb.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool identification_in_progress_last = false;
  bool identification_in_progress = matter_bulb.get_identify_in_progress();
  // Handle the identification enable/disable transitions
  if (identification_in_progress && !identification_in_progress_last) {
    // Identification starts
    Serial.println("Identification started");
  } else if (!identification_in_progress && identification_in_progress_last) {
    // Identification ends
    // Turn off the LED once the identification ends
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Identification ended");
  }
  identification_in_progress_last = identification_in_progress;

  // Blink the LED while the identification is in progress
  if (identification_in_progress) {
    if (millis() % 1000 < 500) {
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    } else {
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    }
  }

  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Bulb ON");
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Bulb OFF");
  }
}
