/*
   Matter color lightbulb example

   The example shows the basic usage of the Arduino Matter API.

   The example lets users control a WS2812 LED strip/ring acting as a Matter lightbulb.
   It's possible to switch the LEDs on/off, adjust the brightness and the color as well.
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

MatterColorLightbulb matter_color_bulb;
ezWS2812 ledRing(8);
void update_leds();

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_color_bulb.begin();
  matter_color_bulb.boost_saturation(51); // Boost saturation by 20 percent

  ledRing.begin();
  noInterrupts();
  ledRing.set_all(0, 0, 0, 0);
  interrupts();

  Serial.println("Matter color lightbulb");

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
  while (!matter_color_bulb.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_color_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Bulb ON");
    // Set the LEDs to the last received state
    update_leds();
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Bulb OFF");
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
    update_leds();
    hue_prev = hue_curr;
    saturation_prev = saturation_curr;
    brightness_prev = brightness_curr;
  }
}

void update_leds()
{
  uint8_t r, g, b;
  matter_color_bulb.get_rgb(&r, &g, &b);
  noInterrupts();
  ledRing.set_all(r, g, b);
  interrupts();
  Serial.printf("Setting bulb color to > r: %u  g: %u  b: %u\n", r, g, b);
}
