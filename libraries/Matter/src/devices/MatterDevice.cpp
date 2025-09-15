/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/* This file is part of the Silicon Labs Arduino Core */

#include "MatterDevice.h"

#include <cstdio>
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/callback.h>

using namespace ::chip;
using namespace chip::app::Clusters::Actions;

Device::Device(const char* device_name) :
  reachable(false),
  online(false),
  device_type(kDeviceType_Unspecified),
  identify_in_progress(false),
  identify_time(0),
  identify_type(0),
  groups_cluster_name_support(0),
  location(""),
  endpoint_id(0),
  device_change_callback(nullptr)
{
  chip::Platform::CopyString(this->device_name, device_name);
  chip::Platform::CopyString(this->vendor_name, "Silicon Labs");
  chip::Platform::CopyString(this->product_name, "Matter device");
  chip::Platform::CopyString(this->serial_number, "0000000042");
}

bool Device::IsReachable()
{
  return this->reachable;
}

bool Device::IsOnline()
{
  return this->online;
}

void Device::SetOnline(bool online)
{
  this->online = online;
}

void Device::SetReachable(bool reachable)
{
  bool changed = (this->reachable != reachable);
  if (changed) {
    this->reachable = reachable;
    if (reachable) {
      ChipLogProgress(DeviceLayer, "Device[%s]: ONLINE", this->device_name);
    } else {
      ChipLogProgress(DeviceLayer, "Device[%s]: OFFLINE", this->device_name);
    }
    this->HandleDeviceStatusChanged(kChanged_Reachable);
  }
}

void Device::SetName(const char* name)
{
  bool changed = (strncmp(this->device_name, name, sizeof(this->device_name)) != 0);
  if (changed) {
    ChipLogProgress(DeviceLayer, "Device[%s]: New DeviceName=\"%s\"", this->device_name, name);
    chip::Platform::CopyString(this->device_name, name);
    this->HandleDeviceStatusChanged(kChanged_Name);
  }
}

void Device::SetVendorName(const char* vendorname)
{
  bool changed = (strncmp(this->vendor_name, vendorname, sizeof(this->vendor_name)) != 0);
  if (changed) {
    ChipLogProgress(DeviceLayer, "Device[%s]: New VendorName=\"%s\"", this->device_name, vendorname);
    chip::Platform::CopyString(this->vendor_name, vendorname);
    this->HandleDeviceStatusChanged(kChanged_VendorName);
  }
}

void Device::SetProductName(const char* productname)
{
  bool changed = (strncmp(this->product_name, productname, sizeof(this->product_name)) != 0);
  if (changed) {
    ChipLogProgress(DeviceLayer, "Device[%s]: New ProductName=\"%s\"", this->device_name, productname);
    chip::Platform::CopyString(this->product_name, productname);
    this->HandleDeviceStatusChanged(kChanged_ProductName);
  }
}

void Device::SetSerialNumber(const char* serialnumber)
{
  bool changed = (strncmp(this->serial_number, serialnumber, sizeof(this->serial_number)) != 0);
  if (changed) {
    ChipLogProgress(DeviceLayer, "Device[%s]: New SerialNumber=\"%s\"", this->device_name, serialnumber);
    chip::Platform::CopyString(this->serial_number, serialnumber);
    this->HandleDeviceStatusChanged(kChanged_SerialNumber);
  }
}

void Device::SetLocation(std::string location)
{
  bool changed = (this->location.compare(location) != 0);
  if (changed) {
    this->location = location;
    ChipLogProgress(DeviceLayer, "Device[%s]: New location=\"%s\"", this->device_name, this->location.c_str());
    this->HandleDeviceStatusChanged(kChanged_Location);
  }
}

void Device::SetDeviceChangeCallback(void (*matter_device_change_cb)(void))
{
  this->device_change_callback = matter_device_change_cb;
}

void Device::CallDeviceChangeCallback()
{
  if (this->device_change_callback) {
    this->device_change_callback();
  }
}

uint32_t Device::GetBridgedDeviceBasicInformationClusterFeatureMap()
{
  return this->bridged_device_basic_information_cluster_feature_map;
}

uint16_t Device::GetBridgedDeviceBasicInformationClusterRevision()
{
  return this->bridged_device_basic_information_cluster_revision;
}

CHIP_ERROR Device::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                               chip::AttributeId attributeId,
                                               uint8_t* buffer)
{
  return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR Device::HandleReadEmberAfAttribute(ClusterId clusterId,
                                              chip::AttributeId attributeId,
                                              uint8_t* buffer,
                                              uint16_t maxReadLength)
{
  return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR Device::HandleReadBridgedDeviceBasicAttribute(ClusterId clusterId,
                                                         chip::AttributeId attributeId,
                                                         uint8_t * buffer,
                                                         uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::BridgedDeviceBasicInformation::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadBridgedDeviceBasicAttribute: clusterId='%lu' attrId=%ld, maxReadLength=%d", clusterId, attributeId, maxReadLength);

  if (clusterId != chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return CHIP_ERROR_INTERNAL;
  }

  if ((attributeId == Reachable::Id) && (maxReadLength == 1)) {
    *buffer = this->IsReachable() ? 1 : 0;
  } else if ((attributeId == NodeLabel::Id) && (maxReadLength == 32)) {
    MutableByteSpan zclNameSpan(buffer, maxReadLength);
    MakeZclCharString(zclNameSpan, this->GetName());
  } else if ((attributeId == VendorName::Id) && (maxReadLength == 32)) {
    MutableByteSpan zclNameSpan(buffer, maxReadLength);
    MakeZclCharString(zclNameSpan, this->GetVendorName());
  } else if ((attributeId == ProductName::Id) && (maxReadLength == 32)) {
    MutableByteSpan zclNameSpan(buffer, maxReadLength);
    MakeZclCharString(zclNameSpan, this->GetProductName());
  } else if ((attributeId == SerialNumber::Id) && (maxReadLength == 32)) {
    MutableByteSpan zclNameSpan(buffer, maxReadLength);
    MakeZclCharString(zclNameSpan, this->GetSerialNumber());
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t rev = this->GetBridgedDeviceBasicInformationClusterRevision();
    memcpy(buffer, &rev, sizeof(rev));
  } else if ((attributeId == FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->GetBridgedDeviceBasicInformationClusterFeatureMap();
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else {
    return CHIP_ERROR_INTERNAL;
  }

  return CHIP_NO_ERROR;
}

void Device::HandleDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_Reachable) {
    ScheduleMatterReportingCallback(this->endpoint_id, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::Reachable::Id);
  }
  if (itemChangedMask & kChanged_Name) {
    ScheduleMatterReportingCallback(this->endpoint_id, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::NodeLabel::Id);
  }
  if (itemChangedMask & kChanged_VendorName) {
    ScheduleMatterReportingCallback(this->endpoint_id, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::VendorName::Id);
  }
  if (itemChangedMask & kChanged_ProductName) {
    ScheduleMatterReportingCallback(this->endpoint_id, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::ProductName::Id);
  }
  if (itemChangedMask & kChanged_SerialNumber) {
    ScheduleMatterReportingCallback(this->endpoint_id, BridgedDeviceBasicInformation::Id, BridgedDeviceBasicInformation::Attributes::SerialNumber::Id);
  }
}

CHIP_ERROR Device::HandleReadIdentifyAttribute(ClusterId clusterId,
                                               chip::AttributeId attributeId,
                                               uint8_t* buffer,
                                               uint16_t maxReadLength)
{
  using namespace chip::app::Clusters;

  if (clusterId != Identify::Id) {
    return CHIP_ERROR_INTERNAL;
  }

  if ((attributeId == Identify::Attributes::IdentifyTime::Id) && (maxReadLength == 2)) {
    *buffer = this->identify_time;
  } else if ((attributeId == Identify::Attributes::IdentifyType::Id) && (maxReadLength == 1)) {
    *buffer = this->identify_type;
  } else if ((attributeId == Identify::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->identify_cluster_feature_map;
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == Identify::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->identify_cluster_revision;
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INTERNAL;
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR Device::HandleWriteIdentifyAttribute(ClusterId clusterId,
                                                chip::AttributeId attributeId,
                                                uint8_t* buffer)
{
  using namespace chip::app::Clusters;

  if (clusterId != Identify::Id) {
    return CHIP_ERROR_INTERNAL;
  }

  if (attributeId == Identify::Attributes::IdentifyTime::Id) {
    this->identify_time = *buffer;
    app::ConcreteAttributePath attributePath(this->endpoint_id, Identify::Id, Identify::Attributes::IdentifyTime::Id);
    MatterIdentifyClusterServerAttributeChangedCallback(attributePath);
  } else if (attributeId == Identify::Attributes::IdentifyType::Id) {
    this->identify_type = *buffer;
    app::ConcreteAttributePath attributePath(this->endpoint_id, Identify::Id, Identify::Attributes::IdentifyType::Id);
    MatterIdentifyClusterServerAttributeChangedCallback(attributePath);
  } else {
    return CHIP_ERROR_INTERNAL;
  }

  return CHIP_NO_ERROR;
}

void Device::HandleIdentifyStart()
{
  this->identify_in_progress = true;
}

void Device::HandleIdentifyStop()
{
  this->identify_in_progress = false;
}

bool Device::GetIdentifyInProgress()
{
  return this->identify_in_progress;
}

CHIP_ERROR Device::HandleReadGroupsAttribute(ClusterId clusterId,
                                             chip::AttributeId attributeId,
                                             uint8_t* buffer,
                                             uint16_t maxReadLength)
{
  using namespace chip::app::Clusters;

  if (clusterId != Groups::Id) {
    return CHIP_ERROR_INTERNAL;
  }

  if ((attributeId == Groups::Attributes::NameSupport::Id) && (maxReadLength == 1)) {
    *buffer = this->groups_cluster_name_support;
  } else if ((attributeId == Groups::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {
    uint32_t featureMap = this->groups_cluster_feature_map;
    memcpy(buffer, &featureMap, sizeof(featureMap));
  } else if ((attributeId == Groups::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->groups_cluster_revision;
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INTERNAL;
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR Device::HandleWriteGroupsAttribute(ClusterId clusterId,
                                              chip::AttributeId attributeId,
                                              uint8_t* buffer)
{
  using namespace chip::app::Clusters;

  if (clusterId != Groups::Id) {
    return CHIP_ERROR_INTERNAL;
  }

  if (attributeId == Groups::Attributes::NameSupport::Id) {
    this->groups_cluster_name_support = *buffer;
  } else {
    return CHIP_ERROR_INTERNAL;
  }

  return CHIP_NO_ERROR;
}
