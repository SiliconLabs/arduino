/*
   Matter switch with Binding cluster example

   The example shows how to create a Matter momentary switch that uses the
   standard Matter Binding cluster to control a bound light directly, without
   a hub relaying every button press (see MatterSwitch::beginWithBinding()).

   The device has to be commissioned to a Matter hub/controller first, same as
   the plain matter_switch example. Once commissioned:
     1. Commission this device AND a bindable target device onto the same
        Matter fabric - any Matter OnOff device works, e.g. the plain
        matter_lightbulb example; it needs no special code on its side, a
        Matter Binding only requires configuration on the initiating side
        (this switch's Binding cluster + MatterSwitch::beginWithBinding()).
     2. Create a binding from this switch's endpoint to the light's endpoint.
        This isn't done from Home Assistant's own UI - use the underlying
        Matter Server's dashboard (matterjs-server, the component behind HA's
        Matter integration) to add the binding, or another controller with
        binding support such as chip-tool.
     3. Make sure the light grants this switch's node the "Operate" ACL
        privilege on its OnOff cluster - most controllers set this up
        automatically as part of creating the binding.

   Once bound, pressing the on-board button toggles the bound light directly,
   switch-to-light, without going through the controller for that command.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Ludovic BOUÉ
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
  Matter.set_product_name("Matter switch");
  Matter.begin();
  matter_switch.beginWithBinding();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);
  attachInterrupt(BTN_BUILTIN, &handle_button_release, RISING);

  Serial.println("Matter switch with binding");

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
  Serial.println("Use your Matter controller to bind this switch to a light, then press the button to toggle it directly");
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
    Serial.println("Switch pressed - toggling bound light(s)");
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
