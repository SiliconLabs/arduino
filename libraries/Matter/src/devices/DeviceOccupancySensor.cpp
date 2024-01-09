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

#include "DeviceOccupancySensor.h"

DeviceOccupancySensor::DeviceOccupancySensor(const char* device_name, std::string location) :
  Device(device_name, location),
  device_changed_callback(nullptr)
{
}

bool DeviceOccupancySensor::GetOccupancy()
{
  return this->occupancy;
};

void DeviceOccupancySensor::SetOccupancy(bool occupied)
{
  bool changed = this->occupancy != occupied;
  ChipLogProgress(DeviceLayer, "OccupancySensorDevice[%s]: New state='%u'", mName, occupied);
  this->occupancy = occupied;

  if (changed && this->device_changed_callback) {
    this->device_changed_callback(this, kChanged_OccupancyValue);
  }
}

void DeviceOccupancySensor::SetChangeCallback(DeviceCallback_fn device_changed_callback)
{
  this->device_changed_callback = device_changed_callback;
}

void DeviceOccupancySensor::HandleDeviceChange(Device* device, Device::Changed_t change_mask)
{
  if (this->device_changed_callback) {
    this->device_changed_callback(this, (DeviceOccupancySensor::Changed_t) change_mask);
  }
}

uint32_t DeviceOccupancySensor::GetOccupancySensorClusterFeatureMap()
{
  return this->occupancy_sensor_cluster_feature_map;
}

uint16_t DeviceOccupancySensor::GetOccupancySensorClusterRevision()
{
  return this->occupancy_sensor_cluster_revision;
}
