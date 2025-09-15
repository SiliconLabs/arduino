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

#pragma once

#include "MatterDevice.h"

class DeviceDoorLock : public Device
{
public:
  enum Changed_t{
    kChanged_LockState = kChanged_Last << 1,
  } Changed;

  DeviceDoorLock(const char* device_name);

  enum lock_state_t : uint8_t {
    NOT_FULLY_LOCKED = 0,
    LOCKED,
    UNLOCKED,
    UNLATCHED,
    UNKNOWN = 255,
  };

  lock_state_t GetLockState();
  void SetLockState(lock_state_t state);

  uint32_t GetDoorLockClusterFeatureMap();
  uint16_t GetDoorLockClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

private:
  void HandleDoorLockDeviceStatusChanged(Changed_t itemChangedMask);

  lock_state_t lock_state;

  static const uint8_t lock_type = 2u;                      // Other type
  static const uint8_t actuator_enabled = 1u;               // Actuator enabled - able to process lock/unlock commands
  static const uint8_t operating_mode = 0u;                 // Normal
  static const uint16_t supported_operating_modes = 0xFFF6; // Normal and NoRemoteUnlock supported

  static const uint32_t door_lock_cluster_feature_map = 0u;   // No additional features enabled
  static const uint16_t door_lock_cluster_revision    = 7u;
};
