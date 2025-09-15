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

#include <cstdio>
#include "silabs_additional.h"
#include "arduino_i2c_config.h"
extern "C" {
  #include "em_emu.h"
  #include "em_cmu.h"
  #include "em_system_generic.h"
  #include "sl_i2cspm.h"
  #include "sl_memory_manager.h"
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

silabs_board_t getCurrentBoardType()
{
  #if defined(ARDUINO_BOARD_NANO_MATTER)
  return silabs_board_t::BOARD_ARDUINO_NANO_MATTER;
  #elif defined(ARDUINO_BOARD_SILABS_THINGPLUSMATTER)
  return silabs_board_t::BOARD_SPARKFUN_THINGPLUS_MATTER;
  #elif defined(ARDUINO_BOARD_SILABS_XG24EXPLORERKIT)
  return silabs_board_t::BOARD_SILABS_XG24_EXPLORER_KIT;
  #elif defined(ARDUINO_BOARD_SILABS_XG24DEVKIT)
  return silabs_board_t::BOARD_SILABS_XG24_DEV_KIT;
  #elif defined(ARDUINO_BOARD_SILABS_XG27DEVKIT)
  return silabs_board_t::BOARD_SILABS_XG27_DEV_KIT;
  #elif defined(ARDUINO_BOARD_SILABS_BGM220EXPLORERKIT)
  return silabs_board_t::BOARD_SILABS_BGM220_EXPLORER_KIT;
  #elif defined(ARDUINO_BOARD_EZURIO_LYRA24P20)
  return silabs_board_t::BOARD_EURIO_LYRA_24P_20_DBM_DEV_KIT;
  #elif defined(ARDUINO_BOARD_XIAO_MG24)
  return silabs_board_t::BOARD_SEEED_STUDIO_XIAO_MG24;
  #else
  return silabs_board_t::BOARD_UNKNOWN;
  #endif
}

silabs_radio_stack_t getCurrentRadioStackType()
{
  #if defined(ARDUINO_SILABS_STACK_NONE)
  return silabs_radio_stack_t::RADIO_STACK_NONE;
  #elif defined(ARDUINO_SILABS_STACK_BLE_SILABS)
  return silabs_radio_stack_t::RADIO_STACK_BLE_SILABS;
  #elif defined(ARDUINO_SILABS_STACK_BLE_ARDUINO)
  return silabs_radio_stack_t::RADIO_STACK_BLE_ARDUINO;
  #elif defined(ARDUINO_SILABS_STACK_MATTER_OVER_THREAD)
  return silabs_radio_stack_t::RADIO_STACK_MATTER;
  #else
  return silabs_radio_stack_t::RADIO_STACK_UNKNOWN;
  #endif
}

bool isBoardAiMlCapable() {
  #ifdef ARDUINO_SILABS_AIML_CAPABLE
  return true;
  #else
  return false;
  #endif
}

size_t getTotalHeapSize()
{
  return sl_memory_get_total_heap_size();
}

size_t getFreeHeapSize()
{
  return sl_memory_get_free_heap_size();
}

size_t getUsedHeapSize()
{
  return sl_memory_get_used_heap_size();
}

size_t getHeapHighWatermark()
{
  return sl_memory_get_heap_high_watermark();
}

void resetHeapHighWatermark()
{
  sl_memory_reset_heap_high_watermark();
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
