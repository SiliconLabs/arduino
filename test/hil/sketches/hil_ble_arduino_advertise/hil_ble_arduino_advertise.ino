#include <ArduinoBLE.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);

  if (!BLE.begin()) {
    while (1) ;
  }
  BLE.setLocalName("Arduino BLE HIL");
  BLE.advertise();
}

void loop()
{
  ;
}
