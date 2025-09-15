/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2025 Silicon Laboratories Inc. www.silabs.com
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

#ifndef WATCHDOG_TIMER_SILABS_H
#define WATCHDOG_TIMER_SILABS_H

#include "Arduino.h"
extern "C" {
  #include "em_wdog.h"
}

typedef enum {
  WDOG_PERIOD_9_MS = wdogPeriod_9,
  WDOG_PERIOD_17_MS = wdogPeriod_17,
  WDOG_PERIOD_33_MS = wdogPeriod_33,
  WDOG_PERIOD_65_MS = wdogPeriod_65,
  WDOG_PERIOD_129_MS = wdogPeriod_129,
  WDOG_PERIOD_257_MS = wdogPeriod_257,
  WDOG_PERIOD_513_MS = wdogPeriod_513,
  WDOG_PERIOD_1_S = wdogPeriod_1k,
  WDOG_PERIOD_2_S = wdogPeriod_2k,
  WDOG_PERIOD_4_S = wdogPeriod_4k,
  WDOG_PERIOD_8_S = wdogPeriod_8k,
  WDOG_PERIOD_16_S = wdogPeriod_16k,
  WDOG_PERIOD_32_S = wdogPeriod_32k,
  WDOG_PERIOD_64_S = wdogPeriod_64k,
  WDOG_PERIOD_128_S = wdogPeriod_128k,
  WDOG_PERIOD_256_S = wdogPeriod_256k
} WatchdogPeriod;

class WdogTimerClass {
public:
  /***************************************************************************//**
   * Constructor for the WdogTimerClass
   ******************************************************************************/
  WdogTimerClass();

  /***************************************************************************//**
   * Starts the watchdog timer with the default timeout period of 1 second
   ******************************************************************************/
  void begin();

  /***************************************************************************//**
   * Starts the watchdog timer with the specified timeout period
   *
   * @param[in] timeout_period the selected period of the watchdog timer
   ******************************************************************************/
  void begin(WatchdogPeriod timeout_period);

  /***************************************************************************//**
   * Stops the watchdog timer
   ******************************************************************************/
  void end();

  /***************************************************************************//**
   * Feeds the watchdog timer to prevent a reset
   ******************************************************************************/
  void feed();

  /***************************************************************************//**
   * Sets the timeout period of the watchdog timer
   *
   * @param[in] timeout_period the selected period of the watchdog timer
   ******************************************************************************/
  void setTimeout(WatchdogPeriod timeout_period);

  /***************************************************************************//**
   * Attaches a callback function to the watchdog timer
   *
   * When a callback function is attached, the watchdog timer will not reset the
   * device when it overflows. Instead, the callback function will be called.
   *
   * @param[in] callback the function to be called when the watchdog timer overflows
   ******************************************************************************/
  void attachInterrupt(void (*callback)(void));

  /***************************************************************************//**
   * Detaches the interrupt from the watchdog timer
   ******************************************************************************/
  void detachInterrupt();

  /***************************************************************************//**
   * Checks if a watchdog reset has occurred
   *
   * Returns true if the device was previously reset by the watchdog timer.
   *
   * @return true if a watchdog reset has occurred, false otherwise
   ******************************************************************************/
  bool watchdogResetHappened();

  /***************************************************************************//**
   * Sets the watchdog timer to be enabled/disabled while the device
   * is in sleep mode
   *
   * The watchdog timer is running in sleep mode by default. This function can be
   * used to disable the watchdog timer while the device is in sleep mode to
   * avoid resets when the device is sleeping for a longer period.
   *
   * @param[in] off_while_sleeping true, if the watchdog timer should be
   *                               disabled while sleeping
   ******************************************************************************/
  void setWatchdogOffWhileSleeping(bool off_while_sleeping);

  /***************************************************************************//**
   * Internal interrupt handler for the watchdog timer
   ******************************************************************************/
  void _watchdogIRQHandler();

private:
  void set_default_config();

  bool initialized;
  void (*user_on_overflow_cb)(void);
  WDOG_Init_TypeDef wdog_config;
  WDOG_TypeDef *wdog;
};

extern WdogTimerClass WatchdogTimer;

#endif // WATCHDOG_TIMER_SILABS_H
