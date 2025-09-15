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

#include "MatterFlow.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gFlowSensorDeviceTypes[] = { { DEVICE_TYPE_FLOW_SENSOR, DEVICE_VERSION_DEFAULT } };

// Flow measurement cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(flowSensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(FlowMeasurement::Attributes::MeasuredValue::Id, INT16U, 2, 0),        /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(FlowMeasurement::Attributes::MinMeasuredValue::Id, INT16U, 2, 0),     /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(FlowMeasurement::Attributes::MaxMeasuredValue::Id, INT16U, 2, 0),     /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(FlowMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                           /* ClusterRevision auto added by LIST_END */

// Flow sensor endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(flowMeasurementEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(FlowMeasurement::Id, flowSensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterFlow
 ******************************************************************************/
MatterFlow::MatterFlow() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterFlow
 ******************************************************************************/
MatterFlow::~MatterFlow()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterFlow instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterFlow::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceFlowSensor* sensor = new (std::nothrow)DeviceFlowSensor("Flow sensor", 0, UINT16_MAX, 0);
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Flow sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = flowMeasurementEndpointClusters;
  new_endpoint->clusterCount = ArraySize(flowMeasurementEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(flowMeasurementEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gFlowSensorDeviceTypes),
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
 * Deinitializes the MatterFlow instance
 ******************************************************************************/
void MatterFlow::end()
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
 * Sets the sensors's raw measured value
 * Flow is represented with the following formula:
 * raw_value = 10 x flow (in cubic meters per hour)
 *
 * @param[in] value the measured raw flow value to be set
 ******************************************************************************/
void MatterFlow::set_measured_value_raw(uint16_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetMeasuredValue(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the sensors's raw measured value
 * Flow is represented with the following formula:
 * raw_value = 10 x flow (in cubic meters per hour)
 *
 * @return the sensor's current raw flow value
 ******************************************************************************/
uint16_t MatterFlow::get_measured_value_raw()
{
  return this->sensor_device->GetMeasuredValue();
}

/***************************************************************************//**
 * Sets the sensors's measured value in cubic meters per hour
 *
 * @param[in] value the measured flow value to be set
 ******************************************************************************/
void MatterFlow::set_measured_value_cubic_meters_per_hour(double value)
{
  uint16_t raw_value = (uint16_t)(value * 10L);
  this->set_measured_value_raw(raw_value);
}

/***************************************************************************//**
 * Gets the sensors's measured value in cubic meters per hour
 *
 * @return the sensor's current flow value
 ******************************************************************************/
double MatterFlow::get_measured_value_cubic_meters_per_hour()
{
  double converted_value = (double)this->get_measured_value_raw() * 10L;
  return converted_value;
}

/***************************************************************************//**
 * Double conversion operator for getting the sensors's measured value
 * in cubic meters per hour
 *
 * @return the sensor's current flow value
 ******************************************************************************/
MatterFlow::operator double()
{
  return this->get_measured_value_cubic_meters_per_hour();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's raw measured value
 *
 * @param[in] value the measured raw flow value to be set
 ******************************************************************************/
void MatterFlow::operator=(uint16_t value)
{
  this->set_measured_value_raw(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured value in
 * cubic meters per hour
 *
 * @param[in] value the measured flow value to be set
 ******************************************************************************/
void MatterFlow::operator=(double value)
{
  this->set_measured_value_cubic_meters_per_hour(value);
}
