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

#include "MatterEndpointHandler.h"
#include <util/endpoint-config-api.h>
#include <platform/PlatformManager.h>

using namespace ::chip;
using namespace chip::DeviceLayer;

EndpointId gCurrentEndpointId;
EndpointId gFirstDynamicEndpointId;
Device* gDevices[CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT + 1];

void InitDynamicEndpointHandler()
{
  memset(gDevices, 0, sizeof(gDevices));

  gFirstDynamicEndpointId = static_cast<chip::EndpointId>(
    static_cast<int>(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1))) + 1);
  gCurrentEndpointId = gFirstDynamicEndpointId;

  // Disable last fixed endpoint, which is used as a placeholder for all of the
  // supported clusters so that ZAP will generated the requisite code.
  emberAfEndpointEnableDisable(emberAfEndpointFromIndex(static_cast<uint16_t>(emberAfFixedEndpointCount() - 1)), false);
}

int AddDeviceEndpoint(Device* dev, EmberAfEndpointType* ep,
                      const Span<const EmberAfDeviceType>& deviceTypeList,
                      const Span<DataVersion> & dataVersionStorage,
                      chip::EndpointId parentEndpointId)
{
  uint8_t index = 0u;
  while (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT) {
    if (nullptr == gDevices[index]) {
      gDevices[index] = dev;
      CHIP_ERROR err;
      while (true) {
        // Todo: Update this to schedule the work rather than use this lock
        DeviceLayer::StackLock lock;
        dev->SetEndpointId(gCurrentEndpointId);
        dev->SetParentEndpointId(parentEndpointId);
        err = emberAfSetDynamicEndpoint(index, gCurrentEndpointId, ep, dataVersionStorage, deviceTypeList, parentEndpointId);
        if (err == CHIP_NO_ERROR) {
          ChipLogProgress(DeviceLayer, "Added device %s to dynamic endpoint %d (index=%u)",
                          dev->GetName(), gCurrentEndpointId, index);
          return index;
        }
        if (err != CHIP_ERROR_ENDPOINT_EXISTS) {
          gDevices[index] = nullptr;
          ChipLogProgress(DeviceLayer, "Failed to add dynamic endpoint; err='0x%lx'", err.AsInteger());
          return -1;
        }
        // Handle wrap condition
        if (++gCurrentEndpointId < gFirstDynamicEndpointId) {
          gCurrentEndpointId = gFirstDynamicEndpointId;
        }
      }
    }
    index++;
  }
  ChipLogProgress(DeviceLayer, "Failed to add dynamic endpoint: No endpoints available!");
  return -1;
}

int RemoveDeviceEndpoint(Device* dev)
{
  uint8_t index = 0u;
  while (index < CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT) {
    if (gDevices[index] == dev) {
      // Todo: Update this to schedule the work rather than use this lock
      DeviceLayer::StackLock lock;
      EndpointId ep   = emberAfClearDynamicEndpoint(index);
      gDevices[index] = nullptr;
      ChipLogProgress(DeviceLayer, "Removed device %s from dynamic endpoint %d (index=%d)", dev->GetName(), ep, index);
      // Silence complaints about unused ep when progress logging is disabled
      (void)ep;
      return index;
    }
    index++;
  }
  return -1;
}

Device* GetDeviceForEndpointIndex(uint16_t endpointIndex)
{
  if (endpointIndex >= CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT) {
    return nullptr;
  }
  return gDevices[endpointIndex];
}
