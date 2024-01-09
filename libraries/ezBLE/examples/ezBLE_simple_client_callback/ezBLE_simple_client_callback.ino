/*
   ezBLE simple client example with a receive callback

   Starts an ezBLE client which will connect to a server.
   Once it's connected the device will receive any data sent by the server.
   The data is not received in 'loop()', it's received in a callback function which is
   called on data reception.

   Compatible with all Silicon Labs BLE hardware.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

void ezBLE_on_receive(int bytes)
{
  (void)bytes;
  while (ezBLE.available()) {
    Serial.print((char)ezBLE.read());
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("ezBLE simple client callback example");

  ezBLE.beginClient();
  ezBLE.onReceive(ezBLE_on_receive);
  while (!ezBLE.connected()) ;
  Serial.println("ezBLE ready!");
}

void loop()
{
}
