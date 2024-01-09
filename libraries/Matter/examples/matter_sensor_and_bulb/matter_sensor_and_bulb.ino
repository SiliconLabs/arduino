/*
   Matter sensor and lightbulb example

   The example shows how to create a sensor and a dimmable lightbulb with the Arduino Matter API.

   The example lets users control the onboard LED through Matter.
   It's possible to switch the LED on/off and adjust the brightness as well.
   It also creates a temperature sensor device and published the CPU temperature to it.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterLightbulb.h>
#include <MatterTemperature.h>

MatterDimmableLightbulb matter_dimmable_bulb;
MatterTemperature matter_temp_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_dimmable_bulb.begin();
  matter_temp_sensor.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(LED_BUILTIN, 0);

  Serial.println("Matter temperature sensor and lightbulb");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  if (!Matter.isDeviceConnected()) {
    Serial.println("Waiting for network connection...");
  }
  while (!Matter.isDeviceConnected()) {
    delay(200);
  }
  Serial.println("Device connected");
}

void loop()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_dimmable_bulb.get_onoff();

  // If the bulb's current state is ON and the previous was OFF - set the LED to the last brightness value
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    analogWrite(LED_BUILTIN, matter_dimmable_bulb.get_brightness());
    Serial.printf("Bulb ON, brightness: %u%%\n", matter_dimmable_bulb.get_brightness_percent());
  }

  // If bulb's the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    analogWrite(LED_BUILTIN, 0);
    Serial.printf("Bulb OFF\n");
  }

  // If the bulb's brightness changes update the LED brightness accordingly
  static uint8_t last_brightness = matter_dimmable_bulb.get_brightness();
  uint8_t curr_brightness = matter_dimmable_bulb.get_brightness();
  if (last_brightness != curr_brightness) {
    analogWrite(LED_BUILTIN, curr_brightness);
    last_brightness = curr_brightness;
    Serial.printf("Bulb brightness changed to %u%%\n", matter_dimmable_bulb.get_brightness_percent());
  }

  // Handle temperature measurement
  static uint32_t last_action_temp = 0;
  // Wait 10 seconds
  if ((last_action_temp + 10000) < millis()) {
    last_action_temp = millis();
    float current_cpu_temp = getCpuTemp();
    // Publish the temperature value - you can also use 'matter_temp_sensor = current_cpu_temp'
    matter_temp_sensor.set_measured_value_celsius(current_cpu_temp);
    Serial.printf("Current CPU temperature: %.02f C\n", current_cpu_temp);
  }
}
