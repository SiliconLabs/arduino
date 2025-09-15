#include <WatchdogTimer.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Silicon Labs Watchdog timer test");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);

  if (WatchdogTimer.watchdogResetHappened()) {
    Serial.println("Device restarted after a watchdog reset");
    delay(1000);
  } else {
    Serial.println("Device started normally");
  }

  WatchdogTimer.begin(WDOG_PERIOD_513_MS);
}

void loop()
{
  WatchdogTimer.feed();
  Serial.print("Uptime: ");
  Serial.println(millis());
  delay(490);
  Serial.printf("Watchdog reset imminent at %u...\n", millis());
  delay(100);
}
