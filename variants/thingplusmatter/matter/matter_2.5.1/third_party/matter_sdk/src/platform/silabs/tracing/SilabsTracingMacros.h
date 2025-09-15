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

#define _MATTER_TRACE_DISABLE(...)                                                                                                 \
    do                                                                                                                             \
    {                                                                                                                              \
    } while (false)

#include "SilabsTracingTypes.h"
#include <matter/tracing/build_config.h>
#if MATTER_TRACING_ENABLED
#include "SilabsTracing.h"
#include <lib/core/CHIPError.h>
#include <tracing/backend.h>
#include <tracing/metric_event.h>
#include <tracing/registry.h>

// This gets forwarded to the multiplexed instance
#define MATTER_TRACE_BEGIN(label, group) ::chip::Tracing::Internal::Begin(label, group)
#define MATTER_TRACE_END(label, group) ::chip::Tracing::Internal::End(label, group)
#define MATTER_TRACE_INSTANT(label, group) ::chip::Tracing::Internal::Instant(label, group)
#define MATTER_TRACE_COUNTER(label) ::chip::Tracing::Internal::Counter(label)

// We are not using this in our current implementation, so we are disabling it.
#define MATTER_TRACE_SCOPE(...) _MATTER_TRACE_DISABLE(__VA_ARGS__)

#define SILABS_TRACE_BEGIN(operation) ::chip::Tracing::Silabs::SilabsTracer::Instance().TimeTraceBegin(operation)
#define SILABS_TRACE_END(operation) ::chip::Tracing::Silabs::SilabsTracer::Instance().TimeTraceEnd(operation)
#define SILABS_TRACE_END_ERROR(operation, error) ::chip::Tracing::Silabs::SilabsTracer::Instance().TimeTraceEnd(operation, error)
#define SILABS_TRACE_INSTANT(operation) ::chip::Tracing::Silabs::SilabsTracer::Instance().TimeTraceInstant(operation)
#define SILABS_TRACE_INSTANT_ERROR(operation, error)                                                                               \
    ::chip::Tracing::Silabs::SilabsTracer::Instance().TimeTraceInstant(operation, error)

#define SILABS_TRACE_FLUSH_ALL() ::chip::Tracing::Silabs::SilabsTracer::Instance().TraceBufferFlushAll()
#define SILABS_TRACE_REGISTER(operationKey)                                                                                        \
    ::chip::Tracing::Silabs::SilabsTracer::Instance().RegisterAppTimeTraceOperation(operationKey)

#else // MATTER_TRACING_ENABLED

#define MATTER_TRACE_BEGIN(label, group) _MATTER_TRACE_DISABLE(label, group)
#define MATTER_TRACE_END(label, group) _MATTER_TRACE_DISABLE(label, group)
#define MATTER_TRACE_INSTANT(label, group) _MATTER_TRACE_DISABLE(label, group)
#define MATTER_TRACE_COUNTER(label) _MATTER_TRACE_DISABLE(label)
#define MATTER_TRACE_SCOPE(...) _MATTER_TRACE_DISABLE(__VA_ARGS__)

#define SILABS_TRACE_BEGIN(operation) _MATTER_TRACE_DISABLE(operation)
#define SILABS_TRACE_END(operation) _MATTER_TRACE_DISABLE(operation)
#define SILABS_TRACE_END_ERROR(operation, error) _MATTER_TRACE_DISABLE(operation, error)
#define SILABS_TRACE_INSTANT(operation) _MATTER_TRACE_DISABLE(operation)
#define SILABS_TRACE_INSTANT_ERROR(operation, error) _MATTER_TRACE_DISABLE(operation, error)

#define SILABS_TRACE_FLUSH_ALL() _MATTER_TRACE_DISABLE()
#define SILABS_TRACE_REGISTER(operationKey) _MATTER_TRACE_DISABLE(operationKey)

#endif // MATTER_TRACING_ENABLED
