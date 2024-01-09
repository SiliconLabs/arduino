/*
   Debug configuration example for the SparkFun ThingPlus Matter MGM240P board

   The example provides a basic debugger configuration for Windows systems

   In order for the debugging to work you'll need to have the J-Link Software and Documentation pack
   (https://www.segger.com/downloads/jlink/) installed on your system.
   Switch to the 'debug_custom.json' tab to see the provided debugger configuration.
   This example can be easily made compatible with the other boards just by
   changing the "device" property to a different MCU in 'debug_custom.json'.
   Please consult the 'Debugging' section of the readme on GitHub for further details.

   Compatible boards:
   - SparkFun Thing Plus MGM240P

   Author: Tamas Jozsi (Silicon Labs)
 */
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  Serial.println(getCpuTemp());
  Serial.println(millis());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
