/*
   Watchdog timer reset example

   The example demonstrates the basic usage of the watchdog timer in the Silicon Labs Arduino core.

   Compatible with all Silicon Labs Arduino boards.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <WatchdogTimer.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Silicon Labs Watchdog timer example");

  // Check if the device was restarted after a watchdog reset
  if (WatchdogTimer.watchdogResetHappened()) {
    Serial.println("Device restarted after a watchdog reset");
  } else {
    Serial.println("Device started normally");
  }

  // Start the watchdog timer with a 2 second timeout period
  WatchdogTimer.begin(WDOG_PERIOD_2_S);
}

void loop()
{
  // Feed the watchdog timer to prevent a reset
  WatchdogTimer.feed();
  Serial.print("Watchdog timer fed at ");
  Serial.println(millis());

  // Toggle the LED
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);

  // Wait for 1 second
  delay(1000);

  // Toggle the LED
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Feed the watchdog timer again to prevent a reset
  WatchdogTimer.feed();
  Serial.print("Watchdog timer fed at ");
  Serial.println(millis());

  // Wait for 1.5 second
  delay(1500);

  // Delay for too long to trigger a watchdog reset
  Serial.println("Delaying for too long to trigger a watchdog reset...");
  delay(1000);
}
