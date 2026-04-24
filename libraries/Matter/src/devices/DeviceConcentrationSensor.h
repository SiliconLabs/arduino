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

class DeviceConcentrationMeasurementSensor : public Device
{
public:
  enum Changed_t{
    kChanged_MeasurementValue = kChanged_Last << 1,
  } Changed;

  enum measurement_unit_t : uint8_t {
    kUnit_PPM = 0u,
    kUnit_PPB,
    kUnit_PPT,
    kUnit_MGM3,
    kUnit_UGM3,
    kUnit_NGM3,
    kUnit_PM3,
    kUnit_BQM3
  };

  DeviceConcentrationMeasurementSensor(const char* device_name,
                                       device_type_t device_type,
                                       measurement_unit_t measurement_unit,
                                       float min_measured_value,
                                       float max_measured_value,
                                       float measured_value);

  float GetMeasuredValue();
  void SetMeasuredValue(float measurement);
  float GetConcentrationMeasurementClusterMinMeasurementValue();
  float GetConcentrationMeasurementClusterMaxMeasurementValue();
  uint8_t GetConcentrationMeasurementClusterMeasurementUnit();
  uint8_t GetConcentrationMeasurementClusterMeasurementMedium();
  uint32_t GetConcentrationMeasurementClusterFeatureMap();
  uint16_t GetConcentrationMeasurementClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

private:
  void HandleConcentrationMeasurementSensorDeviceStatusChanged(Changed_t itemChangedMask);

  float measured_value;
  float min_measured_value;
  float max_measured_value;

  uint8_t concentration_measurement_cluster_measurement_unit;
  static const uint8_t concentration_measurement_cluster_measurement_medium = 0u; // medium is 'air'
  static const uint32_t concentration_measurement_cluster_feature_map = 1u; // Numeric value measurement enabled
  static const uint16_t concentration_measurement_cluster_revision    = 3u;
};
