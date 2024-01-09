/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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

#include <cstdio>
#include "silabs_additional.h"
extern "C" {
  #include "em_emu.h"
  #include "em_cmu.h"
  #include "em_system_generic.h"
}

float getCpuTemp()
{
  return EMU_TemperatureGet();
}

void systemReset()
{
  NVIC_SystemReset();
  while (1) {
    ;
  }
}

uint64_t getMcuUniqueId()
{
  return SYSTEM_GetUnique();
}

String getMcuUniqueIdStr()
{
  char buf[20];
  uint64_t mcu_unique_id = getMcuUniqueId();
  uint32_t unique_h = (uint32_t)(mcu_unique_id >> 32);
  uint32_t unique_l = (uint32_t)(mcu_unique_id);
  snprintf(buf, sizeof(buf), "0x%08lx%08lx", unique_h, unique_l);
  return String(buf);
}

String getCoreVersion()
{
  return String(ARDUINO_SILABS);
}

void setCPUClock(cpu_clock_t clock)
{
  CMU_DPLLInit_TypeDef pll_init;
  switch (clock) {
    case CPU_40MHZ:
      CMU_CLOCK_SELECT_SET(SYSCLK, HFXO);
      return;
    case CPU_76MHZ:
      pll_init = CMU_DPLL_HFXO_TO_76_8MHZ;
      break;
    case CPU_80MHZ:
      pll_init = CMU_DPLL_HFXO_TO_80MHZ;
      break;
    default:
      CMU_CLOCK_SELECT_SET(SYSCLK, HFXO);
      return;
  }
  bool dpllLock = false;
  while (!dpllLock) {
    dpllLock = CMU_DPLLLock(&pll_init);
  }
  CMU_ClockSelectSet(cmuClock_SYSCLK, cmuSelect_HFRCODPLL);
}

uint32_t getCPUClock()
{
  return SystemCoreClockGet();
}
