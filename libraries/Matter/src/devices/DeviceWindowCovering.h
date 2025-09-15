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

class DeviceWindowCovering : public Device
{
public:
  enum Changed_t{
    kChanged_LiftPosition = kChanged_Last << 1,
    kChanged_LiftPositionCurrentPercent = kChanged_Last << 2,
    kChanged_LiftPositionTargetPercent = kChanged_Last << 3,
    kChanged_OperationalStatus = kChanged_Last << 4,
  } Changed;

  enum OperationalStatus_t{
    kOperationalStatus_Opening = 0x00,
    kOperationalStatus_Closing,
    kOperationalStatus_Stopped,
  } OperationalStatus;

  DeviceWindowCovering(const char* device_name);

  void SetRequestedLiftPosition(uint16_t lift_position);
  uint16_t GetRequestedLiftPosition();

  void SetActualLiftPosition(uint16_t lift_position);
  uint16_t GetActualLiftPosition();

  void SetCurrentOperationalStatus(OperationalStatus_t operational_status);

  uint32_t GetWindowCoveringClusterFeatureMap();
  uint16_t GetWindowCoveringClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  static const uint16_t max_lift_position = 10000u;

private:
  void HandleWindowCoveringDeviceStatusChanged(Changed_t itemChangedMask);

  uint8_t current_operational_status;
  uint16_t requested_lift_pos;
  uint16_t actual_lift_pos;

  static const uint32_t window_covering_cluster_feature_map = 5u; // 'Lift' and 'Position Aware Lift' feature is supported
  static const uint16_t window_covering_cluster_revision    = 5u;
};
