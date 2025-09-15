/*
   Matter color lightbulb example for the Nano Matter

   The example shows the basic usage of the Arduino Matter API.

   The example lets users control the onboard RGB LED on the Nano Matter as a Matter lightbulb.
   It's possible to switch the LED on/off, adjust the brightness and the color as well.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - xG24 Dev Kit

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterLightbulb.h>

#define LED_R LED_BUILTIN
#define LED_G LED_BUILTIN_1
#define LED_B LED_BUILTIN_2

MatterColorLightbulb matter_color_bulb;

void update_led_color();
void led_off();
void handle_button_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_color_bulb.begin();
  matter_color_bulb.boost_saturation(51); // Boost saturation by 20 percent

  // Set up the onboard button
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);

  // Turn the LED off
  led_off();

  Serial.println("Arduino Nano Matter - color lightbulb");

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
  // If the physical button state changes - update the lightbulb's on/off state
  if (button_pressed) {
    button_pressed = false;
    // Toggle the on/off state of the lightbulb
    matter_color_bulb.toggle();
  }

  // Get the current on/off state of the lightbulb
  static bool matter_lightbulb_last_state = false;
  bool matter_lightbulb_current_state = matter_color_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Bulb ON");
    // Set the LEDs to the last received state
    update_led_color();
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    Serial.println("Bulb OFF");
    led_off();
  }

  static uint8_t hue_prev = 0;
  static uint8_t saturation_prev = 0;
  static uint8_t brightness_prev = 0;
  uint8_t hue_curr = matter_color_bulb.get_hue();
  uint8_t saturation_curr = matter_color_bulb.get_saturation_percent();
  uint8_t brightness_curr = matter_color_bulb.get_brightness_percent();

  // If either the hue, saturation or the brightness changes - update the LED to reflect the latest change
  if (hue_prev != hue_curr || saturation_prev != saturation_curr || brightness_prev != brightness_curr) {
    update_led_color();
    hue_prev = hue_curr;
    saturation_prev = saturation_curr;
    brightness_prev = brightness_curr;
  }
}

// Updates the color of the RGB LED to match the Matter lightbulb's color
void update_led_color()
{
  if (!matter_color_bulb.get_onoff()) {
    return;
  }
  uint8_t r, g, b;
  matter_color_bulb.get_rgb(&r, &g, &b);
  // If our built-in LED is active LOW, we need to invert the brightness values
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_R, 255 - r);
    analogWrite(LED_G, 255 - g);
    analogWrite(LED_B, 255 - b);
  } else {
    analogWrite(LED_R, r);
    analogWrite(LED_G, g);
    analogWrite(LED_B, b);
  }
  Serial.printf("Setting bulb color to > r: %u  g: %u  b: %u\n", r, g, b);
}

// Turns the RGB LED off
void led_off()
{
  // If our built-in LED is active LOW, we need to invert the brightness values
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 255);
  } else {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
  }
}

void handle_button_press()
{
  static uint32_t btn_last_press = 0;
  if (millis() < btn_last_press + 200) {
    return;
  }
  btn_last_press = millis();
  button_pressed = true;
}
