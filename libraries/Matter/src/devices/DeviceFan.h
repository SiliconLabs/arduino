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

class DeviceFan : public Device
{
public:
  enum Changed_t{
    kChanged_PercentSetting = kChanged_Last << 1,
    kChanged_PercentCurrent = kChanged_Last << 2,
    kChanged_ModeSetting = kChanged_Last << 3,
  } Changed;

  DeviceFan(const char* device_name);

  uint8_t GetPercentSetting();
  void SetPercentSetting(uint8_t percent);
  uint8_t GetPercentCurrent();
  void SetPercentCurrent(uint8_t percent);

  void SetFanMode(uint8_t fan_mode);
  uint8_t GetFanMode();
  uint8_t GetFanModeSequence();
  uint8_t GetFanSpeedMax();
  uint32_t GetFanClusterFeatureMap();
  uint16_t GetFanClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

  enum fan_mode_t {
    Off,
    Low,
    Med,
    High,
    On,
    Auto,
    Smart
  };

private:
  void HandleFanDeviceStatusChanged(Changed_t itemChangedMask);

  uint8_t current_percent;
  fan_mode_t current_fan_mode;

  static const uint8_t fan_mode_sequence        = 2u;   // Off/Low/Med/High/Auto
  static const uint8_t fan_speed_max            = 100u; // 0-100%
  static const uint32_t fan_cluster_feature_map = 1u;   // 1-100 speeds supported
  static const uint16_t fan_cluster_revision    = 1u;
};
