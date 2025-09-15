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

#pragma once

#include "MatterDevice.h"

class DeviceIlluminanceSensor : public Device
{
public:
  enum Changed_t{
    kChanged_MeasurementValue = kChanged_Last << 1,
  } Changed;

  DeviceIlluminanceSensor(const char* device_name, uint16_t min, uint16_t max, uint16_t measured_value);

  uint16_t GetMeasuredValue();
  void SetMeasuredValue(uint16_t measurement);
  uint32_t GetIlluminanceSensorClusterFeatureMap();
  uint16_t GetIlluminanceSensorClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

  const uint16_t min_value;
  const uint16_t max_value;

private:
  void HandleIlluminanceSensorDeviceStatusChanged(Changed_t itemChangedMask);

  uint16_t measured_value;

  static const uint32_t illuminance_sensor_cluster_feature_map = 0u;   // No additional features enabled
  static const uint16_t illuminance_sensor_cluster_revision    = 3u;
};
