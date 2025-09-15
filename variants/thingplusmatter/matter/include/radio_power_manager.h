/*
 *  Copyright (c) 2024, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file defines the APIs required to support multi-channel feature on EFR devices.
 *
 */

#include <openthread-system.h>
#include <openthread/link.h>
#include <openthread/platform/radio.h>
#if OPENTHREAD_CONFIG_MULTIPAN_RCP_ENABLE
#include <openthread/platform/multipan.h>
#endif

#include "board_config.h"
#include "platform-efr32.h"
#include "sl_common.h"
#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/logging.hpp"
#include "utils/code_utils.h"

#define SL_INVALID_TX_POWER (127)

#if RADIO_CONFIG_2P4GHZ_OQPSK_SUPPORT
#define SL_CHANNEL_MAX OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MAX
#define SL_CHANNEL_MIN OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MIN
#define SL_MAX_CHANNELS_SUPPORTED ((OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MAX - OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MIN) + 1)
#elif RADIO_CONFIG_SUBGHZ_SUPPORT
#define SL_CHANNEL_MAX OPENTHREAD_CONFIG_PLATFORM_RADIO_PROPRIETARY_CHANNEL_MAX
#define SL_CHANNEL_MIN OPENTHREAD_CONFIG_PLATFORM_RADIO_PROPRIETARY_CHANNEL_MIN
#define SL_MAX_CHANNELS_SUPPORTED                                 \
    ((OPENTHREAD_CONFIG_PLATFORM_RADIO_PROPRIETARY_CHANNEL_MAX    \
      - OPENTHREAD_CONFIG_PLATFORM_RADIO_PROPRIETARY_CHANNEL_MIN) \
     + 1)

#elif RADIO_CONFIG_915MHZ_OQPSK_SUPPORT // Not supported
#define SL_CHANNEL_MAX OT_RADIO_915MHZ_OQPSK_CHANNEL_MAX
#define SL_CHANNEL_MIN OT_RADIO_915MHZ_OQPSK_CHANNEL_MIN
#define SL_MAX_CHANNELS_SUPPORTED ((OT_RADIO_915MHZ_OQPSK_CHANNEL_MAX - OT_RADIO_915MHZ_OQPSK_CHANNEL_MIN) + 1)
#endif

/**
 * Set the radio's max tx power for the given channel. THis value is maintained in the device
 *
 * @note This API is not available when @p OPENTHREAD_CONFIG_POWER_CALIBRATION_ENABLE is enabled.
 *
 * @param[in]   instance    The OpenThread instance structure.
 * @param[in]   channel     Radio channel of interest.
 * @param[in]   max_power   Max Radio Tx power for the given @p channel.
 *
 * @retval OT_ERROR_NONE             Successfully set the max transmit power.
 * @retval OT_ERROR_NOT_IMPLEMENTED  Transmit power configuration via dBm is not implemented.
 *
 */
otError sli_set_channel_max_tx_power(otInstance *instance, uint8_t channel, int8_t max_power);

/**
 * Set the radio tx power in RAIL
 *
 * @param[in]   tx_power    Desired tx power in dbm.
 *
 */
void sli_set_tx_power_in_rail(int8_t tx_power);

/**
 * Update the Tx power following a RAIL config change.
 *
 * @param[in]   tx_power_config    Tx power configuration
 * @param[in]   tx_power           Desired tx power in dbm.
 *
 */
void sli_update_tx_power_after_config_update(const RAIL_TxPowerConfig_t *tx_power_config, int8_t tx_power);

/**
 * Initialize the power manager.
 */
void sli_init_power_manager(void);

/**
 * Set Openthreads default power configuration for all the channels.
 *
 * @note This API is not available when @p OPENTHREAD_CONFIG_POWER_CALIBRATION_ENABLE is enabled.
 *
 * @param[in]   instance    The OpenThread instance structure.
 * @param[in]   tx_power    Desired tx power in dbm.
 *
 * @retval OT_ERROR_NONE             Successfully set the max transmit power.
 * @retval OT_ERROR_NOT_IMPLEMENTED  Transmit power configuration via dBm is not implemented.
 *
 */
otError sli_set_default_tx_power(otInstance *instance, int8_t tx_power);

/**
 * Get the operating Tx power based on all Openthread configurations.
 *
 * @note  When @p OPENTHREAD_CONFIG_POWER_CALIBRATION_ENABLE is enabled, the device reads the
 *        calibrated power values from power calibration module in Openthread.
 *
 *        When disabled, the device will look up Max Tx power table and default power configured and select
 *        the minimum of these values.
 *
 *        In multiprotocol the above operation happens for all IIDs and minimum value of those will be selected.
 *
 * @param[in]   instance    The OpenThread instance structure.
 *
 * @retval The transmit power in dBm.
 *
 */
int8_t sl_get_tx_power_for_current_channel(otInstance *instance);

/**
 * This is a callback to parse the Raw Power calibration value received from the Openthread stack. A weak
 * default implementation is provided in the PAL, which picks the 0th byte of @p raw_power_setting as the
 * operating tx power. Application can implement its own parser but the callback is expected to return the
 * radio power in dbm and the FEM configuration if any in respective output pointers.
 *
 * @note  This callback is available when @p OPENTHREAD_CONFIG_POWER_CALIBRATION_ENABLE is enabled.
 *
 * @param[in]   raw_power_calibration Pointer to raw power calibration received from Openthread stack.
 * @param[in]   raw_setting_length    Raw power calibration length.
 * @param[out]  radio_power           Pointer to parsed Radio Power in dbm (for RAIL).
 * @param[out]  fem_setting           Pointer to FEM settings.
 * @param[out]  fem_setting_length    Pointer to length of FEM settings.
 *
 * @retval OT_ERROR_NONE             Successfully set parsed theraw power calibration.
 * @retval OT_ERROR_PARSE            There was a parsing error.
 *
 */
otError sl_parse_raw_power_calibration_cb(uint8_t  *raw_power_calibration,
                                          uint16_t  raw_setting_length,
                                          int8_t   *radio_power,
                                          uint8_t  *fem_setting,
                                          uint16_t *fem_setting_length);

/**
 * This is a callback to con figure the FEM as required. A weak default implementation is provided in the PAL,
 * Application can use its own implementation to configure the FEM based on the values provided.
 *
 * @note  This callback is available when @p OPENTHREAD_CONFIG_POWER_CALIBRATION_ENABLE is enabled.
 *
 * @param[in]   fem_setting           FEM settings.
 * @param[in]   fem_setting_length    Length of FEM settings.
 *
 */
void sl_configure_fem_cb(uint8_t *fem_setting, uint16_t fem_setting_length);
