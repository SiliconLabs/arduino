/*
   SparkFun ThingPlus Matter battery fuel gauge example

   Demonstrates the usage of the MAX17048 battery fuel gauge on the SparkFun ThingPlus Matter board.
   The example measures battery voltage and charge state - then displays it on the serial console.

   Compatible with the SparkFun ThingPlus Matter board - or any board with a MAX1704x connected.

   Author: Tung Do (Silicon Labs)
 */

#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

SFE_MAX1704X lipo(MAX1704X_MAX17048);

void setup()
{
  Serial.begin(115200);
  Serial.println(F("SparkFun ThingPlus Matter battery fuel gauge example (MAX17048)"));
  Wire.begin();

  if (!lipo.begin()) {
    Serial.println(F("MAX17048 not detected, please check wiring"));
    while (1) ;
  }
  uint8_t id = lipo.getID();
  Serial.print("Chip ID: 0x");
  Serial.println(id, HEX);
  lipo.quickStart();
  lipo.setThreshold(20);
}

void loop()
{
  static uint32_t previousTime;
  uint32_t currentTime = millis();

  if (currentTime - previousTime > 500) {
    double voltage = lipo.getVoltage();
    double soc = lipo.getSOC();
    bool alert = lipo.getAlert();

    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    Serial.print("Percentage: ");
    Serial.print(soc);
    Serial.println("%");

    if (alert) {
      Serial.print("Alert: ");
      Serial.println(alert);
      Serial.println();
    }

    previousTime = currentTime;
  }
}
