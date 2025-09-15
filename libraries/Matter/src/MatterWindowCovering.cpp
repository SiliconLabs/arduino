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

#include "MatterWindowCovering.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gWindowCoveringDeviceTypes[] = { { DEVICE_TYPE_WINDOW_COVERING, DEVICE_VERSION_DEFAULT } };

constexpr CommandId windowCoveringIncomingCommands[] = {
  app::Clusters::WindowCovering::Commands::UpOrOpen::Id,
  app::Clusters::WindowCovering::Commands::DownOrClose::Id,
  app::Clusters::WindowCovering::Commands::StopMotion::Id,
  app::Clusters::WindowCovering::Commands::GoToLiftPercentage::Id,
  kInvalidCommandId,
};

// Window Covering cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(windowCoveringAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::Type::Id, INT8U, 1, 0),                               // Type
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Id, INT16U, 2, 0),  // CurrentPositionLiftPercent100ths
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id, INT16U, 2, 0),   // TargetPositionLiftPercent100ths
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::ConfigStatus::Id, INT8U, 1, 0),                       // ConfigStatus
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::OperationalStatus::Id, INT8U, 1, 0),                  // OperationalStatus
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::EndProductType::Id, INT8U, 1, 0),                     // EndProductType
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::Mode::Id, INT8U, 1, 0),                               // Mode
DECLARE_DYNAMIC_ATTRIBUTE(WindowCovering::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                      // FeatureMap
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                       // ClusterRevision auto added by LIST_END

// Window Covering endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(windowCoveringEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(WindowCovering::Id, windowCoveringAttrs, ZAP_CLUSTER_MASK(SERVER), windowCoveringIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

/***************************************************************************//**
 * Constructor for MatterWindowCovering
 ******************************************************************************/
MatterWindowCovering::MatterWindowCovering() :
  window_covering_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterWindowCovering
 ******************************************************************************/
MatterWindowCovering::~MatterWindowCovering()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterWindowCovering instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterWindowCovering::begin()
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceWindowCovering* device = new (std::nothrow)DeviceWindowCovering("Window covering");
  if (device == nullptr) {
    return false;
  }
  device->SetReachable(true);
  device->SetProductName("Window covering");

  // Set the device instance pointer in the base class
  this->base_matter_device = device;

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(device);
    return false;
  }
  new_endpoint->cluster = windowCoveringEndpointClusters;
  new_endpoint->clusterCount = ArraySize(windowCoveringEndpointClusters);
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = ArraySize(windowCoveringEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_sensor_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_sensor_data_version == nullptr) {
    delete(device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = AddDeviceEndpoint(device,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gWindowCoveringDeviceTypes),
                                 Span<DataVersion>(new_sensor_data_version, dataversion_size), 1);
  if (result < 0) {
    delete(device);
    free(new_endpoint);
    free(new_sensor_data_version);
    return false;
  }

  this->window_covering_device = device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_sensor_data_version;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterWindowCovering instance
 ******************************************************************************/
void MatterWindowCovering::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->window_covering_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->window_covering_device);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the actual raw lift position of the window covering
 *
 * Range is 0-10000 where:
 *  - 0 is fully open
 *  - 10000 is fully closed
 *
 * @param lift_position The desired lift position
 ******************************************************************************/
void MatterWindowCovering::set_actual_lift_position_raw(uint16_t lift_position)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->window_covering_device->SetActualLiftPosition(lift_position);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Sets the actual lift position of the window covering as a percentage
 *
 * Range is 0-100 where:
 *  - 100% is fully open
 *  - 0% is fully closed
 *
 * @param lift_position_percent The desired lift position as a percentage
 ******************************************************************************/
void MatterWindowCovering::set_actual_lift_position_percent(uint8_t lift_position_percent)
{
  this->set_actual_lift_position_raw(DeviceWindowCovering::max_lift_position - (lift_position_percent * 100u));
}

/***************************************************************************//**
 * Gets the actual raw lift position of the window covering
 *
 * Range is 0-10000 where:
 *  - 0 is fully open
 *  - 10000 is fully closed
 *
 * @return The actual lift position
 ******************************************************************************/
uint16_t MatterWindowCovering::get_actual_lift_position_raw()
{
  return this->window_covering_device->GetActualLiftPosition();
}

/***************************************************************************//**
 * Gets the actual lift position of the window covering as a percentage
 *
 * Range is 0-100 where:
 *  - 100% is fully open
 *  - 0% is fully closed
 *
 * @return The actual lift position as a percentage
 ******************************************************************************/
uint8_t MatterWindowCovering::get_actual_lift_position_percent()
{
  return 100u - this->window_covering_device->GetActualLiftPosition() / 100u;
}

/***************************************************************************//**
 * Gets the requested raw lift position of the window covering
 *
 * Range is 0-10000 where:
 *  - 0 is fully open
 *  - 10000 is fully closed
 *
 * @return The requested lift position
 ******************************************************************************/
uint16_t MatterWindowCovering::get_requested_lift_position_raw()
{
  return this->window_covering_device->GetRequestedLiftPosition();
}

/***************************************************************************//**
 * Gets the requested lift position of the window covering as a percentage
 *
 * Range is 0-100 where:
 *  - 100% is fully open
 *  - 0% is fully closed
 *
 * @return The requested lift position as a percentage
 ******************************************************************************/
uint8_t MatterWindowCovering::get_requested_lift_position_percent()
{
  return 100u - this->window_covering_device->GetRequestedLiftPosition() / 100u;
}

/***************************************************************************//**
 * Assignment operator to set the window covering's actual raw lift position
 *
 * @param[in] lift_position The actual lift position
 ******************************************************************************/
void MatterWindowCovering::operator=(uint16_t lift_position)
{
  this->set_actual_lift_position_raw(lift_position);
}

/***************************************************************************//**
 * Sets the current ongoing operation of the window covering
 *
 * @param current_operation The current operation
 ******************************************************************************/
void MatterWindowCovering::set_current_operation(window_covering_current_operation_t current_operation)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  switch (current_operation) {
    case WINDOW_COVERING_OPENING:
      this->window_covering_device->SetCurrentOperationalStatus(DeviceWindowCovering::kOperationalStatus_Opening);
      break;
    case WINDOW_COVERING_CLOSING:
      this->window_covering_device->SetCurrentOperationalStatus(DeviceWindowCovering::kOperationalStatus_Closing);
      break;
    case WINDOW_COVERING_STOPPED:
      this->window_covering_device->SetCurrentOperationalStatus(DeviceWindowCovering::kOperationalStatus_Stopped);
      break;
    default:
      break;
  }
  PlatformMgr().UnlockChipStack();
}
