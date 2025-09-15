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

#ifndef MATTER_ENDPOINT_H
#define MATTER_ENDPOINT_H

#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h>
#include "MatterEndpointHandler.h"
#include "devices/MatterDevice.h"

// Device types for dynamic endpoints
// (taken from matter-devices.xml)
#define DEVICE_TYPE_BRIDGED_NODE 0x0013
#define DEVICE_TYPE_ON_OFF_LIGHT 0x0100
#define DEVICE_TYPE_DIMMABLE_LIGHT 0x0101
#define DEVICE_TYPE_EXTENDED_COLOR_LIGHT 0x010D
#define DEVICE_TYPE_POWER_SOURCE 0x0011
#define DEVICE_TYPE_TEMP_SENSOR 0x0302
#define DEVICE_TYPE_HUMIDITY_SENSOR 0x0307
#define DEVICE_TYPE_SWITCH 0x000F
#define DEVICE_TYPE_ILLUMINANCE_SENSOR 0x0106
#define DEVICE_TYPE_PRESSURE_SENSOR 0x0305
#define DEVICE_TYPE_OCCUPANCY_SENSOR 0x0107
#define DEVICE_TYPE_FLOW_SENSOR 0x0306
#define DEVICE_TYPE_FAN 0x002B
#define DEVICE_TYPE_THERMOSTAT 0x0301
#define DEVICE_TYPE_CONTACT_SENSOR 0x0015
#define DEVICE_TYPE_WINDOW_COVERING 0x0202
#define DEVICE_TYPE_ONOFF_PLUGIN_UNIT 0x010A
#define DEVICE_TYPE_AIR_QUALITY_SENSOR 0x002C
#define DEVICE_TYPE_DOOR_LOCK 0x000A
#define DEVICE_TYPE_AIR_PURIFIER 0x002D

// Device Version for dynamic endpoints
#define DEVICE_VERSION_DEFAULT 1

#endif // MATTER_ENDPOINT_H
