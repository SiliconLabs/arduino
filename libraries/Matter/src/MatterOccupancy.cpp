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

#include "MatterOccupancy.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gOccupancySensorDeviceTypes[] = { { DEVICE_TYPE_OCCUPANCY_SENSOR, DEVICE_VERSION_DEFAULT } };

// Occupancy sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(occupancySensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(OccupancySensing::Attributes::Occupancy::Id, INT8U, 1, 0),            /* Occupancy */
DECLARE_DYNAMIC_ATTRIBUTE(OccupancySensing::Attributes::FeatureMap::Id, BITMAP32, 4, 0),        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                           /* ClusterRevision auto added by LIST_END */

// Occupancy sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(occupancySensorEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(OccupancySensing::Id, occupancySensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterOccupancy
 ******************************************************************************/
MatterOccupancy::MatterOccupancy() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterOccupancy
 ******************************************************************************/
MatterOccupancy::~MatterOccupancy()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterOccupancy instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterOccupancy::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceOccupancySensor* sensor = new (std::nothrow)DeviceOccupancySensor("Occupancy sensor");
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Occupancy sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = occupancySensorEndpointClusters;
  new_endpoint->clusterCount = ArraySize(occupancySensorEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(occupancySensorEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gOccupancySensorDeviceTypes),
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
 * Deinitializes the MatterOccupancy instance
 ******************************************************************************/
void MatterOccupancy::end()
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
 * Sets the sensors's state
 *
 * @param[in] occupied the occupancy state to be set
 ******************************************************************************/
void MatterOccupancy::set_occupancy(bool occupied)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetOccupancy(occupied);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the sensors's state
 *
 * @return the sensor's current occupancy state
 ******************************************************************************/
bool MatterOccupancy::get_occupancy()
{
  return this->sensor_device->GetOccupancy();
}

/***************************************************************************//**
 * Bool operator to get the sensors's occupancy state
 *
 * @return the sensor's current occupancy state
 ******************************************************************************/
MatterOccupancy::operator bool()
{
  return this->get_occupancy();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's occupancy state
 *
 * @param[in] occupied the occupancy state to be set
 ******************************************************************************/
void MatterOccupancy::operator=(bool occupied)
{
  this->set_occupancy(occupied);
}
