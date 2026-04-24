/*
   Matter on/off lightbulb firmware for the Nano Matter

   The sketch was used in the CSA Matter Certification process of the Nano Matter and
   implements a fully featured Matter On/Off lightbulb.

   Features:
   - OnOff
   - Decommissioning
   - Identify
   - StartupOnOff with NVM
   - Local control with physical button

   Compatible boards:
   - Arduino Nano Matter

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <Matter.h>
#include <MatterLightbulb.h>

MatterLightbulb matter_bulb;

void handle_lightbulb_identify();
void handle_startuponoff_on_boot();
uint8_t nvm_read_startuponoff();
void nvm_write_startuponoff(uint8_t data);
uint8_t nvm_read_onoff();
void nvm_write_onoff(uint8_t data);
void decommission_handler();
void handle_button_short_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  // Disable the bridge endpoint
  Matter.disableBridgeEndpoint();
  // Begin as a non-bridged lightbulb device
  matter_bulb.begin(false);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_short_press, FALLING);

  Serial.println("Matter lightbulb for Matter Certification");

  handle_startuponoff_on_boot();

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
    delay(1);
    decommission_handler();
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_bulb.is_online()) {
    delay(1);
    decommission_handler();
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  // Handle identification commands
  handle_lightbulb_identify();

  // Handle the decommissioning process if requested
  decommission_handler();

  // If the physical button state changes - update the lightbulb's on/off state
  if (button_pressed) {
    button_pressed = false;
    matter_bulb.toggle();
  }

  static bool matter_lightbulb_last_state = !matter_bulb.get_onoff();
  bool matter_lightbulb_current_state = matter_bulb.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the LED
  if (matter_lightbulb_current_state && !matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Bulb ON");
    nvm_write_onoff(true);
  }

  // If the current state is OFF and the previous was ON - turn off the LED
  if (!matter_lightbulb_current_state && matter_lightbulb_last_state) {
    matter_lightbulb_last_state = matter_lightbulb_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Bulb OFF");
    nvm_write_onoff(false);
  }

  // Handle changes in StartupOnOff
  uint8_t startup_on_off = matter_bulb.get_startup_on_off();
  static uint8_t startup_on_off_prev = nvm_read_startuponoff();
  if (startup_on_off != startup_on_off_prev) {
    nvm_write_startuponoff(startup_on_off);
    Serial.printf("Startup onoff changed to: %u\n", startup_on_off);
    startup_on_off_prev = startup_on_off;
  }
}

void handle_lightbulb_identify()
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
    // Sync the LED to the actual state of the lightbulb after the identification
    if (matter_bulb.get_onoff()) {
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
      nvm_write_onoff(true);
    } else {
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
      nvm_write_onoff(false);
    }
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
}

uint8_t nvm_read_startuponoff()
{
  uint8_t data;
  nvm3_readData(nvm3_defaultHandle, 0x00, (void*)&data, 1);
  return data;
}

void nvm_write_startuponoff(uint8_t data)
{
  nvm3_writeData(nvm3_defaultHandle, 0x00, (void*)&data, 1);
}

uint8_t nvm_read_onoff()
{
  uint8_t data;
  nvm3_readData(nvm3_defaultHandle, 0x01, (void*)&data, 1);
  return data;
}

void nvm_write_onoff(uint8_t data)
{
  nvm3_writeData(nvm3_defaultHandle, 0x01, (void*)&data, 1);
}

void handle_startuponoff_on_boot()
{
  DeviceLightbulb::StartupOnOff_t startup_on_off = (DeviceLightbulb::StartupOnOff_t)nvm_read_startuponoff();
  Serial.printf("StartupOnOff NVM: %u\n", startup_on_off);
  matter_bulb.set_startup_on_off(startup_on_off);
  bool on_off_prev = nvm_read_onoff();
  Serial.printf("OnOff NVM: %u\n", on_off_prev);

  switch (startup_on_off) {
    case DeviceLightbulb::StartupOnOff_t::kOn:
      Serial.println("Setting bulb to ON after power-cycle");
      matter_bulb.set_onoff(true);
      break;

    case DeviceLightbulb::StartupOnOff_t::kOff:
      Serial.println("Setting bulb to OFF after power-cycle");
      matter_bulb.set_onoff(false);
      break;

    case DeviceLightbulb::StartupOnOff_t::kToggle:
      Serial.println("Toggling bulb after power-cycle");
      matter_bulb.set_onoff(!on_off_prev);
      break;

    case DeviceLightbulb::StartupOnOff_t::kNull:
      Serial.println("Setting bulb to its previous state after power-cycle");
      matter_bulb.set_onoff(on_off_prev);
      break;

    default:
      Serial.printf("Invalid StartupOnOff value in NVM! value='%u'\n", startup_on_off);
      break;
  }

  // Sync the LED to the startup state
  if (matter_bulb.get_onoff()) {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    nvm_write_onoff(true);
  } else {
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    nvm_write_onoff(false);
  }
}

void handle_button_short_press()
{
  static uint32_t btn_last_press = 0;
  if (millis() < btn_last_press + 200) {
    return;
  }
  btn_last_press = millis();
  button_pressed = true;
}

void decommission_handler()
{
  static const uint32_t decommissioning_hold_time_ms = 10000u;

  // If the button is not pressed or the device is not commissioned - return
  if (digitalRead(BTN_BUILTIN) != LOW || !Matter.isDeviceCommissioned()) {
    return;
  }

  uint32_t remaining_secs_prev = decommissioning_hold_time_ms / 1000u;

  // Store the time when the button was first pressed
  uint32_t start_time = millis();
  // While the button is being pressed
  while (digitalRead(BTN_BUILTIN) == LOW) {
    // Calculate the elapsed time
    uint32_t elapsed_time = millis() - start_time;
    // If the button has been pressed for less than 10 seconds, continue
    if (elapsed_time < decommissioning_hold_time_ms) {
      uint32_t remaining_secs = (decommissioning_hold_time_ms - elapsed_time) / 1000u;
      // If the seconds changed - and we're at least two seconds into the process - print the remaining time
      // The two second delay is there to avoid printing anything for short button presses
      if (remaining_secs != remaining_secs_prev && remaining_secs <= (decommissioning_hold_time_ms / 1000u) - 2u) {
        Serial.printf("%u seconds until decommissioning\n", remaining_secs);
        remaining_secs_prev = remaining_secs;
      }
      yield();
      continue;
    }

    // Blink the LED to indicate the start of the decommissioning process
    for (uint8_t i = 0u; i < 10u; i++) {
      digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
      delay(100);
    }

    Serial.println("Starting decommissioning process, device will reboot...");
    Serial.println();
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    // This function will not return
    // The device will restart once decommissioning has finished
    Matter.decommission();
  }

  if (remaining_secs_prev <= (decommissioning_hold_time_ms / 1000u) - 2u) {
    Serial.println("Decommissioning cancelled");
    button_pressed = false;
  }
}
