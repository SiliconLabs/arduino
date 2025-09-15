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

#include "WatchdogTimer.h"

WdogTimerClass::WdogTimerClass() :
  initialized(false),
  user_on_overflow_cb(nullptr),
  wdog(WDOG0)
{
  this->set_default_config();
}

void WdogTimerClass::begin()
{
  CMU_ClockEnable(cmuClock_WDOG0, true);
  CMU_ClockSelectSet(cmuClock_WDOG0, cmuSelect_ULFRCO); // 1 kHz
  WDOGn_Init(this->wdog, &this->wdog_config);
  this->initialized = true;
}

void WdogTimerClass::begin(WatchdogPeriod timeout_period)
{
  this->wdog_config.perSel = (WDOG_PeriodSel_TypeDef)timeout_period;
  this->begin();
}

void WdogTimerClass::end()
{
  this->detachInterrupt();
  WDOGn_Enable(this->wdog, false);
  this->set_default_config();
  this->initialized = false;
}

void WdogTimerClass::feed()
{
  if (!this->initialized) {
    return;
  }
  WDOGn_Feed(this->wdog);
}

void WdogTimerClass::setTimeout(WatchdogPeriod timeout_period)
{
  this->begin(timeout_period);
}

void WdogTimerClass::attachInterrupt(void (*callback)(void))
{
  this->user_on_overflow_cb = callback;
  this->wdog_config.resetDisable = true;
  WDOGn_IntEnable(this->wdog, WDOG_IEN_TOUT);
  NVIC_ClearPendingIRQ(WDOG0_IRQn);
  NVIC_EnableIRQ(WDOG0_IRQn);
  this->begin();
}

void WdogTimerClass::detachInterrupt()
{
  this->user_on_overflow_cb = nullptr;
  this->wdog_config.resetDisable = false;
  WDOGn_IntDisable(this->wdog, WDOG_IEN_TOUT);
  NVIC_ClearPendingIRQ(WDOG0_IRQn);
  NVIC_DisableIRQ(WDOG0_IRQn);
  this->begin();
}

bool WdogTimerClass::watchdogResetHappened()
{
  return (get_system_reset_cause() & EMU_RSTCAUSE_WDOG0) != 0;
}

void WdogTimerClass::setWatchdogOffWhileSleeping(bool off_while_sleeping)
{
  #if defined(_WDOG_CFG_EM1RUN_MASK)
  this->wdog_config.em1Run = !off_while_sleeping;
  #endif
  this->wdog_config.em2Run = !off_while_sleeping;
  this->wdog_config.em3Run = !off_while_sleeping;
  this->begin();
}

void WdogTimerClass::_watchdogIRQHandler()
{
  if (this->user_on_overflow_cb) {
    this->user_on_overflow_cb();
  }
}

void WdogTimerClass::set_default_config()
{
  this->wdog_config = WDOG_INIT_DEFAULT;
  this->wdog_config.perSel = (WDOG_PeriodSel_TypeDef)WDOG_PERIOD_1_S;
  #if defined(_WDOG_CFG_EM1RUN_MASK)
  this->wdog_config.em1Run = true;
  #endif
  this->wdog_config.em2Run = true;
  this->wdog_config.em3Run = true;
}

WdogTimerClass WatchdogTimer;

void WDOG0_IRQHandler(void)
{
  uint32_t flags = WDOGn_IntGet(WDOG0);
  WDOGn_IntClear(WDOG0, flags);
  WatchdogTimer._watchdogIRQHandler();
}
