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

#include "Zigbee.h"
#include "ZigbeeAfLock.h"
#include "devices/DeviceTimeClient.h"

extern "C" {
#include "af.h"
#include "sl_zigbee_system_common.h"
#include "zigbee_app_framework_event.h"
#include "network-steering.h"
#include "network-formation.h"
#include "identify.h"
#include "stack-info.h"
#include "nvm3_default.h"
#include "nvm3_generic.h"

uint8_t sli_zigbee_get_local_capabilities(void);
extern uint8_t sli_zigbee_dynamic_capabilities;
}

ZigbeeClass Zigbee;

static sl_zigbee_af_event_t network_steering_retry_event;

static sl_zigbee_node_type_t zigbeeDeviceTypeToStackNodeType(ZigbeeDeviceType device_type)
{
  switch (device_type) {
    case ZIGBEE_DEVICE_TYPE_END_DEVICE:
      return SL_ZIGBEE_END_DEVICE;
    case ZIGBEE_DEVICE_TYPE_ROUTER:
    default:
      return SL_ZIGBEE_ROUTER;
  }
}

static constexpr uint8_t kZigbeeDescriptorPowerModeSeeNodeDescriptor = 0x00;
static constexpr uint8_t kZigbeeDescriptorPowerSourceMains = 0x01;
static constexpr uint8_t kZigbeeDescriptorPowerSourceDisposable = 0x04;
static constexpr uint8_t kZigbeeDescriptorPowerLevel100 = 0x0C;
static constexpr uint8_t kMacCapabilityDeviceType = 0x02;
static constexpr uint8_t kMacCapabilityPowerSource = 0x04;
static constexpr uint8_t kMacCapabilityReceiverOnWhenIdle = 0x08;
static constexpr uint8_t kMacCapabilityAllocateAddress = 0x80;

static uint8_t getBasicClusterPowerSourceValue(ZigbeePowerSourceType power_source)
{
  if (power_source == ZIGBEE_POWER_SOURCE_TYPE_BATTERY) {
    return SL_ZIGBEE_ZCL_POWER_SOURCE_BATTERY;
  }
  return SL_ZIGBEE_ZCL_POWER_SOURCE_SINGLE_PHASE_MAINS;
}

static uint16_t getConfiguredPowerDescriptor(ZigbeePowerSourceType power_source)
{
  uint8_t descriptor_power_source = kZigbeeDescriptorPowerSourceMains;
  if (power_source == ZIGBEE_POWER_SOURCE_TYPE_BATTERY) {
    descriptor_power_source = kZigbeeDescriptorPowerSourceDisposable;
  }

  uint8_t current_power = descriptor_power_source | (kZigbeeDescriptorPowerLevel100 << 4);
  uint8_t available_power = kZigbeeDescriptorPowerModeSeeNodeDescriptor | (descriptor_power_source << 4);
  return static_cast<uint16_t>(current_power << 8) | available_power;
}

static uint8_t getConfiguredMacCapabilities(ZigbeeDeviceType device_type,
                                            ZigbeePowerSourceType power_source)
{
  uint8_t capabilities = kMacCapabilityReceiverOnWhenIdle | kMacCapabilityAllocateAddress;
  if (device_type == ZIGBEE_DEVICE_TYPE_ROUTER) {
    capabilities |= kMacCapabilityDeviceType;
  }
  if (power_source == ZIGBEE_POWER_SOURCE_TYPE_MAINS) {
    capabilities |= kMacCapabilityPowerSource;
  }
  return capabilities;
}

static void networkSteeringRetryEventHandler(sl_zigbee_af_event_t* event)
{
  (void)event;
  if (!Zigbee.isConnectedToNetwork()) {
    sl_zigbee_af_network_steering_start();
  }
}

static void initNetworkSteeringRetryEvent()
{
  static bool network_steering_retry_event_initialized = false;
  if (!network_steering_retry_event_initialized) {
    sl_zigbee_af_event_init(&network_steering_retry_event, networkSteeringRetryEventHandler);
    network_steering_retry_event_initialized = true;
  }
}

static void scheduleNetworkSteeringRetry()
{
  static constexpr uint32_t kNetworkSteeringRetryDelayMs = 250;
  ZigbeeAfLock lock;
  initNetworkSteeringRetryEvent();
  sl_zigbee_af_event_set_delay_ms(&network_steering_retry_event, kNetworkSteeringRetryDelayMs);
}

static void writeServerAttributeToEndpoint(uint8_t endpoint_id,
                                           sl_zigbee_af_cluster_id_t cluster_id,
                                           sl_zigbee_af_attribute_id_t attribute_id,
                                           uint8_t* value,
                                           uint8_t attribute_type)
{
  sl_zigbee_af_write_server_attribute(endpoint_id,
                                      cluster_id,
                                      attribute_id,
                                      value,
                                      attribute_type);
}

static void writeServerAttributeToApplicationEndpoints(sl_zigbee_af_cluster_id_t cluster_id,
                                                       sl_zigbee_af_attribute_id_t attribute_id,
                                                       uint8_t* value,
                                                       uint8_t attribute_type)
{
  for (uint8_t endpoint_id = 1; endpoint_id <= ZigbeeClass::kApplicationEndpointCount; endpoint_id++) {
    writeServerAttributeToEndpoint(endpoint_id,
                                   cluster_id,
                                   attribute_id,
                                   value,
                                   attribute_type);
  }
}

static void writeServerAttributeToMetadataEndpoint(sl_zigbee_af_cluster_id_t cluster_id,
                                                   sl_zigbee_af_attribute_id_t attribute_id,
                                                   uint8_t* value,
                                                   uint8_t attribute_type)
{
  writeServerAttributeToEndpoint(ZigbeeClass::kTimeClientEndpointId,
                                 cluster_id,
                                 attribute_id,
                                 value,
                                 attribute_type);
}

static void writeServerAttributeToApplicationAndMetadataEndpoints(sl_zigbee_af_cluster_id_t cluster_id,
                                                                  sl_zigbee_af_attribute_id_t attribute_id,
                                                                  uint8_t* value,
                                                                  uint8_t attribute_type)
{
  writeServerAttributeToApplicationEndpoints(cluster_id,
                                             attribute_id,
                                             value,
                                             attribute_type);
  writeServerAttributeToMetadataEndpoint(cluster_id,
                                         attribute_id,
                                         value,
                                         attribute_type);
}

static void applyStackPowerSource(ZigbeePowerSourceType power_source)
{
  (void)sl_zigbee_set_power_descriptor(getConfiguredPowerDescriptor(power_source));

  uint8_t capabilities = getConfiguredMacCapabilities(Zigbee.getDeviceType(), power_source);
  sli_zigbee_dynamic_capabilities = capabilities;
}

extern "C" void sl_zigbee_af_stack_status_cb(sl_status_t status)
{
  if (status == SL_STATUS_NETWORK_UP) {
    applyStackPowerSource(Zigbee.getPowerSource());
  }
}

// Callback from EmberZNet when an attribute changes
extern "C" void sl_zigbee_af_post_attribute_change_cb(uint8_t endpoint,
                                                      sl_zigbee_af_cluster_id_t cluster_id,
                                                      sl_zigbee_af_attribute_id_t attribute_id,
                                                      uint8_t mask,
                                                      uint16_t manufacturer_code,
                                                      uint8_t type,
                                                      uint8_t size,
                                                      uint8_t* value)
{
  (void)mask;
  (void)manufacturer_code;
  (void)type;

  ZigbeeDevice* dev = zigbee_endpoint_get_device(endpoint);
  if (dev) {
    dev->HandleAttributeChange(cluster_id, attribute_id, size, value);
  }
}

// Callback from EmberZNet before an attribute changes
extern "C" sl_zigbee_af_status_t sl_zigbee_af_pre_attribute_change_cb(uint8_t endpoint,
                                                                      sl_zigbee_af_cluster_id_t cluster_id,
                                                                      sl_zigbee_af_attribute_id_t attribute_id,
                                                                      uint8_t mask,
                                                                      uint16_t manufacturer_code,
                                                                      uint8_t type,
                                                                      uint8_t size,
                                                                      uint8_t* value)
{
  (void)mask;
  (void)manufacturer_code;
  (void)type;

  ZigbeeDevice* dev = zigbee_endpoint_get_device(endpoint);
  if (dev == nullptr) {
    return SL_ZIGBEE_ZCL_STATUS_SUCCESS;
  }

  return static_cast<sl_zigbee_af_status_t>(dev->HandleAttributePreChange(cluster_id,
                                                                          attribute_id,
                                                                          size,
                                                                          value));
}

extern "C" bool sl_zigbee_af_pre_command_received_cb(sl_zigbee_af_cluster_command_t* cmd)
{
  if (cmd == nullptr
      || cmd->apsFrame == nullptr
      || cmd->buffer == nullptr
      || cmd->payloadStartIndex > cmd->bufLen) {
    return false;
  }

  uint8_t endpoint_id = cmd->apsFrame->destinationEndpoint;
  uint16_t payload_length = cmd->bufLen - cmd->payloadStartIndex;
  const uint8_t* payload = &cmd->buffer[cmd->payloadStartIndex];

  ZigbeeDevice* dev = zigbee_endpoint_get_device(endpoint_id);
  if (dev) {
    uint8_t status = SL_ZIGBEE_ZCL_STATUS_SUCCESS;
    bool handled = dev->HandleCommand(cmd->apsFrame->clusterId,
                                      cmd->clusterSpecific,
                                      cmd->direction,
                                      cmd->commandId,
                                      payload,
                                      payload_length,
                                      status);
    if (handled) {
      sl_zigbee_af_send_default_response(cmd, static_cast<sl_zigbee_af_status_t>(status));
      return true;
    }
  }

  DeviceTimeClient::HandleIncomingCommand(endpoint_id,
                                          cmd->apsFrame->clusterId,
                                          cmd->clusterSpecific,
                                          cmd->direction,
                                          cmd->commandId,
                                          payload,
                                          payload_length);
  return false;
}

// Callbacks from the Identify plugin when a remote Identify command changes state
extern "C" void sl_zigbee_af_identify_start_feedback_cb(uint8_t endpoint, uint16_t identify_time)
{
  ZigbeeDevice* dev = zigbee_endpoint_get_device(endpoint);
  if (dev) {
    dev->HandleIdentifyStart(identify_time);
  }
}

extern "C" void sl_zigbee_af_identify_stop_feedback_cb(uint8_t endpoint)
{
  ZigbeeDevice* dev = zigbee_endpoint_get_device(endpoint);
  if (dev) {
    dev->HandleIdentifyStop();
  }
}

// Called by the Zigbee framework once the stack is fully initialized
extern "C" void sl_zigbee_af_main_init_cb(void)
{
  initNetworkSteeringRetryEvent();
}

// Callback from the network steering plugin when steering completes
extern "C" void sl_zigbee_af_network_steering_complete_cb(sl_status_t status,
                                                          uint8_t total_beacons,
                                                          uint8_t join_attempts,
                                                          uint8_t final_state)
{
  (void)total_beacons;
  (void)join_attempts;
  (void)final_state;

  if (status == SL_STATUS_OK) {
    return;
  }

  // Half-paired state - network credentials were stored but the device is not fully joined (e.g. TCLK failure)
  // Clear credentials and reboot to pair again
  if (Zigbee.isPaired() && !Zigbee.isConnectedToNetwork()) {
    Serial.println("Zigbee pairing failed with stored credentials; clearing network data and rebooting");
    Zigbee.leaveNetwork();
    sl_zigbee_token_factory_reset(false, false);
    NVIC_SystemReset();
  }

  scheduleNetworkSteeringRetry();
}

extern "C" sl_zigbee_node_type_t sl_zigbee_af_network_steering_get_node_type_cb(sl_zigbee_af_plugin_network_steering_joining_state_t state)
{
  (void)state;
  return zigbeeDeviceTypeToStackNodeType(Zigbee.getDeviceType());
}

// ArduinoZigbeeAppliance base class implementation

ArduinoZigbeeAppliance::ArduinoZigbeeAppliance() :
  base_zigbee_device(nullptr)
{
}

ArduinoZigbeeAppliance::~ArduinoZigbeeAppliance()
{
}

bool ArduinoZigbeeAppliance::get_identify_in_progress()
{
  if (this->base_zigbee_device) {
    return this->base_zigbee_device->GetIdentifyInProgress();
  }
  return false;
}

bool ArduinoZigbeeAppliance::is_online()
{
  if (!this->base_zigbee_device) {
    return false;
  }
  return this->base_zigbee_device->IsOnline();
}

void ArduinoZigbeeAppliance::set_device_change_callback(void (*cb)(void))
{
  if (this->base_zigbee_device) {
    this->base_zigbee_device->SetDeviceChangeCallback(cb);
  }
}

// ZigbeeClass implementation

ZigbeeClass::ZigbeeClass() :
  started(false),
  endpoint_allocated(),
  time_client_endpoint_allocated(false),
  device_type(ZIGBEE_DEVICE_TYPE_ROUTER),
  power_source(ZIGBEE_POWER_SOURCE_TYPE_MAINS)
{
}

void ZigbeeClass::setVendorName(const char* name)
{
  uint8_t zcl_string[33];
  uint8_t len = strlen(name);
  if (len > 32) {
    len = 32;
  }
  zcl_string[0] = len;
  memcpy(&zcl_string[1], name, len);
  ZigbeeAfLock lock;
  writeServerAttributeToApplicationAndMetadataEndpoints(ZCL_BASIC_CLUSTER_ID,
                                                        ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID,
                                                        zcl_string,
                                                        ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

void ZigbeeClass::setProductName(const char* name)
{
  uint8_t zcl_string[33];
  uint8_t len = strlen(name);
  if (len > 32) {
    len = 32;
  }
  zcl_string[0] = len;
  memcpy(&zcl_string[1], name, len);
  ZigbeeAfLock lock;
  writeServerAttributeToApplicationAndMetadataEndpoints(ZCL_BASIC_CLUSTER_ID,
                                                        ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
                                                        zcl_string,
                                                        ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

void ZigbeeClass::setFirmwareVersion(const char* version)
{
  if (version == nullptr) {
    return;
  }

  uint8_t zcl_string[17];
  uint8_t len = strlen(version);
  if (len > 16) {
    len = 16;
  }
  zcl_string[0] = len;
  memcpy(&zcl_string[1], version, len);
  ZigbeeAfLock lock;
  writeServerAttributeToMetadataEndpoint(ZCL_BASIC_CLUSTER_ID,
                                         ZCL_SW_BUILD_ID_ATTRIBUTE_ID,
                                         zcl_string,
                                         ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
}

void ZigbeeClass::setFirmwareVersion(uint32_t file_version)
{
  uint8_t application_version = (file_version <= 0xFF) ? file_version : (file_version >> 24);
  ZigbeeAfLock lock;
  writeServerAttributeToMetadataEndpoint(ZCL_BASIC_CLUSTER_ID,
                                         ZCL_APPLICATION_VERSION_ATTRIBUTE_ID,
                                         &application_version,
                                         ZCL_INT8U_ATTRIBUTE_TYPE);

  sl_zigbee_af_write_client_attribute(kTimeClientEndpointId,
                                      ZCL_OTA_BOOTLOAD_CLUSTER_ID,
                                      ZCL_CURRENT_FILE_VERSION_ATTRIBUTE_ID,
                                      reinterpret_cast<uint8_t*>(&file_version),
                                      ZCL_INT32U_ATTRIBUTE_TYPE);
}

void ZigbeeClass::begin()
{
  if (this->started) {
    return;
  }
  this->started = true;
  initNetworkSteeringRetryEvent();

  ZigbeeAfLock lock;
  applyStackPowerSource(this->power_source);
  uint8_t basic_power_source = getBasicClusterPowerSourceValue(this->power_source);
  writeServerAttributeToApplicationAndMetadataEndpoints(ZCL_BASIC_CLUSTER_ID,
                                                        ZCL_POWER_SOURCE_ATTRIBUTE_ID,
                                                        &basic_power_source,
                                                        ZCL_ENUM8_ATTRIBUTE_TYPE);

  for (uint8_t i = 0; i < kApplicationEndpointCount; i++) {
    sl_zigbee_af_endpoint_enable_disable(i + 1, false);
  }
  sl_zigbee_af_endpoint_enable_disable(kTimeClientEndpointId, true);

  if (sl_zigbee_af_network_state() == SL_ZIGBEE_NO_NETWORK) {
    sl_zigbee_af_network_steering_start();
  }
}

uint32_t ZigbeeClass::channelToMask(uint8_t channel)
{
  if (channel < kMinPairingChannel || channel > kMaxPairingChannel) {
    return 0;
  }
  return 1UL << channel;
}

bool ZigbeeClass::isValidPairingChannelMask(uint32_t channel_mask)
{
  return (channel_mask & kAllPairingChannelsMask) != 0;
}

uint32_t ZigbeeClass::sanitizePairingChannelMask(uint32_t channel_mask)
{
  return channel_mask & kAllPairingChannelsMask;
}

bool ZigbeeClass::setPairingChannel(uint8_t channel)
{
  uint32_t channel_mask = channelToMask(channel);
  if (channel_mask == 0) {
    return false;
  }
  return this->setPairingChannelMask(channel_mask);
}

bool ZigbeeClass::setDeviceType(ZigbeeDeviceType device_type)
{
  if (this->started) {
    return false;
  }

  if (device_type != ZIGBEE_DEVICE_TYPE_ROUTER && device_type != ZIGBEE_DEVICE_TYPE_END_DEVICE) {
    return false;
  }

  this->device_type = device_type;
  return true;
}

ZigbeeDeviceType ZigbeeClass::getDeviceType()
{
  return this->device_type;
}

bool ZigbeeClass::setPowerSource(ZigbeePowerSourceType power_source)
{
  if (power_source != ZIGBEE_POWER_SOURCE_TYPE_MAINS
      && power_source != ZIGBEE_POWER_SOURCE_TYPE_BATTERY) {
    return false;
  }

  this->power_source = power_source;
  uint8_t basic_power_source = getBasicClusterPowerSourceValue(power_source);
  ZigbeeAfLock lock;
  writeServerAttributeToApplicationAndMetadataEndpoints(ZCL_BASIC_CLUSTER_ID,
                                                        ZCL_POWER_SOURCE_ATTRIBUTE_ID,
                                                        &basic_power_source,
                                                        ZCL_ENUM8_ATTRIBUTE_TYPE);
  if (this->started) {
    applyStackPowerSource(power_source);
  }
  return true;
}

ZigbeePowerSourceType ZigbeeClass::getPowerSource()
{
  return this->power_source;
}

uint16_t ZigbeeClass::getPowerDescriptor(ZigbeePowerSourceType power_source)
{
  uint8_t descriptor_power_source = kZigbeeDescriptorPowerSourceMains;
  if (power_source == ZIGBEE_POWER_SOURCE_TYPE_BATTERY) {
    descriptor_power_source = kZigbeeDescriptorPowerSourceDisposable;
  }

  uint8_t current_power = descriptor_power_source | (kZigbeeDescriptorPowerLevel100 << 4);
  uint8_t available_power = kZigbeeDescriptorPowerModeSeeNodeDescriptor | (descriptor_power_source << 4);
  return static_cast<uint16_t>(current_power << 8) | available_power;
}

bool ZigbeeClass::setPairingChannelMask(uint32_t primary_channel_mask, uint32_t secondary_channel_mask)
{
  primary_channel_mask = sanitizePairingChannelMask(primary_channel_mask);
  secondary_channel_mask = sanitizePairingChannelMask(secondary_channel_mask);
  if (!isValidPairingChannelMask(primary_channel_mask) && !isValidPairingChannelMask(secondary_channel_mask)) {
    return false;
  }

  ZigbeeAfLock lock;
  sli_zigbee_af_network_steering_set_channel_mask(primary_channel_mask, false);
  sli_zigbee_af_network_steering_set_channel_mask(secondary_channel_mask, true);

  if (this->started && (sl_zigbee_af_network_state() == SL_ZIGBEE_NO_NETWORK)) {
    sl_zigbee_af_network_steering_stop();
    scheduleNetworkSteeringRetry();
  }
  return true;
}

bool ZigbeeClass::isConnectedToNetwork()
{
  ZigbeeAfLock lock;
  return (sl_zigbee_af_network_state() == SL_ZIGBEE_JOINED_NETWORK);
}

bool ZigbeeClass::isPaired()
{
  ZigbeeAfLock lock;
  return (sl_zigbee_af_network_state() != SL_ZIGBEE_NO_NETWORK);
}

uint8_t ZigbeeClass::getChannel()
{
  ZigbeeAfLock lock;
  return sl_zigbee_af_get_radio_channel();
}

uint16_t ZigbeeClass::getPanId()
{
  ZigbeeAfLock lock;
  return sl_zigbee_af_get_pan_id();
}

uint16_t ZigbeeClass::getNodeId()
{
  ZigbeeAfLock lock;
  return sl_zigbee_af_get_node_id();
}

void ZigbeeClass::leaveNetwork()
{
  ZigbeeAfLock lock;
  sl_zigbee_network_status_t state = sl_zigbee_af_network_state();
  if (state != SL_ZIGBEE_NO_NETWORK) {
    sl_zigbee_leave_network(SL_ZIGBEE_LEAVE_NWK_WITH_NO_OPTION);
  }
}

static void erase_nvm3_domain(nvm3_ObjectKey_t domain_base)
{
  static constexpr nvm3_ObjectKey_t kNvm3DomainKeyMask = 0xFFFFU;
  nvm3_ObjectKey_t keys[32];
  const nvm3_ObjectKey_t key_min = domain_base;
  const nvm3_ObjectKey_t key_max = domain_base | kNvm3DomainKeyMask;

  while (true) {
    size_t count = nvm3_enumObjects(nvm3_defaultHandle,
                                    keys,
                                    sizeof(keys) / sizeof(keys[0]),
                                    key_min,
                                    key_max);
    if (count == 0) {
      break;
    }
    for (size_t i = 0; i < count; i++) {
      (void)nvm3_deleteObject(nvm3_defaultHandle, keys[i]);
    }
  }
}

static void erase_zigbee_nvm3_objects()
{
  static constexpr nvm3_ObjectKey_t kNvm3DomainZigbee = 0x10000U;
  static constexpr nvm3_ObjectKey_t kNvm3DomainCommon = 0x80000U;
  erase_nvm3_domain(kNvm3DomainZigbee);
  erase_nvm3_domain(kNvm3DomainCommon);
}

void ZigbeeClass::factoryReset()
{
  {
    ZigbeeAfLock lock;
    sl_zigbee_network_status_t state = sl_zigbee_af_network_state();
    if (state != SL_ZIGBEE_NO_NETWORK) {
      sl_zigbee_leave_network(SL_ZIGBEE_LEAVE_NWK_WITH_NO_OPTION);
    }
    sl_zigbee_token_factory_reset(false, false);
  }
  erase_zigbee_nvm3_objects();
  NVIC_SystemReset();
}

uint8_t ZigbeeClass::allocateEndpoint(ZigbeeEndpointType type)
{
  if (type >= ZIGBEE_ENDPOINT_TYPE_COUNT) {
    return 0;
  }

  uint8_t start = type * kEndpointsPerType;
  if ((start + kEndpointsPerType) > kApplicationEndpointCount) {
    return 0;
  }

  for (uint8_t i = start; i < start + kEndpointsPerType; i++) {
    if (!this->endpoint_allocated[i]) {
      this->endpoint_allocated[i] = true;
      uint8_t ep_id = i + 1;
      ZigbeeAfLock lock;
      sl_zigbee_af_endpoint_enable_disable(ep_id, true);
      return ep_id;
    }
  }
  return 0;
}

uint8_t ZigbeeClass::allocateTimeClientEndpoint()
{
  if (this->time_client_endpoint_allocated) {
    return 0;
  }
  this->time_client_endpoint_allocated = true;
  return kTimeClientEndpointId;
}

void ZigbeeClass::freeEndpoint(uint8_t endpoint_id)
{
  if (endpoint_id == kTimeClientEndpointId && this->time_client_endpoint_allocated) {
    this->time_client_endpoint_allocated = false;
    return;
  }

  uint8_t index = endpoint_id - 1;
  if (index < kApplicationEndpointCount && this->endpoint_allocated[index]) {
    this->endpoint_allocated[index] = false;
    ZigbeeAfLock lock;
    sl_zigbee_af_endpoint_enable_disable(endpoint_id, false);
  }
}
