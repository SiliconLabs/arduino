/*
   Matter multiple dimmable lightbulbs example

   The example shows how to create multiple instances of dimmable lightbulbs with the Arduino Matter API.

   The example lets users control the onboard LEDs through Matter.
   It's possible to switch the LEDs on/off and adjust the brightness as well.
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

// If the board doesn't have a second onboard LED - define a pin on which a second LED can be connected
#ifndef LED_BUILTIN_1
#define LED_BUILTIN_1 PA0
#endif

MatterDimmableLightbulb matter_dimmable_bulb_0;
MatterDimmableLightbulb matter_dimmable_bulb_1;

void update_onboard_led_0(uint8_t brightness);
void update_onboard_led_1(uint8_t brightness);
void handle_bulb_0();
void handle_bulb_1();

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_dimmable_bulb_0.begin();
  matter_dimmable_bulb_1.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  update_onboard_led_0(0);
  pinMode(LED_BUILTIN_1, OUTPUT);
  update_onboard_led_1(0);

  Serial.println("Matter multiple dimmable lightbulbs");

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
  while (!matter_dimmable_bulb_0.is_online() || !matter_dimmable_bulb_1.is_online()) {
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop()
{
  handle_bulb_0();
  handle_bulb_1();
}

void handle_bulb_0()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_dimmable_bulb_0.get_onoff();

  // If the current state is ON and the previous was OFF - set the LED to the last brightness value
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led_0(matter_dimmable_bulb_0.get_brightness());
    Serial.printf("#0 - bulb ON, brightness: %u%%\n", matter_dimmable_bulb_0.get_brightness_percent());
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led_0(0);
    Serial.println("#0 - bulb OFF");
  }

  // If the brightness changes update the LED brightness accordingly
  static uint8_t last_brightness = matter_dimmable_bulb_0.get_brightness();
  uint8_t curr_brightness = matter_dimmable_bulb_0.get_brightness();
  if (last_brightness != curr_brightness) {
    update_onboard_led_0(curr_brightness);
    last_brightness = curr_brightness;
    Serial.printf("#0 - bulb brightness changed to %u%%\n", matter_dimmable_bulb_0.get_brightness_percent());
  }
}

void update_onboard_led_0(uint8_t brightness)
{
  // If our built-in LED is active LOW, we need to invert the brightness value
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_BUILTIN, 255 - brightness);
  } else {
    analogWrite(LED_BUILTIN, brightness);
  }
}

void handle_bulb_1()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_dimmable_bulb_1.get_onoff();

  // If the current state is ON and the previous was OFF - set the LED to the last brightness value
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led_1(matter_dimmable_bulb_1.get_brightness());
    Serial.printf("#1 - bulb ON, brightness: %u%%\n", matter_dimmable_bulb_1.get_brightness_percent());
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led_1(0);
    Serial.println("#1 - bulb OFF");
  }

  // If the brightness changes update the LED brightness accordingly
  static uint8_t last_brightness = matter_dimmable_bulb_1.get_brightness();
  uint8_t curr_brightness = matter_dimmable_bulb_1.get_brightness();
  if (last_brightness != curr_brightness) {
    update_onboard_led_1(curr_brightness);
    last_brightness = curr_brightness;
    Serial.printf("#1 - bulb brightness changed to %u%%\n", matter_dimmable_bulb_1.get_brightness_percent());
  }
}

void update_onboard_led_1(uint8_t brightness)
{
  // If our built-in LED_1 is active LOW, we need to invert the brightness value
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_BUILTIN_1, 255 - brightness);
  } else {
    analogWrite(LED_BUILTIN_1, brightness);
  }
}
