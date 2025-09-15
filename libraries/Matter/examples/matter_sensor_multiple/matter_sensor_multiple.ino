/*
   Matter multiple sensor example

   The example shows how to create multiple sensor instances with the Arduino Matter API.

   The example creates a Matter temperature and humidity sensor device and publishes data through them.
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
#include <MatterTemperature.h>
#include <MatterHumidity.h>

MatterTemperature matter_temp_sensor;
MatterHumidity matter_humidity_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_temp_sensor.begin();
  matter_humidity_sensor.begin();

  Serial.println("Matter temperature and humidity sensor");

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
  while (!matter_temp_sensor.is_online() || !matter_humidity_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop()
{
  // Handle temperature values
  static uint32_t last_action_temp = 0;
  // Wait 10 seconds
  if ((last_action_temp + 10000) < millis()) {
    last_action_temp = millis();
    float current_cpu_temp = getCPUTemp();
    // Publish the temperature value - you can also use 'matter_temp_sensor = current_cpu_temp'
    matter_temp_sensor.set_measured_value_celsius(current_cpu_temp);
    Serial.printf("Current CPU temperature: %.02f C\n", current_cpu_temp);
  }

  // Handle humidity values
  static float current_humidity = 0.0f;
  static uint32_t last_action_humidity = 0;
  // Wait 10 seconds
  if ((last_action_humidity + 10000) < millis()) {
    last_action_humidity = millis();
    // Publish the humidity value - you can also use 'matter_humidity_sensor.set_measured_value(current_humidity)'
    matter_humidity_sensor = current_humidity;
    Serial.printf("Current humidity: %.01f%%\n", current_humidity);
    // Increase the humidity value
    current_humidity += 0.5f;
    if (current_humidity > 100.0f) {
      current_humidity = 0.0f;
    }
  }
}
