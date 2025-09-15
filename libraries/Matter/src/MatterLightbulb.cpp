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

#include "MatterLightbulb.h"
#include "util/hsv_rgb.h"

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;

const EmberAfDeviceType gOnOffDeviceType[] = { { DEVICE_TYPE_ON_OFF_LIGHT, DEVICE_VERSION_DEFAULT } };
const EmberAfDeviceType gDimmableBulbDeviceType[] = { { DEVICE_TYPE_DIMMABLE_LIGHT, DEVICE_VERSION_DEFAULT } };
const EmberAfDeviceType gExtendedColorBulbDeviceType[] = { { DEVICE_TYPE_EXTENDED_COLOR_LIGHT, DEVICE_VERSION_DEFAULT } };

constexpr CommandId onOffIncomingCommands[] = {
  app::Clusters::OnOff::Commands::Off::Id,
  app::Clusters::OnOff::Commands::On::Id,
  app::Clusters::OnOff::Commands::Toggle::Id,
  app::Clusters::OnOff::Commands::OffWithEffect::Id,
  app::Clusters::OnOff::Commands::OnWithRecallGlobalScene::Id,
  app::Clusters::OnOff::Commands::OnWithTimedOff::Id,
  kInvalidCommandId,
};

constexpr CommandId levelControlIncomingCommands[] = {
  app::Clusters::LevelControl::Commands::MoveToLevel::Id,
  app::Clusters::LevelControl::Commands::Move::Id,
  app::Clusters::LevelControl::Commands::Step::Id,
  app::Clusters::LevelControl::Commands::Stop::Id,
  app::Clusters::LevelControl::Commands::MoveToLevelWithOnOff::Id,
  app::Clusters::LevelControl::Commands::MoveWithOnOff::Id,
  app::Clusters::LevelControl::Commands::StepWithOnOff::Id,
  app::Clusters::LevelControl::Commands::StopWithOnOff::Id,
  app::Clusters::LevelControl::Commands::MoveToClosestFrequency::Id,
  kInvalidCommandId,
};

constexpr CommandId colorControlIncomingCommands[] = {
  app::Clusters::ColorControl::Commands::MoveToHue::Id,
  app::Clusters::ColorControl::Commands::MoveHue::Id,
  app::Clusters::ColorControl::Commands::StepHue::Id,
  app::Clusters::ColorControl::Commands::MoveToSaturation::Id,
  app::Clusters::ColorControl::Commands::MoveSaturation::Id,
  app::Clusters::ColorControl::Commands::StepSaturation::Id,
  app::Clusters::ColorControl::Commands::MoveToHueAndSaturation::Id,
  app::Clusters::ColorControl::Commands::MoveToColor::Id,
  app::Clusters::ColorControl::Commands::MoveColor::Id,
  app::Clusters::ColorControl::Commands::StepColor::Id,
  app::Clusters::ColorControl::Commands::MoveToColorTemperature::Id,
  app::Clusters::ColorControl::Commands::EnhancedMoveToHue::Id,
  app::Clusters::ColorControl::Commands::EnhancedMoveHue::Id,
  app::Clusters::ColorControl::Commands::EnhancedStepHue::Id,
  app::Clusters::ColorControl::Commands::EnhancedMoveToHueAndSaturation::Id,
  app::Clusters::ColorControl::Commands::ColorLoopSet::Id,
  app::Clusters::ColorControl::Commands::StopMoveStep::Id,
  app::Clusters::ColorControl::Commands::MoveColorTemperature::Id,
  app::Clusters::ColorControl::Commands::StepColorTemperature::Id,
  kInvalidCommandId,
};

// On/Off cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(onOffAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::OnOff::Id, BOOLEAN, 1, 0),                                       /* OnOff */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::GlobalSceneControl::Id, BOOLEAN, 1, 0),                          /* GlobalSceneControl */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::OnTime::Id, INT16U, 2, 0),                                       /* OnTime */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::OffWaitTime::Id, INT16U, 2, 0),                                  /* OffWaitTime */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::StartUpOnOff::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE_NULLABLE),   /* StartUpOnOff */
DECLARE_DYNAMIC_ATTRIBUTE(OnOff::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                 /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                         /* ClusterRevision auto added by LIST_END */

// LevelControl cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(levelControlAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::CurrentLevel::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),                  /* CurrentLevel */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::RemainingTime::Id, INT16U, 2, 0),                                      /* RemainingTime */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::MinLevel::Id, INT8U, 1, 0),                                            /* MinLevel */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::MaxLevel::Id, INT8U, 1, 0),                                            /* MaxLevel */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::Options::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),                       /* Options */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::OnLevel::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE_NULLABLE),              /* OnLevel */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::StartUpCurrentLevel::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE_NULLABLE),  /* StartupCurrentLevel */
DECLARE_DYNAMIC_ATTRIBUTE(LevelControl::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                       /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                                      /* ClusterRevision auto added by LIST_END */

// ColorControl cluster attributes
DECLARE_DYNAMIC_ATTRIBUTE_LIST_BEGIN(colorControlAttrs)
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::CurrentHue::Id, INT8U, 1, 0),                                   /* CurrentHue */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::CurrentSaturation::Id, INT8U, 1, 0),                            /* CurrentSaturation */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::ColorMode::Id, ENUM8, 1, 0),                                    /* ColorMode */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::EnhancedColorMode::Id, ENUM8, 1, 0),                            /* EnhancedColorMode */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::ColorCapabilities::Id, BITMAP16, 2, 0),                         /* ColorCapabilities */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::Options::Id, INT8U, 1, ATTRIBUTE_MASK_WRITABLE),                /* Options */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::NumberOfPrimaries::Id, INT8U, 1, ATTRIBUTE_MASK_NULLABLE),      /* NumberOfPrimaries */
DECLARE_DYNAMIC_ATTRIBUTE(ColorControl::Attributes::FeatureMap::Id, BITMAP32, 4, 0),                                /* FeatureMap */
DECLARE_DYNAMIC_ATTRIBUTE_LIST_END();                                                                               /* ClusterRevision auto added by LIST_END */

// Simple Lightbulb endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(SimpleLightbulbEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(OnOff::Id, onOffAttrs, ZAP_CLUSTER_MASK(SERVER), onOffIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Groups::Id, groupsAttrs, ZAP_CLUSTER_MASK(SERVER), groupsIncomingCommands, groupsOutgoingCommands),
DECLARE_DYNAMIC_CLUSTER_LIST_END;

// Dimmable Lightbulb endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(DimmableLightbulbEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(OnOff::Id, onOffAttrs, ZAP_CLUSTER_MASK(SERVER), onOffIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(LevelControl::Id, levelControlAttrs, ZAP_CLUSTER_MASK(SERVER), levelControlIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Groups::Id, groupsAttrs, ZAP_CLUSTER_MASK(SERVER), groupsIncomingCommands, groupsOutgoingCommands),
DECLARE_DYNAMIC_CLUSTER_LIST_END;

// Color Lightbulb endpoint cluster list
DECLARE_DYNAMIC_CLUSTER_LIST_BEGIN(ColorLightbulbEndpointClusters)
DECLARE_DYNAMIC_CLUSTER(OnOff::Id, onOffAttrs, ZAP_CLUSTER_MASK(SERVER), onOffIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(LevelControl::Id, levelControlAttrs, ZAP_CLUSTER_MASK(SERVER), levelControlIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(ColorControl::Id, colorControlAttrs, ZAP_CLUSTER_MASK(SERVER), colorControlIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Descriptor::Id, descriptorAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(BridgedDeviceBasicInformation::Id, bridgedDeviceBasicAttrs, ZAP_CLUSTER_MASK(SERVER), nullptr, nullptr),
DECLARE_DYNAMIC_CLUSTER(Identify::Id, identifyAttrs, ZAP_CLUSTER_MASK(SERVER), identifyIncomingCommands, nullptr),
DECLARE_DYNAMIC_CLUSTER(Groups::Id, groupsAttrs, ZAP_CLUSTER_MASK(SERVER), groupsIncomingCommands, groupsOutgoingCommands),
DECLARE_DYNAMIC_CLUSTER_LIST_END;

//##################################################################################################
// Functions for MatterLightbulb
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterLightbulb
 ******************************************************************************/
MatterLightbulb::MatterLightbulb() :
  lightbulb_device(nullptr),
  device_endpoint(nullptr),
  endpoint_dataversion_storage(nullptr),
  identify_server(nullptr),
  initialized(false)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterLightbulb
 ******************************************************************************/
MatterLightbulb::~MatterLightbulb()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterLightbulb instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterLightbulb::begin()
{
  return this->begin_internal(lightbulb_simple,
                              SimpleLightbulbEndpointClusters,
                              ArraySize(SimpleLightbulbEndpointClusters));
}

/***************************************************************************//**
 * Initializes the MatterLightbulb instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterLightbulb::begin_internal(bulb_types_e bulb_type, EmberAfCluster* endpoint_clusters, uint8_t cluster_count)
{
  if (this->initialized) {
    return false;
  }

  // Create new device
  DeviceLightbulb* new_lightbulb_device = new (std::nothrow)DeviceLightbulb("Matter bulb");
  if (new_lightbulb_device == nullptr) {
    return false;
  }
  new_lightbulb_device->SetReachable(true);

  // Set the device instance pointer in the base class
  this->base_matter_device = new_lightbulb_device;

  switch (bulb_type) {
    case lightbulb_simple:
      new_lightbulb_device->SetProductName("On/off lightbulb");
      break;

    case lightbulb_dimmable:
      new_lightbulb_device->SetProductName("Dimmable lightbulb");
      break;

    case lightbulb_color:
      new_lightbulb_device->SetProductName("Color lightbulb");
      break;
  }

  // Create new endpoint
  EmberAfEndpointType* new_endpoint = (EmberAfEndpointType*)malloc(sizeof(EmberAfEndpointType));
  if (new_endpoint == nullptr) {
    delete(new_lightbulb_device);
    return false;
  }
  new_endpoint->cluster = endpoint_clusters;
  new_endpoint->clusterCount = cluster_count;
  new_endpoint->endpointSize = 0;

  // Create data version storage for the endpoint
  size_t dataversion_size = cluster_count * sizeof(DataVersion);
  DataVersion* new_bulb_data_version = (DataVersion*)malloc(dataversion_size);
  if (new_bulb_data_version == nullptr) {
    delete(new_lightbulb_device);
    free(new_endpoint);
    return false;
  }

  // Add new endpoint
  int result = -1;
  if (bulb_type == lightbulb_simple) {
    result = AddDeviceEndpoint(new_lightbulb_device,
                               new_endpoint,
                               Span<const EmberAfDeviceType>(gOnOffDeviceType),
                               Span<DataVersion>(new_bulb_data_version, dataversion_size), 1);
  } else if (bulb_type == lightbulb_dimmable) {
    result = AddDeviceEndpoint(new_lightbulb_device,
                               new_endpoint,
                               Span<const EmberAfDeviceType>(gDimmableBulbDeviceType),
                               Span<DataVersion>(new_bulb_data_version, dataversion_size), 1);
  } else if (bulb_type == lightbulb_color) {
    result = AddDeviceEndpoint(new_lightbulb_device,
                               new_endpoint,
                               Span<const EmberAfDeviceType>(gExtendedColorBulbDeviceType),
                               Span<DataVersion>(new_bulb_data_version, dataversion_size), 1);
  }

  if (result < 0) {
    delete(new_lightbulb_device);
    free(new_endpoint);
    free(new_bulb_data_version);
    return false;
  }

  // Create a new Identify cluster server
  ::Identify* identify_server = new (std::nothrow)::Identify(new_lightbulb_device->GetEndpointId(),
                                                             IdentifyStartHandler,
                                                             IdentifyStopHandler,
                                                             Identify::IdentifyTypeEnum::kLightOutput,
                                                             TriggerIdentifyEffectHandler);
  if (identify_server == nullptr) {
    (void)RemoveDeviceEndpoint(new_lightbulb_device);
    delete(new_lightbulb_device);
    free(new_endpoint);
    free(new_bulb_data_version);
    return false;
  }

  // Start the Identify cluster server
  emberAfIdentifyClusterServerInitCallback(new_lightbulb_device->GetEndpointId());

  if (bulb_type == lightbulb_dimmable || bulb_type == lightbulb_color) {
    // Call the LevelControl init callback one more time to have the min/max values set.
    // It's called first when adding the cluster/endpoint, but the min/max is not initialized at that point.
    // Without this the min/max is at 0 and every level change results in the bulb being turned off.
    emberAfLevelControlClusterServerInitCallback(new_lightbulb_device->GetEndpointId());
    // Set the device back to offline as the LevelControlClusterServer init inadvertently sets it to online
    new_lightbulb_device->SetOnline(false);
  }

  this->lightbulb_device = new_lightbulb_device;
  this->device_endpoint = new_endpoint;
  this->endpoint_dataversion_storage = new_bulb_data_version;
  this->identify_server = identify_server;
  this->initialized = true;
  return true;
}

/***************************************************************************//**
 * Deinitializes the MatterLightbulb instance
 ******************************************************************************/
void MatterLightbulb::end()
{
  if (!this->initialized) {
    return;
  }
  (void)RemoveDeviceEndpoint(this->lightbulb_device);
  free(this->device_endpoint);
  free(this->endpoint_dataversion_storage);
  delete(this->lightbulb_device);
  delete(this->identify_server);
  this->initialized = false;
}

/***************************************************************************//**
 * Sets the lightbulb's on/off state
 *
 * @param[in] value the requested on/off state
 ******************************************************************************/
void MatterLightbulb::set_onoff(bool value)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->lightbulb_device->SetOnOff(value);
  PlatformMgr().UnlockChipStack();
}

/***************************************************************************//**
 * Provides the lightbulb's current on/off state
 *
 * @return the lightbulb's current on/off state
 ******************************************************************************/
bool MatterLightbulb::get_onoff()
{
  if (!this->initialized) {
    return false;
  }
  return this->lightbulb_device->IsOn();
}

/***************************************************************************//**
 * Toggle the lightbulb's current on/off state
 ******************************************************************************/
void MatterLightbulb::toggle()
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->lightbulb_device->Toggle();
  PlatformMgr().UnlockChipStack();
}

void MatterLightbulb::set_startup_on_off(DeviceLightbulb::StartupOnOff_t startup_on_off)
{
  if (!this->initialized) {
    return;
  }
  PlatformMgr().LockChipStack();
  this->lightbulb_device->SetStartupOnOff(startup_on_off);
  PlatformMgr().UnlockChipStack();
}

DeviceLightbulb::StartupOnOff_t MatterLightbulb::get_startup_on_off()
{
  if (!this->initialized) {
    return DeviceLightbulb::StartupOnOff_t::kNull;
  }
  return this->lightbulb_device->GetStartupOnOff();
}

/***************************************************************************//**
 * Bool operator for getting the on/off state of the lightbulb
 *
 * @return the lightbulb's current on/off state
 ******************************************************************************/
MatterLightbulb::operator bool()
{
  return this->get_onoff();
}

/***************************************************************************//**
 * Assignment operator for the MatterLightbulb class to set the on/off state
 *
 * @param[in] state the requested on/off state
 ******************************************************************************/
void MatterLightbulb::operator=(bool state)
{
  this->set_onoff(state);
}

//##################################################################################################
// Functions for MatterDimmableLightbulb
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterDimmableLightbulb
 ******************************************************************************/
MatterDimmableLightbulb::MatterDimmableLightbulb()
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterDimmableLightbulb
 ******************************************************************************/
MatterDimmableLightbulb::~MatterDimmableLightbulb()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterDimmableLightbulb instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterDimmableLightbulb::begin()
{
  return this->begin_internal(lightbulb_dimmable,
                              DimmableLightbulbEndpointClusters,
                              ArraySize(DimmableLightbulbEndpointClusters));
}

/***************************************************************************//**
 * Provides the lightbulb's current brightness value
 * The provided brightness value is in the range of 0-254.
 *
 * @return the lightbulb's current brightness value
 ******************************************************************************/
uint8_t MatterDimmableLightbulb::get_brightness()
{
  if (!this->initialized) {
    return 0u;
  }
  return this->lightbulb_device->GetLevel();
}

/***************************************************************************//**
 * Sets the lightbulb's brightness value
 *
 * @param[in] brightness the requested brightness in the range of 0-254
 ******************************************************************************/
void MatterDimmableLightbulb::set_brightness(uint8_t brightness)
{
  if (!this->initialized) {
    return;
  }
  (void)this->lightbulb_device->SetLevel(brightness);
}

/***************************************************************************//**
 * Provides the lightbulb's current brightness in percents
 * The provided value's range is 0-100.
 *
 * @return the lightbulb's current brightness percentage
 ******************************************************************************/
uint8_t MatterDimmableLightbulb::get_brightness_percent()
{
  if (!this->initialized) {
    return 0;
  }
  uint8_t dev_max_brightness = this->lightbulb_device->GetLevelControlMaxLevel();
  float brightness_percent = (float)this->get_brightness() * 100.0f / (float)dev_max_brightness;
  return (uint8_t)(std::round(brightness_percent - 0.1f));
}

/***************************************************************************//**
 * Sets the lightbulb's brightness percentage
 * The accepted range is 0-100.
 *
 * @param[in] brightness the requested brightness in the range of 0-100
 ******************************************************************************/
void MatterDimmableLightbulb::set_brightness_percent(uint8_t brightness)
{
  if (!this->initialized || brightness > 100) {
    return;
  }
  uint8_t dev_max_brightness = this->lightbulb_device->GetLevelControlMaxLevel();
  float brightness_full_scale = (float)brightness * (float)dev_max_brightness / 100.0f;
  this->set_brightness((uint8_t)(ceil(brightness_full_scale)));
}

/***************************************************************************//**
 * uint8_t operator for getting the lightbulb's brightness percentage
 * The provided value's range is 0-100.
 *
 * @return the lightbulb's current brightness percentage
 ******************************************************************************/
MatterDimmableLightbulb::operator uint8_t()
{
  return this->get_brightness_percent();
}

/***************************************************************************//**
 * Assignment operator for the MatterLightbulb class to set the on/off state
 *
 * @param[in] state the requested on/off state
 ******************************************************************************/
void MatterDimmableLightbulb::operator=(bool state)
{
  this->set_onoff(state);
}

/***************************************************************************//**
 * Assignment operator for the MatterLightbulb class to set the brightness
 *
 * @param[in] brightness the requested brightness percentage
 ******************************************************************************/
void MatterDimmableLightbulb::operator=(unsigned int brightness)
{
  this->set_brightness_percent((uint8_t)brightness);
}

//##################################################################################################
// Functions for MatterColorLightbulb
//##################################################################################################

/***************************************************************************//**
 * Constructor for MatterColorLightbulb
 ******************************************************************************/
MatterColorLightbulb::MatterColorLightbulb() :
  saturation_boost(0)
{
  ;
}

/***************************************************************************//**
 * Destructor for MatterColorLightbulb
 ******************************************************************************/
MatterColorLightbulb::~MatterColorLightbulb()
{
  this->end();
}

/***************************************************************************//**
 * Initializes the MatterColorLightbulb instance
 *
 * @return true if the initialization succeeded, false otherwise
 ******************************************************************************/
bool MatterColorLightbulb::begin()
{
  return this->begin_internal(lightbulb_color,
                              ColorLightbulbEndpointClusters,
                              ArraySize(ColorLightbulbEndpointClusters));
}

/***************************************************************************//**
 * Provides the lightbulb's current hue value
 * The provided hue is in the range of 0-255. It must be converted to the range
 * of 0-360 to represent the true hue.
 *
 * @return the lightbulb's current hue value
 ******************************************************************************/
uint8_t MatterColorLightbulb::get_hue()
{
  if (!this->initialized) {
    return 0u;
  }
  return this->lightbulb_device->GetHue();
}

/***************************************************************************//**
 * Provides the lightbulb's current color saturation value
 * The provided saturation value is in the range of 0-255.
 *
 * @return the lightbulb's current color saturation value
 ******************************************************************************/
uint8_t MatterColorLightbulb::get_saturation()
{
  uint32_t sat = this->lightbulb_device->GetSaturation();
  if (sat + this->saturation_boost > UINT8_MAX) {
    return UINT8_MAX;
  }
  return (uint8_t)sat;
}

/***************************************************************************//**
 * Sets the lightbulb's hue value
 *
 * @param[in] hue the requested hue in the range of 0-255
 ******************************************************************************/
void MatterColorLightbulb::set_hue(uint8_t hue)
{
  if (!this->initialized) {
    return;
  }
  this->lightbulb_device->SetHue(hue);
}

/***************************************************************************//**
 * Sets the lightbulb's color saturation value
 *
 * @param[in] saturation the requested color saturation in the range of 0-255
 ******************************************************************************/
void MatterColorLightbulb::set_saturation(uint8_t saturation)
{
  if (!this->initialized) {
    return;
  }
  this->lightbulb_device->SetSaturation(saturation);
}

/***************************************************************************//**
 * Provides the lightbulb's current true hue value
 * The true hue has a range of 0-360 which can represent the actual color
 * without conversion.
 *
 * @return the lightbulb's current true hue value
 ******************************************************************************/
uint16_t MatterColorLightbulb::get_true_hue()
{
  return map(this->get_hue(), 0, 254, 0, 360);
}

/***************************************************************************//**
 * Provides the lightbulb's current color saturation value in percents
 * The provided value's range is 0-100.
 *
 * @return the lightbulb's current color saturation percentage
 ******************************************************************************/
uint8_t MatterColorLightbulb::get_saturation_percent()
{
  return map(this->get_saturation(), 0, 254, 0, 100);
}

/***************************************************************************//**
 * Sets the lightbulb's true hue value
 * The accepted range is 0-360.
 *
 * @param[in] true_hue the requested true hue in the range of 0-360
 ******************************************************************************/
void MatterColorLightbulb::set_true_hue(uint16_t true_hue)
{
  if (true_hue > 360) {
    return;
  }
  uint8_t hue_scaled_down = map(true_hue, 0, 360, 0, 254);
  this->set_hue(hue_scaled_down);
}

/***************************************************************************//**
 * Sets the lightbulb's color saturation percentage
 * The accepted range is 0-100.
 *
 * @param[in] saturation the requested color saturation in the range of 0-100
 ******************************************************************************/
void MatterColorLightbulb::set_saturation_percent(uint8_t saturation)
{
  if (saturation > 100) {
    return;
  }
  uint8_t saturation_full_scale = map(saturation, 0, 100, 0, 254);
  this->set_saturation(saturation_full_scale);
}

/***************************************************************************//**
 * Provides the lightbulb's currently set color in RGB format
 * The returned value is converted from the internal HSV representation and
 * is brightness adjusted.
 *
 * @param[out] r pointer to a variable to hold the current Red value
 * @param[out] g pointer to a variable to hold the current Green value
 * @param[out] b pointer to a variable to hold the current Blue value
 ******************************************************************************/
void MatterColorLightbulb::get_rgb(uint8_t* r, uint8_t* g, uint8_t* b)
{
  if (!r || !g || !b) {
    return;
  }
  uint8_t hue = this->get_hue();
  uint8_t saturation = this->get_saturation();
  uint8_t value = this->get_brightness();

  // Adjust the hue range from 0-255 to 0-360 to represent the true hue
  double hue_f = (double)hue * 360.0 / 254.0;
  double saturation_f = (double)saturation / 254.0;
  double value_f = (double)value / 255.0;

  double in[3] = { hue_f, saturation_f, value_f };
  uint8_t res[3];
  HSVtoRGB(in, res);
  *r = res[0];
  *g = res[1];
  *b = res[2];
}

/***************************************************************************//**
 * Provides the lightbulb's currently set raw color in RGB format without
 * adjusting for brightness
 * The returned value is converted from the internal HSV representation with
 * the brightness value at the maximum regardless of the configured value.
 *
 * @param[out] r pointer to a variable to hold the current Red value
 * @param[out] g pointer to a variable to hold the current Green value
 * @param[out] b pointer to a variable to hold the current Blue value
 ******************************************************************************/
void MatterColorLightbulb::get_rgb_raw(uint8_t* r, uint8_t* g, uint8_t* b)
{
  if (!r || !g || !b) {
    return;
  }
  uint8_t hue = this->get_hue();
  // Adjust the hue range from 0-255 to 0-360 to represent the true hue
  uint32_t hue_adj = (uint32_t)hue * 360 / 254;
  uint8_t saturation = this->get_saturation();

  double hue_f = (double)hue_adj;
  double saturation_f = (double)saturation / 254.0;
  double value_f = (double)1.0;

  double in[3] = { hue_f, saturation_f, value_f };
  uint8_t res[3];
  HSVtoRGB(in, res);
  *r = res[0];
  *g = res[1];
  *b = res[2];
}

/***************************************************************************//**
 * Sets the lightbulb's current color to the provided RGB values
 *
 * @param[in] r the requested Red value
 * @param[in] g the requested Green value
 * @param[in] b the requested Blue value
 ******************************************************************************/
void MatterColorLightbulb::set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  if (!this->initialized) {
    return;
  }
  uint8_t rgb_in[3] = { r, g, b };
  double hsv_out[3];
  RGBtoHSV(rgb_in, hsv_out);
  uint16_t true_hue = (uint16_t)hsv_out[0];
  uint8_t saturation = (uint8_t)hsv_out[1];
  uint8_t brightness = (uint8_t)hsv_out[2];
  this->set_true_hue(true_hue);
  this->set_saturation(saturation);
  this->set_brightness(brightness);
}

/***************************************************************************//**
 * Boosts the color saturation by the provided amount
 * The boost range is 0-255. This function can be used to achieve
 * more vibrant colors. For example the color picker in the Google Home app
 * caps out at 80% saturation for some reason - this function can be used
 * to avoid washed out colors.
 *
 * @param[in] amount the amount to boost the color saturation value with
 ******************************************************************************/
void MatterColorLightbulb::boost_saturation(uint8_t amount)
{
  this->saturation_boost = amount;
}

/***************************************************************************//**
 * Assignment operator for the MatterLightbulb class to set the on/off state
 *
 * @param[in] state the requested on/off state
 ******************************************************************************/
void MatterColorLightbulb::operator=(bool state)
{
  this->set_onoff(state);
}

/***************************************************************************//**
 * Assignment operator for the MatterLightbulb class to set the brightness
 *
 * @param[in] brightness the requested brightness percentage
 ******************************************************************************/
void MatterColorLightbulb::operator=(unsigned int brightness)
{
  this->set_brightness_percent((uint8_t)brightness);
}
