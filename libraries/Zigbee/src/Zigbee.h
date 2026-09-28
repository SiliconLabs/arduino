/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2026 Silicon Laboratories Inc. www.silabs.com
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

#ifndef ZIGBEE_H
#define ZIGBEE_H

#ifndef ARDUINO_ZIGBEE
  #error "Selected board/variant is not Zigbee compatible - please select the Zigbee protocol stack if your board supports it!"
#endif

#include "Arduino.h"
#include "devices/ZigbeeDevice.h"
#include "ZigbeeEndpoint.h"

class ArduinoZigbeeAppliance {
public:
  ArduinoZigbeeAppliance();
  virtual ~ArduinoZigbeeAppliance();
  virtual bool begin() = 0;
  virtual void end() = 0;
  bool get_identify_in_progress();
  bool is_online();
  void set_device_change_callback(void (*cb)(void));

protected:
  ZigbeeDevice* base_zigbee_device;
};

enum ZigbeeEndpointType {
  ZIGBEE_ON_OFF_LIGHT = 0,
  ZIGBEE_TEMPERATURE_SENSOR = 1,
  ZIGBEE_HUMIDITY_SENSOR = 2,
  ZIGBEE_ON_OFF_SWITCH = 3,
  ZIGBEE_DIMMABLE_LIGHT = 4,
  ZIGBEE_LIGHT_SENSOR = 5,
  ZIGBEE_COLOR_LIGHT = 6,
  ZIGBEE_ON_OFF_PLUGIN_UNIT = 7,
  ZIGBEE_CONTACT_SENSOR = 8,
  ZIGBEE_POWER_SOURCE = 9,
  ZIGBEE_THERMOSTAT = 10,
  ZIGBEE_ENDPOINT_TYPE_COUNT = 11
};

enum ZigbeeDeviceType {
  ZIGBEE_DEVICE_TYPE_ROUTER = 0,
  ZIGBEE_DEVICE_TYPE_END_DEVICE = 1
};

enum ZigbeePowerSourceType {
  ZIGBEE_POWER_SOURCE_TYPE_MAINS = 0,
  ZIGBEE_POWER_SOURCE_TYPE_BATTERY = 1
};

class ZigbeeClass {
public:
  static const uint8_t kEndpointsPerType = 3u;
  static const uint8_t kApplicationEndpointCount = 33u;
  static const uint8_t kTimeClientEndpointId = 240u;
  static constexpr uint8_t kMinPairingChannel = 11u;
  static constexpr uint8_t kMaxPairingChannel = 26u;
  static constexpr uint32_t kAllPairingChannelsMask = 0x07FFF800UL;

  ZigbeeClass();

  void begin();
  void setVendorName(const char* name);
  void setProductName(const char* name);
  void setFirmwareVersion(const char* version);
  void setFirmwareVersion(uint32_t file_version);
  bool setPairingChannel(uint8_t channel);
  bool setDeviceType(ZigbeeDeviceType device_type);
  ZigbeeDeviceType getDeviceType();
  bool setPowerSource(ZigbeePowerSourceType power_source);
  ZigbeePowerSourceType getPowerSource();

  bool isPaired();
  bool isConnectedToNetwork();
  uint8_t getChannel();
  uint16_t getPanId();
  uint16_t getNodeId();

  void leaveNetwork();
  void factoryReset();

  uint8_t allocateEndpoint(ZigbeeEndpointType type);
  uint8_t allocateTimeClientEndpoint();
  void freeEndpoint(uint8_t endpoint_id);

private:
  static uint32_t channelToMask(uint8_t channel);
  static bool isValidPairingChannelMask(uint32_t channel_mask);
  static uint32_t sanitizePairingChannelMask(uint32_t channel_mask);
  static bool parseFirmwareVersion(const char* version, uint32_t* file_version);
  static uint16_t getPowerDescriptor(ZigbeePowerSourceType power_source);
  bool setPairingChannelMask(uint32_t primary_channel_mask, uint32_t secondary_channel_mask = 0);

  bool started;
  bool endpoint_allocated[kApplicationEndpointCount];
  bool time_client_endpoint_allocated;
  ZigbeeDeviceType device_type;
  ZigbeePowerSourceType power_source;
};

extern ZigbeeClass Zigbee;

#endif // ZIGBEE_H
