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

#include "MatterAirPurifier.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gAirPurifierDeviceTypes[] = { { DEVICE_TYPE_AIR_PURIFIER, DEVICE_VERSION_DEFAULT } };

// Fan control cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(fanControlAttrsAirPurifier)
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanMode::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),            /* FanMode */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FanModeSequence::Id, INT8U, 1, 0),                          /* FanModeSequence */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),     /* PercentSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::PercentCurrent::Id, INT8U, 1, 0),                           /* PercentCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedMax::Id, INT8U, 1, 0),                                 /* SpeedMax */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedSetting::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),       /* SpeedSetting */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::SpeedCurrent::Id, INT8U, 1, 0),                             /* SpeedCurrent */
DECLARE_DYNAMIC_ATTRIBUTE(FanControl::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                            /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                         /* ClusterRevision auto added by LIST_END */

// Air quality cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(airQualitySensorAttrsAirPurifier)
DECLARE_DYNAMIC_ATTRIBUTE(AirQuality::Attributes::AirQuality::Id, INT8U, 1, 0),            /* AirQuality */
DECLARE_DYNAMIC_ATTRIBUTE(AirQuality::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                      /* ClusterRevision auto added by LIST_END */

// Air purifier cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(airPurifierEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(FanControl::Id, fanControlAttrsAirPurifier, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(AirQuality::Id, airQualitySensorAttrsAirPurifier, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterAirPurifier
 ******************************************************************************/
MatterAirPurifier::MatterAirPurifier() :
  air_purifier_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterAirPurifier
 ******************************************************************************/
MatterAirPurifier::~MatterAirPurifier()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterAirPurifier instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterAirPurifier::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceAirPurifier* device = new (std::nothrow)DeviceAirPurifier("Air purifier", (uint8_t)MatterAirQuality::AirQuality_t::UNKNOWN);
  if (device == nullptr) {
    return false;
  }
  device->SetReachable(true);
  device->SetProductName("Air purifier");

  // Set the device instance pointer in the base class
  this->base_matter_device = device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(device);
    return false;
  }
  new_endpoint->cluster = airPurifierEndpointClusters;
  new_endpoint->clusterCount = ArraySize(airPurifierEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(airPurifierEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_device_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_device_data_version == nullptr) {
    delete(device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gAirPurifierDeviceTypes),
                                 Span<DataVersion>(new_device_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(device);
    free(new_endpoint);
    free(new_device_data_version);
    return false;
  }

  this->air_purifier_device = device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_device_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterAirPurifier instance
 ******************************************************************************/
void MatterAirPurifier::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->air_purifier_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->air_purifier_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the purifier's measured air quality value
 *
 * @param[in] value the measured air quality value to be set
 ******************************************************************************/
void MatterAirPurifier::set_air_quality(MatterAirQuality::AirQuality_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->air_purifier_device->SetMeasuredAirQualityValue((uint8_t)value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the purifier's measured air quality value
 *
 * @return the air quality value
 ******************************************************************************/
MatterAirQuality::AirQuality_t MatterAirPurifier::get_air_quality()
{
  return (MatterAirQuality::AirQuality_t)this->air_purifier_device->GetMeasuredAirQualityValue();
}

/***************************************************************************//**
 * Sets the on/off state of the purifier's fan
 *
 * @param[in] value the requested on/off state
 ******************************************************************************/
void MatterAirPurifier::set_fan_onoff(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  if (value) {
    this->air_purifier_device->SetFanMode(DeviceFan::fan_mode_t::On);
  } else {
    this->air_purifier_device->SetFanMode(DeviceFan::fan_mode_t::Off);
  }
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Provides the current on/off state of the purifier's fan
 *
 * @return the fan's current on/off state
 ******************************************************************************/
bool MatterAirPurifier::get_fan_onoff()
{
  if (!this->initialized) {
    return false;
  }
  return (this->air_purifier_device->GetFanMode() != DeviceFan::fan_mode_t::Off);
}

/***************************************************************************//**
 * Sets the current speed percentage of the purifier's fan
 *
 * @param[in] percent the requested fan speed percentage
 ******************************************************************************/
void MatterAirPurifier::set_fan_percent(uint8_t percent)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->air_purifier_device->SetFanPercentSetting(percent);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the current speed percentage of the purifier's fan
 *
 * @return the fan's current speed percentage
 ******************************************************************************/
uint8_t MatterAirPurifier::get_fan_percent()
{
  if (!this->initialized) {
    return 0;
  }
  return this->air_purifier_device->GetFanPercentSetting();
}
