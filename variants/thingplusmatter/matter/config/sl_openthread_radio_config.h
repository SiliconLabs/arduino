/*******************************************************************************
 * @file
 * @brief OpenThread radio configuration file.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef _SL_OPENTHREAD_RADIO_CONFIG_H
#define _SL_OPENTHREAD_RADIO_CONFIG_H
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h> Default EFR32 Radio Configuration

// <o SL_OPENTHREAD_RECEIVE_SENSITIVITY> Recieve Sensitivity [dBm]
#define SL_OPENTHREAD_RECEIVE_SENSITIVITY -100   // dBm
// <o SL_OPENTHREAD_RSSI_AVERAGING_TIME> RSSI Average Time [us]
#define SL_OPENTHREAD_RSSI_AVERAGING_TIME 16     // us
// <o SL_OPENTHREAD_RSSI_AVERAGING_TIMEOUT> RSSI Average Timeout [us]
#define SL_OPENTHREAD_RSSI_AVERAGING_TIMEOUT 300 // us
// </h>
// <<< end of configuration section >>>
#endif // _SL_OPENTHREAD_RADIO_CONFIG_H
