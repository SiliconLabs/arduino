/*
   Zigbee thermostat example

   The example shows how to create a thermostat with the Arduino Zigbee API.

   The example creates a Zigbee thermostat device and prints any user setting changes to the Serial terminal.
   The built-in button toggles the thermostat mode between OFF and HEAT.
   The built-in LED is also toggled based on the current HVAC action.
   The device has to be commissioned to a Zigbee network first.
   Open your Zigbee coordinator (e.g. Home Assistant with ZHA) and put it in pairing mode.

   Compatible boards:
   - Arduino Nano Matter
   - Silicon Labs xG24 Explorer Kit
   - SparkFun Thing Plus Matter
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Zigbee.h>
#include <ZigbeeThermostat.h>

using ThermostatMode = ZigbeeThermostat::thermostat_mode_t;
using ThermostatAction = ZigbeeThermostat::thermostat_hvac_action_t;

// If there's no built-in button set a pin where a button is connected
#ifndef BTN_BUILTIN
#define BTN_BUILTIN D0
#endif

ZigbeeThermostat zigbee_thermostat;
const uint8_t button_pin = BTN_BUILTIN;

void setup()
{
  Serial.begin(115200);
  Serial.println("Zigbee thermostat");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  pinMode(button_pin, INPUT_PULLUP);

  // Hold the button during boot to factory reset (clear stored Zigbee network data)
  if (digitalRead(button_pin) == LOW) {
    Serial.println("Factory resetting...");
    Serial.println("Release the button to reboot");
    while (digitalRead(button_pin) == LOW) {
      delay(100);
    }
    Zigbee.factoryReset();
  }

  Zigbee.setVendorName("Arduino");
  Zigbee.setProductName("Zigbee Thermostat");
  Zigbee.setFirmwareVersion(0x00000420);
  Zigbee.begin();
  zigbee_thermostat.begin();

  if (!Zigbee.isPaired()) {
    Serial.println("Device is not commissioned");
    Serial.println("Waiting to join a Zigbee network...");
  }
  while (!Zigbee.isPaired()) {
    delay(200);
  }

  Serial.println("Connecting to Zigbee network...");
  while (!Zigbee.isConnectedToNetwork()) {
    delay(200);
  }
  Serial.print("Connected to Zigbee network; ");
  Serial.print("Channel: ");
  Serial.print(Zigbee.getChannel());
  Serial.print(" | PAN ID: 0x");
  Serial.println(Zigbee.getPanId(), HEX);

  // Set the local temperature to a fixed value
  zigbee_thermostat.set_local_temperature(21.5f);

  // Set the initial setpoint value
  zigbee_thermostat.set_heating_setpoint(23.0f);

  // Set the absolute minimum and maximum heating setpoints
  zigbee_thermostat.set_absolute_minimum_heating_setpoint(10.0f);
  zigbee_thermostat.set_absolute_maximum_heating_setpoint(35.0f);

  // Start up in an OFF state
  zigbee_thermostat.set_system_mode(ThermostatMode::OFF);
  zigbee_thermostat.set_hvac_action(ThermostatAction::IDLE);

  // Publish the full startup state so the coordinator refreshes any cached values
  zigbee_thermostat.send_attribute_report();
}

void loop()
{
  // Print the current setpoint if it changes
  static int16_t setpoint_prev = 0;
  int16_t setpoint = zigbee_thermostat.get_heating_setpoint_raw();
  if (setpoint_prev != setpoint) {
    Serial.printf("Thermostat setpoint: %.01f C\n", zigbee_thermostat.get_heating_setpoint());
    setpoint_prev = setpoint;
  }

  // Print the current setpoint limits if they change
  static bool min_setpoint_limit_initialized = false;
  static float min_setpoint_limit_prev = 0.0f;
  float min_setpoint_limit = zigbee_thermostat.get_minimum_heating_setpoint();
  if (!min_setpoint_limit_initialized || min_setpoint_limit_prev != min_setpoint_limit) {
    Serial.printf("Thermostat min setpoint limit: %.01f C\n", min_setpoint_limit);
    min_setpoint_limit_prev = min_setpoint_limit;
    min_setpoint_limit_initialized = true;
  }

  static bool max_setpoint_limit_initialized = false;
  static float max_setpoint_limit_prev = 0.0f;
  float max_setpoint_limit = zigbee_thermostat.get_maximum_heating_setpoint();
  if (!max_setpoint_limit_initialized || max_setpoint_limit_prev != max_setpoint_limit) {
    Serial.printf("Thermostat max setpoint limit: %.01f C\n", max_setpoint_limit);
    max_setpoint_limit_prev = max_setpoint_limit;
    max_setpoint_limit_initialized = true;
  }

  // Toggle the thermostat mode with the button
  static bool button_last_state = HIGH;
  static unsigned long button_last_press = 0;
  bool button_state = digitalRead(button_pin);
  if (button_last_state == HIGH && button_state == LOW && (millis() - button_last_press) > 200) {
    button_last_press = millis();
    ThermostatMode current_mode = zigbee_thermostat.get_system_mode();
    if (current_mode == ThermostatMode::HEAT) {
      zigbee_thermostat.set_system_mode(ThermostatMode::OFF);
    } else {
      zigbee_thermostat.set_system_mode(ThermostatMode::HEAT);
    }
  }
  button_last_state = button_state;

  // Print the current mode if it changes
  static ThermostatMode mode_prev = ThermostatMode::OFF;
  ThermostatMode mode = zigbee_thermostat.get_system_mode();
  if (mode_prev != mode) {
    if (mode == ThermostatMode::OFF) {
      Serial.println("Thermostat mode: OFF");
    }
    if (mode == ThermostatMode::HEAT) {
      Serial.println("Thermostat mode: HEAT");
    }
    mode_prev = mode;
  }

  // Turn heating on or off automatically based on the current mode and temperature
  ThermostatAction requested_action = ThermostatAction::IDLE;
  bool local_temp_below_setpoint = (zigbee_thermostat.get_local_temperature() < zigbee_thermostat.get_heating_setpoint());
  if (mode == ThermostatMode::HEAT && local_temp_below_setpoint) {
    requested_action = ThermostatAction::HEATING;
  }
  if (zigbee_thermostat.get_hvac_action() != requested_action) {
    zigbee_thermostat.set_hvac_action(requested_action);
  }

  // Print the current HVAC action if it changes
  // Toggle the LED on/off based on whether we're actively heating or not
  static ThermostatAction action_prev = ThermostatAction::IDLE;
  ThermostatAction action = zigbee_thermostat.get_hvac_action();
  if (action_prev != action) {
    if (action == ThermostatAction::IDLE) {
      Serial.println("Thermostat action: IDLE");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
    if (action == ThermostatAction::HEATING) {
      Serial.println("Thermostat action: HEATING");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    }
    action_prev = action;
  }

  delay(50);
}
