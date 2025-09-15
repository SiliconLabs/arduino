/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#pragma once

#include <app/util/attribute-storage.h>
#include <lib/support/ZclString.h>

#include <stdbool.h>
#include <stdint.h>

#include <functional>
#include <vector>

using ::chip::EndpointId;
using ::chip::ClusterId;
using ::chip::AttributeId;

extern void CallMatterReportingCallback(intptr_t closure);
extern void ScheduleMatterReportingCallback(EndpointId endpointId, ClusterId cluster, AttributeId attribute);

class Device
{
public:
  static const int DeviceDescStrSize = 32;

  enum Changed_t{
    kChanged_Reachable    = 1u << 0,
    kChanged_Location     = 1u << 1,
    kChanged_Name         = 1u << 2,
    kChanged_VendorName   = 1u << 3,
    kChanged_ProductName  = 1u << 4,
    kChanged_SerialNumber = 1u << 5,
    kChanged_Last         = kChanged_SerialNumber,
  } Changed;

  enum device_type_t {
    kDeviceType_AirQualitySensor  = 0x0000,
    kDeviceType_ContactSensor     = 0x0001,
    kDeviceType_DoorLock          = 0x0002,
    kDeviceType_Fan               = 0x0003,
    kDeviceType_FlowSensor        = 0x0004,
    kDeviceType_HumiditySensor    = 0x0005,
    kDeviceType_IlluminanceSensor = 0x0006,
    kDeviceType_Lightbulb         = 0x0007,
    kDeviceType_OccupancySensor   = 0x0008,
    kDeviceType_OnOffPluginUnit   = 0x0009,
    kDeviceType_PressureSensor    = 0x000A,
    kDeviceType_Switch            = 0x000B,
    kDeviceType_TempSensor        = 0x000C,
    kDeviceType_Thermostat        = 0x000D,
    kDeviceType_WindowCovering    = 0x000E,
    kDeviceType_AirPurifier       = 0x000F,
    kDeviceType_Unspecified       = 0xFFFF
  };

  Device(const char* device_name);

  virtual ~Device()
  {
    ;
  }

  bool IsReachable();
  bool IsOnline();
  void SetOnline(bool online);
  void SetReachable(bool reachable);
  void SetName(const char* name);
  void SetVendorName(const char* vendorname);
  void SetProductName(const char* productname);
  void SetSerialNumber(const char* serialnumber);
  void SetLocation(std::string location);
  void SetDeviceChangeCallback(void (*matter_device_change_cb)(void));
  void CallDeviceChangeCallback();

  virtual CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                                chip::AttributeId attributeId,
                                                uint8_t* buffer,
                                                uint16_t maxReadLength);

  virtual CHIP_ERROR HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                 chip::AttributeId attributeId,
                                                 uint8_t* buffer);

  CHIP_ERROR HandleReadBridgedDeviceBasicAttribute(ClusterId clusterId,
                                                   chip::AttributeId attributeId,
                                                   uint8_t * buffer,
                                                   uint16_t maxReadLength);

  CHIP_ERROR HandleReadIdentifyAttribute(ClusterId clusterId,
                                         chip::AttributeId attributeId,
                                         uint8_t* buffer,
                                         uint16_t maxReadLength);

  CHIP_ERROR HandleWriteIdentifyAttribute(ClusterId clusterId,
                                          chip::AttributeId attributeId,
                                          uint8_t* buffer);

  CHIP_ERROR HandleReadGroupsAttribute(ClusterId clusterId,
                                       chip::AttributeId attributeId,
                                       uint8_t* buffer,
                                       uint16_t maxReadLength);

  CHIP_ERROR HandleWriteGroupsAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer);

  void HandleIdentifyStart();
  void HandleIdentifyStop();
  bool GetIdentifyInProgress();

  uint32_t GetBridgedDeviceBasicInformationClusterFeatureMap();
  uint16_t GetBridgedDeviceBasicInformationClusterRevision();

  device_type_t GetDeviceType()
  {
    return this->device_type;
  }

  inline void SetEndpointId(chip::EndpointId id)
  {
    this->endpoint_id = id;
  }

  inline chip::EndpointId GetEndpointId()
  {
    return this->endpoint_id;
  }

  inline void SetParentEndpointId(chip::EndpointId id)
  {
    this->parent_endpoint_id = id;
  }

  inline chip::EndpointId GetParentEndpointId()
  {
    return this->parent_endpoint_id;
  }

  inline char* GetName()
  {
    return this->device_name;
  }

  inline char* GetVendorName()
  {
    return this->vendor_name;
  }

  inline char* GetProductName()
  {
    return this->product_name;
  }

  inline char* GetSerialNumber()
  {
    return this->serial_number;
  }

  inline std::string GetLocation()
  {
    return this->location;
  }

protected:
  void HandleDeviceStatusChanged(Changed_t itemChangedMask);
  bool reachable;
  bool online;

  device_type_t device_type;

  void SetDeviceType(device_type_t device_type)
  {
    this->device_type = device_type;
  }

  bool identify_in_progress;
  uint16_t identify_time;
  uint8_t identify_type;

  uint8_t groups_cluster_name_support;

  char device_name[DeviceDescStrSize];
  char vendor_name[DeviceDescStrSize];
  char product_name[DeviceDescStrSize];
  char serial_number[DeviceDescStrSize];
  std::string location;
  chip::EndpointId endpoint_id;
  chip::EndpointId parent_endpoint_id;
  void (*device_change_callback)(void);

  static const uint32_t bridged_device_basic_information_cluster_feature_map = 0u;
  static const uint16_t bridged_device_basic_information_cluster_revision = 2u;

  static const uint32_t identify_cluster_feature_map = 0u;
  static const uint16_t identify_cluster_revision = 4u;

  static const uint32_t groups_cluster_feature_map = 0u;
  static const uint16_t groups_cluster_revision = 4u;
};
