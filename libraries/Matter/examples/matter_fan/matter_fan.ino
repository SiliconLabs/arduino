/*
   Matter fan example

   The example shows how to create a fan device with the Arduino Matter API.

   The example lets users control the fan through Matter and displays the values set by the user.
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
#include <MatterFan.h>

MatterFan matter_fan;
using FanMode = DeviceFan::fan_mode_t;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_fan.begin();

  Serial.println("Matter fan");

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
  while (!matter_fan.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static uint8_t fan_last_speed = 0u;
  uint8_t fan_current_speed = matter_fan.get_percent();

  if (fan_current_speed != fan_last_speed) {
    fan_last_speed = fan_current_speed;
    Serial.print("Fan speed: ");
    Serial.print(fan_current_speed);
    Serial.println("%");
  }

  static FanMode fan_last_mode = FanMode::Off;
  FanMode fan_current_mode = matter_fan.get_mode();

  if (fan_current_mode != fan_last_mode) {
    fan_last_mode = fan_current_mode;
    switch (fan_current_mode) {
      case FanMode::Off:
        Serial.println("Fan mode: Off");
        matter_fan.set_percent(0);
        break;
      case FanMode::Low:
        Serial.println("Fan mode: Low");
        matter_fan.set_percent(20);
        break;
      case FanMode::Med:
        Serial.println("Fan mode: Medium");
        matter_fan.set_percent(50);
        break;
      case FanMode::High:
        Serial.println("Fan mode: High");
        matter_fan.set_percent(100);
        break;
      case FanMode::On:
        Serial.println("Fan mode: On");
        matter_fan.set_percent(fan_last_speed);
        break;
      case FanMode::Auto:
        Serial.println("Fan mode: Auto");
        matter_fan.set_percent(random(1, 100));
        break;
      case FanMode::Smart:
        Serial.println("Fan mode: Smart");
        matter_fan.set_percent(random(42, 69));
        break;
      default:
        Serial.println("Fan mode: Unknown");
        break;
    }
  }
}
