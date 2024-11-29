/*
   Matter door lock example

   The example demonstrates how to create a door lock device with the Arduino Matter API.

   The sketch creates a Matter door lock device and lets users locally or remotely control it.
   The built-in LED is turned on when the lock is in the locked state.
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
#include <MatterDoorLock.h>

#ifndef BTN_BUILTIN
#define BTN_BUILTIN PA0
#endif

MatterDoorLock matter_door_lock;

void handle_button_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_door_lock.begin();

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.println("Matter door lock");

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
  while (!matter_door_lock.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool locked_last = false;
  bool locked = matter_door_lock.is_locked();
  if (locked != locked_last) {
    locked_last = locked;
    if (locked) {
      Serial.println("Locked");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    } else {
      Serial.println("Unlocked");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
  }

  if (button_pressed) {
    button_pressed = false;
    matter_door_lock.toggle();
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
