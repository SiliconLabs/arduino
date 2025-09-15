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

#ifndef SL_BTCTRL_CONFIG_H
#define SL_BTCTRL_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Bluetooth Controller Configuration

// <o SL_BT_CONTROLLER_BUFFER_MEMORY> Bluetooth Controller Buffer Memory
// <i> Default: SL_BT_CONTROLLER_BUFFER_MEMORY
// <i> Define the Amount of memory to allocate for tx/rx buffers in Bluetooth Controller
#define SL_BT_CONTROLLER_BUFFER_MEMORY     (8192)

// <o SL_BT_CONTROLLER_LE_BUFFER_SIZE_MAX> Bluetooth Controller ACL data packets that can be stored
// <i> Default: SL_BT_CONTROLLER_LE_BUFFER_SIZE_MAX
// <i> Define the total number of the maximum sized ACL data packets that can be received from the host
#define SL_BT_CONTROLLER_LE_BUFFER_SIZE_MAX     (3)

// <o SL_BT_CONTROLLER_COMPLETED_PACKETS_THRESHOLD> Total transmitted packets threshold for all connections to send the Number Of Completed Packets HCI event to the host <1-255>
// <i> Default: 4
// <i> Define the number of transmitted air interface ACL packets to trigger the Number Of Completed Packets HCI event.
#define SL_BT_CONTROLLER_COMPLETED_PACKETS_THRESHOLD     (4)

// <o SL_BT_CONTROLLER_COMPLETED_PACKETS_EVENTS_TIMEOUT> Number of connection events to send the Number Of Completed Packets HCI event to the host <1-255>
// <i> Default: 3
// <i> Define the maximum number of connection events since the previous Number Of Completed Packets HCI event to trigger reporting of any unreported completed ACL packets.
#define SL_BT_CONTROLLER_COMPLETED_PACKETS_EVENTS_TIMEOUT     (3)

// <o SL_BT_CONFIG_MAX_QUEUED_ADV_REPORTS> Maximum number of queued advertisement reports <1-255>
// <i> Default: 10
// <i> Define the maximum number of queued advertisement reports. Additional advertisement reports are dropped.
#define SL_BT_CONFIG_MAX_QUEUED_ADV_REPORTS     (10)

// </h> Bluetooth Controller Configuration

// <<< end of configuration section >>>

#endif // SL_BTCTRL_CONFIG_H
