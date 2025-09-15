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

#ifndef MATTER_H
#define MATTER_H

#ifndef ARDUINO_MATTER
  #error "Selected board/variant is not Matter compatible - please select the Matter protocol stack if your board supports it!"
#endif

#include "Arduino.h"
#include "MatterEndpointHandler.h"
#include "MatterEndpoint.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/server/OnboardingCodesUtil.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <app/clusters/identify-server/identify-server.h>

#define ATTRIBUTE_MASK_WRITABLE_NULLABLE (ATTRIBUTE_MASK_WRITABLE | ATTRIBUTE_MASK_NULLABLE)

using ::chip::CommandId;

extern EmberAfAttributeMetadata bridgedDeviceBasicAttrs[7];
extern EmberAfAttributeMetadata descriptorAttrs[6];
extern EmberAfAttributeMetadata identifyAttrs[4];
extern CommandId identifyIncomingCommands[2];
extern EmberAfAttributeMetadata groupsAttrs[3];
extern CommandId groupsIncomingCommands[7];
extern CommandId groupsOutgoingCommands[5];

class ArduinoMatterAppliance {
public:
  ArduinoMatterAppliance();
  ~ArduinoMatterAppliance();
  virtual bool begin() = 0;
  virtual void end() = 0;
  bool get_identify_in_progress();
  void set_device_name(const char* device_name);
  void set_vendor_name(const char* vendor_name);
  void set_product_name(const char* product_name);
  void set_serial_number(const char* serial_number);
  void set_device_change_callback(void (*matter_device_change_cb)(void));
  bool is_online();

protected:
  Device* base_matter_device;
};

class MatterClass {
public:
  void begin();
  void disableBridgeEndpoint();
  static String getManualPairingCode();
  static String getOnboardingQRCodeUrl();
  static String getOnboardingQRCodePayload();
  static bool isDeviceCommissioned();
  static bool isDeviceThreadConnected();
  void decommission();
};

extern MatterClass Matter;

void IdentifyStartHandler(::Identify* identify);
void IdentifyStopHandler(::Identify* identify);
void TriggerIdentifyEffectHandler(::Identify* identify);

void CallMatterReportingCallback(intptr_t closure);
void ScheduleMatterReportingCallback(EndpointId endpointId, ClusterId cluster, AttributeId attribute);

#endif // MATTER_H
