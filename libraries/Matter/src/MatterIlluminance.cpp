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

#include "MatterIlluminance.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gIlluminanceSensorDeviceTypes[] = { { DEVICE_TYPE_ILLUMINANCE_SENSOR, DEVICE_VERSION_DEFAULT } };

// Illuminance measurement cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(illuminanceSensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(IlluminanceMeasurement::Attributes::MeasuredValue::Id, INT16U, 2, 0),        /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(IlluminanceMeasurement::Attributes::MinMeasuredValue::Id, INT16U, 2, 0),     /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(IlluminanceMeasurement::Attributes::MaxMeasuredValue::Id, INT16U, 2, 0),     /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(IlluminanceMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                  /* ClusterRevision auto added by LIST_END */

// Illuminance sensor endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(illuminanceMeasurementEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(IlluminanceMeasurement::Id, illuminanceSensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterIlluminance
 ******************************************************************************/
MatterIlluminance::MatterIlluminance() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false),
  current_light_value_lux(0.0)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterIlluminance
 ******************************************************************************/
MatterIlluminance::~MatterIlluminance()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterIlluminance instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterIlluminance::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceIlluminanceSensor* sensor = new (std::nothrow)DeviceIlluminanceSensor("Light sensor", 0, UINT16_MAX, 0);
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  sensor->SetProductName("Light sensor");

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = illuminanceMeasurementEndpointClusters;
  new_endpoint->clusterCount = ArraySize(illuminanceMeasurementEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(illuminanceMeasurementEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gIlluminanceSensorDeviceTypes),
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
  this->current_light_value_lux = 0.0;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterIlluminance instance
 ******************************************************************************/
void MatterIlluminance::end()
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
 * The value is represented with the following formula:
 * raw_measured_value = 10,000 x log10(measured_lux) + 1
 *
 * @param[in] value the measured raw illuminance value to be set
 ******************************************************************************/
void MatterIlluminance::set_measured_value_raw(uint16_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->sensor_device->SetMeasuredValue(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Sets the sensors's measured value in lux
 *
 * @param[in] lux the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::set_measured_value_lux(double lux)
{
  // raw_measured_value = 10,000 x log10(measured_lux) + 1
  double result = (double)10000.0 * log10(lux) + 1;
  this->set_measured_value_raw((uint16_t)result);
  this->current_light_value_lux = lux;
}

/***************************************************************************//**
 * Sets the sensors's measured value in lux
 *
 * @param[in] lux the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::set_measured_value_lux(float lux)
{
  this->set_measured_value_lux((double)lux);
}

/***************************************************************************//**
 * Sets the sensors's measured value in lux
 *
 * @param[in] lux the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::set_measured_value_lux(uint16_t lux)
{
  this->set_measured_value_lux((double)lux);
}

/***************************************************************************//**
 * Gets the sensors's measured value in lux
 *
 * @return the sensor's current illuminance value in lux
 ******************************************************************************/
double MatterIlluminance::get_measured_value_lux()
{
  return this->current_light_value_lux;
}

/***************************************************************************//**
 * Float conversion operator for getting the measured value
 *
 * @return the sensor's current illuminance value in lux
 ******************************************************************************/
MatterIlluminance::operator float()
{
  return (float)this->get_measured_value_lux();
}

/***************************************************************************//**
 * Double conversion operator for getting the measured value
 *
 * @return the sensor's current illuminance value in lux
 ******************************************************************************/
MatterIlluminance::operator double()
{
  return this->get_measured_value_lux();
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured illuminance value in lux
 *
 * @param[in] value the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::operator=(uint16_t value)
{
  this->set_measured_value_lux(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured illuminance value in lux
 *
 * @param[in] value the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::operator=(float value)
{
  this->set_measured_value_lux(value);
}

/***************************************************************************//**
 * Assignment operator to set the sensors's measured illuminance value in lux
 *
 * @param[in] value the measured illuminance value in lux to be set
 ******************************************************************************/
void MatterIlluminance::operator=(double value)
{
  this->set_measured_value_lux(value);
}
