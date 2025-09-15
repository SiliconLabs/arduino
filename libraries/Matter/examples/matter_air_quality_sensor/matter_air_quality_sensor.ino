/*
   Matter air quality sensor example

   The example shows how to create a air quality sensor using the SparkFun SGP40 Air Quality sensor with the Arduino Matter API.

   The example creates a Matter air quality sensor device, reads the VOC index from the SGP40 sensor and publishes the air quality value through Matter.
   To use the SGP40 sensor, install the 'SparkFun SGP40 Arduino Library'.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Hai Nguyen (Silicon Labs)
 */
#include <Wire.h>
#include <Matter.h>
#include <MatterAirQuality.h>
#include <SparkFun_SGP40_Arduino_Library.h>

SGP40 sgp40_sensor;
MatterAirQuality matter_air_quality_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_air_quality_sensor.begin();

  Serial.println("Matter air quality sensor");

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
  while (!matter_air_quality_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

  // Initialize sensor
  Wire.begin();
  Serial.println("SGP40 sensor initializing...");
  if (!sgp40_sensor.begin()) {
    Serial.println("SGP40 sensor not detected!");
    Serial.println("Check connections and press the reset button to reinitialize");
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::UNKNOWN);
    while (1) {
      yield();
    }
  }
  Serial.println("SGP40 sensor initialized");
}

void loop()
{
  int air_quality_value = sgp40_sensor.getVOCindex();

  // Handle air quality value
  if ((air_quality_value > 0) && (air_quality_value < 50)) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::GOOD);
    Serial.println("Current air quality: good");
  } else if (air_quality_value < 100) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::FAIR);
    Serial.println("Current air quality: fair");
  } else if (air_quality_value < 200) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::MODERATE);
    Serial.println("Current air quality: moderate");
  } else if (air_quality_value < 300) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::POOR);
    Serial.println("Current air quality: poor");
  } else if (air_quality_value < 400) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::VERY_POOR);
    Serial.println("Current air quality: very poor");
  } else if (air_quality_value < 500) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::EXTREMELY_POOR);
    Serial.println("Current air quality: extremely poor");
  } else if (air_quality_value > 500) {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::UNKNOWN);
    Serial.println("Current air quality: unknown");
  } else {
    matter_air_quality_sensor.set_air_quality(MatterAirQuality::AirQuality_t::UNDEFINED_VALUE);
    Serial.println("Current air quality: undefined value");
  }

  delay(2000);
}
