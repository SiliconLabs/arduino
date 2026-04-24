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

#include "MatterConcentrationMeasurement.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gConcentrationMeasurementSensorDeviceTypes[] = { { DEVICE_TYPE_AIR_QUALITY_SENSOR, DEVICE_VERSION_DEFAULT } };

// CO2 sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(co2SensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::MeasuredValue::Id, SINGLE, 4, 0),       /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::MinMeasuredValue::Id, SINGLE, 4, 0),    /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::MaxMeasuredValue::Id, SINGLE, 4, 0),    /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::MeasurementUnit::Id, INT8U, 1, 0),      /* Measurement Unit */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::MeasurementMedium::Id, INT8U, 1, 0),    /* Measurement Medium */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonDioxideConcentrationMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                /* ClusterRevision auto added by LIST_END */

// CO2 sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(co2SensorEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(CarbonDioxideConcentrationMeasurement::Id, co2SensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

// CO sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(coSensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::MeasuredValue::Id, SINGLE, 4, 0),       /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::MinMeasuredValue::Id, SINGLE, 4, 0),    /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::MaxMeasuredValue::Id, SINGLE, 4, 0),    /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::MeasurementUnit::Id, INT8U, 1, 0),      /* Measurement Unit */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::MeasurementMedium::Id, INT8U, 1, 0),    /* Measurement Medium */
DECLARE_DYNAMIC_ATTRIBUTE(CarbonMonoxideConcentrationMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                 /* ClusterRevision auto added by LIST_END */

// CO sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(coSensorEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(CarbonMonoxideConcentrationMeasurement::Id, coSensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

// TVOC sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(tvocSensorAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MeasuredValue::Id, SINGLE, 4, 0),       /* Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MinMeasuredValue::Id, SINGLE, 4, 0),    /* Min Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MaxMeasuredValue::Id, SINGLE, 4, 0),    /* Max Measured Value */
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MeasurementUnit::Id, INT8U, 1, 0),      /* Measurement Unit */
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::MeasurementMedium::Id, INT8U, 1, 0),    /* Measurement Medium */
DECLARE_DYNAMIC_ATTRIBUTE(TotalVolatileOrganicCompoundsConcentrationMeasurement::Attributes::FeatureMap::Id, BITMAP32, 4, 0),        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                                /* ClusterRevision auto added by LIST_END */

// TVOC sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(tvocSensorEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(TotalVolatileOrganicCompoundsConcentrationMeasurement::Id, tvocSensorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterConcentrationMeasurement
 ******************************************************************************/
MatterConcentrationMeasurement::MatterConcentrationMeasurement() :
  sensor_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterConcentrationMeasurement
 ******************************************************************************/
MatterConcentrationMeasurement::~MatterConcentrationMeasurement()
{
  this->end();
}

bool MatterConcentrationMeasurement::begin()
{
  return false;
}

/***************************************************************************//**
 * Initializes the MatterConcentrationMeasurement instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterConcentrationMeasurement::begin_internal(concentration_sensor_t sensor_type,
                                                    EmberAfCluster* endpoint_clusters,
                                                    uint8_t cluster_count)
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceConcentrationMeasurementSensor* sensor = nullptr;
  if (sensor_type == concentration_sensor_t::sensor_co2) {
    sensor = new (std::nothrow)DeviceConcentrationMeasurementSensor("CO2 sensor",
                                                                    Device::device_type_t::kDeviceType_CO2Sensor,
                                                                    DeviceConcentrationMeasurementSensor::measurement_unit_t::kUnit_PPM,
                                                                    0.0f,
                                                                    10000.0f,
                                                                    0.0f);
  } else if (sensor_type == concentration_sensor_t::sensor_co) {
    sensor = new (std::nothrow)DeviceConcentrationMeasurementSensor("CO sensor",
                                                                    Device::device_type_t::kDeviceType_COSensor,
                                                                    DeviceConcentrationMeasurementSensor::measurement_unit_t::kUnit_PPB,
                                                                    0.0f,
                                                                    10000.0f,
                                                                    0.0f);
  } else if (sensor_type == concentration_sensor_t::sensor_tvoc) {
    sensor = new (std::nothrow)DeviceConcentrationMeasurementSensor("TVOC sensor",
                                                                    Device::device_type_t::kDeviceType_TVOCSensor,
                                                                    DeviceConcentrationMeasurementSensor::measurement_unit_t::kUnit_PPB,
                                                                    0.0f,
                                                                    10000.0f,
                                                                    0.0f);
  }
  if (sensor == nullptr) {
    return false;
  }
  sensor->SetReachable(true);
  if (sensor_type == concentration_sensor_t::sensor_co2) {
    sensor->SetProductName("CO2 sensor");
  } else if (sensor_type == concentration_sensor_t::sensor_co) {
    sensor->SetProductName("CO sensor");
  } else if (sensor_type == concentration_sensor_t::sensor_tvoc) {
    sensor->SetProductName("TVOC sensor");
  }

  // Set the device instance pointer in the base class
  this->base_matter_device = sensor;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(sensor);
    return false;
  }
  new_endpoint->cluster = endpoint_clusters;
  new_endpoint->clusterCount = cluster_count;
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = cluster_count * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(sensor);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(sensor,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gConcentrationMeasurementSensorDeviceTypes),
                                 Span<DataVersion>(new_sensor_data_version, cluster_count),
                                 1);
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
 * Deinitializes the MatterConcentrationMeasurement instance
 ******************************************************************************/
void MatterConcentrationMeasurement::end()
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
 * Sets the sensors's measured value
 *
 * @param[in] value the sensor's value to set
 ******************************************************************************/
void MatterConcentrationMeasurement::set_measured_value(float value)
{
  PlatformMgr().LockChipStack();
  this->sensor_device->SetMeasuredValue(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the sensors's current measured value
 *
 * @return the sensor's current value
 ******************************************************************************/
float MatterConcentrationMeasurement::get_measured_value()
{
  return this->sensor_device->GetMeasuredValue();
}

/***************************************************************************//**
 * Sets the sensors's measured value
 *
 * @param[in] value the sensor's value to set
 ******************************************************************************/
void MatterConcentrationMeasurement::set_measured_value(uint32_t value)
{
  this->set_measured_value(static_cast<float>(value));
}

//##################################################################################################
// Functions for MatterCO2
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterCO2
 ******************************************************************************/
MatterCO2::MatterCO2()
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterCO2
 ******************************************************************************/
MatterCO2::~MatterCO2()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterCO2 instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterCO2::begin()
{
  return this->begin_internal(concentration_sensor_t::sensor_co2,
                              co2SensorEndpointClusters,
                              ArraySize(co2SensorEndpointClusters));
}

/***************************************************************************//**
 * Assignment operator to set the sensors's value
 *
 * @param[in] value the value to set
 ******************************************************************************/
void MatterCO2::operator=(float value)
{
  this->set_measured_value(value);
}

//##################################################################################################
// Functions for MatterCO
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterCO
 ******************************************************************************/
MatterCO::MatterCO()
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterCO
 ******************************************************************************/
MatterCO::~MatterCO()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterCO instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterCO::begin()
{
  return this->begin_internal(concentration_sensor_t::sensor_co,
                              coSensorEndpointClusters,
                              ArraySize(coSensorEndpointClusters));
}

/***************************************************************************//**
 * Assignment operator to set the sensors's value
 *
 * @param[in] value the value to set
 ******************************************************************************/
void MatterCO::operator=(float value)
{
  this->set_measured_value(value);
}

//##################################################################################################
// Functions for MatterTVOC
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterTVOC
 ******************************************************************************/
MatterTVOC::MatterTVOC()
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterTVOC
 ******************************************************************************/
MatterTVOC::~MatterTVOC()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterTVOC instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterTVOC::begin()
{
  return this->begin_internal(concentration_sensor_t::sensor_tvoc,
                              tvocSensorEndpointClusters,
                              ArraySize(tvocSensorEndpointClusters));
}

/***************************************************************************//**
 * Assignment operator to set the sensors's value
 *
 * @param[in] value the value to set
 ******************************************************************************/
void MatterTVOC::operator=(float value)
{
  this->set_measured_value(value);
}
