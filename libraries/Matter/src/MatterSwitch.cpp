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

#include "MatterSwitch.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gSwitchDeviceTypes[] = { { DEVICE_TYPE_SWITCH, DEVICE_VERSION_DEFAULT } };

constexpr CommandId switchOutgoingCommands[] = {
  app::Clusters::Switch::Events::SwitchLatched::Id,
  app::Clusters::Switch::Events::InitialPress::Id,
  app::Clusters::Switch::Events::LongPress::Id,
  app::Clusters::Switch::Events::ShortRelease::Id,
  app::Clusters::Switch::Events::LongRelease::Id,
  app::Clusters::Switch::Events::MultiPressOngoing::Id,
  app::Clusters::Switch::Events::MultiPressComplete::Id,
  kInvalidCommandId,
};

// Switch cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(switchAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(Switch::Attributes::NumberOfPositions::Id, INT8U, 1, 0),   /* NumberOfPositions */
DECLARE_DYNAMIC_ATTRIBUTE(Switch::Attributes::CurrentPosition::Id, INT8U, 1, 0),     /* CurrentPosition */
DECLARE_DYNAMIC_ATTRIBUTE(Switch::Attributes::MultiPressMax::Id, INT8U, 1, 0),       /* MultiPressMax */
DECLARE_DYNAMIC_ATTRIBUTE(Switch::Attributes::FeatureMap::Id, BITMAP32, 4, 0),       /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                /* ClusterRevision auto added by LIST_END */

// Switch cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(switchEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(Switch::Id, switchAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, switchOutgoingCommands),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterSwitch
 ******************************************************************************/
MatterSwitch::MatterSwitch() :
  switch_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterSwitch
 ******************************************************************************/
MatterSwitch::~MatterSwitch()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterSwitch instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterSwitch::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceSwitch* new_switch_device = new (std::nothrow)DeviceSwitch("Switch");
  if (new_switch_device == nullptr) {
    return false;
  }
  new_switch_device->SetReachable(true);
  new_switch_device->SetProductName("Switch");

  // Set the device instance pointer in the base class
  this->base_matter_device = new_switch_device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(new_switch_device);
    return false;
  }
  new_endpoint->cluster = switchEndpointClusters;
  new_endpoint->clusterCount = ArraySize(switchEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(switchEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_switch_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_switch_data_version == nullptr) {
    delete(new_switch_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(new_switch_device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gSwitchDeviceTypes),
                                 Span<DataVersion>(new_switch_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(new_switch_device);
    free(new_endpoint);
    free(new_switch_data_version);
    return false;
  }

  new_switch_device->SetNumberOfPositions(2);
  new_switch_device->SetCurrentPosition(0);
  new_switch_device->SetMultiPressMax(1);

  this->switch_device = new_switch_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_switch_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterSwitch instance
 ******************************************************************************/
void MatterSwitch::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->switch_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->switch_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the current state of the switch
 *
 * @param[in] state the requested state
 ******************************************************************************/
void MatterSwitch::set_state(bool state)
{
  if (!this->initialized) {
    return;
  }
  this->switch_device->SetCurrentPosition(state);
}

/***************************************************************************//**
 * Gets the current state of the switch
 *
 * @return the current state of the switch
 ******************************************************************************/
bool MatterSwitch::get_state()
{
  return this->switch_device->GetCurrentPosition();
}

/***************************************************************************//**
 * Assignment operator to set the current switch state
 *
 * @param[in] state the requested state
 ******************************************************************************/
void MatterSwitch::operator=(bool state)
{
  this->set_state(state);
}
