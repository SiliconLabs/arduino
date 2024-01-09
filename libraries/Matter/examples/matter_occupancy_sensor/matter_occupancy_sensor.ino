/*
   Matter occupancy sensor example

   The example shows how to create an occupancy sensor with the Arduino Matter API.

   The example creates a Matter occupancy sensor device and publishes occupancy information through it.
   The occupancy changes every 10 seconds.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterOccupancy.h>

MatterOccupancy matter_occupancy_sensor;

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_occupancy_sensor.begin();

  Serial.println("Matter occupancy sensor");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  if (!Matter.isDeviceConnected()) {
    Serial.println("Waiting for network connection...");
  }
  while (!Matter.isDeviceConnected()) {
    delay(200);
  }
  Serial.println("Device connected");
}

void loop()
{
  static uint32_t last_action = 0;
  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    // Toggle the state of the occupancy sensor
    bool new_state = !matter_occupancy_sensor.get_occupancy();
    // Publish the occupancy value - you can also use 'matter_occupancy_sensor.set_occupancy(new_state)'
    matter_occupancy_sensor = new_state;
    Serial.printf("Current ouccupancy state: %s\n", new_state ? "occupied" : "unoccupied");
  }
}
