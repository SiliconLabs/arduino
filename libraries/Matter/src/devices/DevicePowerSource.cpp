// SPDX-License-Identifier: MIT

#include "DevicePowerSource.h"

DevicePowerSource::DevicePowerSource(const char* device_name,
                                    uint8_t bat_percent_remaining) :
  Device(device_name),
  bat_percent_remaining(bat_percent_remaining)
{
  ;
}

uint8_t DevicePowerSource::GetBatPercentRemaining()
{
  return this->bat_percent_remaining;
}

void DevicePowerSource::SetBatPercentRemaining(uint8_t value)
{
  if (value < 0) {
    value = 0;
  } else if (value > 200) {
    value = 200;
  }

  bool changed = this->bat_percent_remaining != (value); // range 0-200
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new battery percent remaining='%d'", this->device_name, value);
  this->bat_percent_remaining = value;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatPercentRemaining);
  }
}

uint32_t DevicePowerSource::GetPowerSourceClusterFeatureMap()
{
  return this->power_source_cluster_feature_map;
}

uint16_t DevicePowerSource::GetPowerSourceClusterRevision()
{
  return this->power_source_cluster_revision;
}

EmberAfStatus DevicePowerSource::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                            chip::AttributeId attributeId,
                                                            uint8_t* buffer,
                                                            uint16_t maxReadLength)
{
  if (!this->reachable) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  using namespace ::chip::app::Clusters::PowerSource::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadPowerSourceAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::PowerSource::Id) {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  if ((attributeId == BatPercentRemaining::Id) && (maxReadLength == 1)) {
    uint8_t batPercentRemaining = this->GetBatPercentRemaining();
    memcpy(buffer, &batPercentRemaining, sizeof(batPercentRemaining));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetPowerSourceClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetPowerSourceClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return EMBER_ZCL_STATUS_FAILURE;
  }

  return EMBER_ZCL_STATUS_SUCCESS;
}

void DevicePowerSource::HandlePowerSourceDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & DevicePowerSource::kChanged_BatPercentRemaining) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatPercentRemaining::Id);
  }
}
