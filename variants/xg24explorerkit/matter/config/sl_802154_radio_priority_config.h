/***************************************************************************//**
 * @brief Unified radio scheduler priority configuration for 802.15.4 stacks
 *
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
#ifndef __SL_802154_RADIO_PRIORITY_CONFIG_H__
#define __SL_802154_RADIO_PRIORITY_CONFIG_H__

// <<< Use Configuration Wizard in Context Menu >>>

// <h> 802.15.4 Radio Priorities

// <o SL_802154_RADIO_PRIO_TX_MIN> TX priority minimum
// <1-255:1>
// <i> Default: 100
// <i> The 802.15.4 TX priority for the first try of a TX message
#define SL_802154_RADIO_PRIO_TX_MIN 100

// <o SL_802154_RADIO_PRIO_TX_STEP> TX priority step
// <1-50:1>
// <i> Default: 2
// <i> The delta by which the TX priority for 802.15.4 stacks decrements for each retry of the packet
#define SL_802154_RADIO_PRIO_TX_STEP 2

// <o SL_802154_RADIO_PRIO_TX_MAX> TX priority maximum
// <1-255:1>
// <i> Default: 80
// <i> The highest 802.15.4 TX priority of a message. TX priorities are elevated based on number of retries and the step value specified
#define SL_802154_RADIO_PRIO_TX_MAX 80

// <o SL_802154_RADIO_PRIO_ACTIVE_RX_VALUE> Active RX priority
// <1-255:1>
// <i> Default: 255
// <i> The 802.15.4 active RX priority. Active RX is when the device is receiving a packet
#define SL_802154_RADIO_PRIO_ACTIVE_RX_VALUE 255

// <o SL_802154_RADIO_PRIO_BACKGROUND_RX_VALUE> Background RX priority
// <1-255:1>
// <i> Default: 255
// <i> The 802.15.4 background RX priority. Background RX is the general, passive receive state when the device is not transmitting anything
#define SL_802154_RADIO_PRIO_BACKGROUND_RX_VALUE 255

// </h>

// <<< end of configuration section >>>
#endif //__SL_802154_RADIO_PRIORITY_CONFIG_H__
