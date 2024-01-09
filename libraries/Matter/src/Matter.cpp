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

#include "Matter.h"

using namespace chip;
using namespace ::chip::DeviceLayer;
using namespace ::chip::app::Clusters;

ArduinoMatterAppliance::ArduinoMatterAppliance()
{
  ;
}

ArduinoMatterAppliance::~ArduinoMatterAppliance()
{
  ;
}

void MatterClass::begin()
{
  InitDynamicEndpointHandler();
}

String MatterClass::getManualPairingCode()
{
  // Create buffer for manual pairing code that can fit max size + check digit + null terminator.
  char manualPairingCodeBuffer[chip::kManualSetupLongCodeCharLength + 1];
  chip::MutableCharSpan manualPairingCode(manualPairingCodeBuffer);
  GetManualPairingCode(manualPairingCode, chip::RendezvousInformationFlag::kBLE);
  return manualPairingCode.data();
}

String MatterClass::getOnboardingQRCodeUrl()
{
  // Create buffer for QR code that can fit max size and null terminator.
  char qrCodeBuffer[chip::QRCodeBasicSetupPayloadGenerator::kMaxQRCodeBase38RepresentationLength + 1];
  chip::MutableCharSpan QRCode(qrCodeBuffer);
  GetQRCode(QRCode, chip::RendezvousInformationFlag::kBLE);
  char qrCodeUrlBuffer[200];
  GetQRCodeUrl(qrCodeUrlBuffer, sizeof(qrCodeUrlBuffer), QRCode);
  return qrCodeUrlBuffer;
}

bool MatterClass::isDeviceCommissioned()
{
  return ConnectivityMgr().IsThreadProvisioned();
}

bool MatterClass::isDeviceConnected()
{
  return ConnectivityMgr().IsThreadAttached();
}

MatterClass Matter;
