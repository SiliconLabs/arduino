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

#ifndef DEVICE_TIME_CLIENT_H
#define DEVICE_TIME_CLIENT_H

#include "ZigbeeDevice.h"

class DeviceTimeClient : public ZigbeeDevice {
public:
  DeviceTimeClient(const char* device_name, uint8_t endpoint_id);

  bool RequestTime(uint8_t coordinator_endpoint_id = 1);
  bool HasTime();
  uint32_t GetZigbeeTime();
  uint32_t GetUnixTime();
  uint32_t GetLocalUnixTime();
  bool HasTimeZone();
  int32_t GetTimeZone();
  uint8_t GetTimeStatus();
  void SetTimeUpdateCallback(void (*cb)(void));
  void HandleTimeReadAttributesResponse(const uint8_t* payload, uint16_t length);

  static void RegisterTimeClient(DeviceTimeClient* time_client);
  static void UnregisterTimeClient(DeviceTimeClient* time_client);
  static DeviceTimeClient* GetRegisteredTimeClient(uint8_t endpoint_id);
  static void HandleIncomingCommand(uint8_t endpoint_id,
                                    uint16_t cluster_id,
                                    bool cluster_specific,
                                    uint8_t direction,
                                    uint8_t command_id,
                                    const uint8_t* payload,
                                    uint16_t payload_length);

  void HandleAttributeChange(uint16_t cluster_id,
                             uint16_t attribute_id,
                             uint8_t size,
                             uint8_t* value) override;

private:
  static const uint32_t kZigbeeToUnixEpochOffsetSeconds = 946684800UL;
  static const uint32_t kInvalidZigbeeTime = 0xFFFFFFFFUL;
  static DeviceTimeClient* registered_time_client;

  static uint16_t ReadInt16u(const uint8_t* value);
  static uint32_t ReadInt32u(const uint8_t* value);
  static int32_t ReadInt32s(const uint8_t* value);
  static uint8_t AttributeDataSize(uint8_t attribute_type);
  static void AppendAttributeId(uint8_t* buffer, uint16_t attribute_id, uint8_t& offset);

  uint32_t zigbee_time;
  uint32_t local_time;
  int32_t time_zone;
  uint8_t time_status;
  bool time_valid;
  bool local_time_valid;
  bool time_zone_valid;
  void (*time_update_callback)(void);
};

#endif // DEVICE_TIME_CLIENT_H
