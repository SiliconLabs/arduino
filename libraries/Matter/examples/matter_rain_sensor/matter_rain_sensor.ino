/*
   Matter rain sensor example

   The example shows how to create a rain sensor with the Arduino Matter API.

   The example creates a Matter rain sensor device and publishes rain state information through it.
   The rain state changes on button presses.
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
#include <MatterRain.h>

MatterRain matter_rain_sensor;

void handle_button_press();
volatile bool button_pressed = false;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_rain_sensor.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);

  Serial.println("Matter rain sensor");

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
  while (!matter_rain_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  if (!button_pressed) {
    return;
  }
  button_pressed = false;
  // Toggle the state of the rain sensor
  // 'true' means raining - 'false' means no rain
  bool rain_sensor_state = matter_rain_sensor.get_measured_value();
  if (rain_sensor_state) {
    rain_sensor_state = false;
    // Publish the rain sensor value - you can also use 'matter_rain_sensor.set_measured_value(rain_sensor_state)'
    matter_rain_sensor = rain_sensor_state;
    Serial.println("Current rain sensor state: dry");
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  } else {
    rain_sensor_state = true;
    // Publish the rain sensor value - you can also use 'matter_rain_sensor = rain_sensor_state'
    matter_rain_sensor.set_measured_value(rain_sensor_state);
    Serial.println("Current rain sensor state: raining");
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
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
