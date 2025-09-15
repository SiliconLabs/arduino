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

class DeviceSwitch : public Device
{
public:
  enum Changed_t{
    kChanged_NumberOfPositions = kChanged_Last << 1,
    kChanged_CurrentPosition   = kChanged_Last << 2,
    kChanged_MultiPressMax     = kChanged_Last << 3,
  } Changed;

  DeviceSwitch(const char* device_name);

  void SetNumberOfPositions(uint8_t number_of_positions);
  void SetCurrentPosition(uint8_t current_position);
  void SetMultiPressMax(uint8_t multi_press_max);
  uint8_t GetNumberOfPositions();
  uint8_t GetCurrentPosition();
  uint8_t GetMultiPressMax();
  uint32_t GetFeatureMap();
  uint16_t GetSwitchClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

private:
  void HandleSwitchDeviceStatusChanged(Changed_t itemChangedMask);

  uint8_t number_of_positions;
  uint8_t current_position;
  uint8_t multi_press_max;

  static const uint32_t switch_cluster_feature_map = 6u;   // Momentary switch and release support enabled
  static const uint16_t switch_cluster_revision    = 1u;
};
