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

#include "DeviceOccupancySensor.h"

DeviceOccupancySensor::DeviceOccupancySensor(const char* device_name) :
  Device(device_name),
  occupancy(false)
{
  this->SetDeviceType(device_type_t::kDeviceType_OccupancySensor);
}

bool DeviceOccupancySensor::GetOccupancy()
{
  return this->occupancy;
};

void DeviceOccupancySensor::SetOccupancy(bool occupied)
{
  bool changed = this->occupancy != occupied;
  ChipLogProgress(DeviceLayer, "OccupancySensorDevice[%s]: New state='%u'", this->device_name, occupied);
  this->occupancy = occupied;

  if (changed) {
    this->HandleOccupancySensorDeviceStatusChanged(kChanged_OccupancyValue);
    CallDeviceChangeCallback();
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

CHIP_ERROR DeviceOccupancySensor::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                             chip::AttributeId attributeId,
                                                             uint8_t* buffer,
                                                             uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::OccupancySensing::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadOccupancySensingAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::OccupancySensing::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == Occupancy::Id) && (maxReadLength == 1)) {
    bool occupied = this->GetOccupancy();
    memcpy(buffer, &occupied, sizeof(occupied));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetOccupancySensorClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetOccupancySensorClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceOccupancySensor::HandleOccupancySensorDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_OccupancyValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, OccupancySensing::Id, OccupancySensing::Attributes::Occupancy::Id);
  }
}
