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

#include "DeviceFan.h"

DeviceFan::DeviceFan(const char* device_name,
                     std::string location) :
  Device(device_name, location),
  current_percent(0),
  current_fan_mode(fan_mode_t::Off),
  device_changed_callback(nullptr)
{
  ;
}

uint8_t DeviceFan::GetPercentSetting()
{
  return this->current_percent;
}

void DeviceFan::SetPercentSetting(uint8_t percent)
{
  if (percent > 100) {
    percent = 100;
  }

  bool changed = this->current_percent != percent;
  ChipLogProgress(DeviceLayer, "FanDevice[%s]: new percent='%d'", mName, percent);
  this->current_percent = percent;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_PercentSetting);
    this->device_changed_callback(this, kChanged_PercentCurrent);
  }
}

uint8_t DeviceFan::GetPercentCurrent()
{
  return this->current_percent;
}

void DeviceFan::SetPercentCurrent(uint8_t percent)
{
  (void)percent;
}

void DeviceFan::SetChangeCallback(DeviceCallback_fn device_changed_callback)
{
  this->device_changed_callback = device_changed_callback;
}

void DeviceFan::HandleDeviceChange(Device* device, Device::Changed_t change_mask)
{
  if (this->device_changed_callback) {
    this->device_changed_callback(this, (DeviceFan::Changed_t) change_mask);
  }
}

void DeviceFan::SetFanMode(uint8_t fan_mode)
{
  bool changed = this->current_fan_mode != fan_mode;
  ChipLogProgress(DeviceLayer, "FanDevice[%s]: new mode='%d'", mName, fan_mode);
  this->current_fan_mode = (fan_mode_t)fan_mode;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_ModeSetting);
  }

  if (!changed) {
    return;
  }

  // Adjust the percentage to the selected mode
  switch ((fan_mode_t)fan_mode) {
    case fan_mode_t::Low:
      this->SetPercentSetting(20);
      break;

    case fan_mode_t::Med:
      this->SetPercentSetting(50);
      break;

    case fan_mode_t::High:
      this->SetPercentSetting(100);
      break;

    default:
      break;
  }
}

uint8_t DeviceFan::GetFanMode()
{
  return this->current_fan_mode;
}

uint8_t DeviceFan::GetFanModeSequence()
{
  return this->fan_mode_sequence;
}

uint32_t DeviceFan::GetFanClusterFeatureMap()
{
  return this->fan_cluster_feature_map;
}

uint16_t DeviceFan::GetFanClusterRevision()
{
  return this->fan_cluster_revision;
}
