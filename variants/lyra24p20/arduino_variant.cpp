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

#include "Arduino.h"
#include "arduino_variant.h"
#include "arduino_i2c_config.h"
#include "arduino_spi_config.h"

extern "C" {
  #include "sl_component_catalog.h"
  #ifdef SL_CATALOG_RAIL_LIB_PRESENT
    #include "rail.h"
  #endif
}

extern "C" {
  #include "sl_system_init.h"
}

void init_arduino_variant()
{
  sl_system_init();

  // Disable SWO by default and allow PA3 to be used as a GPIO pin
  GPIO_DbgSWOEnable(false);

  #ifdef SL_CATALOG_RAIL_LIB_PRESENT
  // Disable RAIL PTI by default and allow PC6 and PC7 to be used as a GPIO pin
  RAIL_PtiConfig_t railPtiConfig = {};
  railPtiConfig.mode = RAIL_PTI_MODE_DISABLED;
  RAIL_ConfigPti(RAIL_EFR32_HANDLE, &railPtiConfig);
  RAIL_EnablePti(RAIL_EFR32_HANDLE, false);
  #endif

  // Deinit Serial, Wire and SPI by default - sl_system_init() initializes it
  Serial.end();
  Serial1.end();
  I2C_Deinit(SL_I2C_PERIPHERAL); // Wire.end()
  SPIDRV_DeInit(SL_SPIDRV_PERIPHERAL_HANDLE); //SPI.end();

  // Reenable the clock for EUSART0 in CMU (cmuClock_EUSART0) for the SPI1 to successfully deinit
  // This clock is turned off in Serial.end() as they share the same EUSART0 peripheral.
  // We turn it back on so that the SPI1 can deinitialize without running to a fault when accessing the EUSART0 registers.
  CMU_ClockEnable(cmuClock_EUSART0, true);
  SPIDRV_DeInit(SL_SPIDRV1_PERIPHERAL_HANDLE); // SPI1.end();
}

// Variant pin mapping - maps Arduino pin numbers to Silabs ports/pins
// D0 -> Dmax -> A0 -> Amax -> Other peripherals
PinName gPinNames[] = {
  PA4, // D0
  PA5, // D1 - WU
  PC4, // D2 - SPI SDO
  PC5, // D3 - SPI SDI - WU
  PC2, // D4 - SPI SCK
  PC3, // D5 - SPI CS
  PC6, // D6 - SPI1 SS
  PB0, // D7 - SPI1 SCK - DAC0
  PA6, // A0 - 8
  PA8, // A1 - LED - 9
  PD3, // A2 - SDA - 10
  PD2, // A3 - SCL - WU - 11
  PB1, // A4 - Tx - DAC1 - WU - 12
  PB2, // A5 - Rx - DAC2 - 13
  PB3, // A6 - SPI1 SDO - Tx1 - DAC3 - WU - 14
  PB4, // A7 - SPI1 SDI - Rx1 - 15
  PC7, // Button - WU - 16
  PA0, // Rx - 17
  PA7  // Tx - 18
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
