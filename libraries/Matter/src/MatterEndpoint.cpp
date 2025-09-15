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

#include <sl_common.h>
#include <app/CommandHandler.h>
#include <app/data-model/Nullable.h>
#include <app-common/zap-generated/cluster-enums.h>
#include "MatterEndpoint.h"
#include "devices/DeviceDoorLock.h"
#include "devices/DeviceWindowCovering.h"
#include "devices/MatterDevice.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;
using ::chip::app::DataModel::Nullable;

using chip::Protocols::InteractionModel::Status;
using namespace chip::app::Clusters::WindowCovering;

template<typename Base, typename T>
inline bool instanceof(const T* ptr)
{
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

Status emberAfExternalAttributeReadCallback(EndpointId endpoint,
                                            ClusterId clusterId,
                                            const EmberAfAttributeMetadata* attributeMetadata,
                                            uint8_t* buffer,
                                            uint16_t maxReadLength)
{
  ChipLogProgress(DeviceLayer, "emberAfExternalAttributeReadCallback");
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);

  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev) {
    return Status::Failure;
  }

  CHIP_ERROR result = dev->HandleReadEmberAfAttribute(clusterId, attributeMetadata->attributeId, buffer, maxReadLength);
  if (result != CHIP_NO_ERROR) {
    return Status::Failure;
  }
  if (!dev->IsOnline()) {
    dev->SetOnline(true);
  }
  return Status::Success;
}

Status emberAfExternalAttributeWriteCallback(EndpointId endpoint,
                                             ClusterId clusterId,
                                             const EmberAfAttributeMetadata* attributeMetadata,
                                             uint8_t* buffer)
{
  ChipLogProgress(DeviceLayer, "emberAfExternalAttributeWriteCallback");
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpoint);

  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev) {
    return Status::Failure;
  }

  CHIP_ERROR result = dev->HandleWriteEmberAfAttribute(clusterId, attributeMetadata->attributeId, buffer);
  if (result != CHIP_NO_ERROR) {
    return Status::Failure;
  }
  return Status::Success;
}

bool emberAfWindowCoveringClusterUpOrOpenCallback(app::CommandHandler* commandObj,
                                                  const app::ConcreteCommandPath& commandPath,
                                                  const WindowCovering::Commands::UpOrOpen::DecodableType& commandData)
{
  (void)commandData;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterUpOrOpenCallback");

  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    commandObj->AddStatus(commandPath, Status::Failure);
    return false;
  }
  if (dev->GetDeviceType() == Device::device_type_t::kDeviceType_WindowCovering) {
    DeviceWindowCovering* window_covering_device = static_cast<DeviceWindowCovering*>(dev);
    window_covering_device->SetRequestedLiftPosition(0u);
    commandObj->AddStatus(commandPath, Status::Success);
    return true;
  }
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterDownOrCloseCallback(app::CommandHandler* commandObj,
                                                     const app::ConcreteCommandPath& commandPath,
                                                     const WindowCovering::Commands::DownOrClose::DecodableType& commandData)
{
  (void)commandData;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterDownOrCloseCallback");

  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    commandObj->AddStatus(commandPath, Status::Failure);
    return false;
  }
  if (dev->GetDeviceType() == Device::device_type_t::kDeviceType_WindowCovering) {
    DeviceWindowCovering* window_covering_device = static_cast<DeviceWindowCovering*>(dev);
    window_covering_device->SetRequestedLiftPosition(10000u);
    commandObj->AddStatus(commandPath, Status::Success);
    return true;
  }
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterStopMotionCallback(app::CommandHandler* commandObj,
                                                    const app::ConcreteCommandPath& commandPath,
                                                    const WindowCovering::Commands::StopMotion::DecodableType& fields)
{
  (void)fields;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterStopMotionCallback");
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterGoToLiftValueCallback(app::CommandHandler* commandObj,
                                                       const app::ConcreteCommandPath& commandPath,
                                                       const WindowCovering::Commands::GoToLiftValue::DecodableType& commandData)
{
  (void)commandData;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterGoToLiftValueCallback");
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterGoToLiftPercentageCallback(app::CommandHandler* commandObj,
                                                            const app::ConcreteCommandPath& commandPath,
                                                            const WindowCovering::Commands::GoToLiftPercentage::DecodableType& commandData)
{
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterGoToLiftPercentageCallback");
  uint16_t percent = commandData.liftPercent100thsValue;
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(commandPath.mEndpointId);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    commandObj->AddStatus(commandPath, Status::Failure);
    return false;
  }
  if (dev->GetDeviceType() == Device::device_type_t::kDeviceType_WindowCovering) {
    DeviceWindowCovering* window_covering_device = static_cast<DeviceWindowCovering*>(dev);
    window_covering_device->SetRequestedLiftPosition(percent);
    commandObj->AddStatus(commandPath, Status::Success);
    return true;
  }
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterGoToTiltValueCallback(app::CommandHandler* commandObj,
                                                       const app::ConcreteCommandPath& commandPath,
                                                       const WindowCovering::Commands::GoToTiltValue::DecodableType& commandData)
{
  (void)commandData;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterGoToTiltValueCallback");
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

bool emberAfWindowCoveringClusterGoToTiltPercentageCallback(app::CommandHandler* commandObj,
                                                            const app::ConcreteCommandPath& commandPath,
                                                            const WindowCovering::Commands::GoToTiltPercentage::DecodableType& commandData)
{
  (void)commandData;
  ChipLogProgress(DeviceLayer, "emberAfWindowCoveringClusterGoToTiltPercentageCallback");
  commandObj->AddStatus(commandPath, Status::Failure);
  return false;
}

//
// @brief Cluster Attribute Changed Callback
//
// The method is implemented by default as a weak function and it takes care of updating
// the server attribute values by calling the PostAttributeChange method. If the application overrides
// this method, it needs to handle updating attributes (ideally by calling PostAttributeChange).
//
SL_WEAK void MatterWindowCoveringClusterServerAttributeChangedCallback(const app::ConcreteAttributePath& attributePath)
{
  (void)attributePath;
}

void MatterWindowCoveringPluginServerInitCallback()
{
  ;
}

bool emberAfPluginDoorLockOnDoorLockCommand(chip::EndpointId endpointId,
                                            const Nullable<chip::FabricIndex>& fabricIdx,
                                            const Nullable<chip::NodeId>& nodeId,
                                            const Optional<chip::ByteSpan>& pinCode,
                                            DoorLock::OperationErrorEnum& err)
{
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpointId);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    return false;
  }
  if (dev->GetDeviceType() == Device::device_type_t::kDeviceType_DoorLock) {
    DeviceDoorLock* door_lock_device = static_cast<DeviceDoorLock*>(dev);
    door_lock_device->SetLockState(DeviceDoorLock::lock_state_t::LOCKED);
    return true;
  }
  return false;
}

bool emberAfPluginDoorLockOnDoorUnlockCommand(chip::EndpointId endpointId,
                                              const Nullable<chip::FabricIndex>& fabricIdx,
                                              const Nullable<chip::NodeId>& nodeId,
                                              const Optional<chip::ByteSpan>& pinCode,
                                              DoorLock::OperationErrorEnum& err)
{
  uint16_t endpointIndex = emberAfGetDynamicIndexFromEndpoint(endpointId);
  Device* dev = GetDeviceForEndpointIndex(endpointIndex);
  if (!dev || !dev->IsReachable()) {
    return false;
  }
  if (dev->GetDeviceType() == Device::device_type_t::kDeviceType_DoorLock) {
    DeviceDoorLock* door_lock_device = static_cast<DeviceDoorLock*>(dev);
    door_lock_device->SetLockState(DeviceDoorLock::lock_state_t::UNLOCKED);
    return true;
  }
  return false;
}
