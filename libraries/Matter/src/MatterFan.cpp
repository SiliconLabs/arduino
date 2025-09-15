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

#include "MatterFan.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gFanDeviceTypes[] = { { DEVICE_TYPE_FAN, DEVICE_VERSION_DEFAULT } };

// Fan control cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(fanControlAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanMode::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),            /* FanMode */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanModeSequence::Id, INT8U, 1, 0),                          /* FanModeSequence */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),     /* PercentSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentCurrent::Id, INT8U, 1, 0),                           /* PercentCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedMax::Id, INT8U, 1, 0),                                 /* SpeedMax */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),       /* SpeedSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedCurrent::Id, INT8U, 1, 0),                             /* SpeedCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                            /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                         /* ClusterRevision auto added by LIST_END */

// Fan control endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(fanControlEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(FanControl::Id, fanControlAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterFan
 ******************************************************************************/
MatterFan::MatterFan() :
  fan_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterFan
 ******************************************************************************/
MatterFan::~MatterFan()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterFan instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterFan::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceFan* fan_device = new (std::nothrow)DeviceFan("Fan");
  if (fan_device == nullptr) {
    return false;
  }
  fan_device->SetReachable(true);
  fan_device->SetProductName("Fan");

  // Set the device instance pointer in the base class
  this->base_matter_device = fan_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(fan_device);
    return false;
  }
  new_endpoint->cluster = fanControlEndpointClusters;
  new_endpoint->clusterCount = ArraySize(fanControlEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(fanControlEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_device_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_device_data_version == nullptr) {
    delete(fan_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(fan_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gFanDeviceTypes),
                                 Span<DataVersion>(new_device_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(fan_device);
    free(new_endpoint);
    free(new_device_data_version);
    return false;
  }

  this->fan_device = fan_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_device_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterFan instance
 ******************************************************************************/
void MatterFan::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->fan_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->fan_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the fan's on/off state
 *
 * @param[in] value the requested on/off state
 ******************************************************************************/
void MatterFan::set_onoff(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  if (value) {
    this->fan_device->SetFanMode(DeviceFan::fan_mode_t::On);
  } else {
    this->fan_device->SetFanMode(DeviceFan::fan_mode_t::Off);
  }
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Provides the fan's current on/off state
 *
 * @return the fan's current on/off state
 ******************************************************************************/
bool MatterFan::get_onoff()
{
  if (!this->initialized) {
    return false;
  }
  return (this->fan_device->GetFanMode() != DeviceFan::fan_mode_t::Off);
}

/***************************************************************************//**
 * Sets the fan's current speed percentage
 *
 * @param[in] percent the requested fan speed percentage
 ******************************************************************************/
void MatterFan::set_percent(uint8_t percent)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->fan_device->SetPercentSetting(percent);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the fan's current speed percentage
 *
 * @return the fan's current speed percentage
 ******************************************************************************/
uint8_t MatterFan::get_percent()
{
  if (!this->initialized) {
    return 0;
  }
  return this->fan_device->GetPercentSetting();
}

/***************************************************************************//**
 * Sets the fan's current mode
 *
 * @param[in] fan_mode the requested fan mode from DeviceFan::fan_mode_t
 ******************************************************************************/
void MatterFan::set_mode(DeviceFan::fan_mode_t fan_mode)
{
  if (!this->initialized) {
    return;
  }
  this->fan_device->SetFanMode(fan_mode);
}

/***************************************************************************//**
 * Gets the fan's current mode
 *
 * @return the fan's current mode from DeviceFan::fan_mode_t
 ******************************************************************************/
DeviceFan::fan_mode_t MatterFan::get_mode()
{
  if (!this->initialized) {
    return DeviceFan::fan_mode_t::Off;
  }
  return static_cast<DeviceFan::fan_mode_t>(this->fan_device->GetFanMode());
}

/***************************************************************************//**
 * Assignment operator to set the fan's current speed percentage
 *
 * @param[in] percent the requested fan speed percentage
 ******************************************************************************/
void MatterFan::operator=(uint8_t percent)
{
  this->set_percent(percent);
}
