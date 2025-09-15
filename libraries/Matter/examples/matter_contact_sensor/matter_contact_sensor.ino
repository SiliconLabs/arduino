/*
   Matter contact sensor example

   The example shows how to create a simple contact sensor with the Arduino Matter API.

   The example creates a Matter Contact Sensor device and publishes the on-board button's state through it.
   When the button is pressed the contact sensor will indicate contact.
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
#include <MatterContact.h>

MatterContact matter_contact_sensor;

void handle_button_press();
void handle_button_release();

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_contact_sensor.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Set up the onboard button
  #ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
  #endif
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, &handle_button_press, FALLING);
  attachInterrupt(BTN_BUILTIN, &handle_button_release, RISING);

  Serial.println("Matter contact sensor");

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
  while (!matter_contact_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static bool sensor_last_state = false;
  bool sensor_current_state = matter_contact_sensor.get_state();

  // If the current state is ON and the previous was OFF - contact detected
  if (sensor_current_state && !sensor_last_state) {
    sensor_last_state = sensor_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    Serial.println("Contact detected");
  }

  // If the current state is OFF and the previous was ON - contact lost
  if (!sensor_current_state && sensor_last_state) {
    sensor_last_state = sensor_current_state;
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    Serial.println("Contact lost");
  }
}

void handle_button_press()
{
  static uint32_t btn_last_press = 0;
  if (millis() < btn_last_press + 200) {
    return;
  }
  btn_last_press = millis();

  matter_contact_sensor = true;
}

void handle_button_release()
{
  static uint32_t btn_last_press = 0;
  if (millis() < btn_last_press + 200) {
    return;
  }
  btn_last_press = millis();

  matter_contact_sensor = false;
}
