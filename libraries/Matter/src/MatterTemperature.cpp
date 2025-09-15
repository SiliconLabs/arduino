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

#include "MatterTemperature.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gTempSensorDeviceTypes[] = { { DEVICE_TYPE_TEMP_SENSOR, DEVICE_VERSION_DEFAULT } };

// Temperature sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(tempSensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(TemperatureMeasurement::Attributes::MeasuredValue::Id, INT16S, 2, 0),        /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TemperatureMeasurement::Attributes::MinMeasuredValue::Id, INT16S, 2, 0),     /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TemperatureMeasurement::Attributes::MaxMeasuredValue::Id, INT16S, 2, 0),     /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TemperatureMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                  /* ClusterRevision auto added by LIST_END */

// Temperature sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(tempMeasurementEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(TemperatureMeasurement::Id, tempSensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterTemperature
 ******************************************************************************/
MatterTemperature::MatterTemperature() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterTemperature
 ******************************************************************************/
MatterTemperature::~MatterTemperature()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterTemperature instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterTemperature::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceTempSensor* sensor = new (std::nothrow)DeviceTempSensor("Temperature sensor", -4000, 10000, 0);
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Temperature sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = tempMeasurementEndpointClusters;
  new_endpoint->clusterCount = ArraySize(tempMeasurementEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(tempMeasurementEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gTempSensorDeviceTypes),
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
 * Deinitializes the MatterTemperature instance
 ******************************************************************************/
void MatterTemperature::end()
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
 * The value is - (measured_celsius) / 100.
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::set_measured_value_raw(int16_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetMeasuredValue(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Sets the sensors's measured value in celsius
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::set_measured_value_celsius(float value)
{
  int16_t out_value = static_cast<int16_t>(value * 100.0f);
  this->set_measured_value_raw(out_value);
}

/***************************************************************************//**
 * Sets the sensors's measured value in celsius
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::set_measured_value_celsius(double value)
{
  int16_t out_value = static_cast<int16_t>(value * 100.0l);
  this->set_measured_value_raw(out_value);
}

/***************************************************************************//**
 * Gets the sensors's measured raw value
 * The value is - (measured_celsius) / 100.
 *
 * @return the measured raw temperature value
 ******************************************************************************/
int16_t MatterTemperature::get_measured_value_raw()
{
  return this->sensor_device->GetMeasuredValue();
}

/***************************************************************************//**
 * Gets the sensors's measured value in celsius
 *
 * @return the measured temperature value in celsius
 ******************************************************************************/
float MatterTemperature::get_measured_value_celsius()
{
  float celsius_value = (float)this->get_measured_value_raw() / 100.0f;
  return celsius_value;
}

/***************************************************************************//**
 * Float conversion operator for getting the measured temperature in celsius
 *
 * @return the measured temperature value in celsius
 ******************************************************************************/
MatterTemperature::operator float()
{
  return this->get_measured_value_celsius();
}

/***************************************************************************//**
 * Double conversion operator for getting the measured temperature in celsius
 *
 * @return the measured temperature value in celsius
 ******************************************************************************/
MatterTemperature::operator double()
{
  return (double)this->get_measured_value_celsius();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured value in celsius
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::operator=(int16_t value)
{
  this->set_measured_value_raw(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured value in celsius
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::operator=(float value)
{
  this->set_measured_value_celsius(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured value in celsius
 *
 * @param[in] value the measured temperature value to be set
 ******************************************************************************/
void MatterTemperature::operator=(double value)
{
  this->set_measured_value_celsius(value);
}
