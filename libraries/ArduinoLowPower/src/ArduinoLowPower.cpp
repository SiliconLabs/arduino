/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if defined(ARDUINO_SILABS)

#include "ArduinoLowPower.h"
extern "C" {
  #include "em_burtc.h"
  #include "em_emu.h"
  #include "sl_power_manager.h"
}

static uint32_t GetEm4WakeUpPinMask(GPIO_Port_TypeDef port, uint32_t pin);

ArduinoLowPowerClass::ArduinoLowPowerClass() :
  wakeup_callback(nullptr),
  wakeup_pin(PIN_NAME_NC),
  wakeup_mode(FALLING)
{
  ;
}

void ArduinoLowPowerClass::handleWakeup()
{
  if (this->wakeup_callback) {
    this->wakeup_callback();
  }
}

// EM1
void ArduinoLowPowerClass::idle()
{
  sl_power_manager_sleep();
  this->handleWakeup();
}

// EM1 - timed
void ArduinoLowPowerClass::idle(uint32_t millis)
{
  this->timedSleep(millis);
}

// EM2
void ArduinoLowPowerClass::sleep()
{
  Serial.suspend();
  #if (NUM_HW_SERIAL > 1)
  Serial1.suspend();
  #endif

  sl_power_manager_sleep();
  this->handleWakeup();

  Serial.resume();
  #if (NUM_HW_SERIAL > 1)
  Serial1.resume();
  #endif
}

// EM2 - timed
void ArduinoLowPowerClass::sleep(uint32_t millis)
{
  Serial.suspend();
  #if (NUM_HW_SERIAL > 1)
  Serial1.suspend();
  #endif

  this->timedSleep(millis);

  Serial.resume();
  #if (NUM_HW_SERIAL > 1)
  Serial1.resume();
  #endif
}

// EM4 - GPIO wakeup
void ArduinoLowPowerClass::deepSleep()
{
  EMU_EM4Init_TypeDef em4_init = EMU_EM4INIT_DEFAULT;
  em4_init.pinRetentionMode = emuPinRetentionEm4Exit;
  EMU_EM4Init(&em4_init);

  this->setupDeepSleepWakeUpPin();

  EMU_EnterEM4();
}

// EM4 - BURTC wakeup - the device will also wake up on GPIO interrupt if configured
void ArduinoLowPowerClass::deepSleep(uint32_t millis)
{
  CMU_ClockSelectSet(cmuClock_EM4GRPACLK, cmuSelect_ULFRCO);
  CMU_ClockEnable(cmuClock_BURTC, true);
  CMU_ClockEnable(cmuClock_BURAM, true);

  BURTC_Init_TypeDef burtcInit = BURTC_INIT_DEFAULT;
  burtcInit.compare0Top = true; // Reset counter when counter reaches compare value
  burtcInit.em4comp = true;     // BURTC compare interrupt wakes from EM4 (causes reset)
  BURTC_Init(&burtcInit);

  BURTC_CounterReset();
  BURTC_CompareSet(0, millis);

  BURTC_IntEnable(BURTC_IEN_COMP);    // Compare match
  NVIC_EnableIRQ(BURTC_IRQn);
  BURTC_Enable(true);

  this->setupDeepSleepWakeUpPin();

  EMU_EM4Init_TypeDef em4_init = EMU_EM4INIT_DEFAULT;
  EMU_EM4Init(&em4_init);
  EMU_EnterEM4();
}

void ArduinoLowPowerClass::attachInterruptWakeup(uint32_t pin, voidFuncPtr callback, irq_mode mode)
{
  this->wakeup_callback = callback;
  this->wakeup_pin = pin;
  this->wakeup_mode = mode;
}

void ArduinoLowPowerClass::timedSleep(uint32_t millis_to_sleep)
{
  delay(millis_to_sleep);
  this->handleWakeup();
}

// Writes 4 bytes to the specified Backup RAM address which is retained during deep sleep
void ArduinoLowPowerClass::deepSleepMemoryWrite(uint32_t address, uint32_t data)
{
  if (address >= this->deep_sleep_memory_size) {
    return;
  }
  CMU_ClockEnable(cmuClock_BURAM, true);
  BURAM->RET[address].REG = data;
}

// Reads 4 bytes from the specified Backup RAM address
uint32_t ArduinoLowPowerClass::deepSleepMemoryRead(uint32_t address)
{
  if (address >= this->deep_sleep_memory_size) {
    return 0u;
  }
  CMU_ClockEnable(cmuClock_BURAM, true);
  return BURAM->RET[address].REG;
}

// Returns whether the device was reset by deep sleeping or by an other cause
bool ArduinoLowPowerClass::wokeUpFromDeepSleep()
{
  return (get_system_reset_cause() & EMU_RSTCAUSE_EM4);
}

// Configures the specified pin to wake up the device from EM4
void ArduinoLowPowerClass::setupDeepSleepWakeUpPin()
{
  if (this->wakeup_pin == PIN_NAME_NC) {
    return;
  }
  GPIO_Port_TypeDef wakeup_interrupt_port = getSilabsPortFromArduinoPin(pinToPinName(wakeup_pin));
  uint32_t wakeup_interrupt_pin = getSilabsPinFromArduinoPin(pinToPinName(wakeup_pin));
  GPIO_PinModeSet(wakeup_interrupt_port, wakeup_interrupt_pin, gpioModeInputPullFilter, 1);
  uint32_t wakeup_pin_mask = GetEm4WakeUpPinMask(wakeup_interrupt_port, wakeup_interrupt_pin);

  // LOW/FALLING by default
  uint32_t polarity_mask = 0u;
  if (wakeup_mode == RISING || wakeup_mode == HIGH) {
    polarity_mask = __UINT32_MAX__ & _GPIO_EM4WUPOL_MASK;
  }
  GPIO_EM4EnablePinWakeup(wakeup_pin_mask, polarity_mask);
}

ArduinoLowPowerClass LowPower;

// Get EM4 wakeup pin mask for a provided port and pin - note that not all pins can wake up the MCU from EM4
// If the provided pin is not a wakeup pin, the device won't be able to wake up from EM4 - only with a reset
static uint32_t GetEm4WakeUpPinMask(GPIO_Port_TypeDef port, uint32_t pin)
{
  uint32_t wakeup_pin_mask = 0u;
  if (GPIO_EM4WU0_PORT == port && GPIO_EM4WU0_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN0;
  } else if (GPIO_EM4WU3_PORT == port && GPIO_EM4WU3_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN3;
  }
#if defined(GPIO_EM4WU4_PORT) && defined(GPIO_EM4WU4_PIN)
  else if (GPIO_EM4WU4_PORT == port && GPIO_EM4WU4_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN4;
  }
#endif
  else if (GPIO_EM4WU6_PORT == port && GPIO_EM4WU6_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN6;
  } else if (GPIO_EM4WU7_PORT == port && GPIO_EM4WU7_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN7;
  } else if (GPIO_EM4WU8_PORT == port && GPIO_EM4WU8_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN8;
  } else if (GPIO_EM4WU9_PORT == port && GPIO_EM4WU9_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN9;
  }
#if defined(GPIO_EM4WU10_PORT) && defined(GPIO_EM4WU10_PIN)
  else if (GPIO_EM4WU10_PORT == port && GPIO_EM4WU10_PIN == pin) {
    wakeup_pin_mask = GPIO_IEN_EM4WUIEN10;
  }
#endif
  return wakeup_pin_mask;
}

void BURTC_IRQHandler(void)
{
  BURTC_IntClear(BURTC_IF_COMP); // Compare match
}

// Overrides the WEAK escapeHatch() function in main.cpp to provide a way to prevent
// the device from going to EM4 too quickly. If the device enters EM4 sleep the programmer
// is not able to communicate with it - and without this mechanism it can easily be bricked.
// This function will run before any user code and keep the device awake when the built-in button
// (or the configured escape pin) is pressed during startup so that the programmer has a chance to
// communicate with it.
void escape_hatch()
{
  pinMode(DEEP_SLEEP_ESCAPE_PIN, INPUT_PULLUP);
  if (digitalRead(DEEP_SLEEP_ESCAPE_PIN) != LOW) {
    return;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  while (1) {
    ;
  }
}

#endif // ARDUINO_SILABS
