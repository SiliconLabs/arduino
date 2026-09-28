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

#include "DeviceTimeClient.h"
#include "../ZigbeeAfLock.h"

extern "C" {
#include "af.h"
}

DeviceTimeClient* DeviceTimeClient::registered_time_client = nullptr;

DeviceTimeClient::DeviceTimeClient(const char* device_name, uint8_t endpoint_id) :
  ZigbeeDevice(device_name, endpoint_id),
  zigbee_time(kInvalidZigbeeTime),
  local_time(kInvalidZigbeeTime),
  time_zone(0),
  time_status(0),
  time_valid(false),
  local_time_valid(false),
  time_zone_valid(false),
  time_update_callback(nullptr)
{
}

bool DeviceTimeClient::RequestTime(uint8_t coordinator_endpoint_id)
{
  ZigbeeAfLock lock;

  if (sl_zigbee_af_network_state() != SL_ZIGBEE_JOINED_NETWORK) {
    return false;
  }

  uint8_t attribute_ids[8];
  uint8_t offset = 0;
  AppendAttributeId(attribute_ids, ZCL_TIME_ATTRIBUTE_ID, offset);
  AppendAttributeId(attribute_ids, ZCL_TIME_STATUS_ATTRIBUTE_ID, offset);
  AppendAttributeId(attribute_ids, ZCL_TIME_ZONE_ATTRIBUTE_ID, offset);
  AppendAttributeId(attribute_ids, ZCL_LOCAL_TIME_ATTRIBUTE_ID, offset);

  sl_zigbee_af_set_command_endpoints(this->endpoint_id, coordinator_endpoint_id);
  sl_zigbee_af_fill_command_global_client_to_server_read_attributes(ZCL_TIME_CLUSTER_ID,
                                                                    attribute_ids,
                                                                    offset);
  return sl_zigbee_af_send_command_unicast(SL_ZIGBEE_OUTGOING_DIRECT, 0x0000) == SL_STATUS_OK;
}

bool DeviceTimeClient::HasTime()
{
  return this->time_valid;
}

uint32_t DeviceTimeClient::GetZigbeeTime()
{
  return this->time_valid ? this->zigbee_time : 0;
}

uint32_t DeviceTimeClient::GetUnixTime()
{
  if (!this->time_valid) {
    return 0;
  }
  return this->zigbee_time + kZigbeeToUnixEpochOffsetSeconds;
}

uint32_t DeviceTimeClient::GetLocalUnixTime()
{
  if (this->local_time_valid) {
    return this->local_time + kZigbeeToUnixEpochOffsetSeconds;
  }

  if (!this->time_valid) {
    return 0;
  }

  if (!this->time_zone_valid) {
    return GetUnixTime();
  }

  int64_t local_unix_time = static_cast<int64_t>(GetUnixTime()) + this->time_zone;
  if (local_unix_time < 0) {
    return 0;
  }
  if (local_unix_time > 0xFFFFFFFFLL) {
    return 0xFFFFFFFFUL;
  }
  return static_cast<uint32_t>(local_unix_time);
}

bool DeviceTimeClient::HasTimeZone()
{
  return this->time_zone_valid;
}

int32_t DeviceTimeClient::GetTimeZone()
{
  return this->time_zone_valid ? this->time_zone : 0;
}

uint8_t DeviceTimeClient::GetTimeStatus()
{
  return this->time_status;
}

void DeviceTimeClient::SetTimeUpdateCallback(void (*cb)(void))
{
  this->time_update_callback = cb;
}

void DeviceTimeClient::HandleTimeReadAttributesResponse(const uint8_t* payload, uint16_t length)
{
  if (payload == nullptr) {
    return;
  }

  bool updated = false;
  uint16_t offset = 0;
  while ((offset + 3) <= length) {
    uint16_t attribute_id = ReadInt16u(&payload[offset]);
    offset += 2;
    uint8_t status = payload[offset++];

    if (status != static_cast<uint8_t>(SL_ZIGBEE_ZCL_STATUS_SUCCESS)) {
      continue;
    }

    if (offset >= length) {
      break;
    }
    uint8_t attribute_type = payload[offset++];
    uint8_t attribute_size = AttributeDataSize(attribute_type);
    if (attribute_size == 0 || (offset + attribute_size) > length) {
      break;
    }

    const uint8_t* value = &payload[offset];
    if (attribute_id == ZCL_TIME_ATTRIBUTE_ID
        && attribute_type == ZCL_UTC_TIME_ATTRIBUTE_TYPE
        && attribute_size == 4) {
      uint32_t new_time = ReadInt32u(value);
      this->zigbee_time = new_time;
      this->time_valid = (new_time != kInvalidZigbeeTime);
      updated = this->time_valid || updated;
    } else if (attribute_id == ZCL_TIME_STATUS_ATTRIBUTE_ID
               && attribute_type == ZCL_BITMAP8_ATTRIBUTE_TYPE
               && attribute_size == 1) {
      this->time_status = value[0];
      updated = true;
    } else if (attribute_id == ZCL_TIME_ZONE_ATTRIBUTE_ID
               && attribute_type == ZCL_INT32S_ATTRIBUTE_TYPE
               && attribute_size == 4) {
      this->time_zone = ReadInt32s(value);
      this->time_zone_valid = true;
      updated = true;
    } else if (attribute_id == ZCL_LOCAL_TIME_ATTRIBUTE_ID
               && attribute_type == ZCL_UTC_TIME_ATTRIBUTE_TYPE
               && attribute_size == 4) {
      uint32_t new_local_time = ReadInt32u(value);
      this->local_time = new_local_time;
      this->local_time_valid = (new_local_time != kInvalidZigbeeTime);
      updated = this->local_time_valid || updated;
    }

    offset += attribute_size;
  }

  if (updated && this->time_update_callback) {
    this->time_update_callback();
  }
}

void DeviceTimeClient::RegisterTimeClient(DeviceTimeClient* time_client)
{
  registered_time_client = time_client;
}

void DeviceTimeClient::UnregisterTimeClient(DeviceTimeClient* time_client)
{
  if (registered_time_client == time_client) {
    registered_time_client = nullptr;
  }
}

DeviceTimeClient* DeviceTimeClient::GetRegisteredTimeClient(uint8_t endpoint_id)
{
  if (registered_time_client && registered_time_client->GetEndpointId() == endpoint_id) {
    return registered_time_client;
  }
  return nullptr;
}

void DeviceTimeClient::HandleIncomingCommand(uint8_t endpoint_id,
                                             uint16_t cluster_id,
                                             bool cluster_specific,
                                             uint8_t direction,
                                             uint8_t command_id,
                                             const uint8_t* payload,
                                             uint16_t payload_length)
{
  if (payload == nullptr
      || cluster_id != ZCL_TIME_CLUSTER_ID
      || cluster_specific
      || direction != ZCL_DIRECTION_SERVER_TO_CLIENT
      || command_id != ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID) {
    return;
  }

  DeviceTimeClient* time_client = GetRegisteredTimeClient(endpoint_id);
  if (time_client) {
    time_client->HandleTimeReadAttributesResponse(payload, payload_length);
  }
}

void DeviceTimeClient::HandleAttributeChange(uint16_t cluster_id,
                                             uint16_t attribute_id,
                                             uint8_t size,
                                             uint8_t* value)
{
  (void)cluster_id;
  (void)attribute_id;
  (void)size;
  (void)value;
}

uint16_t DeviceTimeClient::ReadInt16u(const uint8_t* value)
{
  return static_cast<uint16_t>(value[0])
         | (static_cast<uint16_t>(value[1]) << 8);
}

uint32_t DeviceTimeClient::ReadInt32u(const uint8_t* value)
{
  return static_cast<uint32_t>(value[0])
         | (static_cast<uint32_t>(value[1]) << 8)
         | (static_cast<uint32_t>(value[2]) << 16)
         | (static_cast<uint32_t>(value[3]) << 24);
}

int32_t DeviceTimeClient::ReadInt32s(const uint8_t* value)
{
  return static_cast<int32_t>(ReadInt32u(value));
}

uint8_t DeviceTimeClient::AttributeDataSize(uint8_t attribute_type)
{
  switch (attribute_type) {
    case ZCL_BOOLEAN_ATTRIBUTE_TYPE:
    case ZCL_BITMAP8_ATTRIBUTE_TYPE:
    case ZCL_INT8U_ATTRIBUTE_TYPE:
    case ZCL_INT8S_ATTRIBUTE_TYPE:
      return 1;
    case ZCL_INT16U_ATTRIBUTE_TYPE:
    case ZCL_INT16S_ATTRIBUTE_TYPE:
      return 2;
    case ZCL_INT32U_ATTRIBUTE_TYPE:
    case ZCL_INT32S_ATTRIBUTE_TYPE:
    case ZCL_UTC_TIME_ATTRIBUTE_TYPE:
      return 4;
    default:
      return 0;
  }
}

void DeviceTimeClient::AppendAttributeId(uint8_t* buffer, uint16_t attribute_id, uint8_t& offset)
{
  buffer[offset++] = static_cast<uint8_t>(attribute_id & 0xFF);
  buffer[offset++] = static_cast<uint8_t>(attribute_id >> 8);
}
