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

#include "DeviceLightbulb.h"

DeviceLightbulb::DeviceLightbulb(const char* device_name, std::string location) :
  Device(device_name, location),
  onoff(false),
  hue(0),
  saturation(0),
  level(50),
  device_changed_callback(nullptr)
{
  ;
}

bool DeviceLightbulb::IsOn()
{
  return this->onoff;
}

void DeviceLightbulb::SetOnOff(bool onoff)
{
  bool changed = onoff ^ this->onoff;
  this->onoff = onoff;
  ChipLogProgress(DeviceLayer, "DeviceLightbulb[%s]: %s", mName, onoff ? "ON" : "OFF");
  if ((changed) && (this->device_changed_callback)) {
    this->device_changed_callback(this, kChanged_OnOff);
  }
}

void DeviceLightbulb::Toggle()
{
  bool inverse_state = !this->IsOn();
  this->SetOnOff(inverse_state);
}

uint8_t DeviceLightbulb::GetLevel()
{
  return this->level;
}

void DeviceLightbulb::SetLevel(uint8_t level)
{
  if (this->level == level) {
    return;
  }

  this->level = level;

  if (this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_Level);
  }
}

void DeviceLightbulb::SetHue(uint8_t hue)
{
  if (this->hue == hue) {
    return;
  }

  this->hue = hue;

  if (this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_Color);
  }
}

uint8_t DeviceLightbulb::GetHue()
{
  return this->hue;
}

void DeviceLightbulb::SetSaturation(uint8_t saturation)
{
  if (this->saturation == saturation) {
    return;
  }

  this->saturation = saturation;

  if (this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_Color);
  }
}

uint8_t DeviceLightbulb::GetSaturation()
{
  return this->saturation;
}

void DeviceLightbulb::SetChangeCallback(DeviceCallback_fn device_changed_callback)
{
  this->device_changed_callback = device_changed_callback;
}

void DeviceLightbulb::HandleDeviceChange(Device* device, Device::Changed_t change_mask)
{
  if (this->device_changed_callback) {
    this->device_changed_callback(this, (DeviceLightbulb::Changed_t) change_mask);
  }
}

uint32_t DeviceLightbulb::GetOnoffClusterFeatureMap()
{
  return this->onoff_cluster_feature_map;
}

uint32_t DeviceLightbulb::GetLevelControlClusterFeatureMap()
{
  return this->level_control_cluster_feature_map;
}

uint32_t DeviceLightbulb::GetColorControlClusterFeatureMap()
{
  return this->color_control_cluster_feature_map;
}

uint16_t DeviceLightbulb::GetOnoffClusterRevision()
{
  return this->onoff_cluster_revision;
}

uint16_t DeviceLightbulb::GetLevelControlClusterRevision()
{
  return this->level_control_cluster_revision;
}

uint16_t DeviceLightbulb::GetColorControlClusterRevision()
{
  return this->color_control_cluster_revision;
}

uint8_t DeviceLightbulb::GetLevelControlMaxLevel()
{
  return this->level_control_max_level;
}

uint8_t DeviceLightbulb::GetLevelControlMinLevel()
{
  return this->level_control_min_level;
}

uint8_t DeviceLightbulb::GetLevelControlOptions()
{
  return this->level_control_options;
}

uint8_t DeviceLightbulb::GetLevelControlOnLevel()
{
  return this->level_control_on_level;
}

uint8_t DeviceLightbulb::GetLevelControlStartupCurrentLevel()
{
  return this->level_control_startup_current_level;
}

uint16_t DeviceLightbulb::GetLevelControlRemainingTime()
{
  return this->level_control_remaining_time;
}

uint8_t DeviceLightbulb::GetColorControlOptions()
{
  return this->color_control_options;
}

uint8_t DeviceLightbulb::GetColorControlColorMode()
{
  return this->color_control_color_mode;
}

uint8_t DeviceLightbulb::GetColorControlEnhancedColorMode()
{
  return this->color_control_enhanced_color_mode;
}

uint8_t DeviceLightbulb::GetColorControlColorCapabilities()
{
  return this->color_control_color_capabilities;
}
