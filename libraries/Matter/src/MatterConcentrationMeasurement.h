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

#ifndef MATTER_CONCENTRATION_MEASUREMENT_H
#define MATTER_CONCENTRATION_MEASUREMENT_H

#include "Matter.h"
#include "devices/DeviceConcentrationSensor.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterConcentrationMeasurement : public ArduinoMatterAppliance {
public:
  MatterConcentrationMeasurement();
  ~MatterConcentrationMeasurement();
  virtual bool begin();
  void end();
  void set_measured_value(float value);
  float get_measured_value();
  void set_measured_value(uint32_t value);

protected:
  enum concentration_sensor_t {
    sensor_co2,
    sensor_co,
    sensor_tvoc
  };

  bool begin_internal(concentration_sensor_t sensor_type,
                      EmberAfCluster* endpoint_clusters,
                      uint8_t cluster_count);

  DeviceConcentrationMeasurementSensor* sensor_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  bool initialized;
};

class MatterCO2 : public MatterConcentrationMeasurement {
public:
  MatterCO2();
  ~MatterCO2();
  virtual bool begin() override;
  void operator=(float value);
};

class MatterCO : public MatterConcentrationMeasurement {
public:
  MatterCO();
  ~MatterCO();
  virtual bool begin() override;
  void operator=(float value);
};

class MatterTVOC : public MatterConcentrationMeasurement {
public:
  MatterTVOC();
  ~MatterTVOC();
  virtual bool begin() override;
  void operator=(float value);
};

#endif // MATTER_CONCENTRATION_MEASUREMENT_H
