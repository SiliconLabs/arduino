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

#ifndef MATTER_LIGHTBULB_H
#define MATTER_LIGHTBULB_H

#include "Matter.h"
#include "devices/DeviceLightbulb.h"
#include <platform/CHIPDeviceLayer.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/callback.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

class MatterLightbulb : public ArduinoMatterAppliance {
public:
  MatterLightbulb();
  ~MatterLightbulb();
  virtual bool begin();
  void end();

  void set_onoff(bool value);
  bool get_onoff();
  void toggle();
  void set_startup_on_off(DeviceLightbulb::StartupOnOff_t startup_on_off);
  DeviceLightbulb::StartupOnOff_t get_startup_on_off();

  operator bool();
  void operator=(bool state);

protected:
  enum bulb_types_e {
    lightbulb_simple,
    lightbulb_dimmable,
    lightbulb_color
  };

  bool begin_internal(bulb_types_e bulb_type, EmberAfCluster* endpoint_clusters, uint8_t cluster_count);
  DeviceLightbulb* lightbulb_device;
  EmberAfEndpointType* device_endpoint;
  DataVersion* endpoint_dataversion_storage;
  ::Identify* identify_server;
  bool initialized;
};

class MatterDimmableLightbulb : public MatterLightbulb {
public:
  MatterDimmableLightbulb();
  ~MatterDimmableLightbulb();
  virtual bool begin() override;

  uint8_t get_brightness();
  void set_brightness(uint8_t brightness);
  uint8_t get_brightness_percent();
  void set_brightness_percent(uint8_t brightness);

  operator uint8_t();
  void operator=(bool state);
  void operator=(unsigned int brightness);
};

class MatterColorLightbulb : public MatterDimmableLightbulb {
public:
  MatterColorLightbulb();
  ~MatterColorLightbulb();
  virtual bool begin() override;

  uint8_t get_hue();
  uint8_t get_saturation();
  void set_hue(uint8_t hue);
  void set_saturation(uint8_t saturation);

  uint16_t get_true_hue();
  uint8_t get_saturation_percent();
  void set_true_hue(uint16_t true_hue);
  void set_saturation_percent(uint8_t saturation);

  void get_rgb(uint8_t* r, uint8_t* g, uint8_t* b);
  void get_rgb_raw(uint8_t* r, uint8_t* g, uint8_t* b);
  void set_rgb(uint8_t r, uint8_t g, uint8_t b);
  void boost_saturation(uint8_t amount);

  void operator=(bool state);
  void operator=(unsigned int brightness);

protected:
  uint8_t saturation_boost;
};

#endif // MATTER_LIGHTBULB_H
