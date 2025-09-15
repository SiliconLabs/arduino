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

#include "MatterOnOffPluginUnit.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gOnOffPluginUnitDeviceType[] = { { DEVICE_TYPE_ONOFF_PLUGIN_UNIT, DEVICE_VERSION_DEFAULT } };

constexpr CommandId onOffIncomingCommands[] = {
  app::Clusters::OnOff::Commands::Off::Id,
  app::Clusters::OnOff::Commands::On::Id,
  app::Clusters::OnOff::Commands::Toggle::Id,
  kInvalidCommandId,
};

// On/Off cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(onOffUnitAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::OnOff::Id, BOOLEAN, 1, 0),              /* OnOff */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::FeatureMap::Id, BITMAP32, 4, 0),        /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                /* ClusterRevision auto added by LIST_END */

// On/Off Plug-in Unit endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(OnOffPluginUnitEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(OnOff::Id, onOffUnitAttrs, ZAP_CLUSTER_MASK(SERVER), onOffIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterOnOffPluginUnit
 ******************************************************************************/
MatterOnOffPluginUnit::MatterOnOffPluginUnit() :
  pluginunit_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterOnOffPluginUnit
 ******************************************************************************/
MatterOnOffPluginUnit::~MatterOnOffPluginUnit()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterOnOffPluginUnit instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterOnOffPluginUnit::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceOnOffPluginUnit* pluginunit_device = new (std::nothrow)DeviceOnOffPluginUnit("Matter On/Off Outlet");
  if (pluginunit_device == nullptr) {
    return false;
  }
  pluginunit_device->SetReachable(true);

  // Set the device instance pointer in the base class
  this->base_matter_device = pluginunit_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(pluginunit_device);
    return false;
  }
  new_endpoint->cluster = OnOffPluginUnitEndpointClusters;
  new_endpoint->clusterCount = ArraySize(OnOffPluginUnitEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(OnOffPluginUnitEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_pluginunit_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_pluginunit_data_version == nullptr) {
    delete(pluginunit_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(pluginunit_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gOnOffPluginUnitDeviceType),
                                 Span<DataVersion>(new_pluginunit_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(pluginunit_device);
    free(new_endpoint);
    free(new_pluginunit_data_version);
    return false;
  }

  this->pluginunit_device = pluginunit_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_pluginunit_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterOnOffPluginUnit instance
 ******************************************************************************/
void MatterOnOffPluginUnit::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->pluginunit_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->pluginunit_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the unit's on/off state
 *
 * @param[in] value the requested on/off state
 ******************************************************************************/
void MatterOnOffPluginUnit::set_onoff(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->pluginunit_device->SetOnOff(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Provides the unit's current on/off state
 *
 * @return the unit's current on/off state
 ******************************************************************************/
bool MatterOnOffPluginUnit::get_onoff()
{
  if (!this->initialized) {
    return false;
  }
  return this->pluginunit_device->IsOn();
}

/***************************************************************************//**
 * Toggle the unit's current on/off state
 ******************************************************************************/
void MatterOnOffPluginUnit::toggle()
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->pluginunit_device->Toggle();
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Bool operator for getting the on/off state of the unit
 *
 * @return the unit's current on/off state
 ******************************************************************************/
MatterOnOffPluginUnit::operator bool()
{
  return this->get_onoff();
}

/***************************************************************************//**
 * Assignment operator for the MatterOnOffPluginUnit class to set the on/off state
 *
 * @param[in] state the requested on/off state
 ******************************************************************************/
void MatterOnOffPluginUnit::operator=(bool state)
{
  this->set_onoff(state);
}
