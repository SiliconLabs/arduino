/*
   ezBLE simple client example

   Starts an ezBLE client which will connect to a server.
   Once it's connected the device will receive any data sent by the server.

   Compatible with all Silicon Labs BLE hardware.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("ezBLE simple client example");

  ezBLE.beginClient();
  while (!ezBLE.connected()) ;
  Serial.println("ezBLE ready!");
}

void loop()
{
  while (ezBLE.available()) {
    Serial.print((char)ezBLE.read());
  }
}
