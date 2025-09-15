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

#include "MatterAirQuality.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gAirQualitySensorDeviceTypes[] = { { DEVICE_TYPE_AIR_QUALITY_SENSOR, DEVICE_VERSION_DEFAULT } };

// Air Quality sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(airQualitySensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(AirQuality::Attributes::AirQuality::Id, INT8U, 1, 0),            /* AirQuality */
DECLARE_DYNAMIC_ATTRIBUTE(AirQuality::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                      /* ClusterRevision auto added by LIST_END */

// Air Quality sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(airQualityEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(AirQuality::Id, airQualitySensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterAirQuality
 ******************************************************************************/
MatterAirQuality::MatterAirQuality() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterAirQuality
 ******************************************************************************/
MatterAirQuality::~MatterAirQuality()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterAirQuality instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterAirQuality::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceAirQualitySensor* sensor = new (std::nothrow)DeviceAirQualitySensor("Air Quality sensor", 0);
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Air Quality sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = airQualityEndpointClusters;
  new_endpoint->clusterCount = ArraySize(airQualityEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(airQualityEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gAirQualitySensorDeviceTypes),
                                 Span<DataVersion>(new_sensor_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(sensor);
    free(new_endpoint);
    free(new_sensor_data_version);
    return false;
  }

  this->sensor_device = sensor;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_sensor_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterAirQuality instance
 ******************************************************************************/
void MatterAirQuality::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->sensor_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->sensor_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the sensors's air quality value
 *
 * @param[in] value the measured air quality value to be set
 ******************************************************************************/
void MatterAirQuality::set_air_quality(MatterAirQuality::AirQuality_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetMeasuredValue((uint8_t)value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the sensors's air quality value
 *
 * @return the air quality value
 ******************************************************************************/
MatterAirQuality::AirQuality_t MatterAirQuality::get_air_quality()
{
  return (MatterAirQuality::AirQuality_t)this->sensor_device->GetMeasuredValue();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's air quality value
 *
 * @param[in] value the measured air quality value to be set
 ******************************************************************************/
void MatterAirQuality::operator=(MatterAirQuality::AirQuality_t value)
{
  this->set_air_quality(value);
}
