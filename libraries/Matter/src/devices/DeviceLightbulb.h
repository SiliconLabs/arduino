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

#pragma once

#include "MatterDevice.h"

class DeviceLightbulb : public Device
{
public:
  enum Changed_t{
    kChanged_OnOff = kChanged_Last << 1,
    kChanged_Level = kChanged_Last << 2,
    kChanged_Color = kChanged_Last << 3,
  } Changed;

  DeviceLightbulb(const char* deviceName, std::string location);
  using DeviceCallback_fn = std::function<void(DeviceLightbulb*, DeviceLightbulb::Changed_t)>;

  bool IsOn();
  void SetOnOff(bool onoff);
  void Toggle();
  uint8_t GetLevel();
  void SetLevel(uint8_t level);
  void SetHue(uint8_t hue);
  uint8_t GetHue();
  void SetSaturation(uint8_t saturation);
  uint8_t GetSaturation();
  void SetChangeCallback(DeviceCallback_fn device_changed_callback);

  uint32_t GetOnoffClusterFeatureMap();
  uint32_t GetLevelControlClusterFeatureMap();
  uint32_t GetColorControlClusterFeatureMap();
  uint16_t GetOnoffClusterRevision();
  uint16_t GetLevelControlClusterRevision();
  uint16_t GetColorControlClusterRevision();

  uint8_t GetLevelControlMaxLevel();
  uint8_t GetLevelControlMinLevel();
  uint8_t GetLevelControlOptions();
  uint8_t GetLevelControlOnLevel();
  uint8_t GetLevelControlStartupCurrentLevel();
  uint16_t GetLevelControlRemainingTime();

  uint8_t GetColorControlOptions();
  uint8_t GetColorControlColorMode();
  uint8_t GetColorControlEnhancedColorMode();
  uint8_t GetColorControlColorCapabilities();

private:
  void HandleDeviceChange(Device* device, Device::Changed_t change_mask);

  bool onoff;
  uint8_t hue;
  uint8_t saturation;
  uint8_t level;
  DeviceCallback_fn device_changed_callback;

  static const uint32_t onoff_cluster_feature_map         = 1u;   // Level control for lighting (bit 0) enabled
  static const uint32_t level_control_cluster_feature_map = 3u;   // On/Off (bit 0) and Lighting support (bit 1) enabled
  static const uint32_t color_control_cluster_feature_map = 1u;   // Hue/Saturation support (bit 0) enabled

  static const uint16_t onoff_cluster_revision = 4u;
  static const uint16_t level_control_cluster_revision = 5u;
  static const uint16_t color_control_cluster_revision = 5u;

  static const uint8_t level_control_min_level = 1u;
  static const uint8_t level_control_max_level = 254u;
  static const uint8_t level_control_options = 0u;                // No extra options enabled
  static const uint8_t level_control_on_level = 255u;
  static const uint8_t level_control_startup_current_level = 255;
  static const uint16_t level_control_remaining_time = 0u;

  static const uint8_t color_control_options = 0u;                // No extra options enabled
  static const uint8_t color_control_color_mode = 0u;             // Current hue and saturation determines the color
  static const uint8_t color_control_enhanced_color_mode = 0u;    // CurrentHue and CurrentSaturation
  static const uint16_t color_control_color_capabilities = color_control_cluster_feature_map & 0x000F;  // Bits 0-4 must be the same as the FeatureMap - all other bits must be 0
};
