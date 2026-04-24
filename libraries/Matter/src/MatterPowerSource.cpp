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

#include "MatterPowerSource.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gPowerSourceDeviceTypes[] = { { DEVICE_TYPE_POWER_SOURCE, DEVICE_VERSION_DEFAULT } };

// Power source cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(powerSourceAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Status::Id, INT8U, 1, 0),                                              /* Status */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Order::Id, INT8U, 1, 0),                                               /* Order */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Description::Id, CHAR_STRING, DevicePowerSource::kDescriptionSize, 0), /* Description */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatVoltage::Id, INT32U, 4, ATTRIBUTE_MASK_NULLABLE),                   /* BatVoltage */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatPercentRemaining::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),           /* BatPercentRemaining */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatTimeRemaining::Id, INT32U, 4, ATTRIBUTE_MASK_NULLABLE),             /* BatTimeRemaining */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatChargeLevel::Id, INT8U, 1, 0),                                      /* BatChargeLevel */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatReplacementNeeded::Id, BOOLEAN, 1, 0),                              /* BatReplacementNeeded */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatReplaceability::Id, INT8U, 1, 0),                                   /* BatReplaceability */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatPresent::Id, BOOLEAN, 1, 0),                                        /* BatPresent */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                       /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                     /* ClusterRevision auto added by LIST_END */

// Power source endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(powerSourceEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(PowerSource::Id, powerSourceAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

// Simple power source cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(simplePowerSourceAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Status::Id, INT8U, 1, 0),                                              /* Status */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Order::Id, INT8U, 1, 0),                                               /* Order */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::Description::Id, CHAR_STRING, DevicePowerSource::kDescriptionSize, 0), /* Description */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatChargeLevel::Id, INT8U, 1, 0),                                      /* BatChargeLevel */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatReplacementNeeded::Id, BOOLEAN, 1, 0),                              /* BatReplacementNeeded */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatReplaceability::Id, INT8U, 1, 0),                                   /* BatReplaceability */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::BatPercentRemaining::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),           /* BatPercentRemaining */
DECLARE_DYNAMIC_ATTRIBUTE(PowerSource::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                       /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                     /* ClusterRevision auto added by LIST_END */

// Simple power source endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(simplePowerSourceEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(PowerSource::Id, simplePowerSourceAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr)
DECLARE_DYNAMIC_CLUSTER_LIST_END;

MatterSimplePowerSource::MatterSimplePowerSource() :
  power_source_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  initialized(false)
{
  ;
}

MatterSimplePowerSource::~MatterSimplePowerSource()
{
  this->end();
}

bool MatterSimplePowerSource::begin()
{
  if (this->initialized) {
    return false;
  }

  DevicePowerSource* power_source = new (std::nothrow)DevicePowerSource("Power source");
  if (power_source == nullptr) {
    return false;
  }
  power_source->SetReachable(true);
  power_source->SetProductName("Power source");
  power_source->SetDescription("Battery");
  power_source->SetStatus(PowerSource::PowerSourceStatusEnum::kActive);
  power_source->SetOrder(0u);
  power_source->SetBatChargeLevel(PowerSource::BatChargeLevelEnum::kOk);
  power_source->SetBatReplacementNeeded(false);
  power_source->SetBatReplaceability(PowerSource::BatReplaceabilityEnum::kUserReplaceable);
  power_source->SetBatPresent(true);

  this->base_matter_device = power_source;

  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(power_source);
    return false;
  }
  new_endpoint->cluster = simplePowerSourceEndpointClusters;
  new_endpoint->clusterCount = ArraySize(simplePowerSourceEndpointClusters);
  new_endpoint->endpointSize = 0;

  size_t dataversion_size = ArraySize(simplePowerSourceEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_device_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_device_data_version == nullptr) {
    delete(power_source);
    free(new_endpoint);
    return false;
  }

  int result = AddDeviceEndpoint(power_source,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gPowerSourceDeviceTypes),
                                 Span<DataVersion>(new_device_data_version, ArraySize(simplePowerSourceEndpointClusters)),
                                 1);
  if (result < 0) {
    delete(power_source);
    free(new_endpoint);
    free(new_device_data_version);
    return false;
  }

  this->power_source_device = power_source;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_device_data_version;
  this->initialized = true;
  this->set_battery_percent(100u);
  return true;
}

void MatterSimplePowerSource::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->power_source_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->power_source_device);
  this->initialized = false;
}

void MatterSimplePowerSource::set_battery_percent(uint8_t value)
{
  if (!this->initialized) {
    return;
  }
  if (value > 100u) {
    value = 100u;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatPercentRemaining(static_cast<uint8_t>(value * 2u));
  PlatformMgr().UnlockChipStack();
}

uint8_t MatterSimplePowerSource::get_battery_percent()
{
  if (!this->initialized) {
    return 0;
  }
  return static_cast<uint8_t>(this->power_source_device->GetBatPercentRemaining() / 2u);
}

void MatterSimplePowerSource::set_battery_charge_level(MatterSimplePowerSource::charge_level_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatChargeLevel(static_cast<PowerSource::BatChargeLevelEnum>(value));
  PlatformMgr().UnlockChipStack();
}

MatterSimplePowerSource::charge_level_t MatterSimplePowerSource::get_battery_charge_level()
{
  if (!this->initialized) {
    return MatterSimplePowerSource::charge_level_t::CHARGE_LEVEL_OK;
  }
  return static_cast<MatterSimplePowerSource::charge_level_t>(this->power_source_device->GetBatChargeLevel());
}

MatterSimplePowerSource::operator uint8_t()
{
  return this->get_battery_percent();
}

void MatterSimplePowerSource::operator=(uint8_t value)
{
  this->set_battery_percent(value);
}

MatterPowerSource::MatterPowerSource()
{
  ;
}

MatterPowerSource::~MatterPowerSource()
{
  ;
}

bool MatterPowerSource::begin()
{
  if (this->initialized) {
    return false;
  }

  DevicePowerSource* power_source = new (std::nothrow)DevicePowerSource("Power source");
  if (power_source == nullptr) {
    return false;
  }
  power_source->SetReachable(true);
  power_source->SetProductName("Power source");
  power_source->SetDescription("Battery");
  power_source->SetBatReplaceability(PowerSource::BatReplaceabilityEnum::kUserReplaceable);

  this->base_matter_device = power_source;

  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(power_source);
    return false;
  }
  new_endpoint->cluster = powerSourceEndpointClusters;
  new_endpoint->clusterCount = ArraySize(powerSourceEndpointClusters);
  new_endpoint->endpointSize = 0;

  size_t dataversion_size = ArraySize(powerSourceEndpointClusters) * sizeof(DataVersion);
  DataVersion* new_device_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_device_data_version == nullptr) {
    delete(power_source);
    free(new_endpoint);
    return false;
  }

  int result = AddDeviceEndpoint(power_source,
                                 new_endpoint,
                                 Span<const EmberAfDeviceType>(gPowerSourceDeviceTypes),
                                 Span<DataVersion>(new_device_data_version, ArraySize(powerSourceEndpointClusters)),
                                 1);
  if (result < 0) {
    delete(power_source);
    free(new_endpoint);
    free(new_device_data_version);
    return false;
  }

  this->power_source_device = power_source;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_device_data_version;
  this->initialized = true;
  return true;
}

void MatterPowerSource::set_status(MatterPowerSource::status_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetStatus(static_cast<PowerSource::PowerSourceStatusEnum>(value));
  PlatformMgr().UnlockChipStack();
}

MatterPowerSource::status_t MatterPowerSource::get_status()
{
  if (!this->initialized) {
    return MatterPowerSource::status_t::UNSPECIFIED;
  }
  return static_cast<MatterPowerSource::status_t>(this->power_source_device->GetStatus());
}

void MatterPowerSource::set_order(uint8_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetOrder(value);
  PlatformMgr().UnlockChipStack();
}

uint8_t MatterPowerSource::get_order()
{
  if (!this->initialized) {
    return 0;
  }
  return this->power_source_device->GetOrder();
}

void MatterPowerSource::set_description(const char* value)
{
  if (!this->initialized || value == nullptr) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetDescription(value);
  PlatformMgr().UnlockChipStack();
}

const char* MatterPowerSource::get_description()
{
  if (!this->initialized) {
    return "";
  }
  return this->power_source_device->GetDescription();
}

void MatterPowerSource::set_bat_voltage(uint32_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatVoltage(value);
  PlatformMgr().UnlockChipStack();
}

uint32_t MatterPowerSource::get_bat_voltage()
{
  if (!this->initialized) {
    return 0;
  }
  return this->power_source_device->GetBatVoltage();
}

void MatterPowerSource::set_bat_percent_remaining(uint8_t value)
{
  this->set_battery_percent(value);
}

uint8_t MatterPowerSource::get_bat_percent_remaining()
{
  return this->get_battery_percent();
}

void MatterPowerSource::set_bat_time_remaining(uint32_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatTimeRemaining(value);
  PlatformMgr().UnlockChipStack();
}

uint32_t MatterPowerSource::get_bat_time_remaining()
{
  if (!this->initialized) {
    return 0;
  }
  return this->power_source_device->GetBatTimeRemaining();
}

void MatterPowerSource::set_bat_charge_level(MatterSimplePowerSource::charge_level_t value)
{
  this->set_battery_charge_level(value);
}

MatterSimplePowerSource::charge_level_t MatterPowerSource::get_bat_charge_level()
{
  return this->get_battery_charge_level();
}

void MatterPowerSource::set_bat_replacement_needed(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatReplacementNeeded(value);
  PlatformMgr().UnlockChipStack();
}

bool MatterPowerSource::get_bat_replacement_needed()
{
  if (!this->initialized) {
    return false;
  }
  return this->power_source_device->GetBatReplacementNeeded();
}

void MatterPowerSource::set_bat_replaceability(uint8_t value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatReplaceability(static_cast<PowerSource::BatReplaceabilityEnum>(value));
  PlatformMgr().UnlockChipStack();
}

uint8_t MatterPowerSource::get_bat_replaceability()
{
  if (!this->initialized) {
    return static_cast<uint8_t>(PowerSource::BatReplaceabilityEnum::kUserReplaceable);
  }
  return static_cast<uint8_t>(this->power_source_device->GetBatReplaceability());
}

void MatterPowerSource::set_bat_present(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->power_source_device->SetBatPresent(value);
  PlatformMgr().UnlockChipStack();
}

bool MatterPowerSource::get_bat_present()
{
  if (!this->initialized) {
    return false;
  }
  return this->power_source_device->GetBatPresent();
}
