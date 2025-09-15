/*
   Matter multiple lightbulb example

   The example shows how to create multiple Matter lightbulb instances from a single device.

   The example lets users control the onboard LED and a WS2812 LED strip/ring - both acting as a Matter lightbulb.
   It's possible to switch the LEDs on/off, adjust the brightness and the color (only on the WS2812) as well.
   The device has to be commissioned to a Matter hub first.
   A WS2812 (NeoPixel) strip/ring/device needs to be connected to the board to the SPI SDO.

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
#include <ezWS2812.h>

MatterDimmableLightbulb matter_dimmable_bulb;
MatterColorLightbulb matter_color_bulb;
ezWS2812 ledRing(8);
void update_onboard_led(uint8_t brightness);
void update_led_ring();
void handle_dimmable_bulb();
void handle_color_bulb();

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_dimmable_bulb.begin();
  matter_color_bulb.begin();
  matter_color_bulb.boost_saturation(51); // Boost saturation by 20 percent

  // Set up the on-board LED for the dimmable bulb
  pinMode(LED_BUILTIN, OUTPUT);
  update_onboard_led(0);

  // Set up the LED ring for the color bulb
  ledRing.begin();
  noInterrupts();
  ledRing.set_all(0, 0, 0, 0);
  interrupts();

  Serial.println("Matter multiple lightbulbs");

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
  while (!matter_dimmable_bulb.is_online() || !matter_color_bulb.is_online()) {
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop()
{
  handle_dimmable_bulb();
  handle_color_bulb();
}

void handle_dimmable_bulb()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_dimmable_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - set the LED to the last brightness value
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led(matter_dimmable_bulb.get_brightness());
    Serial.printf("Dimmable bulb ON, brightness: %u%%\n", matter_dimmable_bulb.get_brightness_percent());
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    update_onboard_led(0);
    Serial.println("Dimmable bulb OFF");
  }

  // If the brightness changes update the LED brightness accordingly
  static uint8_t last_brightness = matter_dimmable_bulb.get_brightness();
  uint8_t curr_brightness = matter_dimmable_bulb.get_brightness();
  if (last_brightness != curr_brightness) {
    update_onboard_led(curr_brightness);
    last_brightness = curr_brightness;
    Serial.printf("Dimmable bulb brightness changed to %u%%\n", matter_dimmable_bulb.get_brightness_percent());
  }
}

void handle_color_bulb()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_color_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Color bulb ON");
    // Set the LEDs to the last received state
    update_led_ring();
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Color bulb OFF");
    noInterrupts();
    // Turn off the LEDs
    ledRing.set_all(0, 0, 0, 0);
    interrupts();
  }

  static uint8_t hue_prev = matter_color_bulb.get_hue();
  static uint8_t saturation_prev = matter_color_bulb.get_saturation_percent();
  static uint8_t brightness_prev = matter_color_bulb.get_brightness_percent();
  uint8_t hue_curr = matter_color_bulb.get_hue();
  uint8_t saturation_curr = matter_color_bulb.get_saturation_percent();
  uint8_t brightness_curr = matter_color_bulb.get_brightness_percent();

  // If either the hue, saturation or the brightness changes - update the LEDs to reflect the latest change
  if (hue_prev != hue_curr || saturation_prev != saturation_curr || brightness_prev != brightness_curr) {
    update_led_ring();
    hue_prev = hue_curr;
    saturation_prev = saturation_curr;
    brightness_prev = brightness_curr;
  }
}

void update_led_ring()
{
  uint8_t r, g, b;
  matter_color_bulb.get_rgb(&r, &g, &b);
  noInterrupts();
  ledRing.set_all(r, g, b);
  interrupts();
  Serial.printf("Setting color bulb to > r: %u  g: %u  b: %u\n", r, g, b);
}

void update_onboard_led(uint8_t brightness)
{
  // If our built-in LED is active LOW, we need to invert the brightness value
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_BUILTIN, 255 - brightness);
  } else {
    analogWrite(LED_BUILTIN, brightness);
  }
}
