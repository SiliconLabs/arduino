/*
   Matter switch example

   The example shows how to create a simple momentary switch with the Arduino Matter API.

   The example creates a Matter Switch device and publishes the on-board button's state through it.
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
#include <MatterSwitch.h>

MatterSwitch matter_switch;

void handle_button_press();
void handle_button_release();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_switch.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);
  attachInterrupt(BTN_BUILTIN, &handle_button_release, RISING);

  Serial.println("Matter switch");

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
  while (!matter_switch.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  // If the physical button state changes - update the switch's state
  static bool button_pressed_last = false;
  if (button_pressed != button_pressed_last) {
    button_pressed_last = button_pressed;
    matter_switch.set_state(button_pressed);
  }

  // Get the current state of the Matter switch
  static bool switch_last_state = false;
  bool switch_current_state = matter_switch.get_state();

  // If the current state is 'pressed' and the previous was 'not pressed' - switch pressed
  if (switch_current_state && !switch_last_state) {
    switch_last_state = switch_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Switch pressed");
  }

  // If the current state is 'not pressed' and the previous was 'pressed' - switch released
  if (!switch_current_state && switch_last_state) {
    switch_last_state = switch_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Switch released");
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

void handle_button_release()
{
  static uint32_t btn_last_press = 0;
  if (millis() < btn_last_press + 200) {
    return;
  }
  btn_last_press = millis();
  button_pressed = false;
}
