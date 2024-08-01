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
  #include "sl_system_init.h"
  #include "sl_component_catalog.h"
  #ifdef SL_CATALOG_RAIL_LIB_PRESENT
    #include "rail.h"
  #endif
}

void init_arduino_variant()
{
  sl_system_init();

  // Disable SWO by default and allow PA3 to be used as a GPIO pin
  GPIO_DbgSWOEnable(false);

  #ifdef SL_CATALOG_RAIL_LIB_PRESENT
  // Disable RAIL PTI by default and allow PC4 and PC5 to be used as a GPIO pin
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
}

// Variant pin mapping - maps Arduino pin numbers to Silabs ports/pins
// D0 -> Dmax -> A0 -> Amax -> Other peripherals
PinName gPinNames[] = {
  PC0, // D0 - SPI SDO - WU
  PC1, // D1 - SPI SDI
  PC2, // D2 - SPI SCK
  PB2, // D3 - SPI CS
  PA5, // D4 - Tx - WU
  PA6, // D5 - Rx
  PD2, // D6 - SDA - WU
  PA8, // A0 - Tx1
  PA7, // A1 - Rx1
  PB0, // A2
  PB1, // A3 - WU
  PA4, // A4 - LED
  PB3, // A5 - Button - WU
  PD3, // A6 - SCL
  PA4, // LED - 14
  PB3, // Button - 15
  PC6, // Sensor array power - 16
  PC7, // Microphone power - WU - 17
  PB4, // IMU power - 18
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
