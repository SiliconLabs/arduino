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

#include <cstdio>
#include "silabs_additional.h"
#include "arduino_i2c_config.h"
extern "C" {
  #include "em_emu.h"
  #include "em_cmu.h"
  #include "em_system_generic.h"
  #include "sl_i2cspm.h"
}

float getCPUTemp()
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

uint64_t getDeviceUniqueId()
{
  return SYSTEM_GetUnique();
}

String getDeviceUniqueIdStr()
{
  char buf[20];
  uint64_t unique_id = getDeviceUniqueId();
  uint32_t unique_h = (uint32_t)(unique_id >> 32);
  uint32_t unique_l = (uint32_t)(unique_id);
  snprintf(buf, sizeof(buf), "0x%08lx%08lx", unique_h, unique_l);
  return String(buf);
}

String getCoreVersion()
{
  return String(ARDUINO_SILABS);
}

// DPLL config for 78 MHz CPU clock - Matter variants default to this
#define DPLL_HFXO_TO_78MHZ                                                \
  {                                                                       \
    78000000,                     /* Target frequency                  */ \
    3839,                         /* Factor N                          */ \
    1919,                         /* Factor M                          */ \
    cmuSelect_HFXO,               /* Select HFXO as reference clock    */ \
    cmuDPLLEdgeSel_Fall,          /* Select falling edge of ref clock  */ \
    cmuDPLLLockMode_Phase,        /* Use frequency lock mode           */ \
    true,                         /* Enable automatic lock recovery    */ \
    false                         /* Don't enable dither function      */ \
  }

void setCPUClock(cpu_clock_t clock)
{
  CMU_DPLLInit_TypeDef pll_init;
  switch (clock) {
    case CPU_39MHZ:
      CMU_CLOCK_SELECT_SET(SYSCLK, HFXO);
      return;
    case CPU_76MHZ:
      pll_init = CMU_DPLL_HFXO_TO_76_8MHZ;
      break;
    case CPU_78MHZ:
      pll_init = DPLL_HFXO_TO_78MHZ;
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

void I2C_Deinit(I2C_TypeDef* i2c_peripheral)
{
  I2C_Reset(i2c_peripheral);

  // Reset the I2C to GPIO peripheral routing to enable the pins to function as GPIO
  #if defined(I2C0)
  if (i2c_peripheral == I2C0) {
    GPIO->I2CROUTE[0].ROUTEEN = 0;
    GPIO->I2CROUTE[0].SCLROUTE = 0;
    GPIO->I2CROUTE[0].SDAROUTE = 0;
    NVIC_DisableIRQ(I2C0_IRQn);
  }
  #endif

  #if defined(I2C1)
  if (i2c_peripheral == I2C1) {
    GPIO->I2CROUTE[1].ROUTEEN = 0;
    GPIO->I2CROUTE[1].SCLROUTE = 0;
    GPIO->I2CROUTE[1].SDAROUTE = 0;
    NVIC_DisableIRQ(I2C1_IRQn);
  }
  #endif

  #if defined(I2C2)
  if (i2c_peripheral == I2C2) {
    GPIO->I2CROUTE[2].ROUTEEN = 0;
    GPIO->I2CROUTE[2].SCLROUTE = 0;
    GPIO->I2CROUTE[2].SDAROUTE = 0;
    NVIC_DisableIRQ(I2C2_IRQn);
  }
  #endif

  // Reset the I2C pins to floating input
  GPIO_PinModeSet(SL_I2C_SCL_PORT, SL_I2C_SCL_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(SL_I2C_SDA_PORT, SL_I2C_SDA_PIN, gpioModeInput, 0);

  #if (NUM_HW_I2C > 1)
  GPIO_PinModeSet(SL_I2C1_SCL_PORT, SL_I2C1_SCL_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(SL_I2C1_SDA_PORT, SL_I2C1_SDA_PIN, gpioModeInput, 0);
  #endif
}
