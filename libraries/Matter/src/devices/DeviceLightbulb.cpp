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

#include "DeviceLightbulb.h"

DeviceLightbulb::DeviceLightbulb(const char* device_name) :
  Device(device_name),
  onoff(false),
  global_scene_control(true),
  on_time(0u),
  off_wait_time(0u),
  startup_on_off(0u),
  hue(0u),
  saturation(0u),
  level(255u),
  level_control_options(0u),
  level_control_on_level(255u),
  level_control_startup_current_level(254u),
  color_control_options(0u)
{
  this->SetDeviceType(device_type_t::kDeviceType_Lightbulb);
}

bool DeviceLightbulb::IsOn()
{
  return this->onoff;
}

void DeviceLightbulb::SetOnOff(bool onoff)
{
  bool changed = onoff ^ this->onoff;
  this->onoff = onoff;
  ChipLogProgress(DeviceLayer, "DeviceLightbulb[%s]: %s", this->device_name, onoff ? "ON" : "OFF");
  if (changed) {
    this->HandleLightbulbDeviceStatusChanged(kChanged_OnOff);
    CallDeviceChangeCallback();
  }
}

void DeviceLightbulb::Toggle()
{
  bool inverse_state = !this->IsOn();
  this->SetOnOff(inverse_state);
}

uint8_t DeviceLightbulb::GetLevel()
{
  return this->level;
}

bool DeviceLightbulb::SetLevel(uint8_t level)
{
  if (this->level == level) {
    return true;
  }

  bool level_in_bounds = (level >= this->level_control_min_level) && (level <= this->level_control_max_level);

  // The value 255 represents the NULL value for the level attribute
  if (!level_in_bounds && level != 255) {
    return false;
  }

  this->level = level;
  this->HandleLightbulbDeviceStatusChanged(kChanged_Level);
  CallDeviceChangeCallback();
  return true;
}

void DeviceLightbulb::SetHue(uint8_t hue)
{
  if (this->hue == hue) {
    return;
  }
  this->hue = hue;
  this->HandleLightbulbDeviceStatusChanged(kChanged_Color);
  CallDeviceChangeCallback();
}

uint8_t DeviceLightbulb::GetHue()
{
  return this->hue;
}

void DeviceLightbulb::SetSaturation(uint8_t saturation)
{
  if (this->saturation == saturation) {
    return;
  }
  this->saturation = saturation;
  this->HandleLightbulbDeviceStatusChanged(kChanged_Color);
  CallDeviceChangeCallback();
}

uint8_t DeviceLightbulb::GetSaturation()
{
  return this->saturation;
}

uint32_t DeviceLightbulb::GetOnoffClusterFeatureMap()
{
  return this->onoff_cluster_feature_map;
}

uint32_t DeviceLightbulb::GetLevelControlClusterFeatureMap()
{
  return this->level_control_cluster_feature_map;
}

uint32_t DeviceLightbulb::GetColorControlClusterFeatureMap()
{
  return this->color_control_cluster_feature_map;
}

uint16_t DeviceLightbulb::GetOnoffClusterRevision()
{
  return this->onoff_cluster_revision;
}

uint16_t DeviceLightbulb::GetLevelControlClusterRevision()
{
  return this->level_control_cluster_revision;
}

uint16_t DeviceLightbulb::GetColorControlClusterRevision()
{
  return this->color_control_cluster_revision;
}

uint8_t DeviceLightbulb::GetLevelControlMaxLevel()
{
  return this->level_control_max_level;
}

uint8_t DeviceLightbulb::GetLevelControlMinLevel()
{
  return this->level_control_min_level;
}

uint8_t DeviceLightbulb::GetLevelControlOptions()
{
  return this->level_control_options;
}

void DeviceLightbulb::SetLevelControlOptions(uint8_t options)
{
  this->level_control_options = options;
}

uint8_t DeviceLightbulb::GetLevelControlOnLevel()
{
  return this->level_control_on_level;
}

void DeviceLightbulb::SetLevelControlOnLevel(uint8_t on_level)
{
  this->level_control_on_level = on_level;
}

uint8_t DeviceLightbulb::GetLevelControlStartupCurrentLevel()
{
  return this->level_control_startup_current_level;
}

void DeviceLightbulb::SetLevelControlStartupCurrentLevel(uint8_t startup_current_level)
{
  this->level_control_startup_current_level = startup_current_level;
}

uint16_t DeviceLightbulb::GetLevelControlRemainingTime()
{
  return this->level_control_remaining_time;
}

uint8_t DeviceLightbulb::GetColorControlOptions()
{
  return this->color_control_options;
}

void DeviceLightbulb::SetColorControlOptions(uint8_t options)
{
  this->color_control_options = options;
}

uint8_t DeviceLightbulb::GetColorControlColorMode()
{
  return this->color_control_color_mode;
}

uint8_t DeviceLightbulb::GetColorControlEnhancedColorMode()
{
  return this->color_control_enhanced_color_mode;
}

uint8_t DeviceLightbulb::GetColorControlColorCapabilities()
{
  return this->color_control_color_capabilities;
}

void DeviceLightbulb::SetStartupOnOff(DeviceLightbulb::StartupOnOff_t startup_on_off)
{
  if (startup_on_off == this->startup_on_off) {
    return;
  }
  this->startup_on_off = startup_on_off;
  this->HandleLightbulbDeviceStatusChanged(kChanged_StartupOnOff);
  CallDeviceChangeCallback();
}

DeviceLightbulb::StartupOnOff_t DeviceLightbulb::GetStartupOnOff()
{
  return (DeviceLightbulb::StartupOnOff_t)this->startup_on_off;
}

CHIP_ERROR DeviceLightbulb::HandleReadEmberAfAttribute(ClusterId clusterId,
                                                       chip::AttributeId attributeId,
                                                       uint8_t* buffer,
                                                       uint16_t maxReadLength)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters;
  ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == BridgedDeviceBasicInformation::Id) {
    return this->HandleReadBridgedDeviceBasicAttribute(clusterId, attributeId, buffer, maxReadLength);
  }
  if (clusterId == Identify::Id) {
    return this->HandleReadIdentifyAttribute(clusterId, attributeId, buffer, maxReadLength);
  }
  if (clusterId == Groups::Id) {
    return this->HandleReadGroupsAttribute(clusterId, attributeId, buffer, maxReadLength);
  }

  if (clusterId != OnOff::Id && clusterId != LevelControl::Id && clusterId != ColorControl::Id) {
    ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute: invalid cluster");
    return CHIP_ERROR_INVALID_ARGUMENT;
  }

  if (clusterId == OnOff::Id) {                                                                       // -- OnOff cluster
    if ((attributeId == OnOff::Attributes::OnOff::Id) && (maxReadLength == 1)) {                      // OnOff
      *buffer = this->IsOn() ? 1 : 0;
    } else if ((attributeId == OnOff::Attributes::GlobalSceneControl::Id) && (maxReadLength == 1)) {  // GlobalSceneControl
      *buffer = this->global_scene_control ? 1 : 0;
    } else if ((attributeId == OnOff::Attributes::OnTime::Id) && (maxReadLength == 2)) {              // OnTime
      uint16_t onTime = this->on_time;
      memcpy(buffer, &onTime, sizeof(onTime));
    } else if ((attributeId == OnOff::Attributes::OffWaitTime::Id) && (maxReadLength == 2)) {         // OffWaitTime
      uint16_t offWaitTime = this->off_wait_time;
      memcpy(buffer, &offWaitTime, sizeof(offWaitTime));
    } else if ((attributeId == OnOff::Attributes::StartUpOnOff::Id) && (maxReadLength == 1)) {        // StartUpOnOff
      *buffer = this->startup_on_off;
    } else if ((attributeId == OnOff::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {     // ClusterRevision
      uint16_t rev = this->GetOnoffClusterRevision();
      memcpy(buffer, &rev, sizeof(rev));
    } else if ((attributeId == OnOff::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {          // FeatureMap
      uint32_t featureMap = this->GetOnoffClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - OnOff: invalid read");
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }

  if (clusterId == LevelControl::Id) {                                                                        // -- LevelControl cluster
    if ((attributeId == LevelControl::Attributes::CurrentLevel::Id) && (maxReadLength == 1)) {                // CurrentLevel
      uint8_t level = this->GetLevel();
      memcpy(buffer, &level, sizeof(level));
    } else if ((attributeId == LevelControl::Attributes::Options::Id) && (maxReadLength == 1)) {              // Options
      uint8_t options = this->GetLevelControlOptions();
      memcpy(buffer, &options, sizeof(options));
    } else if ((attributeId == LevelControl::Attributes::OnLevel::Id) && (maxReadLength == 1)) {              // OnLevel
      uint8_t onLevel = this->GetLevelControlOnLevel();
      memcpy(buffer, &onLevel, sizeof(onLevel));
    } else if ((attributeId == LevelControl::Attributes::MaxLevel::Id) && (maxReadLength == 1)) {             // MaxLevel
      uint8_t maxLevel = this->GetLevelControlMaxLevel();
      memcpy(buffer, &maxLevel, sizeof(maxLevel));
    } else if ((attributeId == LevelControl::Attributes::MinLevel::Id) && (maxReadLength == 1)) {             // MinLevel
      uint8_t minLevel = this->GetLevelControlMinLevel();
      memcpy(buffer, &minLevel, sizeof(minLevel));
    } else if ((attributeId == LevelControl::Attributes::RemainingTime::Id) && (maxReadLength == 2)) {        // RemainingTime
      uint16_t remainingTime = this->GetLevelControlRemainingTime();
      memcpy(buffer, &remainingTime, sizeof(remainingTime));
    } else if ((attributeId == LevelControl::Attributes::StartUpCurrentLevel::Id) && (maxReadLength == 1)) {  // StartUpCurrentLevel
      uint8_t startupCurrentLevel = this->GetLevelControlStartupCurrentLevel();
      memcpy(buffer, &startupCurrentLevel, sizeof(startupCurrentLevel));
    } else if ((attributeId == LevelControl::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {           // FeatureMap
      uint32_t featureMap = this->GetLevelControlClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == LevelControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {      // ClusterRevision
      uint16_t clusterRevision = this->GetLevelControlClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - LevelControl: invalid read");
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }

  if (clusterId == ColorControl::Id) {                                                                      // -- ColorControl cluster
    if ((attributeId == ColorControl::Attributes::CurrentHue::Id) && (maxReadLength == 1)) {
      uint8_t hue = this->GetHue();
      memcpy(buffer, &hue, sizeof(hue));
    } else if ((attributeId == ColorControl::Attributes::CurrentSaturation::Id) && (maxReadLength == 1)) {  // CurrentSaturation
      uint8_t saturation = this->GetSaturation();
      memcpy(buffer, &saturation, sizeof(saturation));
    } else if ((attributeId == ColorControl::Attributes::Options::Id) && (maxReadLength == 1)) {            // Options
      uint8_t options = this->GetColorControlOptions();
      memcpy(buffer, &options, sizeof(options));
    } else if ((attributeId == ColorControl::Attributes::ColorMode::Id) && (maxReadLength == 1)) {          // ColorMode
      uint8_t colorMode = this->GetColorControlColorMode();
      memcpy(buffer, &colorMode, sizeof(colorMode));
    } else if ((attributeId == ColorControl::Attributes::EnhancedColorMode::Id) && (maxReadLength == 1)) {  // EnhancedColorMode
      uint8_t enhancedColorMode = this->GetColorControlEnhancedColorMode();
      memcpy(buffer, &enhancedColorMode, sizeof(enhancedColorMode));
    } else if ((attributeId == ColorControl::Attributes::ColorCapabilities::Id) && (maxReadLength == 2)) {  // ColorCapabilities
      uint16_t colorCapabilities = this->GetColorControlColorCapabilities();
      memcpy(buffer, &colorCapabilities, sizeof(colorCapabilities));
    } else if ((attributeId == ColorControl::Attributes::NumberOfPrimaries::Id) && (maxReadLength == 1)) {  // NumberOfPrimaries
      uint8_t numberOfPrimaries = this->color_control_number_of_primaries;
      memcpy(buffer, &numberOfPrimaries, sizeof(numberOfPrimaries));
    } else if ((attributeId == ColorControl::Attributes::FeatureMap::Id) && (maxReadLength == 4)) {         // FeatureMap
      uint32_t featureMap = this->GetColorControlClusterFeatureMap();
      memcpy(buffer, &featureMap, sizeof(featureMap));
    } else if ((attributeId == ColorControl::Attributes::ClusterRevision::Id) && (maxReadLength == 2)) {    // ClusterRevision
      uint16_t clusterRevision = this->GetColorControlClusterRevision();
      memcpy(buffer, &clusterRevision, sizeof(clusterRevision));
    } else {
      ChipLogProgress(DeviceLayer, "HandleReadLightbulbAttribute - ColorControl: invalid read");
      return CHIP_ERROR_INVALID_ARGUMENT;
    }
  }

  return CHIP_NO_ERROR;
}

CHIP_ERROR DeviceLightbulb::HandleWriteEmberAfAttribute(ClusterId clusterId,
                                                        chip::AttributeId attributeId,
                                                        uint8_t* buffer)
{
  if (!this->reachable) {
    return CHIP_ERROR_INTERNAL;
  }

  using namespace ::chip::app::Clusters;
  ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute: clusterId=%lu attrId=%ld", clusterId, attributeId);

  if (clusterId == Identify::Id) {
    return this->HandleWriteIdentifyAttribute(clusterId, attributeId, buffer);
  }
  if (clusterId == Groups::Id) {
    return this->HandleWriteGroupsAttribute(clusterId, attributeId, buffer);
  }

  if (clusterId != OnOff::Id && clusterId != LevelControl::Id && clusterId != ColorControl::Id) {
    ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute: invalid cluster");
    return CHIP_ERROR_INTERNAL;
  }

  if (clusterId == OnOff::Id) {                                                   // -- OnOff cluster
    if ((attributeId == OnOff::Attributes::OnOff::Id)) {                          // OnOff
      if (*buffer) {
        this->SetOnOff(true);
      } else {
        this->SetOnOff(false);
      }
    } else if ((attributeId == OnOff::Attributes::GlobalSceneControl::Id)) {      // GlobalSceneControl
      this->global_scene_control = *buffer;
    } else if ((attributeId == OnOff::Attributes::OnTime::Id)) {                  // OnTime
      memcpy(&this->on_time, buffer, sizeof(this->on_time));
    } else if ((attributeId == OnOff::Attributes::OffWaitTime::Id)) {             // OffWaitTime
      memcpy(&this->off_wait_time, buffer, sizeof(this->off_wait_time));
    } else if ((attributeId == OnOff::Attributes::StartUpOnOff::Id)) {            // StartUpOnOff
      uint8_t startup_on_off_new = *buffer;
      if (startup_on_off_new > StartupOnOff_t::kToggle && startup_on_off_new != StartupOnOff_t::kNull) {
        return CHIP_ERROR_INVALID_ARGUMENT;
      }
      this->startup_on_off = startup_on_off_new;
    } else {
      ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute - OnOff: invalid write");
      return CHIP_ERROR_INTERNAL;
    }
  }

  if (clusterId == LevelControl::Id) {                                                // -- LevelControl cluster
    if ((attributeId == LevelControl::Attributes::CurrentLevel::Id)) {                // CurrentLevel
      if (!this->SetLevel(*buffer)) {
        return CHIP_ERROR_INVALID_ARGUMENT;
      }
    } else if ((attributeId == LevelControl::Attributes::Options::Id)) {              // Options
      this->SetLevelControlOptions(*buffer);
    } else if ((attributeId == LevelControl::Attributes::OnLevel::Id)) {              // OnLevel
      this->SetLevelControlOnLevel(*buffer);
    } else if ((attributeId == LevelControl::Attributes::StartUpCurrentLevel::Id)) {  // StartUpCurrentLevel
      this->SetLevelControlStartupCurrentLevel(*buffer);
    } else {
      ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute - LevelControl: invalid write");
      return CHIP_ERROR_INTERNAL;
    }
  }

  if (clusterId == ColorControl::Id) {                                              // -- ColorControl cluster
    if ((attributeId == ColorControl::Attributes::CurrentHue::Id)) {                // CurrentHue
      this->SetHue(*buffer);
    } else if ((attributeId == ColorControl::Attributes::CurrentSaturation::Id)) {  // CurrentSaturation
      this->SetSaturation(*buffer);
    } else if ((attributeId == ColorControl::Attributes::Options::Id)) {            // Options
      this->SetColorControlOptions(*buffer);
    } else {
      ChipLogProgress(DeviceLayer, "HandleWriteLightbulbAttribute - ColorControl: invalid write");
      return CHIP_ERROR_INTERNAL;
    }
  }

  return CHIP_NO_ERROR;
}

void DeviceLightbulb::HandleLightbulbDeviceStatusChanged(Changed_t itemChangedMask)
{
  using namespace ::chip::app::Clusters;

  if (itemChangedMask & kChanged_OnOff) {
    ScheduleMatterReportingCallback(this->endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
  }
  if (itemChangedMask & kChanged_Level) {
    ScheduleMatterReportingCallback(this->endpoint_id, LevelControl::Id, LevelControl::Attributes::CurrentLevel::Id);
  }
  if (itemChangedMask & kChanged_Color) {
    ScheduleMatterReportingCallback(this->endpoint_id, ColorControl::Id, ColorControl::Attributes::CurrentHue::Id);
    ScheduleMatterReportingCallback(this->endpoint_id, ColorControl::Id, ColorControl::Attributes::CurrentSaturation::Id);
  }
  if (itemChangedMask & kChanged_StartupOnOff) {
    ScheduleMatterReportingCallback(this->endpoint_id, OnOff::Id, OnOff::Attributes::StartUpOnOff::Id);
  }
}
