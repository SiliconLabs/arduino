/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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

#ifndef MATTER_ENDPOINT_H
#define MATTER_ENDPOINT_H

#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h>
#include "MatterEndpointHandler.h"
#include "devices/MatterDevice.h"
#include "devices/DeviceLightbulb.h"
#include "devices/DeviceHumiditySensor.h"
#include "devices/DeviceTempSensor.h"
#include "devices/DeviceSwitch.h"
#include "devices/DeviceIlluminanceSensor.h"
#include "devices/DevicePressureSensor.h"
#include "devices/DeviceOccupancySensor.h"
#include "devices/DeviceFlowSensor.h"
#include "devices/DeviceFan.h"
#include "devices/DeviceThermostat.h"

// Device types for dynamic endpoints: TODO Need a generated file from ZAP to define these!
// (taken from matter-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
#define DEVICE_TYPE_ON_OFF_LIGHT 0x0100
#define DEVICE_TYPE_DIMMABLE_LIGHT 0x0101
#define DEVICE_TYPE_POWER_SOURCE 0x0011
#define DEVICE_TYPE_TEMP_SENSOR 0x0302
#define DEVICE_TYPE_HUMIDITY_SENSOR 0x0307
#define DEVICE_TYPE_SWITCH 0x000f
#define DEVICE_TYPE_ILLUMINANCE_SENSOR 0x0106
#define DEVICE_TYPE_PRESSURE_SENSOR 0x0305
#define DEVICE_TYPE_OCCUPANCY_SENSOR 0x0107
#define DEVICE_TYPE_FLOW_SENSOR 0x0306
#define DEVICE_TYPE_FAN 0x002B
#define DEVICE_TYPE_THERMOSTAT 0x0301

// Device Version for dynamic endpoints:
#define DEVICE_VERSION_DEFAULT 1

#define ZCL_DESCRIPTOR_CLUSTER_REVISION (1u)
#define ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_CLUSTER_REVISION (2u)
#define ZCL_BRIDGED_DEVICE_BASIC_INFORMATION_FEATURE_MAP (0u)
#define ZCL_FIXED_LABEL_CLUSTER_REVISION (1u)
#define ZCL_ON_OFF_CLUSTER_REVISION (4u)
#define ZCL_POWER_SOURCE_CLUSTER_REVISION (2u)

extern const uint16_t kDescriptorAttributeArraySize;

using namespace ::chip;

void HandleDeviceOnOffStatusChanged(DeviceOnOff* dev, DeviceOnOff::Changed_t itemChangedMask);
void HandleDeviceTempSensorStatusChanged(DeviceTempSensor* dev, DeviceTempSensor::Changed_t itemChangedMask);
void HandleDeviceHumiditySensorStatusChanged(DeviceHumiditySensor* dev, DeviceHumiditySensor::Changed_t itemChangedMask);
void HandleDeviceLightbulbStatusChanged(DeviceLightbulb* dev, DeviceLightbulb::Changed_t itemChangedMask);
void HandleDeviceSwitchStatusChanged(DeviceSwitch* dev, DeviceSwitch::Changed_t itemChangedMask);
void HandleDeviceIlluminanceSensorStatusChanged(DeviceIlluminanceSensor* dev, DeviceIlluminanceSensor::Changed_t itemChangedMask);
void HandleDevicePressureSensorStatusChanged(DevicePressureSensor* dev, DevicePressureSensor::Changed_t itemChangedMask);
void HandleDeviceOccupancySensorStatusChanged(DeviceOccupancySensor* dev, DeviceOccupancySensor::Changed_t itemChangedMask);
void HandleDeviceFlowSensorStatusChanged(DeviceFlowSensor* dev, DeviceFlowSensor::Changed_t itemChangedMask);
void HandleDeviceFanStatusChanged(DeviceFan* dev, DeviceFan::Changed_t itemChangedMask);
void HandleDeviceThermostatStatusChanged(DeviceThermostat* dev, DeviceThermostat::Changed_t itemChangedMask);

#endif // MATTER_ENDPOINT_H
