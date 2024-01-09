/*
   ezBLE callbacks example

   Demonstrates the usage of callbacks in ezBLE. Starts an ezBLE client which will connect to a server.
   Callback functions are registered for connection disconnection and data receive events.

   Compatible with all Silicon Labs BLE hardware.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

volatile bool ezBLE_connected = false;

void ezBLE_on_receive(int bytes)
{
  (void)bytes;
  while (ezBLE.available()) {
    Serial.print((char)ezBLE.read());
  }
}

void ezBLE_on_connect()
{
  ezBLE_connected = true;
  Serial.println("ezBLE connected");
}

void ezBLE_on_disconnect()
{
  ezBLE_connected = false;
  Serial.println("ezBLE disconnected");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("ezBLE callbacks example");

  ezBLE.beginClient();
  ezBLE.onReceive(ezBLE_on_receive);
  ezBLE.onConnect(ezBLE_on_connect);
  ezBLE.onDisconnect(ezBLE_on_disconnect);

  while (!ezBLE_connected) ;
  Serial.println("ezBLE ready!");
}

void loop()
{
}
