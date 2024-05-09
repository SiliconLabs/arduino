// SPDX-License-Identifier: MIT

#pragma once

#include "MatterDevice.h"

class DevicePowerSource : public Device
{
public:
  enum Changed_t{
    kChanged_BatPercentRemaining = kChanged_Last << 1,
  } Changed;

  DevicePowerSource(const char* device_name, uint8_t bat_percent_remaining);

  uint8_t GetBatPercentRemaining();
  void SetBatPercentRemaining(uint8_t percent);
  uint32_t GetPowerSourceClusterFeatureMap();
  uint16_t GetPowerSourceClusterRevision();

  EmberAfStatus HandleReadEmberAfAttribute(ClusterId clusterId,
                                           chip::AttributeId attributeId,
                                           uint8_t* buffer,
                                           uint16_t maxReadLength) override;

private:
  void HandlePowerSourceDeviceStatusChanged(Changed_t itemChangedMask);

  uint8_t bat_percent_remaining;

  static const uint32_t power_source_cluster_feature_map = 0u;   // No additional features enabled
  static const uint16_t power_source_cluster_revision    = 1u;
};
