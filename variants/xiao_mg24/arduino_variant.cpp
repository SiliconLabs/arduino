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

#ifdef ARDUINO_MATTER

#include "AppConfig.h"
#include <DeviceInfoProviderImpl.h>
#include <MatterConfig.h>
#include <app/server/Server.h>
#include <platform/silabs/platformAbstraction/SilabsPlatform.h>
#include <provision/ProvisionManager.h>

#ifdef BLE_DEV_NAME
#undef BLE_DEV_NAME
#endif
#define BLE_DEV_NAME "Arduino Matter device"

static chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;

using namespace ::chip;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;
using namespace chip::DeviceLayer::Silabs;

#else //ARDUINO_MATTER

extern "C" {
  #include "sl_system_init.h"
}

#endif //ARDUINO_MATTER

void init_arduino_variant()
{
  #ifdef ARDUINO_MATTER
  // Initialize the Matter stack
  GetPlatform().Init();

  if (Provision::Manager::GetInstance().ProvisionRequired()) {
    Provision::Manager::GetInstance().Start();
  } else {
    if (SilabsMatterConfig::InitMatter(BLE_DEV_NAME) != CHIP_NO_ERROR) {
      appError(CHIP_ERROR_INTERNAL);
    }
    gExampleDeviceInfoProvider.SetStorageDelegate(&chip::Server::GetInstance().GetPersistentStorage());
    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    chip::DeviceLayer::PlatformMgr().LockChipStack();
    // Initialize device attestation config
    SetDeviceAttestationCredentialsProvider(&Provision::Manager::GetInstance().GetStorage());
    chip::DeviceLayer::PlatformMgr().UnlockChipStack();
  }

  #else //ARDUINO_MATTER

  sl_system_init();

  #endif //ARDUINO_MATTER
}

// Variant pin mapping - maps Arduino pin numbers to Silabs ports/pins
// D0 -> Dmax -> A0 -> Amax -> Other peripherals
// USART0 - Serial
// EUSART0 - SPI
// EUSART1 - Serial1/SPI1
PinName gPinNames[] = {
  PC0, // D0 - SPI SS - WU
  PC1, // D1
  PC2, // D2
  PC3, // D3
  PC4, // D4 - SDA
  PC5, // D5 - SCL - WU
  PC6, // D6 - Serial1 Tx
  PC7, // D7 - Serial1 Rx - WU
  PA3, // D8 - SPI SCK
  PA4, // D9 - SPI SDI
  PA5, // D10 - SPI SDO - WU
  PA9, // D11 - Serial Rx - 11
  PA8, // D12 - Serial Tx - 12
  PB2, // D13 - SDA1 - DAC2 - 13
  PB3, // D14 - SCL1 - DAC3 - WU - 14
  PB0, // D15 - SPI1 SDO - DAC0 - 15
  PB1, // D16 - SPI1 SDI - DAC1 - WU - 16
  PA0, // D17 - SPI1 SCK - 17
  PD2, // D18 - WU - 18
  PD5, // IMU enable - WU - 19
  PA7, // LED - 20
  PA6, // Flash CS - 21
  PC8, // MIC enable - 22
  PC9, // MIC data - 23
  PD4, // VBAT ADC - 24
  PD3, // Battery charger enable - 25
  PB4, // RF switch - 26
  PB5  // RF switch enable - 27
};

unsigned int getPinCount()
{
  return sizeof(gPinNames) / sizeof(gPinNames[0]);
}
