#ifndef _ARDUINO_LOW_POWER_H_
#define _ARDUINO_LOW_POWER_H_

#include <Arduino.h>

#ifdef ARDUINO_ARCH_AVR
#error The library is not compatible with AVR boards
#endif

#ifdef ARDUINO_API_VERSION
using irq_mode = PinStatus;
#else
using irq_mode = uint32_t;
#endif

class ArduinoLowPowerClass {
public:
  void idle(void);
  void idle(uint32_t millis);
  void idle(int millis)
  {
    idle((uint32_t)millis);
  }

  void sleep(void);
  void sleep(uint32_t millis);
  void sleep(int millis)
  {
    sleep((uint32_t)millis);
  }

  void deepSleep(void);
  void deepSleep(uint32_t millis);
  void deepSleep(int millis)
  {
    deepSleep((uint32_t)millis);
  }

  void attachInterruptWakeup(uint32_t pin, voidFuncPtr callback, irq_mode mode);

#ifdef ARDUINO_SILABS
  ArduinoLowPowerClass();
  void deepSleepMemoryWrite(uint32_t address, uint32_t data);
  uint32_t deepSleepMemoryRead(uint32_t address);
  uint32_t deepSleepMemorySize()
  {
    return deep_sleep_memory_size;
  }
  bool wokeUpFromDeepSleep();
#endif

private:

#ifdef ARDUINO_SILABS
  static const uint32_t deep_sleep_memory_size = 32u;
  voidFuncPtr wakeup_callback;
  uint32_t wakeup_pin;
  irq_mode wakeup_mode;
  void handleWakeup();
  void timedSleep(uint32_t millis_to_sleep);
  void setupDeepSleepWakeUpPin();
#endif
};

extern ArduinoLowPowerClass LowPower;

#endif
