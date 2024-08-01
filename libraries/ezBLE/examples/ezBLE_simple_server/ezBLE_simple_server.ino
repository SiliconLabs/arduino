/*
   ezBLE simple server example

   Starts an ezBLE server which will accept client connections.
   Once a client is connected the device will periodically send it's uptime in milliseconds.

   Compatible with all Silicon Labs BLE hardware with the 'BLE (Silabs)' protocol stack variant selected.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("ezBLE simple server example");

  ezBLE.beginServer();
  while (!ezBLE.connected()) ;
  Serial.println("ezBLE ready!");
}

void loop()
{
  ezBLE.print("Hello ezBLE! uptime: ");
  ezBLE.println(millis());
  Serial.println("Sending data over ezBLE");
  delay(1000);
}
