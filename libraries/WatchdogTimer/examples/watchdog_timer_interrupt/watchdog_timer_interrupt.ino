/*
   Watchdog timer interrupt example

   The example demonstrates the basic usage of the watchdog timer in the Silicon Labs Arduino core.
   Instead of resetting the device, the watchdog timer will trigger an interrupt and call a user-defined callback function.

   Compatible with all Silicon Labs Arduino boards.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <WatchdogTimer.h>

volatile bool watchdog_overflowed = false;

void onWatchdogOverflow()
{
  watchdog_overflowed = true;
}

void checkWatchdogOverflow()
{
  if (watchdog_overflowed) {
    Serial.println("Watchdog timer overflowed!");
    watchdog_overflowed = false;
  } else {
    Serial.println("Watchdog timer is fed properly, no overflow");
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.begin(115200);
  Serial.println("Silicon Labs Watchdog timer example");

  // Start the watchdog timer with a 2 second timeout period
  WatchdogTimer.begin(WDOG_PERIOD_2_S);

  // Attach a callback function to the watchdog timer
  WatchdogTimer.attachInterrupt(onWatchdogOverflow);
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
  checkWatchdogOverflow();

  // Toggle the LED
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Feed the watchdog timer again to prevent a reset
  WatchdogTimer.feed();
  Serial.print("Watchdog timer fed at ");
  Serial.println(millis());

  // Wait for 1.5 second
  delay(1500);
  checkWatchdogOverflow();

  // Delay for too long to trigger a watchdog interrupt
  // No reset will happen, the watchdog timer will trigger a callback instead
  Serial.println("Delaying for too long to trigger a watchdog interrupt...");
  delay(1000);
  checkWatchdogOverflow();
}
