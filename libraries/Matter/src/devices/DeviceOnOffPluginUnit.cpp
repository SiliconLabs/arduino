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

#include "DeviceOnOffPluginUnit.h"

DeviceOnOffPluginUnit::DeviceOnOffPluginUnit(const char* device_name) :
  Device(device_name),
  is_on(false)
{
  this->SetDeviceType(device_type_t::kDeviceType_OnOffPluginUnit);
}

bool DeviceOnOffPluginUnit::IsOn()
{
  return this->is_on;
}

void DeviceOnOffPluginUnit::SetOnOff(bool onoff)
{
  bool changed = onoff ^ this->is_on;
  this->is_on = onoff;
  ChipLogProgress(DeviceLayer, "DeviceOnOffPluginUnit[%s]: %s", this->device_name, onoff ? "ON" : "OFF");
  if (changed) {
    this->HandleDeviceOnOffPluginUnitStatusChanged(kChanged_OnOff);
    CallDeviceChangeCallback();
  }
}

void DeviceOnOffPluginUnit::Toggle()
{
  bool inverse_state = !this->IsOn();
  this->SetOnOff(inverse_state);
}

uint32_t DeviceOnOffPluginUnit::GetOnoffClusterFeatureMap()
{
  return this->onoff_cluster_feature_map;
}

uint16_t DeviceOnOffPluginUnit::GetOnoffClusterRevision()
{
  return this->onoff_cluster_revision;
}

CHIP_ERROR DeviceOnOffPluginUnit::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                             chip::AttributeId attributeId,
                                                             uint8_t* buffer,
                                                             uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters;
  ChipLogProgress(DeviceLayer, "HandleReadOnOffPluginUnitAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId == OnOff::Id) {
    if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1)) {
      *buffer = this->IsOn() ? 1 : 0;
    } else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
      uint16_t rev = this->GetOnoffClusterRevision();
      memcpy(buffer, &rev, sizeof(rev));
    } else if ((attributeId == OnOff::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
      uint32_t featureMap = this->GetOnoffClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadOnOffPluginUnitAttribute - OnOff: invalid read");
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceOnOffPluginUnit::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                              chip::AttributeId attributeId,
                                                              uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters;
  ChipLogProgress(DeviceLayer, "HandleWriteOnOffPluginUnitAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if ((attributeId == OnOff::Attributes::OnOff::Id) && (clusterId == OnOff::Id)) {
    if (*buffer) {
      this->SetOnOff(true);
    } else {
      this->SetOnOff(false);
    }
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceOnOffPluginUnit::HandleDeviceOnOffPluginUnitStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_OnOff) {
    ScheduleMatterReportingCallback(this->endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
  }
}
