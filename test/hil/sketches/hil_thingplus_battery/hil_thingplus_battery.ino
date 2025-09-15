#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
SFE_MAX1704X lipo; // Defaults to the MAX17043

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  lipo.enableDebugging();

  if (!lipo.begin()) {
    Serial.println(F("MAX17043 not detected. Please check wiring. Freezing."));
    while (1) {
      ;
    }
  }

  lipo.quickStart();
  lipo.setThreshold(20);
}

void loop()
{
  double voltage = lipo.getVoltage();
  Serial.println(voltage);
  delay(500);
}
