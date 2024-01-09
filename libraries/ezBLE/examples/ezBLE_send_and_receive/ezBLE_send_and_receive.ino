/*
   ezBLE send and receive example

   Starts an ezBLE server and waits for a  client to connect.
   Once the client is connected the server will periodically send the CPU ID and the CPU temperature to the client
   while also simultaneously receiving any data sent by the client.

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
  Serial.println("ezBLE send and receive example");

  ezBLE.beginServer();
  ezBLE.onReceive(ezBLE_on_receive);
  while (!ezBLE.connected()) ;
  Serial.println("ezBLE ready!");
}

void loop()
{
  if (ezBLE.connected()) {
    ezBLE.printf("Hello! CPU ID: %s\n", getMcuUniqueIdStr().c_str());
    ezBLE.printf("CPU temp: %.02f C\n", getCpuTemp());
    Serial.println("Sending data");
  } else {
    Serial.println("Couldn't send data, ezBLE not connected");
  }
  delay(1000);
}
