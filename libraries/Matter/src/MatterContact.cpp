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

#include "MatterContact.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gContactSensorDeviceTypes[] = { { DEVICE_TYPE_CONTACT_SENSOR, DEVICE_VERSION_DEFAULT } };

// Boolean state cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(booleanStateAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(BooleanState::Attributes::StateValue::Id, INT8U, 1, 0),               /* StateValue */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                           /* ClusterRevision auto added by LIST_END */

// Contact sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(contactSensorEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(BooleanState::Id, booleanStateAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterContact
 ******************************************************************************/
MatterContact::MatterContact() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterContact
 ******************************************************************************/
MatterContact::~MatterContact()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterContact instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterContact::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceContactSensor* sensor = new (std::nothrow)DeviceContactSensor("Contact sensor");
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Contact sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = contactSensorEndpointClusters;
  new_endpoint->clusterCount = ArraySize(contactSensorEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(contactSensorEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gContactSensorDeviceTypes),
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
 * Deinitializes the MatterContact instance
 ******************************************************************************/
void MatterContact::end()
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
 * @param[in] state the state to be set
 ******************************************************************************/
void MatterContact::set_state(bool state)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetStateValue(state);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the sensors's state
 *
 * @return the sensor's current state
 ******************************************************************************/
bool MatterContact::get_state()
{
  return this->sensor_device->GetStateValue();
}

/***************************************************************************//**
 * Bool operator to get the sensors's state
 *
 * @return the sensor's current state
 ******************************************************************************/
MatterContact::operator bool()
{
  return this->get_state();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's state
 *
 * @param[in] state the state to be set
 ******************************************************************************/
void MatterContact::operator=(bool state)
{
  this->set_state(state);
}
