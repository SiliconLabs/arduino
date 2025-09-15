/***************************************************************************//**
 * @file
 * @brief NVM3 trace macros
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef NVM3_TRACE_H
#define NVM3_TRACE_H

#include "nvm3_config.h"
#include <stdint.h>
#if (NVM3_TRACE_PORT == NVM3_TRACE_PORT_PRINTF)
#include <stdio.h>
#endif
#if NVM3_TRACE_PORT == NVM3_TRACE_PORT_UNITYPRINTF
#include "unity.h"
#endif

/***************************************************************************//**
 * @addtogroup nvm3trace NVM3 Trace
 * @brief NVM3 Trace functions
 * @{
 ******************************************************************************/

/*** */
/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

// Temporary solution, shoud use NVM3_TRACE_LEVEL as well
#define NVM3_TRACE_ENABLED (NVM3_TRACE_PORT != NVM3_TRACE_PORT_NONE)

#ifdef NVM3_HOST_BUILD
#define UnityPrintf(...)      nvm3_tracePrint(NVM3_TRACE_LEVEL, __VA_ARGS__)
#define UNITY_PRINT_EOL()     nvm3_tracePrint(NVM3_TRACE_LEVEL, "\n")
#define TEST_PRINTF           UnityPrintf
#define TEST_MESSAGE          UnityPrintf
#define UNITY_OUTPUT_CHAR     UnityPrintf
#endif

#if (NVM3_TRACE_PORT == NVM3_TRACE_PORT_PRINTF)
#define nvm3_tracePrint(lev, ...) do { if (lev <= NVM3_TRACE_LEVEL) { printf(__VA_ARGS__); } } while (0)
#elif (NVM3_TRACE_PORT == NVM3_TRACE_PORT_UNITYPRINTF)
#define nvm3_tracePrint(lev, ...) do { if (lev <= NVM3_TRACE_LEVEL) { UnityPrintf(__VA_ARGS__); } } while (0)
#else
#define nvm3_tracePrint(lev, ...)
#endif

/// @endcond

/** @} (end addtogroup nvm3trace) */

#endif /* NVM3_TRACE_H */
