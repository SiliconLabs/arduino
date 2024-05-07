/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
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

#include <app/CommandHandler.h>
#include <app/util/attribute-storage.h>
#include <app/reporting/reporting.h>

using namespace ::chip;
using namespace ::chip::Platform;
using namespace ::chip::Credentials;
using namespace ::chip::app::Clusters;
using chip::Protocols::InteractionModel::Status;
using namespace chip::app::Clusters::WindowCovering;

// Matter Ember AF weak stubs - allows sketches to be compiled without the Matter library

bool SL_WEAK emberAfWindowCoveringClusterUpOrOpenCallback(app::CommandHandler* commandObj,
                                                          const app::ConcreteCommandPath& commandPath,
                                                          const Commands::UpOrOpen::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterDownOrCloseCallback(app::CommandHandler* commandObj,
                                                             const app::ConcreteCommandPath& commandPath,
                                                             const Commands::DownOrClose::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterStopMotionCallback(app::CommandHandler* commandObj,
                                                            const app::ConcreteCommandPath& commandPath,
                                                            const Commands::StopMotion::DecodableType& fields)
{
  (void)commandObj;
  (void)commandPath;
  (void)fields;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToLiftValueCallback(app::CommandHandler* commandObj,
                                                               const app::ConcreteCommandPath& commandPath,
                                                               const Commands::GoToLiftValue::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToLiftPercentageCallback(app::CommandHandler* commandObj,
                                                                    const app::ConcreteCommandPath& commandPath,
                                                                    const Commands::GoToLiftPercentage::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToTiltValueCallback(app::CommandHandler* commandObj,
                                                               const app::ConcreteCommandPath& commandPath,
                                                               const Commands::GoToTiltValue::DecodableType& commandData)
{
  (void)commandObj;
  (void)commandPath;
  (void)commandData;
  return false;
}

bool SL_WEAK emberAfWindowCoveringClusterGoToTiltPercentageCallback(app::CommandHandler* commandObj,
                                                                    const app::ConcreteCommandPath& commandPath,
                                                                    const Commands::GoToTiltPercentage::DecodableType& commandData)
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

#endif // ARDUINO_MATTER
