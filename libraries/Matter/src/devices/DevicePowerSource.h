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

#pragma once

#include "MatterDevice.h"
#include <app-common/zap-generated/cluster-enums.h>

class DevicePowerSource : public Device
{
public:
  enum Changed_t{
    kChanged_Status               = kChanged_Last << 1,
    kChanged_Order                = kChanged_Last << 2,
    kChanged_Description          = kChanged_Last << 3,
    kChanged_BatVoltage           = kChanged_Last << 4,
    kChanged_BatPercentRemaining  = kChanged_Last << 5,
    kChanged_BatTimeRemaining     = kChanged_Last << 6,
    kChanged_BatChargeLevel       = kChanged_Last << 7,
    kChanged_BatReplacementNeeded = kChanged_Last << 8,
    kChanged_BatReplaceability    = kChanged_Last << 9,
    kChanged_BatPresent           = kChanged_Last << 10,
  } Changed;

  static const uint8_t kDescriptionSize = 61u;

  DevicePowerSource(const char* device_name);

  chip::app::Clusters::PowerSource::PowerSourceStatusEnum GetStatus();
  void SetStatus(chip::app::Clusters::PowerSource::PowerSourceStatusEnum status);

  uint8_t GetOrder();
  void SetOrder(uint8_t order);

  const char* GetDescription();
  void SetDescription(const char* description);

  uint32_t GetBatVoltage();
  void SetBatVoltage(uint32_t voltage);

  uint8_t GetBatPercentRemaining();
  void SetBatPercentRemaining(uint8_t percent_remaining);

  uint32_t GetBatTimeRemaining();
  void SetBatTimeRemaining(uint32_t time_remaining);

  chip::app::Clusters::PowerSource::BatChargeLevelEnum GetBatChargeLevel();
  void SetBatChargeLevel(chip::app::Clusters::PowerSource::BatChargeLevelEnum charge_level);

  bool GetBatReplacementNeeded();
  void SetBatReplacementNeeded(bool replacement_needed);

  chip::app::Clusters::PowerSource::BatReplaceabilityEnum GetBatReplaceability();
  void SetBatReplaceability(chip::app::Clusters::PowerSource::BatReplaceabilityEnum replaceability);

  bool GetBatPresent();
  void SetBatPresent(bool present);

  uint32_t GetPowerSourceClusterFeatureMap();
  uint16_t GetPowerSourceClusterRevision();

  CHIP_ERROR HandleReadEmberAfAttribute(ClusterId clusterId,
                                        chip::AttributeId attributeId,
                                        uint8_t* buffer,
                                        uint16_t maxReadLength) override;

private:
  void HandlePowerSourceDeviceStatusChanged(Changed_t itemChangedMask);

  chip::app::Clusters::PowerSource::PowerSourceStatusEnum status;
  uint8_t order;
  char description[kDescriptionSize];
  uint32_t bat_voltage;
  uint8_t bat_percent_remaining;
  uint32_t bat_time_remaining;
  chip::app::Clusters::PowerSource::BatChargeLevelEnum bat_charge_level;
  bool bat_replacement_needed;
  chip::app::Clusters::PowerSource::BatReplaceabilityEnum bat_replaceability;
  bool bat_present;

  static const uint32_t power_source_cluster_feature_map;
  static const uint16_t power_source_cluster_revision = 1u;
};
