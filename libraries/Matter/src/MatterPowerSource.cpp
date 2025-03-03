// SPDX-License-Identifier: MIT

#include "MatterPowerSource.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gPowerSourceDeviceTypes[] = { { DEVICE_TYPE_POWER_SOURCE, DEVICE_VERSION_DEFAULT } };

// Power source cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(powerSourceAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatPercentRemaining::Id, INT8U, 1, 0),   /* BatPercentRemaining */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::FeatureMap::Id, BITMAP32, 4, 0),         /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::ClusterRevision::Id, INT16U, 2, 0),      /* ClusterRevision */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();

// Power source cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(powerSourceEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(PowerSource::Id, powerSourceAttrs, nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterPowerSource
 ******************************************************************************/
MatterPowerSource::MatterPowerSource() :
  power_source_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterPowerSource
 ******************************************************************************/
MatterPowerSource::~MatterPowerSource()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterPowerSource instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterPowerSource::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DevicePowerSource* power_source = new (std::nothrow)DevicePowerSource("Power Source", 0);
  if (power_source == nullptr) {
    return false;
  }
  power_source->SetReachable(true);
  power_source->SetProductName("Power Source");

  // Set the device instance pointer in the base class
  this->base_matter_device = power_source;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(power_source);
    return false;
  }
  new_endpoint->cluster = powerSourceEndpointClusters;
  new_endpoint->clusterCount = ArraySize(powerSourceEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(powerSourceEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_power_source_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_power_source_data_version == nullptr) {
    delete(power_source);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(power_source,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gPowerSourceDeviceTypes),
                                 Span<DataVersion>(new_power_source_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(power_source);
    free(new_endpoint);
    free(new_power_source_data_version);
    return false;
  }

  this->power_source_device = power_source;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_power_source_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterPowerSource instance
 ******************************************************************************/
void MatterPowerSource::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->power_source_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->power_source_device);
  this->initialized = false;
}

void MatterPowerSource::set_bat_percent_remaining_raw(uint8_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatPercentRemaining(value);
  PlatformMgr().UnlockChipStack();
}

void MatterPowerSource::set_bat_percent_remaining(double percent)
{
  uint8_t out_value = static_cast<uint8_t>(percent * 2.0l);
  this->set_bat_percent_remaining_raw(out_value);
}

uint8_t MatterPowerSource::get_bat_percent_remaining()
{
  return this->power_source_device->GetBatPercentRemaining();
}
