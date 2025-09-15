/*
   Matter air purifier example

   The example shows how to create an air purifier device with the Arduino Matter API.

   The example lets users control the device through Matter and displays the values set by the user.
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
#include <MatterAirPurifier.h>

MatterAirPurifier matter_air_purifier;
using AirQuality = MatterAirQuality::AirQuality_t;

void handle_button_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_air_purifier.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);

  Serial.println("Matter Air Purifier");

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
  while (!matter_air_purifier.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  matter_air_purifier.set_air_quality(MatterAirQuality::AirQuality_t::FAIR);
  matter_air_purifier.set_fan_onoff(false);
  matter_air_purifier.set_fan_percent(80);
}

void loop()
{
  // Handle fan speed changes
  static uint8_t fan_last_speed = 0u;
  uint8_t fan_current_speed = matter_air_purifier.get_fan_percent();
  if (fan_current_speed != fan_last_speed) {
    fan_last_speed = fan_current_speed;
    Serial.print("Fan speed: ");
    Serial.print(fan_current_speed);
    Serial.println("%");
  }

  // Handle fan mode changes
  static bool fan_last_mode = false;
  bool fan_current_mode = matter_air_purifier.get_fan_onoff();
  if (fan_current_mode != fan_last_mode) {
    fan_last_mode = fan_current_mode;
    if (fan_current_mode) {
      Serial.println("Fan ON");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    } else {
      Serial.println("Fan OFF");
      digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    }
  }

  // Change the measured air quality on button press
  if (button_pressed) {
    button_pressed = false;
    AirQuality current_air_quality = matter_air_purifier.get_air_quality();
    // Increment the current air quality value
    current_air_quality = static_cast<AirQuality>(static_cast<uint8_t>(current_air_quality) + 1);
    // Handle wrap around
    if (current_air_quality == AirQuality::UNDEFINED_VALUE) {
      current_air_quality = AirQuality::UNKNOWN;
    }
    matter_air_purifier.set_air_quality(current_air_quality);
    switch (current_air_quality) {
      case AirQuality::UNKNOWN:
        Serial.println("Air Quality: unknown");
        break;
      case AirQuality::GOOD:
        Serial.println("Air Quality: good");
        break;
      case AirQuality::FAIR:
        Serial.println("Air Quality: fair");
        break;
      case AirQuality::MODERATE:
        Serial.println("Air Quality: moderate");
        break;
      case AirQuality::POOR:
        Serial.println("Air Quality: poor");
        break;
      case AirQuality::VERY_POOR:
        Serial.println("Air Quality: very poor");
        break;
      case AirQuality::EXTREMELY_POOR:
        Serial.println("Air Quality: extremely poor");
        break;
      default:
        Serial.println("Air Quality: invalid");
        break;
    }
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
