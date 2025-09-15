/***************************************************************************//**
 * @file
 * @brief Configuration of the Bluetooth host stack system adaptation layer
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_BT_HOST_ADAPTATION_CONFIG_H
#define SL_BT_HOST_ADAPTATION_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Bluetooth Host Adaptation Configuration

// <o SL_BT_HOST_ADAPTATION_RADIO_IRQ_PRIORITY> Radio interrupt priority in baremetal applications <1..7:1>
// <i> Default: 4
// <i> Define the ISR priority for radio interrupts in baremetal applications. The Bluetooth host stack sets this priority when the Bluetooth stack is started.
#define SL_BT_HOST_ADAPTATION_RADIO_IRQ_PRIORITY      (4)

// <o SL_BT_HOST_ADAPTATION_LINKLAYER_IRQ_PRIORITY> Linklayer interrupt priority in baremetal applications <1..7:1>
// <i> Default: 4
// <i> Define the ISR priority for linklayer interrupts in baremetal applications. The Bluetooth host stack sets this priority when the Bluetooth stack is started.
#define SL_BT_HOST_ADAPTATION_LINKLAYER_IRQ_PRIORITY  (5)

// </h> Bluetooth Host Adaptation Configuration

// <<< end of configuration section >>>

#endif // SL_BT_HOST_ADAPTATION_CONFIG_H
