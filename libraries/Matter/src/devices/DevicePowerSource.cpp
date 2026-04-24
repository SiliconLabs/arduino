/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2026 Silicon Laboratories Inc. www.silabs.com
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

#include "DevicePowerSource.h"

using namespace ::chip;
using namespace ::chip::app::Clusters;

const uint32_t DevicePowerSource::power_source_cluster_feature_map = static_cast<uint32_t>(PowerSource::Feature::kBattery);

DevicePowerSource::DevicePowerSource(const char* device_name) :
  Device(device_name),
  status(PowerSource::PowerSourceStatusEnum::kActive),
  order(0u),
  bat_voltage(0u),
  bat_percent_remaining(0u),
  bat_time_remaining(0u),
  bat_charge_level(PowerSource::BatChargeLevelEnum::kOk),
  bat_replacement_needed(false),
  bat_replaceability(PowerSource::BatReplaceabilityEnum::kUserReplaceable),
  bat_present(true)
{
  this->SetDeviceType(device_type_t::kDeviceType_PowerSource);
  chip::Platform::CopyString(this->description, "Battery");
}

PowerSource::PowerSourceStatusEnum DevicePowerSource::GetStatus()
{
  return this->status;
}

void DevicePowerSource::SetStatus(PowerSource::PowerSourceStatusEnum status)
{
  bool changed = this->status != status;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new status='%u'", this->device_name, static_cast<uint8_t>(status));
  this->status = status;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_Status);
    CallDeviceChangeCallback();
  }
}

uint8_t DevicePowerSource::GetOrder()
{
  return this->order;
}

void DevicePowerSource::SetOrder(uint8_t order)
{
  bool changed = this->order != order;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new order='%u'", this->device_name, order);
  this->order = order;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_Order);
    CallDeviceChangeCallback();
  }
}

const char* DevicePowerSource::GetDescription()
{
  return this->description;
}

void DevicePowerSource::SetDescription(const char* description)
{
  bool changed = (strncmp(this->description, description, sizeof(this->description)) != 0);
  if (!changed) {
    return;
  }

  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new description='%s'", this->device_name, description);
  chip::Platform::CopyString(this->description, description);
  this->HandlePowerSourceDeviceStatusChanged(kChanged_Description);
  CallDeviceChangeCallback();
}

uint32_t DevicePowerSource::GetBatVoltage()
{
  return this->bat_voltage;
}

void DevicePowerSource::SetBatVoltage(uint32_t voltage)
{
  bool changed = this->bat_voltage != voltage;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat voltage='%lu'", this->device_name, static_cast<uint32_t>(voltage));
  this->bat_voltage = voltage;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatVoltage);
    CallDeviceChangeCallback();
  }
}

uint8_t DevicePowerSource::GetBatPercentRemaining()
{
  return this->bat_percent_remaining;
}

void DevicePowerSource::SetBatPercentRemaining(uint8_t percent_remaining)
{
  bool changed = this->bat_percent_remaining != percent_remaining;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat percent remaining='%u'", this->device_name, percent_remaining);
  this->bat_percent_remaining = percent_remaining;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatPercentRemaining);
    CallDeviceChangeCallback();
  }
}

uint32_t DevicePowerSource::GetBatTimeRemaining()
{
  return this->bat_time_remaining;
}

void DevicePowerSource::SetBatTimeRemaining(uint32_t time_remaining)
{
  bool changed = this->bat_time_remaining != time_remaining;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat time remaining='%lu'", this->device_name, static_cast<uint32_t>(time_remaining));
  this->bat_time_remaining = time_remaining;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatTimeRemaining);
    CallDeviceChangeCallback();
  }
}

PowerSource::BatChargeLevelEnum DevicePowerSource::GetBatChargeLevel()
{
  return this->bat_charge_level;
}

void DevicePowerSource::SetBatChargeLevel(PowerSource::BatChargeLevelEnum charge_level)
{
  bool changed = this->bat_charge_level != charge_level;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat charge level='%u'", this->device_name, static_cast<uint8_t>(charge_level));
  this->bat_charge_level = charge_level;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatChargeLevel);
    CallDeviceChangeCallback();
  }
}

bool DevicePowerSource::GetBatReplacementNeeded()
{
  return this->bat_replacement_needed;
}

void DevicePowerSource::SetBatReplacementNeeded(bool replacement_needed)
{
  bool changed = this->bat_replacement_needed != replacement_needed;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat replacement needed='%u'", this->device_name, replacement_needed ? 1u : 0u);
  this->bat_replacement_needed = replacement_needed;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatReplacementNeeded);
    CallDeviceChangeCallback();
  }
}

PowerSource::BatReplaceabilityEnum DevicePowerSource::GetBatReplaceability()
{
  return this->bat_replaceability;
}

void DevicePowerSource::SetBatReplaceability(PowerSource::BatReplaceabilityEnum replaceability)
{
  bool changed = this->bat_replaceability != replaceability;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat replaceability='%u'", this->device_name, static_cast<uint8_t>(replaceability));
  this->bat_replaceability = replaceability;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatReplaceability);
    CallDeviceChangeCallback();
  }
}

bool DevicePowerSource::GetBatPresent()
{
  return this->bat_present;
}

void DevicePowerSource::SetBatPresent(bool present)
{
  bool changed = this->bat_present != present;
  ChipLogProgress(DeviceLayer, "PowerSourceDevice[%s]: new bat present='%u'", this->device_name, present ? 1u : 0u);
  this->bat_present = present;

  if (changed) {
    this->HandlePowerSourceDeviceStatusChanged(kChanged_BatPresent);
    CallDeviceChangeCallback();
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

CHIP_ERROR DevicePowerSource::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                         chip::AttributeId attributeId,
                                                         uint8_t* buffer,
                                                         uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::PowerSource::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadPowerSourceAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::PowerSource::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == Status::Id) && (maxReadLength >= 1)) {
    uint8_t status = static_cast<uint8_t>(this->GetStatus());
    memcpy(buffer, &status, sizeof(status));
  } else if ((attributeId == Order::Id) && (maxReadLength >= 1)) {
    uint8_t order = this->GetOrder();
    memcpy(buffer, &order, sizeof(order));
  } else if ((attributeId == Description::Id) && (maxReadLength >= kDescriptionSize)) {
    MutableByteSpan zclDescriptionSpan(buffer, kDescriptionSize);
    MakeZclCharString(zclDescriptionSpan, this->GetDescription());
  } else if ((attributeId == BatVoltage::Id) && (maxReadLength >= 4)) {
    uint32_t batVoltage = this->GetBatVoltage();
    memcpy(buffer, &batVoltage, sizeof(batVoltage));
  } else if ((attributeId == BatPercentRemaining::Id) && (maxReadLength >= 1)) {
    uint8_t batPercentRemaining = this->GetBatPercentRemaining();
    memcpy(buffer, &batPercentRemaining, sizeof(batPercentRemaining));
  } else if ((attributeId == BatTimeRemaining::Id) && (maxReadLength >= 4)) {
    uint32_t batTimeRemaining = this->GetBatTimeRemaining();
    memcpy(buffer, &batTimeRemaining, sizeof(batTimeRemaining));
  } else if ((attributeId == BatChargeLevel::Id) && (maxReadLength >= 1)) {
    uint8_t batChargeLevel = static_cast<uint8_t>(this->GetBatChargeLevel());
    memcpy(buffer, &batChargeLevel, sizeof(batChargeLevel));
  } else if ((attributeId == BatReplacementNeeded::Id) && (maxReadLength >= 1)) {
    uint8_t batReplacementNeeded = this->GetBatReplacementNeeded() ? 1u : 0u;
    memcpy(buffer, &batReplacementNeeded, sizeof(batReplacementNeeded));
  } else if ((attributeId == BatReplaceability::Id) && (maxReadLength >= 1)) {
    uint8_t batReplaceability = static_cast<uint8_t>(this->GetBatReplaceability());
    memcpy(buffer, &batReplaceability, sizeof(batReplaceability));
  } else if ((attributeId == BatPresent::Id) && (maxReadLength >= 1)) {
    uint8_t batPresent = this->GetBatPresent() ? 1u : 0u;
    memcpy(buffer, &batPresent, sizeof(batPresent));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength >= 4)) {
    uint32_t featureMap = this->GetPowerSourceClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength >= 2)) {
    uint16_t clusterRevision = this->GetPowerSourceClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DevicePowerSource::HandlePowerSourceDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_Status) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::Status::Id);
  }
  if (itemChangedMask & kChanged_Order) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::Order::Id);
  }
  if (itemChangedMask & kChanged_Description) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::Description::Id);
  }
  if (itemChangedMask & kChanged_BatVoltage) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatVoltage::Id);
  }
  if (itemChangedMask & kChanged_BatPercentRemaining) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatPercentRemaining::Id);
  }
  if (itemChangedMask & kChanged_BatTimeRemaining) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatTimeRemaining::Id);
  }
  if (itemChangedMask & kChanged_BatChargeLevel) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatChargeLevel::Id);
  }
  if (itemChangedMask & kChanged_BatReplacementNeeded) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatReplacementNeeded::Id);
  }
  if (itemChangedMask & kChanged_BatReplaceability) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatReplaceability::Id);
  }
  if (itemChangedMask & kChanged_BatPresent) {
    ScheduleMatterReportingCallback(this->endpoint_id, PowerSource::Id, PowerSource::Attributes::BatPresent::Id);
  }
}
