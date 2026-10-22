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

#ifndef MATTER_DOOR_LOCK_H
#define MATTER_DOOR_LOCK_H

#include "Matter.h"
#include "devices/DeviceDoorLock.h"
#include <initializer_list>
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterDoorLock : public ArduinoMatterAppliance {
public:
  using CredentialTypeEnum = chip::app::Clusters::DoorLock::CredentialTypeEnum;
  using Feature = chip::app::Clusters::DoorLock::Feature;

  MatterDoorLock();
  ~MatterDoorLock();
  bool setFeatures(uint32_t feature_map);
  bool setFeatures(Feature feature);
  bool setFeatures(Feature feature1, Feature feature2);
  bool setFeatures(std::initializer_list<Feature> features);
  bool begin();
  void end();
  bool is_locked();
  void set_locked(bool locked);
  void toggle();
  void operator=(bool state);
  operator bool();

  // Provisions/clears a PIN or RFID credential from the sketch, outside of a Matter controller's
  // SetUser/SetCredential flow (e.g. to pre-provision a default code, or to mirror a physical
  // keypad/RFID enrollment). 'index' is 1-based, up to the configured per-type storage capacity.
  bool set_credential(CredentialTypeEnum type, uint16_t index, const uint8_t* data, uint8_t len);
  bool clear_credential(CredentialTypeEnum type, uint16_t index);

  // Called whenever a credential is provisioned or cleared, whether by a Matter controller
  // (SetCredential/ClearCredential commands) or by the sketch itself (set_credential/clear_credential).
  void set_credential_changed_callback(void (*credential_changed_cb)(CredentialTypeEnum type, uint16_t index));

  // Reports that the sketch itself has matched a physically-presented credential (e.g. a scanned
  // RFID UID) against a credential previously provisioned via set_credential() or by a controller,
  // unlocking the door and emitting a LockOperation event with the matching credential attribution.
  void report_credential_unlock(CredentialTypeEnum type, uint16_t index);

private:
  DeviceDoorLock* door_lock_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  uint32_t feature_map;
  bool initialized;
};

#endif // MATTER_DOOR_LOCK_H
