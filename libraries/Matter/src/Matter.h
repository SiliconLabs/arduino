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

#ifndef MATTER_H
#define MATTER_H

#ifndef ARDUINO_MATTER
  #error "Selected board is not Matter compatible - please select a board with Matter compatibility!"
#endif

#include "Arduino.h"
#include "MatterEndpointHandler.h"
#include "MatterEndpoint.h"
#include "WString.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/server/OnboardingCodesUtil.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>

#define matter_log SILABS_LOG

class ArduinoMatterAppliance {
public:
  ArduinoMatterAppliance();
  ~ArduinoMatterAppliance();
  virtual bool begin() = 0;
  virtual void end() = 0;
};

class MatterClass {
public:
  void begin();
  static String getManualPairingCode();
  static String getOnboardingQRCodeUrl();
  static bool isDeviceCommissioned();
  static bool isDeviceConnected();
};

extern MatterClass Matter;

#endif // MATTER_H
