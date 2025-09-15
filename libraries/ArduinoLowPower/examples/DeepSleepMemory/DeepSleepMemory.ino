/*
   ArduinoLowPower deep sleep example with timed wakeup and memory retention

   The example shows the basic usage of the Arduino Low Power library by putting the device into deep sleep.
   The device will remain in deep sleep until an external interrupt is triggered or the sleep timer expires.
   During deep sleep the whole device is powered down except for a minimal set of peripherals (like the Back-up RAM and RTC).
   This means that the CPU is stopped and the RAM contents are lost - the device will start from the beginning of the sketch after waking up.

   This example utilizes the deep sleep memory (Back-up RAM) to retain data during deep sleep.
   The back-up RAM is a small amount of RAM (128 bytes) which is retained even in deep sleep.

   This example is compatible with all Silicon Labs Arduino boards.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include "ArduinoLowPower.h"

// Make sure to always select a wake-up capable pin - see the pinout diagram of your device for details
#define WAKE_UP_PIN PC0

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  Serial.println("-------------------------");
  Serial.println("Deep sleep memory example");

  // Set up the wake-up pin
  LowPower.attachInterruptWakeup(WAKE_UP_PIN, nullptr, RISING);

  // Check if we woke up from deep sleep or were reset by an other cause
  if (LowPower.wokeUpFromDeepSleep()) {
    Serial.println("Woke up from deep sleep");
  } else {
    Serial.println("Something else caused a reset other than sleeping");
    // Clear the deep sleep memory at address 0 and 1
    LowPower.deepSleepMemoryWrite(0, 0);
    LowPower.deepSleepMemoryWrite(1, 0);
  }
}

void loop()
{
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  delay(500);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  // Read the current sleep cycle counter from the deep sleep memory (address 0)
  uint32_t sleep_cycles = LowPower.deepSleepMemoryRead(0);
  Serial.printf("Sleep cycle counter: %lu\n", sleep_cycles);
  // Increment the sleep cycle counter and write it back to the deep sleep memory
  sleep_cycles++;
  LowPower.deepSleepMemoryWrite(0, sleep_cycles);

  // Read the total uptime counter from the deep sleep memory (address 1)
  uint32_t total_uptime = LowPower.deepSleepMemoryRead(1);
  Serial.printf("Total uptime: %lu\n", total_uptime + millis());
  // Increment the total uptime counter and write it back to the deep sleep memory
  total_uptime += millis();
  LowPower.deepSleepMemoryWrite(1, total_uptime);

  // Go to deep sleep
  Serial.println("Going to deep sleep for 10s");
  LowPower.deepSleep(10000);
}
