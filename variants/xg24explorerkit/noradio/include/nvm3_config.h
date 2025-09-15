/***************************************************************************//**
 * @file
 * @brief NVM3 configuration file.
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
#ifndef NVM3_CONFIG_H
#define NVM3_CONFIG_H

/***************************************************************************//**
 * @addtogroup nvm3
 * @{
 ******************************************************************************/

/*** Driver instrumentation options */
#define NVM3_TRACE_PORT_NONE               0               // Nothing is printed
#define NVM3_TRACE_PORT_PRINTF             1               // Print is available
#define NVM3_TRACE_PORT_UNITYPRINTF        2               // Unity print is available

#define NVM3_TRACE_PORT                    NVM3_TRACE_PORT_NONE

/*** Event level
     0 Critical: Trace only critical events
     1 Warning : Trace warning events and above
     2 Info    : Trace info events and above
 */
#define NVM3_TRACE_LEVEL_ERROR             0
#define NVM3_TRACE_LEVEL_WARNING           1
#define NVM3_TRACE_LEVEL_INFO              2
#define NVM3_TRACE_LEVEL_LOW               3

#define NVM3_TRACE_LEVEL                   NVM3_TRACE_LEVEL_WARNING

#define NVM3_ASSERT_ON_ERROR               false

/** @} (end addtogroup nvm3) */

#endif /* NVM3_CONFIG_H */
