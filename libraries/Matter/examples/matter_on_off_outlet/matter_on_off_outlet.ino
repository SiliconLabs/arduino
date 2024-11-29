/*
   Matter On/Off Plug-in Unit / Outlet example

   The example shows how to create a smart outlet with the Arduino Matter API.

   The example displays the outlet's state with the onboard LED.
   The example also lets users to control the outlet locally with the built-in button.
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
#include <MatterOnOffPluginUnit.h>

MatterOnOffPluginUnit matter_outlet;

void handle_button_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_outlet.begin();

  // Set up the onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);

  Serial.println("Matter On/Off Plug-in Unit / Outlet");

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
  while (!matter_outlet.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool matter_outlet_last_state = false;
  bool matter_outlet_current_state = matter_outlet.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_outlet_current_state && !matter_outlet_last_state) {
    matter_outlet_last_state = matter_outlet_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Outlet ON");
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_outlet_current_state && matter_outlet_last_state) {
    matter_outlet_last_state = matter_outlet_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Outlet OFF");
  }

  // If the physical button's state changes - toggle the outlet's state
  if (button_pressed) {
    button_pressed = false;
    matter_outlet.toggle();
  }
}

void handle_button_press()
{
  static uint32_t btn_last_press = 0u;
  if (millis() < btn_last_press + 200u) {
    return;
  }
  btn_last_press = millis();
  button_pressed = true;
}
