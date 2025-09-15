/***************************************************************************
 * @file SilabsTracing.h
 * @brief Instrumenting for matter operation tracing for the Silicon Labs platform.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#pragma once

#include <stdint.h>

namespace chip {
namespace Tracing {
namespace Silabs {

// Enum for the different operation to trace
enum class TimeTraceOperation : uint8_t
{
    kSpake2p,
    kPake1,
    kPake2,
    kPake3,
    kOperationalCredentials,
    kAttestationVerification,
    kCSR,
    kNOC,
    kTransportLayer,
    kTransportSetup,
    kFindOperational,
    kCaseSession,
    kSigma1,
    kSigma2,
    kSigma3,
    kOTA,
    kImageUpload,
    kImageVerification,
    kAppApplyTime,
    kBootup,
    kSilabsInit,
    kMatterInit,
    kAppInit,
    kBufferFull,
    kNumTraces,
};

enum class OperationType : uint8_t
{
    kBegin,
    kEnd,
    kInstant,
};

} // namespace Silabs
} // namespace Tracing
} // namespace chip