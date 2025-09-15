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

#include "MatterThermostat.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gThermostatDeviceTypes[] = { { DEVICE_TYPE_THERMOSTAT, DEVICE_VERSION_DEFAULT } };

constexpr CommandId thermostatIncomingCommands[] = {
  app::Clusters::Thermostat::Commands::SetpointRaiseLower::Id,
  kInvalidCommandId,
};

// Temperature sensor cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(thermostatAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::LocalTemperature::Id, INT16S, 2, 0),                                /* Local Temperature */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::ControlSequenceOfOperation::Id, INT8U, 1, 0),                       /* Control Sequence Of Operation */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::SystemMode::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),                 /* System Mode */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::OccupiedHeatingSetpoint::Id, INT16S, 2, ATTRIBUTE_MASK_WRITABLE),   /* Occupied Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMinHeatSetpointLimit::Id, INT16S, 2, 0),                         /* Absolute Minimum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::AbsMaxHeatSetpointLimit::Id, INT16S, 2, 0),                         /* Absolute Maximum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MinHeatSetpointLimit::Id, INT16S, 2, 0),                            /* Minimum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::MaxHeatSetpointLimit::Id, INT16S, 2, 0),                            /* Maximum Heating Setpoint */
DECLARE_DYNAMIC_ATTRIBUTE(Thermostat::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                    /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                 /* ClusterRevision auto added by LIST_END */

// Temperature sensor cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(thermostatEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(Thermostat::Id, thermostatAttrs, ZAP_CLUSTER_MASK(SERVER), thermostatIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterThermostat
 ******************************************************************************/
MatterThermostat::MatterThermostat() :
  thermostat_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterThermostat
 ******************************************************************************/
MatterThermostat::~MatterThermostat()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterThermostat instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterThermostat::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceThermostat* new_thermostat_device = new (std::nothrow)DeviceThermostat("Thermostat", 20, 20);
  if (new_thermostat_device == nullptr) {
    return false;
  }
  new_thermostat_device->SetReachable(true);
  new_thermostat_device->SetProductName("Thermostat");

  // Set the device instance pointer in the base class
  this->base_matter_device = new_thermostat_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(new_thermostat_device);
    return false;
  }
  new_endpoint->cluster = thermostatEndpointClusters;
  new_endpoint->clusterCount = ArraySize(thermostatEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(thermostatEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(new_thermostat_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(new_thermostat_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gThermostatDeviceTypes),
                                 Span<DataVersion>(new_sensor_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(new_thermostat_device);
    free(new_endpoint);
    free(new_sensor_data_version);
    return false;
  }

  this->thermostat_device = new_thermostat_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_sensor_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterThermostat instance
 ******************************************************************************/
void MatterThermostat::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->thermostat_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->thermostat_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Gets the thermostat's currently set raw local temperature value
 * The value is in celsius x 100.
 *
 * @return the locally measured raw temperature value
 ******************************************************************************/
int16_t MatterThermostat::get_local_temperature_raw()
{
  return this->thermostat_device->GetLocalTemperatureValue();
}

/***************************************************************************//**
 * Sets the thermostat's locally measured raw temperature value
 * The accepted value is in celsius x 100.
 *
 * @param[in] local_temp the locally measured raw temperature value to set
 ******************************************************************************/
void MatterThermostat::set_local_temperature_raw(int16_t local_temp)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->thermostat_device->SetLocalTemperatureValue(local_temp);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Sets the thermostat's raw heating setpoint value
 * The accepted value is in celsius x 100.
 *
 * @param[in] heating_setpoint the raw heating setpoint to be set
 ******************************************************************************/
void MatterThermostat::set_heating_setpoint_raw(int16_t heating_setpoint)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->thermostat_device->SetHeatingSetpointValue(heating_setpoint);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Gets the thermostat's currently set raw heating setpoint value
 * The value is in celsius x 100.
 *
 * @return the currently set raw heating setpoint value
 ******************************************************************************/
int16_t MatterThermostat::get_heating_setpoint_raw()
{
  return this->thermostat_device->GetHeatingSetpointValue();
}

/***************************************************************************//**
 * Gets the thermostat's currently set local temperature value in celsius
 *
 * @return the locally measured temperature value
 ******************************************************************************/
float MatterThermostat::get_local_temperature()
{
  return (float)this->get_local_temperature_raw() / 100.0f;
}

/***************************************************************************//**
 * Sets the thermostat's locally measured temperature value in celsius
 *
 * @param[in] local_temp the locally measured temperature value to set
 ******************************************************************************/
void MatterThermostat::set_local_temperature(float local_temp)
{
  uint16_t raw_temp = (int16_t)(local_temp * 100.0f);
  this->set_local_temperature_raw(raw_temp);
}

/***************************************************************************//**
 * Gets the thermostat's currently set heating setpoint value in celsius
 *
 * @return the currently set heating setpoint value
 ******************************************************************************/
float MatterThermostat::get_heating_setpoint()
{
  return (float)this->get_heating_setpoint_raw() / 100.0f;
}

/***************************************************************************//**
 * Sets the thermostat's heating setpoint value in celsius
 *
 * @param[in] heating_setpoint the heating setpoint to be set
 ******************************************************************************/
void MatterThermostat::set_heating_setpoint(float heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(heating_setpoint * 100.0f);
  this->set_heating_setpoint_raw(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute minimum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @param[in] abs_min_heating_setpoint the absolute minimum value heating can be set to
 ******************************************************************************/
void MatterThermostat::set_absolute_minimum_heating_setpoint(float abs_min_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_min_heating_setpoint * 100.0f);
  this->thermostat_device->SetAbsMinHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's minimum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @param[in] min_heating_setpoint the minimum value heating can be set to
 ******************************************************************************/
void MatterThermostat::set_minimum_heating_setpoint(float min_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(min_heating_setpoint * 100.0f);
  this->thermostat_device->SetMinHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's absolute maximum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @param[in] abs_max_heating_setpoint the absolute maximum value heating can be set to
 ******************************************************************************/
void MatterThermostat::set_absolute_maximum_heating_setpoint(float abs_max_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(abs_max_heating_setpoint * 100.0f);
  this->thermostat_device->SetAbsMaxHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Sets the thermostat's maximum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @param[in] max_heating_setpoint the maximum value heating can be set to
 ******************************************************************************/
void MatterThermostat::set_maximum_heating_setpoint(float max_heating_setpoint)
{
  uint16_t raw_temp = (int16_t)(max_heating_setpoint * 100.0f);
  this->thermostat_device->SetMaxHeatingSetpoint(raw_temp);
}

/***************************************************************************//**
 * Gets the thermostat's absolute minimum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @return the absolute minimum value heating can be set to
 ******************************************************************************/
float MatterThermostat::get_absolute_minimum_heating_setpoint()
{
  return (float)this->thermostat_device->GetAbsMinHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's minimum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be lower than the absolute minimum value.
 *
 * @return the minimum value heating can be set to
 ******************************************************************************/
float MatterThermostat::get_minimum_heating_setpoint()
{
  return (float)this->thermostat_device->GetMinHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's absolute maximum heating setpoint value in celsius
 * This is the heating device's physical limit which cannot be changed.
 *
 * @return the absolute maximum value heating can be set to
 ******************************************************************************/
float MatterThermostat::get_absolute_maximum_heating_setpoint()
{
  return (float)this->thermostat_device->GetAbsMaxHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's maximum heating setpoint value in celsius
 * This limit may be changed by the user to enforce heating policies.
 * Cannot be higher than the absolute maximum value.
 *
 * @return the maximum value heating can be set to
 ******************************************************************************/
float MatterThermostat::get_maximum_heating_setpoint()
{
  return (float)this->thermostat_device->GetMaxHeatingSetpoint() / 100.0f;
}

/***************************************************************************//**
 * Gets the thermostat's currently set system mode
 *
 * @return the currently set system mode
 ******************************************************************************/
MatterThermostat::thermostat_mode_t MatterThermostat::get_system_mode()
{
  return (thermostat_mode_t)this->thermostat_device->GetSystemMode();
}

/***************************************************************************//**
 * Sets the thermostat's system mode
 *
 * @param[in] system_mode the system mode to be set
 ******************************************************************************/
void MatterThermostat::set_system_mode(MatterThermostat::thermostat_mode_t system_mode)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->thermostat_device->SetSystemMode((uint8_t)system_mode);
  PlatformMgr().UnlockChipStack();
}
