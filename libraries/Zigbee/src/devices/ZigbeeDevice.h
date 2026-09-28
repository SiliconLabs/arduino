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

#ifndef ZIGBEE_DEVICE_H
#define ZIGBEE_DEVICE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

class ZigbeeDevice {
public:
  static const uint8_t kMaxNameSize = 32;
  static const uint8_t kMaxEndpoints = 34;

  ZigbeeDevice(const char* device_name, uint8_t endpoint_id);
  virtual ~ZigbeeDevice();

  bool IsOnline();
  void SetOnline(bool online);
  uint8_t GetEndpointId();
  const char* GetName();
  bool GetIdentifyInProgress();

  void HandleIdentifyStart(uint16_t identify_time);
  void HandleIdentifyStop();

  void SetDeviceChangeCallback(void (*cb)(void));
  void CallDeviceChangeCallback();

  virtual uint32_t HandleAttributePreChange(uint16_t cluster_id,
                                            uint16_t attribute_id,
                                            uint8_t size,
                                            uint8_t* value);

  virtual bool HandleCommand(uint16_t cluster_id,
                             bool cluster_specific,
                             uint8_t direction,
                             uint8_t command_id,
                             const uint8_t* payload,
                             uint16_t payload_length,
                             uint8_t& status);

  virtual void HandleAttributeChange(uint16_t cluster_id,
                                     uint16_t attribute_id,
                                     uint8_t size,
                                     uint8_t* value) = 0;

protected:
  char device_name[kMaxNameSize + 1];
  uint8_t endpoint_id;
  bool online;
  bool identify_in_progress;
  uint16_t identify_time;
  void (*device_change_callback)(void);
};

#endif // ZIGBEE_DEVICE_H
