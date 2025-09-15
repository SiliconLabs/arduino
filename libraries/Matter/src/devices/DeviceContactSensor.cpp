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

#include "DeviceContactSensor.h"

DeviceContactSensor::DeviceContactSensor(const char* device_name) :
  Device(device_name),
  state_value(false)
{
  this->SetDeviceType(device_type_t::kDeviceType_ContactSensor);
}

bool DeviceContactSensor::GetStateValue()
{
  return this->state_value;
}

void DeviceContactSensor::SetStateValue(bool state_value)
{
  bool changed = this->state_value != state_value;
  if (changed) {
    ChipLogProgress(DeviceLayer, "ContactSensorDevice[%s]: new state='%d'", this->device_name, state_value);
    this->state_value = state_value;
    this->HandleContactSensorDeviceStatusChanged(kChanged_StateValue);
    CallDeviceChangeCallback();
  }
}

uint16_t DeviceContactSensor::GetBooleanStateClusterRevision()
{
  return this->boolean_state_cluster_revision;
}

CHIP_ERROR DeviceContactSensor::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                           chip::AttributeId attributeId,
                                                           uint8_t* buffer,
                                                           uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters::BooleanState::Attributes;
  ChipLogProgress(DeviceLayer, "HandleReadContactSensorAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == chip::app::Clusters::BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != chip::app::Clusters::BooleanState::Id) {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if ((attributeId == StateValue::Id) && (maxReadLength == 1)) {
    uint8_t stateValue = (uint8_t)this->GetStateValue();
    memcpy(buffer, &stateValue, sizeof(stateValue));
  } else if ((attributeId == ClusterRevision::Id) && (maxReadLength == 2)) {
    uint16_t clusterRevision = this->GetBooleanStateClusterRevision();
    memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
  } else {
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  return CHIP_NO_ERROR;
}

void DeviceContactSensor::HandleContactSensorDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_StateValue) {
    ScheduleMatterReportingCallback(this->endpoint_id, BooleanState::Id, BooleanState::Attributes::StateValue::Id);
  }
}
