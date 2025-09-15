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

#ifdef ARDUINO_MATTER

#include "sl_common.h"
#include <app/CommandHandler.h>
#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h>
#include <MatterBLE.h>

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;
using chip::Protocols::InteractionModel::Status;
using namespace chip::app::Clusters::WindowCovering;
using namespace chip::app::Clusters::MediaPlayback;
using namespace chip::app::Clusters::Messages;
using namespace chip::app::Clusters::KeypadInput;
using namespace chip::app::Clusters::TemperatureControl;

// Matter BLE event handler weak stub - allows Matter sketches to be compiled without using BLE
void SL_WEAK matter_ble_on_event(sl_bt_msg_t *evt)
{
  (void)evt;
}

// Matter Ember AF weak stubs - allows sketches to be compiled without the Matter library
bool SL_WEAK emberAfWindowCoveringClusterUpOrOpenCallback(app::CommandHandler* commandObj,
                                                          const app::ConcreteCommandPath& commandPath,
                                                          const WindowCovering::Commands::UpOrOpen::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterDownOrCloseCallback(app::CommandHandler* commandObj,
                                                             const app::ConcreteCommandPath& commandPath,
                                                             const WindowCovering::Commands::DownOrClose::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterStopMotionCallback(app::CommandHandler* commandObj,
                                                            const app::ConcreteCommandPath& commandPath,
                                                            const WindowCovering::Commands::StopMotion::DecodableType& fields)
{
  (void)commandObj;
  (void)commandPath;
  (void)fields;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToLiftValueCallback(app::CommandHandler* commandObj,
                                                               const app::ConcreteCommandPath& commandPath,
                                                               const WindowCovering::Commands::GoToLiftValue::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToLiftPercentageCallback(app::CommandHandler* commandObj,
                                                                    const app::ConcreteCommandPath& commandPath,
                                                                    const WindowCovering::Commands::GoToLiftPercentage::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToTiltValueCallback(app::CommandHandler* commandObj,
                                                               const app::ConcreteCommandPath& commandPath,
                                                               const WindowCovering::Commands::GoToTiltValue::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToTiltPercentageCallback(app::CommandHandler* commandObj,
                                                                    const app::ConcreteCommandPath& commandPath,
                                                                    const WindowCovering::Commands::GoToTiltPercentage::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

void SL_WEAK MatterWindowCoveringClusterServerAttributeChangedCallback(const app::ConcreteAttributePath& attributePath)
{
  (void)attributePath;
}

void SL_WEAK MatterWindowCoveringPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterBindingPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterPowerSourceConfigurationPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterPowerSourcePluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterLaundryWasherControlsPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterTemperatureControlPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterRefrigeratorAlarmPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterSmokeCoAlarmPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterMicrowaveOvenControlPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterMessagesPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterEnergyEvsePluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterMediaPlaybackPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterKeypadInputPluginServerInitCallback()
{
  ;
}

void SL_WEAK MatterLaundryDryerControlsPluginServerInitCallback()
{
  ;
}

Status SL_WEAK MatterLaundryDryerControlsClusterServerPreAttributeChangedCallback(const chip::app::ConcreteAttributePath & attributePath,
                                                                                  EmberAfAttributeType attributeType, uint16_t size,
                                                                                  uint8_t * value)
{
  (void)attributePath;
  (void)attributeType;
  (void)size;
  (void)value;
  return Status::Failure;
}

Status SL_WEAK MatterLaundryWasherControlsClusterServerPreAttributeChangedCallback(const chip::app::ConcreteAttributePath & attributePath,
                                                                                   EmberAfAttributeType attributeType, uint16_t size,
                                                                                   uint8_t * value)
{
  (void)attributePath;
  (void)attributeType;
  (void)size;
  (void)value;
  return Status::Failure;
}

bool SL_WEAK emberAfKeypadInputClusterSendKeyCallback(app::CommandHandler * command, const app::ConcreteCommandPath & commandPath,
                                                      const KeypadInput::Commands::SendKey::DecodableType & commandData)
{
  (void)command;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfMediaPlaybackClusterPlayCallback(app::CommandHandler * command, const app::ConcreteCommandPath & commandPath,
                                                     const MediaPlayback::Commands::Play::DecodableType & commandData)
{
  (void)command;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfMediaPlaybackClusterPauseCallback(app::CommandHandler * command, const app::ConcreteCommandPath & commandPath,
                                                      const MediaPlayback::Commands::Pause::DecodableType & commandData)
{
  (void)command;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfMediaPlaybackClusterStopCallback(app::CommandHandler * command, const app::ConcreteCommandPath & commandPath,
                                                     const MediaPlayback::Commands::Stop::DecodableType & commandData)
{
  (void)command;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfMessagesClusterPresentMessagesRequestCallback(
  chip::app::CommandHandler * commandObj, const chip::app::ConcreteCommandPath & commandPath,
  const chip::app::Clusters::Messages::Commands::PresentMessagesRequest::DecodableType & commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfMessagesClusterCancelMessagesRequestCallback(
  chip::app::CommandHandler * commandObj, const chip::app::ConcreteCommandPath & commandPath,
  const chip::app::Clusters::Messages::Commands::CancelMessagesRequest::DecodableType & commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfTemperatureControlClusterSetTemperatureCallback(app::CommandHandler * commandObj,
                                                                    const app::ConcreteCommandPath & commandPath,
                                                                    const TemperatureControl::Commands::SetTemperature::DecodableType & commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

#endif // ARDUINO_MATTER
