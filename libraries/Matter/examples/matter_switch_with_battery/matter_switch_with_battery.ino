// SPDX-License-Identifier: MIT

#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#include <Matter.h>
#include <MatterSwitch.h>
#include <MatterPowerSource.h>

SFE_MAX1704X lipo(MAX1704X_MAX17048);
uint32_t battery_gauge_timeout = 900000; // 15min
void read_battery();

MatterSwitch matter_switch;
MatterPowerSource matter_power_source;
volatile bool button_pressed = false;
void handle_button_press();
void handle_button_release();

void setup() {
  Serial.begin(115200);
  Serial.println("Matter Switch with Battery");
  Wire.begin();
  Matter.begin();
  matter_switch.begin();
  matter_power_source.begin();

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);

  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);
  attachInterrupt(BTN_BUILTIN, &handle_button_release, RISING);

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }
  Serial.println("Matter device is commissioned");

  if (!Matter.isDeviceThreadConnected()) {
    Serial.println("Waiting for Thread network connection...");
  }
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  Serial.println("Connected to Thread network");

  if (!matter_switch.is_online()) {
    Serial.println("Waiting for Matter switch device discovery...");
  }
  while (!matter_switch.is_online()) {
    delay(200);
  }
  if (!matter_power_source.is_online()) {
    Serial.println("Waiting for Matter power source device discovery...");
  }
  while (!matter_power_source.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  if (!lipo.begin()) {
    Serial.println("No MAX17048 LiPo fuel gauge detected");
  } else {
    lipo.quickStart();
    lipo.setThreshold(20);
    read_battery();
  }
}

void loop() {
  static bool button_pressed_last = false;
  static bool switch_state_last = false;
  static uint32_t battery_gauge_last = 0;

  // If the physical button state changes - update the switch's state
  if (button_pressed != button_pressed_last) {
    button_pressed_last = button_pressed;
    matter_switch.set_state(button_pressed);
  }

  // Get the current state of the Matter switch
  bool switch_state_current = matter_switch.get_state();

  // If the current state is 'pressed' and the previous was 'not pressed' - switch pressed
  if (switch_state_current && !switch_state_last) {
    switch_state_last = switch_state_current;
    Serial.println("Button pressed");
    read_battery();
  }

  // If the current state is 'not pressed' and the previous was 'pressed' - switch released
  if (!switch_state_current && switch_state_last) {
    switch_state_last = switch_state_current;
    Serial.println("Button released");
  }

  uint32_t time = millis();
  if (time > battery_gauge_last + battery_gauge_timeout) {
    battery_gauge_last = time;
    read_battery();
  }
}

void read_battery() {
    double voltage = lipo.getVoltage();
    double soc = lipo.getSOC();
    bool alert = lipo.getAlert();

    Serial.printf("Battery Voltage: %4.2f V\n", voltage);
    Serial.printf("Battery Percent: %4.0f %%\n", soc);
    matter_power_source.set_bat_percent_remaining(soc);

    if (alert) {
      Serial.println("Battery Alert!");
    }
}

void handle_button_press() {
  static uint32_t button_press_last = 0;
  uint32_t time = millis();
  if (time < button_press_last + 200) {
    return;
  }

  button_press_last = time;
  button_pressed = true;
}

void handle_button_release() {
  static uint32_t button_press_last = 0;
  uint32_t time = millis();
  if (time < button_press_last + 200) {
    return;
  }

  button_press_last = time;
  button_pressed = false;
}
