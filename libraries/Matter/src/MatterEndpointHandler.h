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

#ifndef MATTER_ENDPOINT_HANDLER_H
#define MATTER_ENDPOINT_HANDLER_H

#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h>
#include "devices/MatterDevice.h"

using ::chip::Span;
using ::chip::DataVersion;
using ::chip::EndpointId;

void InitDynamicEndpointHandler();

int AddDeviceEndpoint(Device* dev, EmberAfEndpointType* ep,
                      const Span<const EmberAfDeviceType>& deviceTypeList,
                      const Span<DataVersion>& dataVersionStorage,
                      chip::EndpointId parentEndpointId = chip::kInvalidEndpointId);

int RemoveDeviceEndpoint(Device* dev);

Device* GetDeviceForEndpointIndex(uint16_t endpointIndex);

#endif // MATTER_ENDPOINT_HANDLER_H
