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

#include "DeviceSwitch.h"

DeviceSwitch::DeviceSwitch(const char* device_name,
                           std::string location) :
  Device(device_name, location),
  number_of_positions(2),
  current_position(0),
  multi_press_max(2),
  device_changed_callback(nullptr)
{
  ;
}

void DeviceSwitch::SetNumberOfPositions(uint8_t number_of_positions)
{
  bool changed = this->number_of_positions != number_of_positions;
  this->number_of_positions = number_of_positions;
  if ((changed) && (this->device_changed_callback)) {
    this->device_changed_callback(this, kChanged_NumberOfPositions);
  }
}

void DeviceSwitch::SetCurrentPosition(uint8_t current_position)
{
  bool changed = this->current_position != current_position;
  this->current_position = current_position;
  if ((changed) && (this->device_changed_callback)) {
    this->device_changed_callback(this, kChanged_CurrentPosition);
  }
}

void DeviceSwitch::SetMultiPressMax(uint8_t multi_press_max)
{
  bool changed = this->multi_press_max != multi_press_max;
  this->multi_press_max = multi_press_max;
  if ((changed) && (this->device_changed_callback)) {
    this->device_changed_callback(this, kChanged_MultiPressMax);
  }
}

uint8_t DeviceSwitch::GetNumberOfPositions()
{
  return this->number_of_positions;
}

uint8_t DeviceSwitch::GetCurrentPosition()
{
  return this->current_position;
}

uint8_t DeviceSwitch::GetMultiPressMax()
{
  return this->multi_press_max;
}

uint32_t DeviceSwitch::GetFeatureMap()
{
  return this->switch_cluster_feature_map;
}

uint16_t DeviceSwitch::GetSwitchClusterRevision()
{
  return this->switch_cluster_revision;
}

void DeviceSwitch::SetChangeCallback(DeviceCallback_fn device_changed_callback)
{
  this->device_changed_callback = device_changed_callback;
}

void DeviceSwitch::HandleDeviceChange(Device* device, Device::Changed_t change_mask)
{
  if (this->device_changed_callback) {
    this->device_changed_callback(this, (DeviceSwitch::Changed_t) change_mask);
  }
}
