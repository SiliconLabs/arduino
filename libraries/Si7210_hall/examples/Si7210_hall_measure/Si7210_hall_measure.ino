/*
   Si7210 Hall Effect Magnetic Sensor example

   Basic example for measuring magnetic field strength using the Si7210 hall effect sensor.

   Author: Nghia Bui (Silicon Labs)
 */

#include <Si7210_hall.h>

Si7210 hall_sensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Si7210 hall sensor example");

  #ifdef ARDUINO_BOARD_SILABS_XG27DEVKIT
  // The xG27 Dev Kit has a separate sensor power rail
  // which needs to be enabled by pulling PC6 high
  pinMode(PC6, OUTPUT);
  digitalWrite(PC6, HIGH);
  delay(500);
  #endif

  if (hall_sensor.begin()) {
    Serial.println("Initialization successful");
  } else {
    Serial.println("Initialization failed");
    while (1) ;
  }
}

void loop()
{
  static const uint32_t hall_scale = 20000u;
  float mag = hall_sensor.measure(hall_scale);

  Serial.print("Magnetic flux: ");
  Serial.print(mag, 3);
  Serial.println(" mT");
  delay(500);
}
