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

class DeviceLightbulb : public Device
{
public:
  enum Changed_t{
    kChanged_OnOff = kChanged_Last << 1,
    kChanged_Level = kChanged_Last << 2,
    kChanged_Color = kChanged_Last << 3,
    kChanged_StartupOnOff = kChanged_Last << 4,
  } Changed;

  enum StartupOnOff_t : uint8_t {
    kOff = 0,
    kOn = 1,
    kToggle = 2,
    kNull = 255,
  };

  DeviceLightbulb(const char* device_name);

  bool IsOn();
  void SetOnOff(bool onoff);
  void Toggle();
  uint8_t GetLevel();
  bool SetLevel(uint8_t level);
  void SetHue(uint8_t hue);
  uint8_t GetHue();
  void SetSaturation(uint8_t saturation);
  uint8_t GetSaturation();

  uint32_t GetOnoffClusterFeatureMap();
  uint32_t GetLevelControlClusterFeatureMap();
  uint32_t GetColorControlClusterFeatureMap();
  uint16_t GetOnoffClusterRevision();
  uint16_t GetLevelControlClusterRevision();
  uint16_t GetColorControlClusterRevision();

  uint8_t GetLevelControlMaxLevel();
  uint8_t GetLevelControlMinLevel();
  uint8_t GetLevelControlOptions();
  void SetLevelControlOptions(uint8_t options);
  uint8_t GetLevelControlOnLevel();
  void SetLevelControlOnLevel(uint8_t on_level);
  uint8_t GetLevelControlStartupCurrentLevel();
  void SetLevelControlStartupCurrentLevel(uint8_t startup_current_level);
  uint16_t GetLevelControlRemainingTime();

  uint8_t GetColorControlOptions();
  void SetColorControlOptions(uint8_t options);
  uint8_t GetColorControlColorMode();
  uint8_t GetColorControlEnhancedColorMode();
  uint8_t GetColorControlColorCapabilities();

  void SetStartupOnOff(StartupOnOff_t startup_on_off);
  StartupOnOff_t GetStartupOnOff();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer) override;

private:
  void HandleLightbulbDeviceStatusChanged(Changed_t itemChangedMask);

  bool onoff;
  bool global_scene_control;
  uint16_t on_time;
  uint16_t off_wait_time;
  uint8_t startup_on_off;

  uint8_t hue;
  uint8_t saturation;
  uint8_t level;

  uint8_t level_control_options;
  uint8_t level_control_on_level;
  uint8_t level_control_startup_current_level;
  uint8_t color_control_options;

  static const uint32_t onoff_cluster_feature_map         = 1u;   // Level control for lighting (bit 0) enabled
  static const uint32_t level_control_cluster_feature_map = 3u;   // On/Off (bit 0) and Lighting support (bit 1) enabled
  static const uint32_t color_control_cluster_feature_map = 1u;   // Hue/Saturation support (bit 0) enabled

  static const uint16_t onoff_cluster_revision = 4u;
  static const uint16_t level_control_cluster_revision = 5u;
  static const uint16_t color_control_cluster_revision = 6u;

  static const uint8_t level_control_min_level = 1u;
  static const uint8_t level_control_max_level = 254u;
  static const uint16_t level_control_remaining_time = 0u;

  static const uint8_t color_control_number_of_primaries = 0u;    // No color primaries
  static const uint8_t color_control_color_mode = 0u;             // Current hue and saturation determines the color
  static const uint8_t color_control_enhanced_color_mode = 0u;    // CurrentHue and CurrentSaturation
  static const uint16_t color_control_color_capabilities = color_control_cluster_feature_map & 0x000F;  // Bits 0-4 must be the same as the FeatureMap - all other bits must be 0
};
