/***************************************************************************//**
 * @brief SL_BT_API command declarations
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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


#ifndef SL_BT_API_H
#define SL_BT_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sl_status.h"
#include "sl_bgapi.h"
#include "sl_bt_api_compatibility.h"


/* SL_BT_ synonyms for BGAPI header macros */
#define SL_BT_MSG_ID(HDR)        SL_BGAPI_MSG_ID(HDR)
#define SL_BT_MSG_HEADER_LEN     SL_BGAPI_MSG_HEADER_LEN
#define SL_BT_MSG_LEN(HDR)       SL_BGAPI_MSG_LEN(HDR)
#define SL_BT_BIT_ENCRYPTED      SL_BGAPI_BIT_ENCRYPTED
#define SL_BT_MSG_ENCRYPTED(HDR) SL_BGAPI_MSG_ENCRYPTED(HDR)

/**
 * @addtogroup bluetooth_event_system
 * @{
 *
 * @addtogroup sl_bt_event_masks BT Event System Event Masks
 * @{
 *
 * @brief Event Mask values used with Event System
 *
 * When the component `bluetooth_event_system_ipc` is included in the
 * application, the Bluetooth host stack events are published using the Event
 * System provided by the `event_system` component. The constants in this group
 * define the event masks used for Bluetooth host stack events.
 */

/**
 * @brief Event mask bit set in all public Bluetooth host events
 */
#define SL_BT_EVENT_MASK_PUBLIC ((uint32_t) 0x01)

/** @} */ // end addtogroup bluetooth_event_system

/** @} */ // end addtogroup sl_bt_event_masks

/**
 * @addtogroup sl_bt_common_types BT Common Types
 * @{
 *
 * @brief BT common types
 */

/**
 * @brief Value used to indicate an invalid connection handle
 */
#define SL_BT_INVALID_CONNECTION_HANDLE ((uint8_t) 0xFF)

/**
 * @brief Value used to indicate an invalid bonding handle
 */
#define SL_BT_INVALID_BONDING_HANDLE ((uint8_t) 0xFF)

/**
 * @brief Value used to indicate an invalid advertising set handle
 */
#define SL_BT_INVALID_ADVERTISING_SET_HANDLE ((uint8_t) 0xFF)

/**
 * @brief Value used to indicate an invalid sync handle
 */
#define SL_BT_INVALID_SYNC_HANDLE ((uint16_t) 0xFFFF)

/**
 * @brief Value used to indicate an invalid connection analyzer handle
 */
#define SL_BT_INVALID_CONNECTION_ANALYZER_HANDLE ((uint8_t) 0xFF)

/** @} */ // end addtogroup sl_bt_common_types


/**
 * @brief CS subevent length
 */
typedef struct {
  uint8_t data[3]; /**< CS subevent length */
} sl_bt_cs_subevent_length_t;

/**
 * @brief DRBG key
 */
typedef struct {
  uint8_t data[16]; /**< DRBG key */
} sl_bt_drbg_key_t;

/**
 * @brief 79 1-bit fields containing the values of the CS channel index
 */
typedef struct {
  uint8_t data[10]; /**< 79 1-bit fields containing the values of the CS channel index */
} sl_bt_cs_channel_map_t;

/**
 * @brief 37 1-bit fields containing the values of the link layer channel index on a Bluetooth connection
 */
typedef struct {
  uint8_t data[5]; /**< 37 1-bit fields containing the values of the link layer channel index on a Bluetooth connection */
} sl_bt_connection_channel_map_t;



/**
 * @addtogroup sl_bt_dfu Device Firmware Update
 * @{
 *
 * @brief Device Firmware Update
 *
 * These commands and events are related to controlling firmware updates over
 * the configured host interface and are available only when the device is
 * booted in DFU mode. <b>DFU process:</b>
 *   1. Boot device to DFU mode with the Bootloader interface
 *   2. Wait for @ref sl_bt_evt_dfu_boot event
 *   3. Send command @ref sl_bt_dfu_flash_set_address command to start the
 *      firmware update
 *   4. Upload the firmware with @ref sl_bt_dfu_flash_upload commands until all
 *      data is uploaded
 *   5. Send @ref sl_bt_dfu_flash_upload_finish command when all data is
 *      uploaded
 *   6. Finalize DFU firmware update with @ref sl_bt_system_reboot command
 *
 * DFU mode is using the UART baudrate set in bootloader.
 */

/* Command and Response IDs */
#define sl_bt_cmd_dfu_flash_set_address_id                           0x01000020
#define sl_bt_cmd_dfu_flash_upload_id                                0x02000020
#define sl_bt_cmd_dfu_flash_upload_finish_id                         0x03000020
#define sl_bt_rsp_dfu_flash_set_address_id                           0x01000020
#define sl_bt_rsp_dfu_flash_upload_id                                0x02000020
#define sl_bt_rsp_dfu_flash_upload_finish_id                         0x03000020

/**
 * @addtogroup sl_bt_evt_dfu_boot sl_bt_evt_dfu_boot
 * @{
 * @brief This event indicates that the device booted in DFU mode and is now
 * ready to receive commands related to device firmware upgrade (DFU).
 */

/** @brief Identifier of the boot event */
#define sl_bt_evt_dfu_boot_id                                        0x000000a0

/***************************************************************************//**
 * @brief Data structure of the boot event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_dfu_boot_s
{
  uint32_t version; /**< The version of the bootloader */
});

typedef struct sl_bt_evt_dfu_boot_s sl_bt_evt_dfu_boot_t;

/** @} */ // end addtogroup sl_bt_evt_dfu_boot

/**
 * @addtogroup sl_bt_evt_dfu_boot_failure sl_bt_evt_dfu_boot_failure
 * @{
 * @brief This event indicates that an error, which prevents the device from
 * booting, has occurred in bootloader.
 */

/** @brief Identifier of the boot_failure event */
#define sl_bt_evt_dfu_boot_failure_id                                0x010000a0

/***************************************************************************//**
 * @brief Data structure of the boot_failure event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_dfu_boot_failure_s
{
  uint16_t reason; /**< An sl_status_t code describing the error that occurred */
});

typedef struct sl_bt_evt_dfu_boot_failure_s sl_bt_evt_dfu_boot_failure_t;

/** @} */ // end addtogroup sl_bt_evt_dfu_boot_failure

/***************************************************************************//**
 *
 * After re-booting the local device in DFU mode, this command defines the
 * starting address on the flash where the new firmware will be written.
 *
 * @param[in] address The offset in the flash where the new firmware is uploaded
 *   to. Always use the value 0x00000000.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_dfu_flash_set_address(uint32_t address);

/***************************************************************************//**
 *
 * Upload the whole firmware image file into the Bluetooth device. The passed
 * data length must be a multiple of 4 bytes. Because the BGAPI command payload
 * size is limited, multiple commands need to be issued one after the other
 * until the whole firmware image file is uploaded to the device. After each
 * command, the next address of the flash sector in memory to write to is
 * automatically updated by the bootloader.
 *
 * @param[in] data_len Length of data in @p data
 * @param[in] data An array of data which will be written onto the flash.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_dfu_flash_upload(size_t data_len, const uint8_t* data);

/***************************************************************************//**
 *
 * Inform the device that the DFU file is fully uploaded. To return the device
 * back to user application mode, issue command @ref sl_bt_system_reboot.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_dfu_flash_upload_finish();

/** @} */ // end addtogroup sl_bt_dfu

/**
 * @addtogroup sl_bt_system System
 * @{
 *
 * @brief System
 *
 * Commands and events in this class can be used to access and query the local
 * device.
 */

/* Command and Response IDs */
#define sl_bt_cmd_system_hello_id                                    0x00010020
#define sl_bt_cmd_system_start_bluetooth_id                          0x1c010020
#define sl_bt_cmd_system_stop_bluetooth_id                           0x1d010020
#define sl_bt_cmd_system_forcefully_stop_bluetooth_id                0x1e010020
#define sl_bt_cmd_system_get_version_id                              0x1b010020
#define sl_bt_cmd_system_reboot_id                                   0x1f010020
#define sl_bt_cmd_system_halt_id                                     0x0c010020
#define sl_bt_cmd_system_linklayer_configure_id                      0x0e010020
#define sl_bt_cmd_system_set_tx_power_id                             0x17010020
#define sl_bt_cmd_system_get_tx_power_setting_id                     0x18010020
#define sl_bt_cmd_system_set_identity_address_id                     0x13010020
#define sl_bt_cmd_system_get_identity_address_id                     0x15010020
#define sl_bt_cmd_system_get_random_data_id                          0x0b010020
#define sl_bt_cmd_system_data_buffer_write_id                        0x12010020
#define sl_bt_cmd_system_data_buffer_clear_id                        0x14010020
#define sl_bt_cmd_system_get_counters_id                             0x0f010020
#define sl_bt_cmd_system_set_lazy_soft_timer_id                      0x1a010020
#define sl_bt_cmd_system_reset_id                                    0x01010020
#define sl_bt_rsp_system_hello_id                                    0x00010020
#define sl_bt_rsp_system_start_bluetooth_id                          0x1c010020
#define sl_bt_rsp_system_stop_bluetooth_id                           0x1d010020
#define sl_bt_rsp_system_forcefully_stop_bluetooth_id                0x1e010020
#define sl_bt_rsp_system_get_version_id                              0x1b010020
#define sl_bt_rsp_system_reboot_id                                   0x1f010020
#define sl_bt_rsp_system_halt_id                                     0x0c010020
#define sl_bt_rsp_system_linklayer_configure_id                      0x0e010020
#define sl_bt_rsp_system_set_tx_power_id                             0x17010020
#define sl_bt_rsp_system_get_tx_power_setting_id                     0x18010020
#define sl_bt_rsp_system_set_identity_address_id                     0x13010020
#define sl_bt_rsp_system_get_identity_address_id                     0x15010020
#define sl_bt_rsp_system_get_random_data_id                          0x0b010020
#define sl_bt_rsp_system_data_buffer_write_id                        0x12010020
#define sl_bt_rsp_system_data_buffer_clear_id                        0x14010020
#define sl_bt_rsp_system_get_counters_id                             0x0f010020
#define sl_bt_rsp_system_set_lazy_soft_timer_id                      0x1a010020
#define sl_bt_rsp_system_reset_id                                    0x01010020

/**
 * @brief These Keys are used to configure Link Layer Operation
 */
typedef enum
{
  sl_bt_system_linklayer_config_key_halt                                   = 0x1,  /**<
                                                                                        (0x1)
                                                                                        Same
                                                                                        as
                                                                                        system_halt
                                                                                        command,
                                                                                        value-0
                                                                                        Stop
                                                                                        Radio
                                                                                        1-
                                                                                        Start
                                                                                        Radio */
  sl_bt_system_linklayer_config_key_priority_range                         = 0x2,  /**<
                                                                                        (0x2)
                                                                                        Sets
                                                                                        the
                                                                                        RAIL
                                                                                        priority_mapping
                                                                                        offset
                                                                                        field
                                                                                        of
                                                                                        the
                                                                                        link
                                                                                        layer
                                                                                        priority
                                                                                        configuration
                                                                                        structure
                                                                                        to
                                                                                        the
                                                                                        first
                                                                                        byte
                                                                                        of
                                                                                        the
                                                                                        value
                                                                                        field. */
  sl_bt_system_linklayer_config_key_scan_channels                          = 0x3,  /**<
                                                                                        (0x3)
                                                                                        Sets
                                                                                        channels
                                                                                        to
                                                                                        scan
                                                                                        on.
                                                                                        The
                                                                                        first
                                                                                        byte
                                                                                        of
                                                                                        the
                                                                                        value
                                                                                        is
                                                                                        the
                                                                                        channel
                                                                                        map.
                                                                                        0x1
                                                                                        =
                                                                                        Channel
                                                                                        37,
                                                                                        0x2
                                                                                        =
                                                                                        Channel
                                                                                        38,
                                                                                        0x4
                                                                                        =
                                                                                        Channel
                                                                                        39 */
  sl_bt_system_linklayer_config_key_set_flags                              = 0x4,  /**<
                                                                                        (0x4)
                                                                                        Sets
                                                                                        the
                                                                                        link
                                                                                        layer
                                                                                        configuration
                                                                                        flags.
                                                                                        The
                                                                                        value
                                                                                        is
                                                                                        a
                                                                                        little
                                                                                        endian
                                                                                        32-bit
                                                                                        integer.
                                                                                        Flag
                                                                                        Values:
                                                                                          - 0x00000001
                                                                                            \-
                                                                                            Disable
                                                                                            Feature
                                                                                            Exchange
                                                                                            in
                                                                                            peripheral
                                                                                            role
                                                                                            of
                                                                                            the
                                                                                            connection
                                                                                          - 0x00000002
                                                                                            \-
                                                                                            Disable
                                                                                            Feature
                                                                                            Exchange
                                                                                            in
                                                                                            central
                                                                                            role
                                                                                            of
                                                                                            the
                                                                                            connection */
  sl_bt_system_linklayer_config_key_clr_flags                              = 0x5,  /**<
                                                                                        (0x5)
                                                                                        The
                                                                                        value
                                                                                        is
                                                                                        flags
                                                                                        to
                                                                                        clear.
                                                                                        Flags
                                                                                        are
                                                                                        the
                                                                                        same
                                                                                        as
                                                                                        in
                                                                                        SET_FLAGS
                                                                                        command. */
  sl_bt_system_linklayer_config_key_set_afh_interval                       = 0x7,  /**<
                                                                                        (0x7)
                                                                                        Set
                                                                                        the
                                                                                        afh_scan_interval.
                                                                                        Value
                                                                                        is
                                                                                        in
                                                                                        units
                                                                                        of
                                                                                        10
                                                                                        ms.
                                                                                        Setting
                                                                                        the
                                                                                        interval
                                                                                        to
                                                                                        0
                                                                                        will
                                                                                        result
                                                                                        in
                                                                                        using
                                                                                        the
                                                                                        default
                                                                                        value
                                                                                        of
                                                                                        1
                                                                                        second. */
  sl_bt_system_linklayer_config_key_set_periodic_advertising_status_report = 0x8,  /**<
                                                                                        (0x8)
                                                                                        Enable
                                                                                        or
                                                                                        disable
                                                                                        the
                                                                                        status
                                                                                        report
                                                                                        of
                                                                                        periodic
                                                                                        advertising
                                                                                        on
                                                                                        an
                                                                                        advertising
                                                                                        set.
                                                                                        When
                                                                                        enabled,
                                                                                        event
                                                                                        @ref
                                                                                        sl_bt_evt_periodic_advertiser_status
                                                                                        will
                                                                                        be
                                                                                        generated
                                                                                        for
                                                                                        each
                                                                                        scheduled
                                                                                        transmission.
                                                                                        Value:
                                                                                        2
                                                                                        bytes
                                                                                          - Byte
                                                                                            1
                                                                                            \-
                                                                                            The
                                                                                            advertising
                                                                                            set
                                                                                            handle
                                                                                          - Byte
                                                                                            2
                                                                                            \-
                                                                                            The
                                                                                            status
                                                                                            report
                                                                                            state
                                                                                              - 0:
                                                                                                Disable
                                                                                                the
                                                                                                status
                                                                                                report
                                                                                              - 1:
                                                                                                Enable
                                                                                                the
                                                                                                status
                                                                                                report


                                                                                        Default:
                                                                                        The
                                                                                        status
                                                                                        report
                                                                                        on
                                                                                        any
                                                                                        advertising
                                                                                        sets
                                                                                        is
                                                                                        disabled. */
  sl_bt_system_linklayer_config_key_set_priority_table                     = 0x9,  /**<
                                                                                        (0x9)
                                                                                        The
                                                                                        value
                                                                                        contains
                                                                                        a
                                                                                        priority
                                                                                        table
                                                                                        to
                                                                                        be
                                                                                        copied
                                                                                        over
                                                                                        the
                                                                                        existing
                                                                                        table.
                                                                                        If
                                                                                        the
                                                                                        value
                                                                                        is
                                                                                        smaller
                                                                                        than
                                                                                        the
                                                                                        full
                                                                                        table,
                                                                                        only
                                                                                        those
                                                                                        values
                                                                                        are
                                                                                        updated.
                                                                                        See
                                                                                        sl_bt_bluetooth_ll_priorities
                                                                                        struct
                                                                                        for
                                                                                        the
                                                                                        definition
                                                                                        of
                                                                                        a
                                                                                        priority
                                                                                        table. */
  sl_bt_system_linklayer_config_key_set_rx_packet_filtering                = 0xa,  /**<
                                                                                        (0xa)
                                                                                        Configure
                                                                                        and
                                                                                        enable
                                                                                        or
                                                                                        disable
                                                                                        RX
                                                                                        packet
                                                                                        filtering
                                                                                        feature.
                                                                                        Value:
                                                                                        >=
                                                                                        5
                                                                                        bytes.
                                                                                          - Byte
                                                                                            1
                                                                                            \-
                                                                                            The
                                                                                            filter
                                                                                            count
                                                                                          - Byte
                                                                                            2
                                                                                            \-
                                                                                            The
                                                                                            filter
                                                                                            offset
                                                                                          - Byte
                                                                                            3
                                                                                            \-
                                                                                            The
                                                                                            length
                                                                                            of
                                                                                            the
                                                                                            filter
                                                                                            list
                                                                                          - Byte
                                                                                            4
                                                                                            \-
                                                                                            The
                                                                                            bitmask
                                                                                            flags
                                                                                          - Rest
                                                                                            of
                                                                                            the
                                                                                            data
                                                                                            \-
                                                                                            The
                                                                                            filter
                                                                                            list */
  sl_bt_system_linklayer_config_key_set_simultaneous_scanning              = 0xb,  /**<
                                                                                        (0xb)
                                                                                        Enable
                                                                                        or
                                                                                        disable
                                                                                        simultaneous
                                                                                        scanning
                                                                                        on
                                                                                        the
                                                                                        1M
                                                                                        and
                                                                                        Coded
                                                                                        PHYs.
                                                                                        Value:
                                                                                        1
                                                                                        byte.
                                                                                          - 0
                                                                                            \-
                                                                                            Disable
                                                                                            simultaneous
                                                                                            scanning.
                                                                                          - 1
                                                                                            \-
                                                                                            Enable
                                                                                            simultaneous
                                                                                            scanning. */
  sl_bt_system_linklayer_config_key_set_channelmap_flags                   = 0xc,  /**<
                                                                                        (0xc)
                                                                                        Configure
                                                                                        channelmap
                                                                                        adaptivity
                                                                                        flags.
                                                                                        Value:
                                                                                        4
                                                                                        bytes.
                                                                                          - 0
                                                                                            \-
                                                                                            Disable
                                                                                            the
                                                                                            adaptivity
                                                                                            in
                                                                                            the
                                                                                            AFH
                                                                                            feature.
                                                                                          - 1
                                                                                            \-
                                                                                            Enable
                                                                                            the
                                                                                            adaptivity
                                                                                            in
                                                                                            the
                                                                                            AFH
                                                                                            feature. */
  sl_bt_system_linklayer_config_key_low_power_mode_power_limit             = 0xd,  /**<
                                                                                        (0xd)
                                                                                        Set
                                                                                        power
                                                                                        limits
                                                                                        for
                                                                                        the
                                                                                        low
                                                                                        power
                                                                                        mode
                                                                                        where
                                                                                        the
                                                                                        frequency
                                                                                        adaptivity
                                                                                        is
                                                                                        disabled
                                                                                        or
                                                                                        the
                                                                                        current
                                                                                        channel
                                                                                        mapping
                                                                                        with
                                                                                        adaptivity
                                                                                        enabled
                                                                                        does
                                                                                        not
                                                                                        allow
                                                                                        to
                                                                                        use
                                                                                        a
                                                                                        high
                                                                                        transmission
                                                                                        power.
                                                                                        A
                                                                                        limit
                                                                                        value
                                                                                        must
                                                                                        not
                                                                                        exceed
                                                                                        the
                                                                                        maximum
                                                                                        that
                                                                                        the
                                                                                        regulations
                                                                                        in
                                                                                        the
                                                                                        region
                                                                                        of
                                                                                        use
                                                                                        allow.
                                                                                        The
                                                                                        value
                                                                                        is
                                                                                        an
                                                                                        8-bytes
                                                                                        long
                                                                                        array
                                                                                        consisting
                                                                                        of
                                                                                        4
                                                                                        int16_t
                                                                                        items
                                                                                        specifying
                                                                                        the
                                                                                        power
                                                                                        limits
                                                                                        in
                                                                                        0.1
                                                                                        dBm
                                                                                        unit
                                                                                        for
                                                                                        different
                                                                                        PHYs
                                                                                        in
                                                                                        the
                                                                                        following
                                                                                        order,
                                                                                          - Item
                                                                                            1
                                                                                            \-
                                                                                            The
                                                                                            power
                                                                                            limit
                                                                                            for
                                                                                            1M
                                                                                            PHY
                                                                                          - Item
                                                                                            2
                                                                                            \-
                                                                                            The
                                                                                            power
                                                                                            limit
                                                                                            for
                                                                                            2M
                                                                                            PHY
                                                                                          - Item
                                                                                            3
                                                                                            \-
                                                                                            The
                                                                                            power
                                                                                            limit
                                                                                            for
                                                                                            Coded
                                                                                            PHY
                                                                                            S=8
                                                                                          - Item
                                                                                            4
                                                                                            \-
                                                                                            The
                                                                                            power
                                                                                            limit
                                                                                            for
                                                                                            Coded
                                                                                            PHY
                                                                                            S=2 */
  sl_bt_system_linklayer_config_key_power_control_golden_range             = 0x10, /**<
                                                                                        (0x10)
                                                                                        Set
                                                                                        Power
                                                                                        Control
                                                                                        golden
                                                                                        range
                                                                                        parameters.
                                                                                        The
                                                                                        value
                                                                                        is
                                                                                        a
                                                                                        8-bytes
                                                                                        long
                                                                                        array
                                                                                        that
                                                                                        consists
                                                                                        of
                                                                                        4
                                                                                        pairs
                                                                                        of
                                                                                        golden
                                                                                        range
                                                                                        configurations.
                                                                                        In
                                                                                        each
                                                                                        pair,
                                                                                        the
                                                                                        first
                                                                                        byte
                                                                                        is
                                                                                        the
                                                                                        lower
                                                                                        RSSI
                                                                                        boundary
                                                                                        and
                                                                                        the
                                                                                        second
                                                                                        byte
                                                                                        is
                                                                                        the
                                                                                        upper
                                                                                        RSSI
                                                                                        boundary.
                                                                                        RSSI
                                                                                        values
                                                                                        are
                                                                                        in
                                                                                        dBm.
                                                                                        This
                                                                                        configuration
                                                                                        is
                                                                                        not
                                                                                        allowed
                                                                                        if
                                                                                        there
                                                                                        are
                                                                                        active
                                                                                        Bluetooth
                                                                                        connections.
                                                                                          - Byte
                                                                                            1
                                                                                            \-
                                                                                            Minimal
                                                                                            RSSI
                                                                                            on
                                                                                            1M
                                                                                            PHY
                                                                                          - Byte
                                                                                            2
                                                                                            \-
                                                                                            Maximal
                                                                                            RSSI
                                                                                            on
                                                                                            1M
                                                                                            PHY
                                                                                          - Byte
                                                                                            3
                                                                                            \-
                                                                                            Minimal
                                                                                            RSSI
                                                                                            on
                                                                                            2M
                                                                                            PHY
                                                                                          - Byte
                                                                                            4
                                                                                            \-
                                                                                            Maximal
                                                                                            RSSI
                                                                                            on
                                                                                            2M
                                                                                            PHY
                                                                                          - Byte
                                                                                            5
                                                                                            \-
                                                                                            Minimal
                                                                                            RSSI
                                                                                            on
                                                                                            Coded
                                                                                            PHY
                                                                                            S=8
                                                                                          - Byte
                                                                                            6
                                                                                            \-
                                                                                            Maximal
                                                                                            RSSI
                                                                                            on
                                                                                            Coded
                                                                                            PHY
                                                                                            S=8
                                                                                          - Byte
                                                                                            7
                                                                                            \-
                                                                                            Minimal
                                                                                            RSSI
                                                                                            on
                                                                                            Coded
                                                                                            PHY
                                                                                            S=2
                                                                                          - Byte
                                                                                            8
                                                                                            \-
                                                                                            Maximal
                                                                                            RSSI
                                                                                            on
                                                                                            Coded
                                                                                            PHY
                                                                                            S=2 */
  sl_bt_system_linklayer_config_key_active_scanner_backoff_upper_limit     = 0x11, /**<
                                                                                        (0x11)
                                                                                        Value:
                                                                                        uint16_t
                                                                                        Adjust
                                                                                        upper
                                                                                        limit
                                                                                        for
                                                                                        backoff
                                                                                        counter.
                                                                                        If
                                                                                        0
                                                                                        restores
                                                                                        default
                                                                                        value
                                                                                        of
                                                                                        256
                                                                                        Value
                                                                                        must
                                                                                        be
                                                                                        between
                                                                                        16
                                                                                        \-
                                                                                        256 */
  sl_bt_system_linklayer_config_key_afh_rssi_threshold                     = 0x12, /**<
                                                                                        (0x12)
                                                                                        Value:
                                                                                        int8_t
                                                                                        Configures
                                                                                        RSSI
                                                                                        limit
                                                                                        for
                                                                                        AFH
                                                                                        channel
                                                                                        blocking */
  sl_bt_system_linklayer_config_key_afh_channel_cooldown                   = 0x13, /**<
                                                                                        (0x13)
                                                                                        Value:
                                                                                        int16_t
                                                                                        Configures
                                                                                        how
                                                                                        long
                                                                                        channel
                                                                                        is
                                                                                        blocked
                                                                                        after
                                                                                        activity
                                                                                        is
                                                                                        detected
                                                                                        Default:
                                                                                        8000 */
  sl_bt_system_linklayer_config_key_set_report_all_scan_rsps               = 0x14  /**<
                                                                                        (0x14)
                                                                                        Value:
                                                                                        uint8_t
                                                                                        0
                                                                                        \-
                                                                                        default,
                                                                                        only
                                                                                        reports
                                                                                        scan
                                                                                        responses
                                                                                        that
                                                                                        is
                                                                                        received
                                                                                        after
                                                                                        sending
                                                                                        scan_req
                                                                                        nonzero
                                                                                        \-
                                                                                        Will
                                                                                        report
                                                                                        all
                                                                                        scan
                                                                                        responses
                                                                                        that
                                                                                        are
                                                                                        received
                                                                                        on
                                                                                        primary
                                                                                        advertising
                                                                                        channels */
} sl_bt_system_linklayer_config_key_t;

/**
 * @addtogroup sl_bt_evt_system_boot sl_bt_evt_system_boot
 * @{
 * @brief Indicates that the device has started and the radio is ready
 *
 * This event carries the firmware build number and other software and hardware
 * identification codes.
 */

/** @brief Identifier of the boot event */
#define sl_bt_evt_system_boot_id                                     0x000100a0

/***************************************************************************//**
 * @brief Data structure of the boot event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_system_boot_s
{
  uint16_t major;      /**< Major release version */
  uint16_t minor;      /**< Minor release version */
  uint16_t patch;      /**< Patch release number */
  uint16_t build;      /**< Build number */
  uint32_t bootloader; /**< Bootloader version if a bootloader is present in the
                            application. Set to 0 if bootloader is not present. */
  uint16_t hw;         /**< Hardware type: the major chip revision number in the
                            most significant byte and the minor revision in the
                            least significant byte */
  uint32_t hash;       /**< Version hash */
});

typedef struct sl_bt_evt_system_boot_s sl_bt_evt_system_boot_t;

/** @} */ // end addtogroup sl_bt_evt_system_boot

/**
 * @addtogroup sl_bt_evt_system_stopped sl_bt_evt_system_stopped
 * @{
 * @brief When the Bluetooth on-demand start component is included in the
 * application build, this event is triggered when stopping the Bluetooth stack
 * using commands @ref sl_bt_system_stop_bluetooth or @ref
 * sl_bt_system_forcefully_stop_bluetooth has completed.
 */

/** @brief Identifier of the stopped event */
#define sl_bt_evt_system_stopped_id                                  0x010100a0

/** @} */ // end addtogroup sl_bt_evt_system_stopped

/**
 * @addtogroup sl_bt_evt_system_error sl_bt_evt_system_error
 * @{
 * @brief Indicates that an error has occurred
 *
 * See error codes table for more information.
 */

/** @brief Identifier of the error event */
#define sl_bt_evt_system_error_id                                    0x060100a0

/***************************************************************************//**
 * @brief Data structure of the error event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_system_error_s
{
  uint16_t   reason; /**< SL_STATUS_OK if successful. Error code otherwise. */
  uint8array data;   /**< Data related to the error; this field can be empty. */
});

typedef struct sl_bt_evt_system_error_s sl_bt_evt_system_error_t;

/** @} */ // end addtogroup sl_bt_evt_system_error

/**
 * @addtogroup sl_bt_evt_system_resource_exhausted sl_bt_evt_system_resource_exhausted
 * @{
 * @brief Indicates that a system resource has been exhausted during the
 * operation of the Bluetooth stack
 *
 * If resource errors occur already when the Bluetooth stack is starting, the
 * error is reported directly as a return value from sl_bt_stack_init (when the
 * Bluetooth on-demand start component is not included in the application
 * build), or from @ref sl_bt_system_start_bluetooth (when the on-demand start
 * component is included). The fields of this event indicate how many failures
 * have occurred for a specific resource. If further resource failures occur
 * while this event is already queued in the BGAPI event queue but not yet
 * delivered to the application, the new failures are included in the already
 * queued event. When the application receives this event, the fields represent
 * the number of failures that have occurred since the previous @ref
 * sl_bt_evt_system_resource_exhausted event.
 */

/** @brief Identifier of the resource_exhausted event */
#define sl_bt_evt_system_resource_exhausted_id                       0x080100a0

/***************************************************************************//**
 * @brief Data structure of the resource_exhausted event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_system_resource_exhausted_s
{
  uint8_t num_buffers_discarded;           /**< The system has temporarily run
                                                out of the pre-allocated data
                                                buffers that are allocated based
                                                on SL_BT_CONFIG_BUFFER_SIZE
                                                configuration and some
                                                expendable data or event had to
                                                be discarded to satisfy a
                                                non-expendble buffer allocation.
                                                A typical case is discarding
                                                scan reports when a large inflow
                                                of scan reports exceeds the
                                                speed at which the application
                                                drains the BGAPI event queue. */
  uint8_t num_buffer_allocation_failures;  /**< The system has run out of the
                                                pre-allocated data buffers that
                                                are allocated based on
                                                SL_BT_CONFIG_BUFFER_SIZE
                                                configuration and a buffer
                                                allocation has failed. */
  uint8_t num_heap_allocation_failures;    /**< The Bluetooth stack has failed
                                                to make an allocation from the
                                                heap. Note that only allocations
                                                made by the Bluetooth stack are
                                                detected and reported by this
                                                field. Allocation failures in
                                                other components that use
                                                sl_malloc() or malloc() are not
                                                included in this count. */
  uint8_t num_message_allocation_failures; /**< The system has run out of
                                                internal pre-allocated message
                                                items and the creation of an
                                                internal message has failed. */
});

typedef struct sl_bt_evt_system_resource_exhausted_s sl_bt_evt_system_resource_exhausted_t;

/** @} */ // end addtogroup sl_bt_evt_system_resource_exhausted

/**
 * @addtogroup sl_bt_evt_system_external_signal sl_bt_evt_system_external_signal
 * @{
 * @brief Indicates that the external signals have been received
 *
 * External signals are generated from the native application.
 */

/** @brief Identifier of the external_signal event */
#define sl_bt_evt_system_external_signal_id                          0x030100a0

/***************************************************************************//**
 * @brief Data structure of the external_signal event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_system_external_signal_s
{
  uint32_t extsignals; /**< Bitmask of external signals received since last
                            event. */
});

typedef struct sl_bt_evt_system_external_signal_s sl_bt_evt_system_external_signal_t;

/** @} */ // end addtogroup sl_bt_evt_system_external_signal

/**
 * @addtogroup sl_bt_evt_system_awake sl_bt_evt_system_awake
 * @{
 * @brief Indicates that the device is awake and no longer in sleep mode
 *
 * <b>NOTE:</b> Stack does not generate this event by itself because sleep and
 * wakeup are managed by applications. If this event is needed, call function
 * @ref sl_bt_send_system_awake, which signals the stack to send the event.
 */

/** @brief Identifier of the awake event */
#define sl_bt_evt_system_awake_id                                    0x040100a0

/** @} */ // end addtogroup sl_bt_evt_system_awake

/**
 * @addtogroup sl_bt_evt_system_soft_timer sl_bt_evt_system_soft_timer
 * @{
 * @brief Indicates that a soft timer has lapsed.
 */

/** @brief Identifier of the soft_timer event */
#define sl_bt_evt_system_soft_timer_id                               0x070100a0

/***************************************************************************//**
 * @brief Data structure of the soft_timer event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_system_soft_timer_s
{
  uint8_t handle; /**< Timer Handle */
});

typedef struct sl_bt_evt_system_soft_timer_s sl_bt_evt_system_soft_timer_t;

/** @} */ // end addtogroup sl_bt_evt_system_soft_timer

/***************************************************************************//**
 *
 * Verify whether the communication between the host and the device is
 * functional.
 *
 * <b>NOTE:</b> This command is available even if the Bluetooth stack has not
 * been started. See @ref sl_bt_system_start_bluetooth for description of how
 * the Bluetooth stack is started.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_hello();

/***************************************************************************//**
 *
 * If the Bluetooth on-demand start component is not included in the application
 * build, the Bluetooth stack is automatically started when the device boots up.
 * In this configuration, the on-demand start command is not available and the
 * command returns the error SL_STATUS_NOT_AVAILABLE.
 *
 * When the Bluetooth on-demand start component is included in the application
 * build, this command is used by the application to request starting the
 * Bluetooth stack when the application needs it. If the command returns a
 * success result, the stack starts to asynchronously allocate the resources and
 * configure the Bluetooth stack based on the configuration passed at
 * initialization time.
 *
 * The Bluetooth stack cannot be restarted while it's still stopping after
 * issuing the command @ref sl_bt_system_stop_bluetooth. If @ref
 * sl_bt_system_start_bluetooth is called when stopping is still on-going the
 * command returns the error SL_STATUS_INVALID_STATE. The application must wait
 * for the @ref sl_bt_evt_system_stopped event before attempting to restart the
 * stack.
 *
 * Successful start of the stack is indicated by the @ref sl_bt_evt_system_boot
 * event. The configured classes and Bluetooth stack features are available
 * after the application has received the @ref sl_bt_evt_system_boot event. If
 * starting the Bluetooth stack fails, the error is indicated to the application
 * with the @ref sl_bt_evt_system_error event.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_system_boot - Triggered when the Bluetooth stack has
 *     succesfully started and is ready to accept commands from the application
 *   - @ref sl_bt_evt_system_error - Triggered if the command to start the
 *     Bluetooth stack was accepted but the asynchronous starting of the stack
 *     has failed
 *
 ******************************************************************************/
sl_status_t sl_bt_system_start_bluetooth();

/***************************************************************************//**
 *
 * If the Bluetooth on-demand start component is not included in the application
 * build, the Bluetooth stack is automatically started when the device boots up.
 * In this configuration, the stop command is not available and the command
 * returns the error SL_STATUS_NOT_AVAILABLE.
 *
 * When the Bluetooth on-demand start component is included in the application
 * build, this command is used by the application to stop the Bluetooth stack
 * when the application no longer needs it. This command gracefully restores
 * Bluetooth to an idle state by disconnecting any active connections and
 * stopping any on-going advertising and scanning. Any resources that were
 * allocated when the stack was started are freed when the stack has finished
 * stopping. After this command, the BGAPI classes other than @ref sl_bt_system
 * become unavailable.
 *
 * Stopping the Bluetooth stack with this command is asynchronous and the
 * completion is indicated by the @ref sl_bt_evt_system_stopped event. The
 * application can use the command @ref sl_bt_system_start_bluetooth to restart
 * the stack any time after it has received the @ref sl_bt_evt_system_stopped
 * event. If the application needs to stop the Bluetooth stack immediately, use
 * the command @ref sl_bt_system_forcefully_stop_bluetooth. That command can
 * also be used to immediately complete the asynchronous stopping if the command
 * @ref sl_bt_system_stop_bluetooth has not completed in expected time period.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_system_stopped - Triggered when stopping the Bluetooth
 *     stack has completed
 *
 ******************************************************************************/
sl_status_t sl_bt_system_stop_bluetooth();

/***************************************************************************//**
 *
 * If the Bluetooth on-demand start component is not included in the application
 * build, the Bluetooth stack is automatically started when the device boots up.
 * In this configuration, the stop command is not available and the command
 * returns the error SL_STATUS_NOT_AVAILABLE.
 *
 * When the Bluetooth on-demand start component is included in the application
 * build, this command is used by the application to forcefully stop the
 * Bluetooth stack when the application no longer needs it. This command
 * immediately stops all active Bluetooth operations such as advertising,
 * scanning, and connections. Active connections are forcefully closed without
 * performing the ACL Termination procedure. This can result in the observation
 * of connection loss or supervision timeout on the remote device. Only use this
 * command for special cases, for example, when stopping Bluetooth with @ref
 * sl_bt_system_stop_bluetooth did not complete in expected time period.
 *
 * Stopping the Bluetooth stack with this command is immediate and it directly
 * triggers the @ref sl_bt_evt_system_stopped event. Any resources that were
 * allocated when the stack was started are freed. After this command, the BGAPI
 * classes other than @ref sl_bt_system become unavailable. The application can
 * use the command @ref sl_bt_system_start_bluetooth to continue using Bluetooth
 * later.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_system_stopped - Triggered immediately to indicate the
 *     Bluetooth stack has stopped
 *
 ******************************************************************************/
sl_status_t sl_bt_system_forcefully_stop_bluetooth();

/***************************************************************************//**
 *
 * Get the firmware version information.
 *
 * <b>NOTE:</b> This command is available even if the Bluetooth stack has not
 * been started. See @ref sl_bt_system_start_bluetooth for description of how
 * the Bluetooth stack is started.
 *
 * @param[out] major Major release version
 * @param[out] minor Minor release version
 * @param[out] patch Patch release number
 * @param[out] build Build number
 * @param[out] bootloader Unused. Ignore this field.
 * @param[out] hash Version hash
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_get_version(uint16_t *major,
                                     uint16_t *minor,
                                     uint16_t *patch,
                                     uint16_t *build,
                                     uint32_t *bootloader,
                                     uint32_t *hash);

/***************************************************************************//**
 *
 * Reset the system into user application mode, i.e., reboot the user
 * application. This command does not have a response. To boot into a DFU mode,
 * use the Bootloader API @c bootloader_rebootAndInstall.
 *
 * <b>NOTE:</b> This command is available even if the Bluetooth stack has not
 * been started. See @ref sl_bt_system_start_bluetooth for description of how
 * the Bluetooth stack is started.
 *
 *
 * @b Events
 *   - @ref sl_bt_evt_system_boot -
 *
 *     Sent after the device has booted in user application mode and Bluetooth
 *     stack has started.
 *
 *     <b>Note:</b> This event will not be sent when the Bluetooth on-demand
 *     start feature is used because Bluetooth stack will not start
 *     automatically.
 *
 ******************************************************************************/
void sl_bt_system_reboot();

/***************************************************************************//**
 *
 * Force radio to idle state and allow device to sleep. Advertising, scanning,
 * connections, and software timers are halted by this command. Halted
 * operations resume after calling this command with parameter 0. Connections
 * stay alive if the system is resumed before connection supervision timeout.
 *
 * Use this command only for a short time period (maximum few seconds). Although
 * it halts Bluetooth activity, all tasks and operations still exist inside the
 * stack with their own concepts of time. Halting the system for a long time
 * period may have negative consequences on stack's internal states.
 *
 * <b>NOTE:</b> The software timer is also halted. Hardware interrupts are the
 * only way to wake up from energy mode 2 when the system is halted.
 *
 * @param[in] halt Values:
 *     - <b>1:</b> halt
 *     - <b>0:</b> resume
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_halt(uint8_t halt);

/***************************************************************************//**
 *
 * Send configuration data to the link layer. This command fine tunes low-level
 * Bluetooth operations.
 *
 * @param[in] key @parblock
 *   Enum @ref sl_bt_system_linklayer_config_key_t. Key to configure. Values:
 *     - <b>sl_bt_system_linklayer_config_key_halt (0x1):</b> Same as
 *       system_halt command, value-0 Stop Radio 1- Start Radio
 *     - <b>sl_bt_system_linklayer_config_key_priority_range (0x2):</b> Sets the
 *       RAIL priority_mapping offset field of the link layer priority
 *       configuration structure to the first byte of the value field.
 *     - <b>sl_bt_system_linklayer_config_key_scan_channels (0x3):</b> Sets
 *       channels to scan on. The first byte of the value is the channel map.
 *       0x1 = Channel 37, 0x2 = Channel 38, 0x4 = Channel 39
 *     - <b>sl_bt_system_linklayer_config_key_set_flags (0x4):</b> Sets the link
 *       layer configuration flags. The value is a little endian 32-bit integer.
 *       Flag Values:
 *         - 0x00000001 - Disable Feature Exchange in peripheral role of the
 *           connection
 *         - 0x00000002 - Disable Feature Exchange in central role of the
 *           connection
 *
 *     - <b>sl_bt_system_linklayer_config_key_clr_flags (0x5):</b> The value is
 *       flags to clear. Flags are the same as in SET_FLAGS command.
 *     - <b>sl_bt_system_linklayer_config_key_set_afh_interval (0x7):</b> Set
 *       the afh_scan_interval. Value is in units of 10 ms. Setting the interval
 *       to 0 will result in using the default value of 1 second.
 *     - <b>sl_bt_system_linklayer_config_key_set_periodic_advertising_status_report
 *       (0x8):</b> Enable or disable the status report of periodic advertising
 *       on an advertising set. When enabled, event @ref
 *       sl_bt_evt_periodic_advertiser_status will be generated for each
 *       scheduled transmission. Value: 2 bytes
 *         - Byte 1 - The advertising set handle
 *         - Byte 2 - The status report state
 *             - 0: Disable the status report
 *             - 1: Enable the status report
 *
 *       Default: The status report on any advertising sets is disabled.
 *     - <b>sl_bt_system_linklayer_config_key_set_priority_table (0x9):</b> The
 *       value contains a priority table to be copied over the existing table.
 *       If the value is smaller than the full table, only those values are
 *       updated. See sl_bt_bluetooth_ll_priorities struct for the definition of
 *       a priority table.
 *     - <b>sl_bt_system_linklayer_config_key_set_rx_packet_filtering (0xa):</b>
 *       Configure and enable or disable RX packet filtering feature. Value: >=
 *       5 bytes.
 *         - Byte 1 - The filter count
 *         - Byte 2 - The filter offset
 *         - Byte 3 - The length of the filter list
 *         - Byte 4 - The bitmask flags
 *         - Rest of the data - The filter list
 *
 *     - <b>sl_bt_system_linklayer_config_key_set_simultaneous_scanning
 *       (0xb):</b> Enable or disable simultaneous scanning on the 1M and Coded
 *       PHYs. Value: 1 byte.
 *         - 0 - Disable simultaneous scanning.
 *         - 1 - Enable simultaneous scanning.
 *
 *     - <b>sl_bt_system_linklayer_config_key_set_channelmap_flags (0xc):</b>
 *       Configure channelmap adaptivity flags. Value: 4 bytes.
 *         - 0 - Disable the adaptivity in the AFH feature.
 *         - 1 - Enable the adaptivity in the AFH feature.
 *
 *     - <b>sl_bt_system_linklayer_config_key_low_power_mode_power_limit
 *       (0xd):</b> Set power limits for the low power mode where the frequency
 *       adaptivity is disabled or the current channel mapping with adaptivity
 *       enabled does not allow to use a high transmission power. A limit value
 *       must not exceed the maximum that the regulations in the region of use
 *       allow. The value is an 8-bytes long array consisting of 4 int16_t items
 *       specifying the power limits in 0.1 dBm unit for different PHYs in the
 *       following order,
 *         - Item 1 - The power limit for 1M PHY
 *         - Item 2 - The power limit for 2M PHY
 *         - Item 3 - The power limit for Coded PHY S=8
 *         - Item 4 - The power limit for Coded PHY S=2
 *
 *     - <b>sl_bt_system_linklayer_config_key_power_control_golden_range
 *       (0x10):</b> Set Power Control golden range parameters. The value is a
 *       8-bytes long array that consists of 4 pairs of golden range
 *       configurations. In each pair, the first byte is the lower RSSI boundary
 *       and the second byte is the upper RSSI boundary. RSSI values are in dBm.
 *       This configuration is not allowed if there are active Bluetooth
 *       connections.
 *         - Byte 1 - Minimal RSSI on 1M PHY
 *         - Byte 2 - Maximal RSSI on 1M PHY
 *         - Byte 3 - Minimal RSSI on 2M PHY
 *         - Byte 4 - Maximal RSSI on 2M PHY
 *         - Byte 5 - Minimal RSSI on Coded PHY S=8
 *         - Byte 6 - Maximal RSSI on Coded PHY S=8
 *         - Byte 7 - Minimal RSSI on Coded PHY S=2
 *         - Byte 8 - Maximal RSSI on Coded PHY S=2
 *
 *     - <b>sl_bt_system_linklayer_config_key_active_scanner_backoff_upper_limit
 *       (0x11):</b> Value: uint16_t Adjust upper limit for backoff counter. If
 *       0 restores default value of 256 Value must be between 16 - 256
 *     - <b>sl_bt_system_linklayer_config_key_afh_rssi_threshold (0x12):</b>
 *       Value: int8_t Configures RSSI limit for AFH channel blocking
 *     - <b>sl_bt_system_linklayer_config_key_afh_channel_cooldown (0x13):</b>
 *       Value: int16_t Configures how long channel is blocked after activity is
 *       detected Default: 8000
 *     - <b>sl_bt_system_linklayer_config_key_set_report_all_scan_rsps
 *       (0x14):</b> Value: uint8_t 0 - default, only reports scan responses
 *       that is received after sending scan_req nonzero - Will report all scan
 *       responses that are received on primary advertising channels
 *   @endparblock
 * @param[in] data_len Length of data in @p data
 * @param[in] data Configuration data. Length and contents of the data field
 *   depend on the key value used.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_linklayer_configure(uint8_t key,
                                             size_t data_len,
                                             const uint8_t* data);

/***************************************************************************//**
 *
 * Set the global minimum and maximum radiated TX power levels for Bluetooth.
 * This returns selected power levels that are radiated from the antenna at TX.
 * The transmitter power at antenna pin will apply the RF TX path gain to match
 * this setting. RF TX path gain can be set in the Bluetooth configuration. If
 * the GATT server contains a TX power service, the TX Power Level attribute
 * will be updated with the selected maximum power level.
 *
 * A selected power level may be different than the requested value because of
 * Bluetooth feature restrictions or the device's radio characteristics. For
 * Bluetooth connections, the maximum radiated TX power is limited to 10 dBm if
 * Adaptive Frequency Hopping (AFH) is not enabled.
 *
 * The minimum TX power setting is used by LE power control. It has no effect in
 * Bluetooth stack if the LE power control feature is not enabled. However, the
 * application may still use this setting for other purposes, e.g., setting the
 * minimum TX power for DTM transmitter test.
 *
 * The minimum and maximum radiated TX power levels can also be configured in
 * the Bluetooth configuration and passed into the Bluetooth stack
 * initialization. By default, the minimum radiated TX power level is configured
 * to -3 dBm and the maximum radiated TX power level to 8 dBm.
 *
 * <b>NOTE:</b> Do not use this command while advertising or scanning.
 * Furthermore, the stack does not allow setting TX powers during connections.
 *
 * @param[in] min_power Minimum radiated TX power. Unit: 0.1 dBm. For example,
 *   the value 10 means 1 dBm.
 * @param[in] max_power Maximum radiated TX power. Unit: 0.1 dBm. For example,
 *   the value 10 means 1 dBm.
 * @param[out] set_min The selected minimum radiated TX power. Unit: 0.1 dBm
 * @param[out] set_max The selected maximum radiated TX power. Unit: 0.1 dBm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_set_tx_power(int16_t min_power,
                                      int16_t max_power,
                                      int16_t *set_min,
                                      int16_t *set_max);

/***************************************************************************//**
 *
 * Get TX power settings including the minimum and maximum radiated TX power
 * levels the device supports, the minimum and maximum radiated TX power levels
 * currently set in the stack, and the TX RF path gain configuration.
 *
 * @param[out] support_min The minimum radiated TX power the device supports.
 *   Unit: 0.1 dBm
 * @param[out] support_max The maximum radiated TX power the device supports.
 *   Unit: 0.1 dBm
 * @param[out] set_min The minimum radiated TX power currently set in stack.
 *   Unit: 0.1 dBm
 * @param[out] set_max The maximum radiated TX power currently set in stack.
 *   Unit: 0.1 dBm
 * @param[out] rf_path_gain TX RF path gain. Unit: 0.1 dBm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_get_tx_power_setting(int16_t *support_min,
                                              int16_t *support_max,
                                              int16_t *set_min,
                                              int16_t *set_max,
                                              int16_t *rf_path_gain);

/***************************************************************************//**
 *
 * <b>Deprecated</b> . The Bluetooth stack does not provide a replacement for
 * storing an address persistently. User application can implement its own logic
 * of retrieving the address from persistent storage or with other means and use
 * it as the identity address in Bluetooth stack with command @ref
 * sl_bt_gap_set_identity_address.
 *
 * Store a custom Bluetooth identity address in the Bluetooth region of NVM3.
 * The address can be a public device address or a static device address. NVM3
 * keys used for the address and its type are in the key range owned by the
 * Bluetooth stack. The stack returns an error if the static device address does
 * not conform to the Bluetooth specification.
 *
 * The new address will be effective in the next system reboot. The stack will
 * use the address in the NVM3 keys when present. Otherwise, it uses the default
 * Bluetooth public device address which is programmed at production.
 *
 * The stack treats 00:00:00:00:00:00 and ff:ff:ff:ff:ff:ff as invalid
 * addresses. Therefore, passing one of them into this command will cause the
 * stack to delete the NVM3 keys and use the default address in the next system
 * reboot.
 *
 * <b>Note:</b> Because the NVM3 keys are located in flash and flash wearing can
 * occur, avoid calling this command regularly.
 *
 * @param[in] address Bluetooth identity address in little endian format
 * @param[in] type Enum @ref sl_bt_gap_address_type_t. Identity address type.
 *   Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_set_identity_address(bd_addr address, uint8_t type);

/***************************************************************************//**
 *
 * <b>Deprecated</b> and replaced by @ref sl_bt_gap_get_identity_address.
 *
 * Read the Bluetooth identity address used by the device, which can be a public
 * or random static device address.
 *
 * @param[out] address Bluetooth identity address in little endian format
 * @param[out] type Enum @ref sl_bt_gap_address_type_t. Identity address type.
 *   Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_get_identity_address(bd_addr *address, uint8_t *type);

/***************************************************************************//**
 *
 * Get random data.
 *
 * @param[in] length Length of random data.
 * @param[in] max_data_size Size of output buffer passed in @p data
 * @param[out] data_len On return, set to the length of output data written to
 *   @p data
 * @param[out] data Random data
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_get_random_data(uint8_t length,
                                         size_t max_data_size,
                                         size_t *data_len,
                                         uint8_t *data);

/***************************************************************************//**
 *
 * Write data into the system data buffer. Data will be appended to the end of
 * existing data.
 *
 * @param[in] data_len Length of data in @p data
 * @param[in] data Data to write
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_data_buffer_write(size_t data_len,
                                           const uint8_t* data);

/***************************************************************************//**
 *
 * Remove all data from the system data buffer.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_data_buffer_clear();

/***************************************************************************//**
 *
 * Get packet and error counters. Passing a non-zero value also resets counters.
 *
 * @param[in] reset Reset counters if the parameter value is not zero.
 * @param[out] tx_packets The number of successfully transmitted packets
 * @param[out] rx_packets The number of successfully received packets
 * @param[out] crc_errors The number of received packets with CRC errors
 * @param[out] failures The number of radio failures, such as aborted TX/RX
 *   packets, scheduling failures, and so on.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_get_counters(uint8_t reset,
                                      uint16_t *tx_packets,
                                      uint16_t *rx_packets,
                                      uint16_t *crc_errors,
                                      uint16_t *failures);

/***************************************************************************//**
 *
 * <b>Deprecated</b> . Use the sleeptimer component (in platform services
 * category) for timers. Because the sleeptimer does not support a timer with
 * slack yet, the Bluetooth stack will continue to support this command until
 * another component provides the functionality.
 *
 * Start a software timer with slack. The slack parameter allows the stack to
 * optimize wakeups and save power. The timer event is triggered between time
 * and time + @p slack.
 *
 * Multiple concurrent timers can be running simultaneously. 256 unique timer
 * handles (IDs) are available. The maximum number of concurrent timers is
 * configurable at device initialization. Up to 16 concurrent timers can be
 * configured. The default configuration is 4. As the RAM for storing timer data
 * is pre-allocated at initialization, an application should not configure the
 * amount more than it needs for minimizing RAM usage.
 *
 * @param[in] time @parblock
 *   An interval between how often to send events in hardware clock ticks (1
 *   second is equal to 32768 ticks).
 *
 *   The smallest interval value supported is 328, which is around 10
 *   milliseconds. Any parameters between 0 and 328 will be rounded up to 328.
 *   The maximum value is 2147483647, which corresponds to about 18.2 hours.
 *
 *   If @p time is 0, removes the scheduled timer with the same handle.
 *   @endparblock
 * @param[in] slack Slack time in hardware clock ticks
 * @param[in] handle Timer handle to use, which is returned in timeout event
 * @param[in] single_shot Timer mode. Values:
 *     - <b>0:</b> false (timer is repeating)
 *     - <b>1:</b> true (timer runs only once)
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_system_soft_timer - Sent after this timer has lapsed.
 *
 ******************************************************************************/
sl_status_t sl_bt_system_set_lazy_soft_timer(uint32_t time,
                                             uint32_t slack,
                                             uint8_t handle,
                                             uint8_t single_shot);

/***************************************************************************//**
 *
 * <b>Deprecated</b> and replaced by @ref sl_bt_system_reboot.
 *
 * Reset the system into user application mode, i.e., reboot the user
 * application. This command does not have a response. To boot into a DFU mode,
 * use the Bootloader API @c bootloader_rebootAndInstall.
 *
 * <b>NOTE:</b> This command is available even if the Bluetooth stack has not
 * been started. See @ref sl_bt_system_start_bluetooth for description of how
 * the Bluetooth stack is started.
 *
 * @param[in] dfu This parameter is unused and is ignored by the Bluetooth
 *   stack.
 *
 * @b Events
 *   - @ref sl_bt_evt_system_boot -
 *
 *     Sent after the device has booted in user application mode and Bluetooth
 *     stack has started.
 *
 *     <b>Note:</b> This event will not be sent when the Bluetooth on-demand
 *     start feature is used because Bluetooth stack will not start
 *     automatically.
 *
 ******************************************************************************/
SL_BGAPI_DEPRECATED void sl_bt_system_reset(uint8_t dfu);

/** @} */ // end addtogroup sl_bt_system

/**
 * @addtogroup sl_bt_resource Resource Report
 * @{
 *
 * @brief Resource Report
 *
 * Commands and events in this class can be used to query and receive the memory
 * buffer usage status. The memory buffer is used by the Bluetooth stack for
 * storing application data (e.g., API commands and events), user data over
 * Bluetooth connections, advertising, scanning, etc. Total buffer size is
 * defined by the application using the SL_BT_CONFIG_BUFFER_SIZE configuration.
 * The Bluetooth stack does not partition the buffer, instead, the buffer is a
 * shared resource for all features. Therefore, the available memory for one
 * feature could be affected by another feature in simultaneous use cases. This
 * API class provides a utility for application to get some insight of the
 * buffer usage. As allocations in the memory buffer have overhead, the actual
 * amount of memory that can be used for user data is less than a reported free
 * memory amount.
 */

/* Command and Response IDs */
#define sl_bt_cmd_resource_get_status_id                             0x005f0020
#define sl_bt_cmd_resource_set_report_threshold_id                   0x015f0020
#define sl_bt_cmd_resource_enable_connection_tx_report_id            0x025f0020
#define sl_bt_cmd_resource_get_connection_tx_status_id               0x035f0020
#define sl_bt_cmd_resource_disable_connection_tx_report_id           0x045f0020
#define sl_bt_rsp_resource_get_status_id                             0x005f0020
#define sl_bt_rsp_resource_set_report_threshold_id                   0x015f0020
#define sl_bt_rsp_resource_enable_connection_tx_report_id            0x025f0020
#define sl_bt_rsp_resource_get_connection_tx_status_id               0x035f0020
#define sl_bt_rsp_resource_disable_connection_tx_report_id           0x045f0020

/**
 * @addtogroup sl_bt_resource_connection_tx_flags Connection TX status flags
 * @{
 *
 * Defines the connection TX status flags reported by @ref
 * sl_bt_resource_get_connection_tx_status.
 */

/**
 *
 * The number of TX packets queued on a connection has overflowed the @p
 * packet_count that was effective at the time the connection opened. The packet
 * count is configured with command @ref
 * sl_bt_resource_enable_connection_tx_report and is valid for subsequent
 * connections.
 *
 * When this bit is set, the @p packet_count returned by @ref
 * sl_bt_resource_get_connection_tx_status is correct, but @p data_len excludes
 * the data bytes in the packets that overflowed the configured packet count.
 *
 * */
#define SL_BT_RESOURCE_CONNECTION_TX_FLAGS_ERROR_PACKET_OVERFLOW 0x1       

/**
 *
 * Internal inconsistency has been detected in the connection TX bookkeeping.
 * When this bit is set, the application should consider both @p packet_count
 * and @p data_len returned by @ref sl_bt_resource_get_connection_tx_status to
 * be unreliable.
 *
 * */
#define SL_BT_RESOURCE_CONNECTION_TX_FLAGS_ERROR_CORRUPT         0x2       

/** @} */ // end Connection TX status flags

/**
 * @addtogroup sl_bt_evt_resource_status sl_bt_evt_resource_status
 * @{
 * @brief Indicates that the memory buffer usage has crossed a threshold
 */

/** @brief Identifier of the status event */
#define sl_bt_evt_resource_status_id                                 0x005f00a0

/***************************************************************************//**
 * @brief Data structure of the status event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_resource_status_s
{
  uint32_t free_bytes; /**< The number of free bytes in the memory buffer */
});

typedef struct sl_bt_evt_resource_status_s sl_bt_evt_resource_status_t;

/** @} */ // end addtogroup sl_bt_evt_resource_status

/***************************************************************************//**
 *
 * Get the present memory buffer usage status.
 *
 * @param[out] total_bytes The number of total bytes in the memory buffer
 * @param[out] free_bytes The number of free bytes in the memory buffer
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resource_get_status(uint32_t *total_bytes,
                                      uint32_t *free_bytes);

/***************************************************************************//**
 *
 * Set low and high thresholds of memory buffer usage reports. Value 0 in
 * parameter @p low for the low threshold disables the reporting, and a non-zero
 * value in @p low enables the reporting.
 *
 * When the reporting is enabled, event @ref sl_bt_evt_resource_status will be
 * generated to report the status when the free buffer amount decreases and
 * crosses the low threshold, and later another event will be generated if the
 * free buffer amount increases and crosses the high threshold. If only the high
 * threshold is crossed but the low threshold isn't, no event will be generated.
 *
 * By default, low and high threshold values are 0, i.e., no report event is
 * generated.
 *
 * @param[in] low The low threshold of free bytes in the memory buffer, or 0 to
 *   disable the reporting
 * @param[in] high A non-zero value as the high threshold that must be greater
 *   than parameter @p low, or 0 for not reporting the status for high threshold
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resource_set_report_threshold(uint32_t low, uint32_t high);

/***************************************************************************//**
 *
 * Enable tracking and reporting data packet TX status of future new
 * connections. Existing connections are not affected by this command.
 *
 * @param[in] packet_count The maximum number of data packets to track on a
 *   connection
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_server_notification_tx_completed - Sent when GATT
 *     notifications from the GATT server were transmitted.
 *
 ******************************************************************************/
sl_status_t sl_bt_resource_enable_connection_tx_report(uint16_t packet_count);

/***************************************************************************//**
 *
 * Get the data packet TX status of a connection.
 *
 * @param[in] connection Connection handle
 * @param[out] flags Flags that indicate the status of connection TX packet
 *   reporting. This value is a bitmask of @ref
 *   sl_bt_resource_connection_tx_flags.
 * @param[out] packet_count Number of data packets in the TX queue waiting to be
 *   transmitted
 * @param[out] data_len Total number of bytes of data packets in the TX queue
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resource_get_connection_tx_status(uint8_t connection,
                                                    uint16_t *flags,
                                                    uint16_t *packet_count,
                                                    uint32_t *data_len);

/***************************************************************************//**
 *
 * Disable tracking and reporting data packet TX status of future new
 * connections.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resource_disable_connection_tx_report();

/** @} */ // end addtogroup sl_bt_resource

/**
 * @addtogroup sl_bt_gap GAP
 * @{
 *
 * @brief GAP
 *
 * The commands and events in this class are related to the Generic Access
 * Profile (GAP) in Bluetooth.
 */

/* Command and Response IDs */
#define sl_bt_cmd_gap_set_privacy_mode_id                            0x01020020
#define sl_bt_cmd_gap_set_data_channel_classification_id             0x02020020
#define sl_bt_cmd_gap_set_identity_address_id                        0x04020020
#define sl_bt_cmd_gap_get_identity_address_id                        0x05020020
#define sl_bt_rsp_gap_set_privacy_mode_id                            0x01020020
#define sl_bt_rsp_gap_set_data_channel_classification_id             0x02020020
#define sl_bt_rsp_gap_set_identity_address_id                        0x04020020
#define sl_bt_rsp_gap_get_identity_address_id                        0x05020020

/**
 * @brief These values define Bluetooth device address types. Commands and
 * events that have Bluetooth device address parameters will specify which
 * values are valid for that particular command or event.
 *
 * If the application does not include the
 * bluetooth_feature_use_accurate_api_address_types component, most Bluetooth
 * commands and events use a coarse address type that only differentiates
 * between a public address and any random address. When the application
 * includes the bluetooth_feature_use_accurate_api_address_types component,
 * Bluetooth commands and events that include an address type will
 * systematically use values of this @ref sl_bt_gap_address_type_t enumeration
 * to indicate the accurate address type.
 *
 * The values @ref sl_bt_gap_public_address_resolved_from_rpa and @ref
 * sl_bt_gap_static_address_resolved_from_rpa are reported by the Bluetooth
 * stack only when the application includes the bluetooth_feature_resolving_list
 * component and the address was resolved in the Bluetooth controller. If the
 * application uses these two types in input parameters, they are treated as
 * synonyms of @ref sl_bt_gap_public_address and @ref sl_bt_gap_static_address,
 * respectively.
 */
typedef enum
{
  sl_bt_gap_public_address                   = 0x0,  /**< (0x0) Public device
                                                          address */
  sl_bt_gap_static_address                   = 0x1,  /**< (0x1) Static device
                                                          address */
  sl_bt_gap_random_resolvable_address        = 0x2,  /**< (0x2) Resolvable
                                                          private random address */
  sl_bt_gap_random_nonresolvable_address     = 0x3,  /**< (0x3) Non-resolvable
                                                          private random address */
  sl_bt_gap_public_address_resolved_from_rpa = 0x4,  /**< (0x4) Public identity
                                                          address resolved from
                                                          a resolvable private
                                                          address (RPA) */
  sl_bt_gap_static_address_resolved_from_rpa = 0x5,  /**< (0x5) Static identity
                                                          address resolved from
                                                          a resolvable private
                                                          address (RPA) */
  sl_bt_gap_anonymous_address                = 0xff  /**< (0xff) No address
                                                          provided (anonymous
                                                          advertising) */
} sl_bt_gap_address_type_t;

/**
 * @brief Types of PHYs
 */
typedef enum
{
  sl_bt_gap_phy_1m    = 0x1,  /**< (0x1) 1M PHY */
  sl_bt_gap_phy_2m    = 0x2,  /**< (0x2) 2M PHY */
  sl_bt_gap_phy_coded = 0x4,  /**< (0x4) Coded PHY, 125k (S=8) or 500k (S=2) */
  sl_bt_gap_phy_any   = 0xff  /**< (0xff) Any PHYs the device supports */
} sl_bt_gap_phy_t;

/**
 * @brief PHY types with coding schemes
 */
typedef enum
{
  sl_bt_gap_phy_coding_1m_uncoded = 0x1, /**< (0x1) 1M PHY */
  sl_bt_gap_phy_coding_2m_uncoded = 0x2, /**< (0x2) 2M PHY */
  sl_bt_gap_phy_coding_125k_coded = 0x4, /**< (0x4) 125k Coded PHY (S=8) */
  sl_bt_gap_phy_coding_500k_coded = 0x8  /**< (0x8) 500k Coded PHY (S=2) */
} sl_bt_gap_phy_coding_t;

/**
 * @brief Types of channel selection algorithms in Link Layer
 */
typedef enum
{
  sl_bt_gap_channel_selection_algorithm_1 = 0x0, /**< (0x0) Channel selection
                                                      algorithm #1 */
  sl_bt_gap_channel_selection_algorithm_2 = 0x1  /**< (0x1) Channel selection
                                                      algorithm #2 */
} sl_bt_gap_channel_selection_algorithm_t;

/***************************************************************************//**
 *
 * Enable or disable the privacy feature on all GAP roles. New privacy mode will
 * take effect for advertising next time advertising is enabled, for scanning
 * next time scanning is enabled, and for initiating on the next open connection
 * command.
 *
 * When privacy is enabled and the device is advertising or scanning, the stack
 * will maintain a periodic timer with the specified time interval as a timeout
 * value. At each timeout, the stack generates a new resolvable private address
 * and uses it in scanning requests. For advertisers, the stack generates a new
 * resolvable or non-resolvable private address and uses it in advertising data
 * packets for each advertising set if its address is not application-managed,
 * i.e., the address was not set by the application (with the @ref
 * sl_bt_advertiser_set_random_address command). The application is fully
 * responsible for application-managed advertiser addresses. For an
 * application-managed resolvable private address, the application should
 * schedule periodic address updates for enhancing the privacy. It is
 * recommended to use different schedules for different advertising sets.
 *
 * Disabling the privacy during active advertising or scanning is not allowed.
 *
 * By default, privacy feature is disabled.
 *
 * @param[in] privacy Values:
 *     - <b>0:</b> Disable privacy
 *     - <b>1:</b> Enable privacy
 * @param[in] interval The minimum time interval between a private address
 *   change. This parameter is ignored if this command is issued to disable
 *   privacy mode. Values:
 *     - <b>0:</b> Use default interval, 15 minutes
 *     - <b>others:</b> The time interval in minutes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gap_set_privacy_mode(uint8_t privacy, uint8_t interval);

/***************************************************************************//**
 *
 * Specify a channel classification for data channels. This classification
 * persists until overwritten with a subsequent command or until the system is
 * reset. The value length of @p channel_map must be 5 bytes.
 *
 * @param[in] channel_map_len Length of data in @p channel_map
 * @param[in] channel_map @parblock
 *   5 byte bit field in little endian format. Only the first 37 bits are used.
 *   Bit 0 of the first byte is channel 0, bit 0 of the second byte is channel
 *   8, etc. Bits 37-39 are reserved for future use and must be set to 0.
 *
 *   A channel is bad when its bit is 0. A channel is unknown when its bit is 1.
 *   At least two channels shall be marked as unknown.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gap_set_data_channel_classification(size_t channel_map_len,
                                                      const uint8_t* channel_map);

/***************************************************************************//**
 *
 * Set the device's Bluetooth identity address to be used in advertising,
 * scanning, connection initiation, and identity address exchange in bonding.
 * The address is stored in RAM only and does not change the identity address in
 * persistent storage. The address can be a public device address or static
 * device address. It will be effective immediately in the next advertising,
 * scanning, connection initiation, and bonding. Error
 * SL_STATUS_INVALID_PARAMETER is returned if the address does not conform to
 * the Bluetooth specification.
 *
 * Note that advertising sets that have own addresses set by @ref
 * sl_bt_advertiser_set_random_address are not affected by this command, i.e.,
 * they will continue to use their own user defined addresses.
 *
 * @param[in] address The address in little endian format
 * @param[in] addr_type Enum @ref sl_bt_gap_address_type_t. The address type.
 *   Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gap_set_identity_address(bd_addr address, uint8_t addr_type);

/***************************************************************************//**
 *
 * Get the Bluetooth identity address used by the device, which can be a public
 * or random static device address.
 *
 * @param[out] address Bluetooth identity address in little endian format
 * @param[out] type Enum @ref sl_bt_gap_address_type_t. Identity address type.
 *   Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gap_get_identity_address(bd_addr *address, uint8_t *type);

/** @} */ // end addtogroup sl_bt_gap

/**
 * @addtogroup sl_bt_advertiser Advertiser
 * @{
 *
 * @brief Advertiser
 *
 * This is the base class of legacy, extended, and periodic advertisings for
 * common functionalities including advertising set management, TX power
 * setting, advertising address, and so on.
 *
 * On an advertising set, either the legacy or extended advertising can be
 * enabled at a time but they cannot be enabled simultaneously on the same
 * advertising set. For example, the following sequence shows how to start the
 * legacy advertising on an advertising set. Starting the extended advertising
 * is similar. The only difference is to use the extended_advertiser API class.
 *   1. Create an advertise set with the @ref sl_bt_advertiser_create_set
 *      command.
 *   2. Configure and set advertising parameters for the advertising set as
 *      needed.
 *   3. Set the advertising data with the @ref sl_bt_legacy_advertiser_set_data
 *      or @ref sl_bt_legacy_advertiser_generate_data command.
 *   4. Start the legacy advertising with the @ref sl_bt_legacy_advertiser_start
 *      command.
 *
 * Periodic advertising can be enabled independently on the advertising set
 * regardless of the state of the legacy or extended advertising. However, to
 * ensure that scanners can find the periodic advertising information and
 * establish a synchronization, the extended advertising must be enabled
 * simultaneously with the periodic advertising.
 */

/* Command and Response IDs */
#define sl_bt_cmd_advertiser_create_set_id                           0x01040020
#define sl_bt_cmd_advertiser_configure_id                            0x12040020
#define sl_bt_cmd_advertiser_set_timing_id                           0x03040020
#define sl_bt_cmd_advertiser_set_channel_map_id                      0x04040020
#define sl_bt_cmd_advertiser_set_tx_power_id                         0x0b040020
#define sl_bt_cmd_advertiser_set_report_scan_request_id              0x05040020
#define sl_bt_cmd_advertiser_set_random_address_id                   0x10040020
#define sl_bt_cmd_advertiser_clear_random_address_id                 0x11040020
#define sl_bt_cmd_advertiser_stop_id                                 0x0a040020
#define sl_bt_cmd_advertiser_delete_set_id                           0x02040020
#define sl_bt_rsp_advertiser_create_set_id                           0x01040020
#define sl_bt_rsp_advertiser_configure_id                            0x12040020
#define sl_bt_rsp_advertiser_set_timing_id                           0x03040020
#define sl_bt_rsp_advertiser_set_channel_map_id                      0x04040020
#define sl_bt_rsp_advertiser_set_tx_power_id                         0x0b040020
#define sl_bt_rsp_advertiser_set_report_scan_request_id              0x05040020
#define sl_bt_rsp_advertiser_set_random_address_id                   0x10040020
#define sl_bt_rsp_advertiser_clear_random_address_id                 0x11040020
#define sl_bt_rsp_advertiser_stop_id                                 0x0a040020
#define sl_bt_rsp_advertiser_delete_set_id                           0x02040020

/**
 * @brief These values define the available discovery modes, which dictate how
 * the device is visible to other devices in the legacy and extended
 * advertising.
 */
typedef enum
{
  sl_bt_advertiser_non_discoverable     = 0x0, /**< (0x0) Not discoverable */
  sl_bt_advertiser_limited_discoverable = 0x1, /**< (0x1) Discoverable by both
                                                    limited and general
                                                    discovery procedures */
  sl_bt_advertiser_general_discoverable = 0x2  /**< (0x2) Discoverable by the
                                                    general discovery procedure */
} sl_bt_advertiser_discovery_mode_t;

/**
 * @brief Address type to use for the legacy and extended advertising
 */
typedef enum
{
  sl_bt_advertiser_identity_address = 0x0, /**< (0x0) Use public or static
                                                device address, or an identity
                                                address if privacy mode is
                                                enabled. */
  sl_bt_advertiser_non_resolvable   = 0x1  /**< (0x1) Use non-resolvable address
                                                type; the advertising must be
                                                non-connectable. */
} sl_bt_advertiser_adv_address_type_t;

/**
 * @brief These values define the packet types in legacy and extended
 * advertising.
 */
typedef enum
{
  sl_bt_advertiser_advertising_data_packet = 0x0, /**< (0x0) Advertising data
                                                       packet */
  sl_bt_advertiser_scan_response_packet    = 0x1  /**< (0x1) Scan response
                                                       packet */
} sl_bt_advertiser_packet_type_t;

/**
 * @addtogroup sl_bt_advertiser_flags Generic Advertising Configuration Flags
 * @{
 *
 * This enum defines configuration flags common for legacy and extended
 * advertisings.
 */

/** Use a non-resolvable private address managed by the stack. The advertising
 * must be non-connectable when using this configuration. The stack generates a
 * non-resolvable private address for the advertising set and the stack will
 * update the address periodically in privacy mode. By default this flag is not
 * set, i.e., the advertising address uses the device identity address. This
 * configuration has no effect if the advertising address has been set with the
 * @ref sl_bt_advertiser_set_random_address command. */
#define SL_BT_ADVERTISER_USE_NONRESOLVABLE_ADDRESS          0x4       

/** Use the device identity address when privacy mode is enabled. By default,
 * this flag is not set, i.e., the advertising address uses a resolvable private
 * address managed by the stack in privacy mode. This configuration has no
 * effect if the @ref SL_BT_ADVERTISER_USE_NONRESOLVABLE_ADDRESS flag is set or
 * the advertising address has been set with the @ref
 * sl_bt_advertiser_set_random_address command. */
#define SL_BT_ADVERTISER_USE_DEVICE_IDENTITY_IN_PRIVACY     0x10      

/**
 *
 * Use the Filter Accept List to filter scan requests received while performing
 * scannable advertising with this advertising set. By default, this flag is not
 * set and scan requests from all devices are processed. If the application sets
 * this flag, scan requests are processed only from those devices that the
 * application has added to the Filter Accept List.
 *
 * This configuration is supported only when the application has included the
 * Bluetooth component bluetooth_feature_accept_list.
 *
 * */
#define SL_BT_ADVERTISER_USE_FILTER_FOR_SCAN_REQUESTS       0x20      

/**
 *
 * Use the Filter Accept List to filter connection requests received while
 * performing connectable advertising with this advertising set. By default,
 * this flag is not set and connection requests from all devices are processed.
 * If the application sets this flag, connection requests are processed only
 * from those devices that the application has added to the Filter Accept List.
 *
 * This configuration is supported only when the application has included the
 * Bluetooth component bluetooth_feature_accept_list.
 *
 * */
#define SL_BT_ADVERTISER_USE_FILTER_FOR_CONNECTION_REQUESTS 0x40      

/** @} */ // end Generic Advertising Configuration Flags

/**
 * @addtogroup sl_bt_evt_advertiser_timeout sl_bt_evt_advertiser_timeout
 * @{
 * @brief Indicates the legacy or extended advertising on an advertising set has
 * stopped because the advertiser has completed the configured number of
 * advertising events or the advertising has reached the configured duration
 *
 * The maximum number of advertising events or advertising duration can be
 * configured by the @p maxevents or @p duration parameter in the command @ref
 * sl_bt_advertiser_set_timing.
 */

/** @brief Identifier of the timeout event */
#define sl_bt_evt_advertiser_timeout_id                              0x010400a0

/***************************************************************************//**
 * @brief Data structure of the timeout event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_advertiser_timeout_s
{
  uint8_t handle; /**< The advertising set handle */
});

typedef struct sl_bt_evt_advertiser_timeout_s sl_bt_evt_advertiser_timeout_t;

/** @} */ // end addtogroup sl_bt_evt_advertiser_timeout

/**
 * @addtogroup sl_bt_evt_advertiser_scan_request sl_bt_evt_advertiser_scan_request
 * @{
 * @brief Reports a scan request received during the legacy or extended
 * advertising advertising if the scan request notification is enabled
 */

/** @brief Identifier of the scan_request event */
#define sl_bt_evt_advertiser_scan_request_id                         0x020400a0

/***************************************************************************//**
 * @brief Data structure of the scan_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_advertiser_scan_request_s
{
  uint8_t handle;       /**< Advertising set handle where the scan request was
                             received */
  bd_addr address;      /**< Bluetooth address of the scanner */
  uint8_t address_type; /**< Enum @ref sl_bt_gap_address_type_t.

                             Scanner address type.

                             If the application does not include the
                             bluetooth_feature_use_accurate_api_address_types
                             component, @p address_type uses the following
                             values:
                               - <b>0:</b> Public address
                               - <b>1:</b> Random address

                             If the application includes the
                             bluetooth_feature_use_accurate_api_address_types
                             component, @p address_type uses enum @ref
                             sl_bt_gap_address_type_t values:
                               - <b>sl_bt_gap_public_address (0x0):</b> Public
                                 device address
                               - <b>sl_bt_gap_static_address (0x1):</b> Static
                                 device address
                               - <b>sl_bt_gap_random_resolvable_address
                                 (0x2):</b> Resolvable private random address
                               - <b>sl_bt_gap_random_nonresolvable_address
                                 (0x3):</b> Non-resolvable private random
                                 address
                               - <b>sl_bt_gap_public_address_resolved_from_rpa
                                 (0x4):</b> Public identity address resolved
                                 from a resolvable private address (RPA)
                               - <b>sl_bt_gap_static_address_resolved_from_rpa
                                 (0x5):</b> Static identity address resolved
                                 from a resolvable private address (RPA) */
  uint8_t bonding;      /**< Bonding handle if the remote scanning device has
                             previously bonded with the local device. Values:
                               - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b> No
                                 bonding
                               - <b>Other:</b> Bonding handle */
});

typedef struct sl_bt_evt_advertiser_scan_request_s sl_bt_evt_advertiser_scan_request_t;

/** @} */ // end addtogroup sl_bt_evt_advertiser_scan_request

/***************************************************************************//**
 *
 * Create an advertising set that can be used for legacy, extended, or periodic
 * advertising. The handle of the created advertising set is returned in
 * response if the operation succeeds.
 *
 * The maximum number of advertising sets for user advertisers is limited by the
 * SL_BT_CONFIG_USER_ADVERTISERS configuration.
 *
 * @param[out] handle Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_create_set(uint8_t *handle);

/***************************************************************************//**
 *
 * Configure the legacy and extended advertising on an advertising set. The
 * configuration will take effect next time the legacy or extended advertising
 * is enabled.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] flags @parblock
 *   Configuration flags. Value: 0 or bitmask of @ref sl_bt_advertiser_flags
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_configure(uint8_t advertising_set, uint32_t flags);

/***************************************************************************//**
 *
 * Set the timing parameters for legacy or extended advertising on an
 * advertising set. This setting will take effect next time the legacy or
 * extended advertising is enabled.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] interval_min @parblock
 *   Minimum advertising interval. Value in units of 0.625 ms
 *     - <b>Range:</b> 0x20 to 0xFFFF
 *
 *     - Time range: 20 ms to 40.96 s
 *
 *     - <b>Default</b> : 100 ms
 *   @endparblock
 * @param[in] interval_max @parblock
 *   Maximum advertising interval. Value in units of 0.625 ms
 *     - <b>Range:</b> 0x20 to 0xFFFF
 *
 *     - Time range: 20 ms to 40.96 s
 *     - Note: interval_max should be bigger than interval_min
 *
 *     - <b>Default</b> : 200 ms
 *   @endparblock
 * @param[in] duration @parblock
 *   Advertising duration for this advertising set. Value 0 indicates no
 *   advertising duration limit and advertising continues until it is disabled.
 *   A non-zero value sets the duration in units of 10 ms. The duration begins
 *   at the start of the first advertising event of this advertising set.
 *     - <b>Range:</b> 0x0001 to 0xFFFF
 *
 *     - Time range: 10 ms to 655.35 s
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] maxevents If non-zero, indicates the maximum number of advertising
 *   events to send before the advertiser is stopped. Value 0 indicates no
 *   maximum number limit.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_set_timing(uint8_t advertising_set,
                                        uint32_t interval_min,
                                        uint32_t interval_max,
                                        uint16_t duration,
                                        uint8_t maxevents);

/***************************************************************************//**
 *
 * Set the primary advertising channel map on an advertising set. This setting
 * will take effect next time when the legacy or extended advertising is
 * enabled.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] channel_map @parblock
 *   Advertising channel map which determines, which of the three channels will
 *   be used for advertising. This value is given as a bitmask. Values:
 *     - <b>1:</b> Advertise on CH37
 *     - <b>2:</b> Advertise on CH38
 *     - <b>3:</b> Advertise on CH37 and CH38
 *     - <b>4:</b> Advertise on CH39
 *     - <b>5:</b> Advertise on CH37 and CH39
 *     - <b>6:</b> Advertise on CH38 and CH39
 *     - <b>7:</b> Advertise on all channels
 *
 *   Recommended value: 7
 *
 *     - <b>Default</b> : 7
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_set_channel_map(uint8_t advertising_set,
                                             uint8_t channel_map);

/***************************************************************************//**
 *
 * Limit the maximum advertising TX power on an advertising set. If the value
 * goes over the global value that was set using the @ref
 * sl_bt_system_set_tx_power command, the global value will be the maximum
 * limit. The maximum TX power of legacy advertising is further constrained to
 * be less than +10 dBm. Extended advertising TX power can be +10 dBm and over
 * if Adaptive Frequency Hopping is enabled. This setting has no effect on
 * periodic advertising.
 *
 * This setting will take effect next time the legacy or extended advertising is
 * enabled.
 *
 * By default, maximum advertising TX power is limited by the global value.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] power TX power in 0.1 dBm steps. For example, the value of 10 is 1
 *   dBm and 55 is 5.5 dBm.
 * @param[out] set_power The selected maximum advertising TX power
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_set_tx_power(uint8_t advertising_set,
                                          int16_t power,
                                          int16_t *set_power);

/***************************************************************************//**
 *
 * Enable or disable the scan request notification on an advertising set. This
 * setting will take effect next time the legacy or extended advertising is
 * enabled.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] report_scan_req If non-zero, enables scan request notification and
 *   scan requests will be reported as events.
 *     - <b>Default</b> : 0
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_advertiser_scan_request - Triggered when a scan request is
 *     received during advertising if the scan request notification is enabled
 *     by this command.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_set_report_scan_request(uint8_t advertising_set,
                                                     uint8_t report_scan_req);

/***************************************************************************//**
 *
 * Set the advertiser on an advertising set to use a random address. This
 * overrides the default advertiser address, which is either the public device
 * address programmed at production or the address written into persistent
 * storage using @ref sl_bt_system_set_identity_address command. This setting is
 * stored in RAM only and does not change the identity address in persistent
 * storage. In privacy mode, the stack does not change an advertiser address set
 * by this command. To ensure that the stack can manage the address update
 * periodically in privacy mode, the address setting should be removed with the
 * @ref sl_bt_advertiser_clear_random_address command.
 *
 * When setting a resolvable random address, the address parameter is ignored.
 * The stack generates one and set it as the advertiser address. The generated
 * address is returned in the response. To enhance the privacy, the application
 * should schedule periodic address updates by calling this command
 * periodically. Use different schedules for different advertising sets.
 *
 * To use the default advertiser address, remove this setting using @ref
 * sl_bt_advertiser_clear_random_address command.
 *
 * Wrong state error is returned if advertising has been enabled on the
 * advertising set. Invalid parameter error is returned if the advertising set
 * handle is invalid or the address does not conform to the Bluetooth
 * specification.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] addr_type Enum @ref sl_bt_gap_address_type_t. Address type.
 *   Values:
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address. This type can only be used for non-connectable
 *       advertising.
 * @param[in] address The random address to set. Ignore this field when setting
 *   a resolvable random address.
 * @param[out] address_out The resolvable random address set for the advertiser.
 *   Ignore this field when setting other types of random addresses.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_set_random_address(uint8_t advertising_set,
                                                uint8_t addr_type,
                                                bd_addr address,
                                                bd_addr *address_out);

/***************************************************************************//**
 *
 * Clear the random address previously set for the advertiser address on an
 * advertising set. To set a random address, use @ref
 * sl_bt_advertiser_set_random_address command. The default advertiser address
 * will be used after this operation.
 *
 * The error SL_STATUS_INVALID_STATE is returned if advertising has been enabled
 * on the advertising set. An invalid parameter error is returned if the
 * advertising set handle is invalid.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_clear_random_address(uint8_t advertising_set);

/***************************************************************************//**
 *
 * Stop the legacy or extended advertising on an advertising set. Counterpart
 * with @ref sl_bt_legacy_advertiser_start or @ref
 * sl_bt_extended_advertiser_start.
 *
 * This command does not affect the enable state of the periodic advertising on
 * the advertising set, i.e., periodic advertising is not stopped.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_stop(uint8_t advertising_set);

/***************************************************************************//**
 *
 * Delete an advertising set. Any enabled legacy, extended, or periodic
 * advertising is stopped before the deletion.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_delete_set(uint8_t advertising_set);

/** @} */ // end addtogroup sl_bt_advertiser

/**
 * @addtogroup sl_bt_legacy_advertiser Legacy Advertiser
 * @{
 *
 * @brief Legacy Advertiser
 *
 * The commands and events in this class are related to legacy advertising
 * functionalities.
 */

/* Command and Response IDs */
#define sl_bt_cmd_legacy_advertiser_set_data_id                      0x00560020
#define sl_bt_cmd_legacy_advertiser_generate_data_id                 0x01560020
#define sl_bt_cmd_legacy_advertiser_start_id                         0x02560020
#define sl_bt_cmd_legacy_advertiser_start_directed_id                0x03560020
#define sl_bt_rsp_legacy_advertiser_set_data_id                      0x00560020
#define sl_bt_rsp_legacy_advertiser_generate_data_id                 0x01560020
#define sl_bt_rsp_legacy_advertiser_start_id                         0x02560020
#define sl_bt_rsp_legacy_advertiser_start_directed_id                0x03560020

/**
 * @brief These values define the available connection modes of undirected
 * legacy advertising.
 */
typedef enum
{
  sl_bt_legacy_advertiser_non_connectable = 0x0, /**< (0x0) Undirected
                                                      non-connectable and
                                                      non-scannable legacy
                                                      advertising */
  sl_bt_legacy_advertiser_connectable     = 0x2, /**< (0x2) Undirected
                                                      connectable and scannable
                                                      legacy advertising */
  sl_bt_legacy_advertiser_scannable       = 0x3  /**< (0x3) Undirected scannable
                                                      and non-connectable legacy
                                                      advertising */
} sl_bt_legacy_advertiser_connection_mode_t;

/**
 * @brief These values define the available connection modes of directed legacy
 * advertising.
 */
typedef enum
{
  sl_bt_legacy_advertiser_high_duty_directed_connectable = 0x1, /**< (0x1) High
                                                                     duty cycle
                                                                     directed
                                                                     connectable
                                                                     legacy
                                                                     advertising */
  sl_bt_legacy_advertiser_low_duty_directed_connectable  = 0x5  /**< (0x5) Low
                                                                     duty cycle
                                                                     directed
                                                                     connectable
                                                                     legacy
                                                                     advertising */
} sl_bt_legacy_advertiser_directed_connection_mode_t;

/***************************************************************************//**
 *
 * Set user-defined advertising data packet or scan response packet on an
 * advertising set. This overwrites the existing advertising data packet and
 * scan response packet on this advertising set regardless of whether the data
 * was set for the legacy or extended advertising. Maximum 31 bytes of data can
 * be set with this command.
 *
 * If advertising mode is currently enabled, the new advertising data will be
 * used immediately. Advertising mode can be enabled using command @ref
 * sl_bt_legacy_advertiser_start.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] type Enum @ref sl_bt_advertiser_packet_type_t. The advertising
 *   packet type
 * @param[in] data_len Length of data in @p data
 * @param[in] data Data to set
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_legacy_advertiser_set_data(uint8_t advertising_set,
                                             uint8_t type,
                                             size_t data_len,
                                             const uint8_t* data);

/***************************************************************************//**
 *
 * Ask the stack to generate the advertising data packet and scan response
 * packet on an advertising set. Alternatively, the user-defined advertising
 * data can be set using the @ref sl_bt_legacy_advertiser_set_data command.
 *
 * This overwrites the existing advertising data packet and scan response packet
 * on this advertising set regardless of whether the data was set for the legacy
 * or extended advertising.
 *
 * If advertising mode is currently enabled, the new advertising data will be
 * used immediately. To enable advertising mode, use command @ref
 * sl_bt_legacy_advertiser_start.
 *
 * The stack generates the advertising data and scan response packet using the
 * following logic.
 *   1. Add a flags field to advertising data.
 *   2. Add a TX power level field to advertising data if the TX power service
 *      exists in the local GATT database.
 *   3. Add a peripheral connection interval range field to advertising data if
 *      the GAP peripheral preferred connection parameters characteristic exists
 *      in the local GATT database.
 *   4. Add a list of 16-bit service UUIDs to advertising data if there are one
 *      or more 16-bit service UUIDs to advertise. The list is complete if all
 *      advertised 16-bit UUIDs are in advertising data. Otherwise, the list is
 *      incomplete.
 *   5. Add a list of 128-bit service UUIDs to advertising data if there are one
 *      or more 128-bit service UUIDs to advertise and there is still free space
 *      for this field. The list is complete if all advertised 128-bit UUIDs are
 *      in advertising data. Otherwise, the list is incomplete. Note that an
 *      advertising data packet can contain at most one 128-bit service UUID.
 *   6. Try to add the full local name to advertising data if the device is not
 *      in privacy mode. If the full local name does not fit into the remaining
 *      free space, the advertised name is a shortened version by cutting off
 *      the end if the free space has at least 6 bytes. Otherwise, the local
 *      name is added to scan response data.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] discover Enum @ref sl_bt_advertiser_discovery_mode_t. The
 *   discovery mode for the Flags data field in the packet. Values:
 *     - <b>sl_bt_advertiser_non_discoverable (0x0):</b> Not discoverable
 *     - <b>sl_bt_advertiser_limited_discoverable (0x1):</b> Discoverable by
 *       both limited and general discovery procedures
 *     - <b>sl_bt_advertiser_general_discoverable (0x2):</b> Discoverable by the
 *       general discovery procedure
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_legacy_advertiser_generate_data(uint8_t advertising_set,
                                                  uint8_t discover);

/***************************************************************************//**
 *
 * Start undirected legacy advertising on an advertising set with the specified
 * connection mode. Use @ref sl_bt_advertiser_stop to stop the advertising.
 *
 * Use the @ref sl_bt_legacy_advertiser_set_data or @ref
 * sl_bt_legacy_advertiser_generate_data command to set the advertising data
 * before calling this command. The advertising data is added into the
 * advertising data packet and scan response packet if the connection mode is
 * connectable and/or scannable. The data is only added into the advertising
 * data packet when the connection mode is non-connectable and non-scannable.
 *
 * The number of concurrent connectable advertisings is limited by the number of
 * connections reserved by the user application (the
 * SL_BT_CONFIG_MAX_CONNECTIONS configuration) and the number reserved by other
 * software components (the SL_BT_COMPONENT_CONNECTIONS configuration). This
 * command fails with the connection limit exceeded error if it may cause the
 * number of connections exceeding the configured value in future. For example,
 * only one connectable advertising can be enabled if the device has
 * (SL_BT_CONFIG_MAX_CONNECTIONS + SL_BT_COMPONENT_CONNECTIONS - 1) connections.
 * This limitation does not apply to non-connectable advertising.
 *
 * This command fails with the invalid parameter error if non-resolvable random
 * address is used but the connection mode is @ref
 * sl_bt_legacy_advertiser_connectable.
 *
 * Event @ref sl_bt_evt_connection_opened will be received when a remote device
 * opens a connection to the advertiser on this advertising set. As a result,
 * the advertising stops.
 *
 * Event @ref sl_bt_evt_advertiser_timeout will be received when the number of
 * advertising events set by @ref sl_bt_advertiser_set_timing command is done
 * and the advertising has stopped.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] connect Enum @ref sl_bt_legacy_advertiser_connection_mode_t.
 *   Connection mode. Values:
 *     - <b>sl_bt_legacy_advertiser_non_connectable (0x0):</b> Undirected
 *       non-connectable and non-scannable legacy advertising
 *     - <b>sl_bt_legacy_advertiser_connectable (0x2):</b> Undirected
 *       connectable and scannable legacy advertising
 *     - <b>sl_bt_legacy_advertiser_scannable (0x3):</b> Undirected scannable
 *       and non-connectable legacy advertising
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_advertiser_timeout - Triggered when the number of
 *     advertising events set by @ref sl_bt_advertiser_set_timing command is
 *     done and the advertising has stopped.
 *   - @ref sl_bt_evt_connection_opened - Triggered when a remote device opens a
 *     connection to the advertiser and the advertising has stopped.
 *
 ******************************************************************************/
sl_status_t sl_bt_legacy_advertiser_start(uint8_t advertising_set,
                                          uint8_t connect);

/***************************************************************************//**
 *
 * Start directed legacy advertising on an advertising set with the specified
 * peer target device and connection mode. Use @ref sl_bt_advertiser_stop to
 * stop the advertising.
 *
 * Directed legacy advertising does not allow any advertising data. When the
 * connection mode is @ref
 * sl_bt_legacy_advertiser_high_duty_directed_connectable, the stack defaults
 * the advertising duration to 0.64 s if the application has not set the
 * parameter. The duration is reduced to 1.28 s if the application has set a
 * larger duration value.
 *
 * The number of concurrent connectable advertisings is limited by the
 * connection number configuration. See @ref sl_bt_legacy_advertiser_start for
 * more details.
 *
 * This command fails with the invalid parameter error if non-resolvable random
 * address is set as the advertising address.
 *
 * Event @ref sl_bt_evt_connection_opened will be received when the target
 * device opens a connection to the advertiser on this advertising set. As a
 * result, the advertising stops.
 *
 * Event @ref sl_bt_evt_advertiser_timeout will be received when the advertising
 * stops and no Bluetooth connection is opened to it.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] connect Enum @ref
 *   sl_bt_legacy_advertiser_directed_connection_mode_t. Connection mode.
 *   Values:
 *     - <b>sl_bt_legacy_advertiser_high_duty_directed_connectable (0x1):</b>
 *       High duty cycle directed connectable legacy advertising
 *     - <b>sl_bt_legacy_advertiser_low_duty_directed_connectable (0x5):</b> Low
 *       duty cycle directed connectable legacy advertising
 * @param[in] peer_addr Address of the peer target device the advertising is
 *   directed to
 * @param[in] peer_addr_type @parblock
 *   Enum @ref sl_bt_gap_address_type_t.
 *
 *   Peer target device address type.
 *
 *   If the application does not include the
 *   bluetooth_feature_use_accurate_api_address_types component, @p
 *   peer_addr_type uses the following values:
 *     - <b>0:</b> Public address
 *     - <b>1:</b> Random address
 *
 *   If the application includes the
 *   bluetooth_feature_use_accurate_api_address_types component, @p
 *   peer_addr_type uses enum @ref sl_bt_gap_address_type_t values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_advertiser_timeout - Triggered when the number of
 *     advertising events set by @ref sl_bt_advertiser_set_timing command is
 *     done and the advertising has stopped.
 *   - @ref sl_bt_evt_connection_opened - Triggered when a remote device opens a
 *     connection to the advertiser and the advertising has stopped.
 *
 ******************************************************************************/
sl_status_t sl_bt_legacy_advertiser_start_directed(uint8_t advertising_set,
                                                   uint8_t connect,
                                                   bd_addr peer_addr,
                                                   uint8_t peer_addr_type);

/** @} */ // end addtogroup sl_bt_legacy_advertiser

/**
 * @addtogroup sl_bt_extended_advertiser Extended Advertiser
 * @{
 *
 * @brief Extended Advertiser
 *
 * The commands and events in this class are related to extended advertising
 * functionalities in GAP peripheral and broadcaster roles.
 */

/* Command and Response IDs */
#define sl_bt_cmd_extended_advertiser_set_phy_id                     0x00570020
#define sl_bt_cmd_extended_advertiser_set_data_id                    0x01570020
#define sl_bt_cmd_extended_advertiser_set_long_data_id               0x02570020
#define sl_bt_cmd_extended_advertiser_generate_data_id               0x03570020
#define sl_bt_cmd_extended_advertiser_start_id                       0x04570020
#define sl_bt_cmd_extended_advertiser_start_directed_id              0x05570020
#define sl_bt_rsp_extended_advertiser_set_phy_id                     0x00570020
#define sl_bt_rsp_extended_advertiser_set_data_id                    0x01570020
#define sl_bt_rsp_extended_advertiser_set_long_data_id               0x02570020
#define sl_bt_rsp_extended_advertiser_generate_data_id               0x03570020
#define sl_bt_rsp_extended_advertiser_start_id                       0x04570020
#define sl_bt_rsp_extended_advertiser_start_directed_id              0x05570020

/**
 * @brief These values define the available connection modes in extended
 * advertising.
 */
typedef enum
{
  sl_bt_extended_advertiser_non_connectable = 0x0, /**< (0x0) Non-connectable
                                                        and non-scannable
                                                        extended advertising */
  sl_bt_extended_advertiser_scannable       = 0x3, /**< (0x3) Scannable extended
                                                        advertising */
  sl_bt_extended_advertiser_connectable     = 0x4  /**< (0x4) Connectable
                                                        extended advertising */
} sl_bt_extended_advertiser_connection_mode_t;

/**
 * @addtogroup sl_bt_extended_advertiser_flags Extended Advertising Configuration Flags
 * @{
 *
 * This enum defines configuration flags for the extended advertising.
 */

/** Omit advertiser's address from all PDUs (anonymous advertising). The
 * advertising cannot be connectable or scannable if this flag is set. */
#define SL_BT_EXTENDED_ADVERTISER_ANONYMOUS_ADVERTISING 0x1       

/** Include the TX power in advertising packets. */
#define SL_BT_EXTENDED_ADVERTISER_INCLUDE_TX_POWER      0x2       

/** @} */ // end Extended Advertising Configuration Flags

/***************************************************************************//**
 *
 * Set the primary and secondary advertising PHYs used for extended and periodic
 * advertising on an advertising set. This setting will take effect next time
 * extended or periodic advertising is enabled. When advertising on the LE Coded
 * PHY, coding scheme S=8 is used. The SL_STATUS_INVALID_PARAMETER error is
 * returned if a PHY value is invalid or the device does not support a given
 * PHY.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] primary_phy @parblock
 *   Enum @ref sl_bt_gap_phy_t. The PHY on which the advertising packets are
 *   transmitted on the primary advertising channel. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8)
 *
 *     - <b>Default</b> : @ref sl_bt_gap_phy_1m
 *   @endparblock
 * @param[in] secondary_phy @parblock
 *   Enum @ref sl_bt_gap_phy_t. The PHY on which the advertising packets are
 *   transmitted on the secondary advertising channel. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8)
 *
 *     - <b>Default</b> : @ref sl_bt_gap_phy_1m
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_set_phy(uint8_t advertising_set,
                                              uint8_t primary_phy,
                                              uint8_t secondary_phy);

/***************************************************************************//**
 *
 * Set user-defined data for extended advertising. This overwrites the existing
 * advertising data packet and scan response packet on this advertising set
 * regardless of whether the data was set for the legacy or extended
 * advertising. Maximum 191 bytes of data can be set for connectable extended
 * advertising. Maximum 253 bytes of data can be set for non-connectable
 * extended advertising. For setting longer advertising data, use command @ref
 * sl_bt_extended_advertiser_set_long_data.
 *
 * If advertising mode is currently enabled, the new advertising data will be
 * used immediately. Advertising mode can be enabled using command @ref
 * sl_bt_extended_advertiser_start.
 *
 * The invalid parameter error is returned if the data is too long to fit into a
 * single advertisement.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] data_len Length of data in @p data
 * @param[in] data Data to be set
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_set_data(uint8_t advertising_set,
                                               size_t data_len,
                                               const uint8_t* data);

/***************************************************************************//**
 *
 * Set long user-defined data for extended advertising. This overwrites the
 * existing advertising data packet and scan response packet on this advertising
 * set regardless of whether the data was set for the legacy or extended
 * advertising.
 *
 * Prior to calling this command, add data to the buffer with one or multiple
 * calls to @ref sl_bt_system_data_buffer_write. When this command is called,
 * the data in the system data buffer is extracted as the advertising data. The
 * buffer will be emptied after this command regardless of the completion
 * status.
 *
 * Maximum 191 bytes of data can be set for connectable extended advertising.
 * Maximum 1650 bytes of data can be set for non-connectable extended
 * advertising. Advertising parameters may limit the amount of data that can be
 * sent in a single advertisement. See @ref sl_bt_extended_advertiser_set_data
 * for more details.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_set_long_data(uint8_t advertising_set);

/***************************************************************************//**
 *
 * Ask the stack to generate the extended advertising data on an advertising
 * set. Alternatively, user-defined advertising data can be set using the @ref
 * sl_bt_extended_advertiser_set_data command.
 *
 * This overwrites the existing advertising data packet and scan response packet
 * on this advertising set regardless of whether the data was set for the legacy
 * or extended advertising.
 *
 * If advertising mode is currently enabled, the new advertising data will be
 * used immediately. To enable advertising mode, use command @ref
 * sl_bt_extended_advertiser_start.
 *
 * See @ref sl_bt_legacy_advertiser_generate_data for the advertising data
 * generation logic.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] discover Enum @ref sl_bt_advertiser_discovery_mode_t. The
 *   discovery mode for the Flags data field in the packet. Values:
 *     - <b>sl_bt_advertiser_non_discoverable (0x0):</b> Not discoverable
 *     - <b>sl_bt_advertiser_limited_discoverable (0x1):</b> Discoverable by
 *       both limited and general discovery procedures
 *     - <b>sl_bt_advertiser_general_discoverable (0x2):</b> Discoverable by the
 *       general discovery procedure
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_generate_data(uint8_t advertising_set,
                                                    uint8_t discover);

/***************************************************************************//**
 *
 * Start undirected extended advertising on an advertising set with the
 * specified connection mode. Use @ref sl_bt_advertiser_stop to stop the
 * advertising.
 *
 * Use the @ref sl_bt_extended_advertiser_set_data or @ref
 * sl_bt_extended_advertiser_generate_data command to set the advertising data
 * before calling this command. Advertising data is added into the scan response
 * packet if the connection mode is scannable. Otherwise, data is in the
 * advertising data packet.
 *
 * The number of concurrent connectable advertisings is limited by the
 * connection number configuration. See @ref sl_bt_legacy_advertiser_start for
 * more details.
 *
 * This command fails with the invalid parameter error if the advertising uses a
 * non-resolvable random address but the connection mode is @ref
 * sl_bt_extended_advertiser_connectable.
 *
 * This command returns the error SL_STATUS_BT_CONTROLLER_COMMAND_DISALLOWED
 * when the connection mode is @ref sl_bt_extended_advertiser_scannable but no
 * scan response data is provided.
 *
 * Event @ref sl_bt_evt_connection_opened will be received when a remote device
 * opens a connection to the advertiser on this advertising set. As a result,
 * the advertising stops.
 *
 * Event @ref sl_bt_evt_advertiser_timeout will be received when the number of
 * advertising events set by @ref sl_bt_advertiser_set_timing command is done
 * and the advertising has stopped.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] connect Enum @ref sl_bt_extended_advertiser_connection_mode_t.
 *   Connection mode. Values:
 *     - <b>sl_bt_extended_advertiser_non_connectable (0x0):</b> Non-connectable
 *       and non-scannable extended advertising
 *     - <b>sl_bt_extended_advertiser_scannable (0x3):</b> Scannable extended
 *       advertising
 *     - <b>sl_bt_extended_advertiser_connectable (0x4):</b> Connectable
 *       extended advertising
 * @param[in] flags Additional extended advertising options. Value: 0 or bitmask
 *   of @ref sl_bt_extended_advertiser_flags
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_advertiser_timeout - Triggered when the number of
 *     advertising events set by @ref sl_bt_advertiser_set_timing command is
 *     done and advertising has stopped on an advertising set.
 *   - @ref sl_bt_evt_connection_opened - Triggered when a remote device opens a
 *     connection to the advertiser and the advertising has stopped.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_start(uint8_t advertising_set,
                                            uint8_t connect,
                                            uint32_t flags);

/***************************************************************************//**
 *
 * Start directed extended advertising on an advertising set with the specified
 * peer target device and connection mode. Use @ref sl_bt_advertiser_stop to
 * stop the advertising.
 *
 * The number of concurrent connectable advertisings is limited by the
 * connection number configuration. See @ref sl_bt_legacy_advertiser_start for
 * more details.
 *
 * This command fails with the invalid parameter error if the advertising uses a
 * non-resolvable random address but the connection mode is @ref
 * sl_bt_extended_advertiser_connectable.
 *
 * Event @ref sl_bt_evt_connection_opened will be received when the target
 * device opens a connection to the advertiser on this advertising set. As a
 * result, the advertising stops.
 *
 * Event @ref sl_bt_evt_advertiser_timeout will be received when the advertising
 * stops and no Bluetooth connection is opened to it.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] connect Enum @ref sl_bt_extended_advertiser_connection_mode_t.
 *   Connection mode. Values:
 *     - <b>sl_bt_extended_advertiser_non_connectable (0x0):</b> Non-connectable
 *       and non-scannable extended advertising
 *     - <b>sl_bt_extended_advertiser_scannable (0x3):</b> Scannable extended
 *       advertising
 *     - <b>sl_bt_extended_advertiser_connectable (0x4):</b> Connectable
 *       extended advertising
 * @param[in] flags Additional extended advertising options. Value: 0 or bitmask
 *   of @ref sl_bt_extended_advertiser_flags
 * @param[in] peer_addr Address of the peer target device the advertising is
 *   directed to
 * @param[in] peer_addr_type @parblock
 *   Enum @ref sl_bt_gap_address_type_t.
 *
 *   Peer target device address type.
 *
 *   If the application does not include the
 *   bluetooth_feature_use_accurate_api_address_types component, @p
 *   peer_addr_type uses the following values:
 *     - <b>0:</b> Public address
 *     - <b>1:</b> Random address
 *
 *   If the application includes the
 *   bluetooth_feature_use_accurate_api_address_types component, @p
 *   peer_addr_type uses enum @ref sl_bt_gap_address_type_t values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_advertiser_timeout - Triggered when the number of
 *     advertising events set by @ref sl_bt_advertiser_set_timing command is
 *     done and the advertising has stopped.
 *   - @ref sl_bt_evt_connection_opened - Triggered when a remote device opens a
 *     connection to the advertiser and the advertising has stopped.
 *
 ******************************************************************************/
sl_status_t sl_bt_extended_advertiser_start_directed(uint8_t advertising_set,
                                                     uint8_t connect,
                                                     uint32_t flags,
                                                     bd_addr peer_addr,
                                                     uint8_t peer_addr_type);

/** @} */ // end addtogroup sl_bt_extended_advertiser

/**
 * @addtogroup sl_bt_periodic_advertiser Periodic Advertiser
 * @{
 *
 * @brief Periodic Advertiser
 *
 * Provides support for advertising with periodic advertising trains that do not
 * have subevents or response slots.
 */

/* Command and Response IDs */
#define sl_bt_cmd_periodic_advertiser_set_data_id                    0x00580020
#define sl_bt_cmd_periodic_advertiser_set_long_data_id               0x01580020
#define sl_bt_cmd_periodic_advertiser_start_id                       0x02580020
#define sl_bt_cmd_periodic_advertiser_stop_id                        0x03580020
#define sl_bt_rsp_periodic_advertiser_set_data_id                    0x00580020
#define sl_bt_rsp_periodic_advertiser_set_long_data_id               0x01580020
#define sl_bt_rsp_periodic_advertiser_start_id                       0x02580020
#define sl_bt_rsp_periodic_advertiser_stop_id                        0x03580020

/**
 * @addtogroup sl_bt_periodic_advertiser_flags Periodic Advertising Configuration Flags
 * @{
 *
 * Defines configuration flags for periodic advertising.
 */

/** Include the TX power in advertising packets. */
#define SL_BT_PERIODIC_ADVERTISER_INCLUDE_TX_POWER                0x1       

/** Automatically start the extended advertising on the advertising set. The
 * advertising will be started in non-connectable and non-scannable mode. */
#define SL_BT_PERIODIC_ADVERTISER_AUTO_START_EXTENDED_ADVERTISING 0x2       

/** @} */ // end Periodic Advertising Configuration Flags

/**
 * @addtogroup sl_bt_evt_periodic_advertiser_status sl_bt_evt_periodic_advertiser_status
 * @{
 * @brief This event reports the latest status of periodic advertising on an
 * advertising set
 *
 * It is generated for each scheduled periodic advertising transmission when the
 * status report has been enabled. By default, this event is disabled in the
 * Bluetooth stack. Use command @ref sl_bt_system_linklayer_configure and key
 * @ref sl_bt_system_linklayer_config_key_set_periodic_advertising_status_report
 * to enable or disable it.
 */

/** @brief Identifier of the status event */
#define sl_bt_evt_periodic_advertiser_status_id                      0x005800a0

/***************************************************************************//**
 * @brief Data structure of the status event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_periodic_advertiser_status_s
{
  uint8_t  advertising_set; /**< The advertising set handle that the periodic
                                 advertising is from */
  uint32_t status;          /**< The status value.

                                 Bit 0:
                                   - <b>0:</b> The transmission was successful.
                                   - <b>1:</b> The transmission failed.

                                 Other bits: Unused and reserved for future */
  uint16_t event_counter;   /**< The event counter of the transmission */
});

typedef struct sl_bt_evt_periodic_advertiser_status_s sl_bt_evt_periodic_advertiser_status_t;

/** @} */ // end addtogroup sl_bt_evt_periodic_advertiser_status

/***************************************************************************//**
 *
 * Set the data for periodic advertising on an advertising set. Maximum 254
 * bytes of data can be set with this command. For setting longer advertising
 * data, use command @ref sl_bt_periodic_advertiser_set_long_data.
 *
 * If the periodic advertising is currently enabled, the new advertising data
 * will be used immediately. Periodic advertising can be enabled using the
 * command @ref sl_bt_periodic_advertiser_start.
 *
 * The invalid parameter error will be returned if the data is too long to fit
 * into the advertisement.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] data_len Length of data in @p data
 * @param[in] data Data to be set
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_periodic_advertiser_set_data(uint8_t advertising_set,
                                               size_t data_len,
                                               const uint8_t* data);

/***************************************************************************//**
 *
 * Set data for periodic advertising on an advertising set. Data currently in
 * the system data buffer will be extracted as the advertising data. The buffer
 * will be emptied after this command regardless of the completion status.
 *
 * Prior to calling this command, add data to the buffer with one or multiple
 * calls to @ref sl_bt_system_data_buffer_write.
 *
 * Maximum 1650 bytes of data can be set for periodic advertising. Advertising
 * parameters may limit the amount of data that can be sent.
 *
 * See @ref sl_bt_periodic_advertiser_set_data for more details.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_periodic_advertiser_set_long_data(uint8_t advertising_set);

/***************************************************************************//**
 *
 * Start periodic advertising on an advertising set.
 *
 * According to the Bluetooth Core specification, periodic advertising PDUs
 * cannot be transmitted until at least one extended advertising event has been
 * completed. If the application needs exact control over the extended
 * advertising data and parameters, use the @ref sl_bt_advertiser class to
 * configure the parameters of the advertising set and the @ref
 * sl_bt_extended_advertiser class to set or generate the desired extended
 * advertising data payload. If the application does not configure the
 * parameters or set the data, the default parameters and empty advertising data
 * are used for the extended advertising.
 *
 * If the application has not already started extended advertising and the flag
 * @ref SL_BT_PERIODIC_ADVERTISER_AUTO_START_EXTENDED_ADVERTISING is set in @p
 * flags, the stack will automatically start extended advertising with the
 * parameters and extended advertising data currently configured to the
 * advertising set. The application may stop the automatically started extended
 * advertising using the @ref sl_bt_advertiser_stop command.
 *
 * If the application has not already started extended advertising and the flag
 * @ref SL_BT_PERIODIC_ADVERTISER_AUTO_START_EXTENDED_ADVERTISING is not set in
 * @p flags, the stack will momentarily start extended advertising with the
 * parameters and extended advertising data currently configured to the
 * advertising set. Unless the application starts extended advertising before
 * the first extended advertising event has completed, the stack will
 * automatically stop the momentary extended advertising after the first
 * extended advertising event.
 *
 * Periodic advertising PDUs are transmitted on the secondary PHY configured for
 * the advertising set with the @ref sl_bt_extended_advertiser_set_phy command.
 *
 * Use @ref sl_bt_periodic_advertiser_stop command to stop the periodic
 * advertising.
 *
 * @param[in] advertising_set Advertising set handle
 * @param[in] interval_min @parblock
 *   Minimum periodic advertising interval. Value in units of 1.25 ms
 *     - <b>Range:</b> 0x06 to 0xFFFF
 *
 *     - Time range: 7.5 ms to 81.92 s
 *
 *     - <b>Default</b> : 100 ms
 *   @endparblock
 * @param[in] interval_max @parblock
 *   Maximum periodic advertising interval. Value in units of 1.25 ms
 *     - <b>Range:</b> 0x06 to 0xFFFF
 *
 *     - Time range: 7.5 ms to 81.92 s
 *     - Note: interval_max should be bigger than interval_min
 *
 *     - <b>Default</b> : 200 ms
 *   @endparblock
 * @param[in] flags Additional periodic advertising options. Value: 0 or bitmask
 *   of @ref sl_bt_periodic_advertiser_flags
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_periodic_advertiser_start(uint8_t advertising_set,
                                            uint16_t interval_min,
                                            uint16_t interval_max,
                                            uint32_t flags);

/***************************************************************************//**
 *
 * Stop the periodic advertising on an advertising set. Counterpart to @ref
 * sl_bt_periodic_advertiser_start.
 *
 * This command does not affect the enable state of the legacy or extended
 * advertising on the advertising set, i.e., the legacy or extended advertising
 * is not stopped.
 *
 * @param[in] advertising_set Advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_periodic_advertiser_stop(uint8_t advertising_set);

/** @} */ // end addtogroup sl_bt_periodic_advertiser

/**
 * @addtogroup sl_bt_scanner Scanner
 * @{
 *
 * @brief Scanner
 *
 * The commands and events in this class provides scanning functionality. There
 * are functionality differences depending on included components:
 *
 * The bluetooth_feature_legacy_scanner component is included but the
 * bluetooth_feature_extended_scanner is not:
 *   - The scanner can only scan advertising devices that use legacy advertising
 *     PDUs.
 *   - Received advertisements are reported in @ref
 *     sl_bt_evt_scanner_legacy_advertisement_report events.
 *
 * The bluetooth_feature_extended_scanner component is included:
 *   - The scanner can scan advertising devices that use legacy or extended
 *     advertising PDUs.
 *   - Received advertisements that use legacy advertising PDUs are reported in
 *     @ref sl_bt_evt_scanner_legacy_advertisement_report events.
 *   - Received advertisements that use extended advertising PDUs are reported
 *     in @ref sl_bt_evt_scanner_extended_advertisement_report events.
 */

/* Command and Response IDs */
#define sl_bt_cmd_scanner_set_parameters_id                          0x06050020
#define sl_bt_cmd_scanner_set_parameters_and_filter_id               0x07050020
#define sl_bt_cmd_scanner_start_id                                   0x03050020
#define sl_bt_cmd_scanner_stop_id                                    0x05050020
#define sl_bt_rsp_scanner_set_parameters_id                          0x06050020
#define sl_bt_rsp_scanner_set_parameters_and_filter_id               0x07050020
#define sl_bt_rsp_scanner_start_id                                   0x03050020
#define sl_bt_rsp_scanner_stop_id                                    0x05050020

/**
 * @brief These values indicate which Bluetooth discovery mode to use when
 * scanning for advertising devices.
 */
typedef enum
{
  sl_bt_scanner_discover_limited     = 0x0, /**< (0x0) Discover only limited
                                                 discoverable devices. */
  sl_bt_scanner_discover_generic     = 0x1, /**< (0x1) Discover limited and
                                                 general discoverable devices. */
  sl_bt_scanner_discover_observation = 0x2  /**< (0x2) Discover
                                                 non-discoverable, limited and
                                                 general discoverable devices. */
} sl_bt_scanner_discover_mode_t;

/**
 * @brief The scanning modes.
 */
typedef enum
{
  sl_bt_scanner_scan_mode_passive = 0x0, /**< (0x0) Passive scanning mode where
                                              the device only listens to
                                              advertising packets and does not
                                              transmit packets */
  sl_bt_scanner_scan_mode_active  = 0x1  /**< (0x1) Active scanning mode where
                                              the device sends out a scan
                                              request packet upon receiving a
                                              scannable advertising packet from
                                              a remote device and listens to the
                                              scan response packet from the
                                              remote device */
} sl_bt_scanner_scan_mode_t;

/**
 * @brief The enum defines the scanning PHYs.
 */
typedef enum
{
  sl_bt_scanner_scan_phy_1m           = 0x1, /**< (0x1) Initiate the scanning on
                                                  the 1M PHY */
  sl_bt_scanner_scan_phy_coded        = 0x4, /**< (0x4) Initiate the scanning on
                                                  the Coded PHY */
  sl_bt_scanner_scan_phy_1m_and_coded = 0x5  /**< (0x5) Simultaneous scanning by
                                                  initiating the scanning on the
                                                  1M and Coded PHY alternatively */
} sl_bt_scanner_scan_phy_t;

/**
 * @brief Defines the data completeness status types of an advertisement
 * reported by the scanner.
 */
typedef enum
{
  sl_bt_scanner_data_status_complete          = 0x0, /**< (0x0) All data of the
                                                          advertisement has been
                                                          reported. */
  sl_bt_scanner_data_status_incomplete_more   = 0x1, /**< (0x1) Data of the
                                                          advertisement is
                                                          incomplete in this
                                                          event, and more data
                                                          will come in new
                                                          events. */
  sl_bt_scanner_data_status_incomplete_nomore = 0x2  /**< (0x2) Data of the
                                                          advertisement is
                                                          incomplete in this
                                                          event, but no more
                                                          data will come, i.e.,
                                                          the data of the
                                                          advertisement is
                                                          truncated. */
} sl_bt_scanner_data_status_t;

/**
 * @brief The scanning filter policy setting determines which advertisements and
 * scan responses are delivered to the application when scanning. See the
 * Bluetooth Core specification Volume 6, Part B, Section 4.3.3 "Scanning filter
 * policy" for a detailed description of this setting.
 *
 * Note that some filter policies require the application to include additional
 * Bluetooth feature components. Filter policies that use the Filter Accept List
 * require that the application has included the bluetooth_feature_accept_list
 * component. Filter policies that require the Bluetooth controller to resolve a
 * Resolvable Private Address require that the application has included the
 * bluetooth_feature_resolving_list component.
 */
typedef enum
{
  sl_bt_scanner_filter_policy_basic_unfiltered    = 0x0, /**< (0x0) Advertising
                                                              and scan response
                                                              PDUs are processed
                                                              from all devices.
                                                              For directed
                                                              advertising, the
                                                              target address
                                                              must additionally
                                                              match the identity
                                                              address of the
                                                              local device or be
                                                              a Resolvable
                                                              Private Address
                                                              that is resolved
                                                              to the local
                                                              device by the
                                                              Bluetooth
                                                              controller. */
  sl_bt_scanner_filter_policy_basic_filtered      = 0x1, /**< (0x1) Advertising
                                                              and scan response
                                                              PDUs are processed
                                                              only from devices
                                                              that the
                                                              application has
                                                              added to the
                                                              Filter Accept
                                                              List. For directed
                                                              advertising, the
                                                              target address
                                                              must additionally
                                                              match the identity
                                                              address of the
                                                              local device or be
                                                              a Resolvable
                                                              Private Address
                                                              that is resolved
                                                              to the local
                                                              device by the
                                                              Bluetooth
                                                              controller. */
  sl_bt_scanner_filter_policy_extended_unfiltered = 0x2, /**< (0x2) Advertising
                                                              and scan response
                                                              PDUs are processed
                                                              from all devices.
                                                              For directed
                                                              advertising, the
                                                              target address
                                                              must additionally
                                                              match the identity
                                                              address of the
                                                              local device or be
                                                              any Resolvable
                                                              Private Address. */
  sl_bt_scanner_filter_policy_extended_filtered   = 0x3  /**< (0x3) Advertising
                                                              and scan response
                                                              PDUs are processed
                                                              only from devices
                                                              that the
                                                              application has
                                                              added to the
                                                              Filter Accept
                                                              List. For directed
                                                              advertising, the
                                                              target address
                                                              must additionally
                                                              match the identity
                                                              address of the
                                                              local device or be
                                                              any Resolvable
                                                              Private Address. */
} sl_bt_scanner_filter_policy_t;

/**
 * @addtogroup sl_bt_scanner_event_flag Event Type Flags of Advertisement Reports
 * @{
 *
 * Defines the event type flags of the advertisement packets the scanner
 * reports. An advertisement packet could have multiple applicable flags.
 */

/** A connectable advertising data packet */
#define SL_BT_SCANNER_EVENT_FLAG_CONNECTABLE   0x1       

/** A scannable advertising data packet */
#define SL_BT_SCANNER_EVENT_FLAG_SCANNABLE     0x2       

/** Directed advertising */
#define SL_BT_SCANNER_EVENT_FLAG_DIRECTED      0x4       

/** A scan response packet that can be received in active scan mode only */
#define SL_BT_SCANNER_EVENT_FLAG_SCAN_RESPONSE 0x8       

/** @} */ // end Event Type Flags of Advertisement Reports

/**
 * @addtogroup sl_bt_scanner_option_flags Option Flags to Specify Scanning Options
 * @{
 *
 * Defines the scanning option flags that can be set with the command @ref
 * sl_bt_scanner_set_parameters_and_filter.
 */

/**
 *
 * If the built-in bonding database is used (the component
 * bluetooth_feature_builtin_bonding_database is included in the application),
 * the scanner component by default tries to search for or resolve the
 * advertiser addresses to find if a bonding has been created with the
 * advertiser device. The bonding handle is then reported in the @p bonding
 * field of @ref sl_bt_evt_scanner_legacy_advertisement_report and @ref
 * sl_bt_evt_scanner_extended_advertisement_report events.
 *
 * If the application does not need the bonding information in advertisement
 * reports, set this scanner option to disable the search for the bonding. When
 * this option is set the @p bonding field in advertisement reports will always
 * be set to SL_BT_INVALID_BONDING_HANDLE (0xff).
 *
 * */
#define SL_BT_SCANNER_IGNORE_BONDING 0x1       

/** @} */ // end Option Flags to Specify Scanning Options

/**
 * @addtogroup sl_bt_evt_scanner_legacy_advertisement_report sl_bt_evt_scanner_legacy_advertisement_report
 * @{
 * @brief Reports an advertising data or scan response packet from an
 * advertising device that uses legacy advertising PDUs.
 */

/** @brief Identifier of the legacy_advertisement_report event */
#define sl_bt_evt_scanner_legacy_advertisement_report_id             0x000500a0

/***************************************************************************//**
 * @brief Data structure of the legacy_advertisement_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_scanner_legacy_advertisement_report_s
{
  uint8_t    event_flags;         /**< The event type flag(s) in the
                                       advertisement packet. Value: One or more
                                       flags defined in @ref
                                       sl_bt_scanner_event_flag */
  bd_addr    address;             /**< Advertiser address */
  uint8_t    address_type;        /**< Enum @ref sl_bt_gap_address_type_t.

                                       Advertiser address type.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses the
                                       following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses enum @ref
                                       sl_bt_gap_address_type_t values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8_t    bonding;             /**< Bonding handle if the remote advertising
                                       device has previously bonded with the
                                       local device. Values:
                                         - <b>SL_BT_INVALID_BONDING_HANDLE
                                           (0xff):</b> No bonding
                                         - <b>Other:</b> Bonding handle */
  int8_t     rssi;                /**< Signal strength indicator (RSSI) in the
                                       last received packet. Units: dBm
                                         - <b>Range:</b> -127 to +20 */
  uint8_t    channel;             /**< The channel number on which the last
                                       packet was received */
  bd_addr    target_address;      /**< The target address if the advertisement
                                       is from directed advertising, otherwise
                                       ignored */
  uint8_t    target_address_type; /**< Enum @ref sl_bt_gap_address_type_t.

                                       The target address type if the
                                       advertisement is from directed
                                       advertising, otherwise ignored.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p target_address_type uses
                                       the following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p target_address_type uses
                                       enum @ref sl_bt_gap_address_type_t
                                       values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8array data;                /**< Advertising or scan response data. Ignore
                                       if this is directed advertising. */
});

typedef struct sl_bt_evt_scanner_legacy_advertisement_report_s sl_bt_evt_scanner_legacy_advertisement_report_t;

/** @} */ // end addtogroup sl_bt_evt_scanner_legacy_advertisement_report

/**
 * @addtogroup sl_bt_evt_scanner_extended_advertisement_report sl_bt_evt_scanner_extended_advertisement_report
 * @{
 * @brief Reports an advertising or scan response packet from an advertising
 * device that uses extended advertising PDUs
 *
 * Multiple events may be reported for single advertisement train.
 */

/** @brief Identifier of the extended_advertisement_report event */
#define sl_bt_evt_scanner_extended_advertisement_report_id           0x020500a0

/***************************************************************************//**
 * @brief Data structure of the extended_advertisement_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_scanner_extended_advertisement_report_s
{
  uint8_t    event_flags;         /**< The event type flag(s) in the
                                       advertisement packet. Value: One or more
                                       flags defined in @ref
                                       sl_bt_scanner_event_flag */
  bd_addr    address;             /**< Advertiser address */
  uint8_t    address_type;        /**< Enum @ref sl_bt_gap_address_type_t.

                                       Advertiser address type.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses the
                                       following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address
                                         - <b>255:</b> No address provided
                                           (anonymous advertising)

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses enum @ref
                                       sl_bt_gap_address_type_t values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_anonymous_address
                                           (0xff):</b> No address provided
                                           (anonymous advertising)
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8_t    bonding;             /**< Bonding handle if the remote advertising
                                       device has previously bonded with the
                                       local device. Values:
                                         - <b>SL_BT_INVALID_BONDING_HANDLE
                                           (0xff):</b> No bonding
                                         - <b>Other:</b> Bonding handle */
  int8_t     rssi;                /**< Signal strength indicator (RSSI) in the
                                       last received packet. Units: dBm
                                         - <b>Range:</b> -127 to +20 */
  uint8_t    channel;             /**< The channel number on which the last
                                       packet was received */
  bd_addr    target_address;      /**< The target address if this is directed
                                       advertising, otherwise ignored */
  uint8_t    target_address_type; /**< Enum @ref sl_bt_gap_address_type_t.

                                       The target address type if the
                                       advertisement is from directed
                                       advertising, otherwise ignored.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p target_address_type uses
                                       the following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p target_address_type uses
                                       enum @ref sl_bt_gap_address_type_t
                                       values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8_t    adv_sid;             /**< Advertising set identifier */
  uint8_t    primary_phy;         /**< Enum @ref sl_bt_gap_phy_t. The PHY on
                                       which advertising packets are transmitted
                                       on the primary advertising channel
                                       Values:
                                         - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                         - <b>sl_bt_gap_phy_coded (0x4):</b>
                                           Coded PHY, 125k (S=8) or 500k (S=2) */
  uint8_t    secondary_phy;       /**< Enum @ref sl_bt_gap_phy_t. The PHY on
                                       which advertising packets are transmitted
                                       on the secondary advertising channel
                                       Values:
                                         - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                         - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
                                         - <b>sl_bt_gap_phy_coded (0x4):</b>
                                           Coded PHY, 125k (S=8) or 500k (S=2) */
  int8_t     tx_power;            /**< TX power value in the received packet
                                       header. Units: dBm
                                         - <b>Range:</b> -127 to 126

                                         - Value 127: information unavailable */
  uint16_t   periodic_interval;   /**< The periodic advertising interval. Value
                                       0 indicates no periodic advertising.
                                       Otherwise,
                                         - <b>Range:</b> 0x06 to 0xFFFF

                                         - Unit: 1.25 ms
                                         - Time range: 7.5 ms to 81.92 s */
  uint8_t    data_completeness;   /**< Enum @ref sl_bt_scanner_data_status_t.
                                       The data completeness status. Values:
                                         - <b>sl_bt_scanner_data_status_complete
                                           (0x0):</b> All data of the
                                           advertisement has been reported.
                                         - <b>sl_bt_scanner_data_status_incomplete_more
                                           (0x1):</b> Data of the advertisement
                                           is incomplete in this event, and more
                                           data will come in new events.
                                         - <b>sl_bt_scanner_data_status_incomplete_nomore
                                           (0x2):</b> Data of the advertisement
                                           is incomplete in this event, but no
                                           more data will come, i.e., the data
                                           of the advertisement is truncated. */
  uint8_t    counter;             /**< The monotonically increasing counter as
                                       the sequence number of the event. This
                                       counter is specific to @ref
                                       sl_bt_evt_scanner_extended_advertisement_report_id.
                                       It can be used to detect if one more @ref
                                       sl_bt_evt_scanner_extended_advertisement_report
                                       events were dropped due to temporarily
                                       out of resources or other reasons. Ignore
                                       this field if event loss is not a
                                       concern. */
  uint8array data;                /**< Advertising or scan response data */
});

typedef struct sl_bt_evt_scanner_extended_advertisement_report_s sl_bt_evt_scanner_extended_advertisement_report_t;

/** @} */ // end addtogroup sl_bt_evt_scanner_extended_advertisement_report

/***************************************************************************//**
 *
 * Set scan parameters for subsequent scanning operations. If the device is
 * currently scanning, new parameters will take effect when scanning is
 * restarted.
 *
 * This command sets the scanning filter policy to the default value @ref
 * sl_bt_scanner_filter_policy_basic_unfiltered. Use the command @ref
 * sl_bt_scanner_set_parameters_and_filter to set a specific scanning filter
 * policy.
 *
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_scanner_scan_mode_t.
 *
 *   Passive or active scan. Values:
 *     - <b>sl_bt_scanner_scan_mode_passive (0x0):</b> Passive scanning mode
 *       where the device only listens to advertising packets and does not
 *       transmit packets
 *     - <b>sl_bt_scanner_scan_mode_active (0x1):</b> Active scanning mode where
 *       the device sends out a scan request packet upon receiving a scannable
 *       advertising packet from a remote device and listens to the scan
 *       response packet from the remote device
 *
 *     - <b>Default</b> : @ref sl_bt_scanner_scan_mode_passive
 *   @endparblock
 * @param[in] interval @parblock
 *   The time interval when the device starts its last scan until it begins the
 *   subsequent scan. In other words, how often to scan
 *     - <b>Range:</b> 0x0004 to 0xFFFF
 *
 *     - Time = Value x 0.625 ms
 *     - Time Range: 2.5 ms to 40.96 s
 *
 *     - <b>Default</b> : 10 ms
 *
 *   A variable delay occurs when switching channels at the end of each scanning
 *   interval, which is included in the scanning interval time. During the
 *   switch time, advertising packets are not received by the device. The switch
 *   time variation is use case dependent. For example, if scanning while
 *   keeping active connections, the channel switch time might be longer than
 *   when scanning without any active connections. Increasing the scanning
 *   interval reduces the amount of time in which the device can't receive
 *   advertising packets because it switches channels less often.
 *
 *   After every scan interval, the scanner changes the frequency at which it
 *   operates. It cycles through all three advertising channels in a round robin
 *   fashion. According to the specification, all three channels must be used by
 *   a scanner.
 *   @endparblock
 * @param[in] window @parblock
 *   The scan window, i.e., the duration of the scan, which must be less than or
 *   equal to the @p interval
 *     - <b>Range:</b> 0x0004 to 0xFFFF
 *
 *     - Time = Value x 0.625 ms
 *     - Time Range: 2.5 ms to 40.96 s
 *
 *     - <b>Default</b> : 10 ms
 *
 *   Note that the packet reception is aborted if it's started just before the
 *   scan window ends.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_scanner_set_parameters(uint8_t mode,
                                         uint16_t interval,
                                         uint16_t window);

/***************************************************************************//**
 *
 * Set scan parameters and the scanning filter policy for subsequent scanning
 * operations. If the device is currently scanning, new parameters will take
 * effect when scanning is restarted.
 *
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_scanner_scan_mode_t.
 *
 *   Passive or active scan. Values:
 *     - <b>sl_bt_scanner_scan_mode_passive (0x0):</b> Passive scanning mode
 *       where the device only listens to advertising packets and does not
 *       transmit packets
 *     - <b>sl_bt_scanner_scan_mode_active (0x1):</b> Active scanning mode where
 *       the device sends out a scan request packet upon receiving a scannable
 *       advertising packet from a remote device and listens to the scan
 *       response packet from the remote device
 *
 *     - <b>Default</b> : @ref sl_bt_scanner_scan_mode_passive
 *   @endparblock
 * @param[in] interval @parblock
 *   The time interval when the device starts its last scan until it begins the
 *   subsequent scan. In other words, how often to scan
 *     - <b>Range:</b> 0x0004 to 0xFFFF
 *
 *     - Time = Value x 0.625 ms
 *     - Time Range: 2.5 ms to 40.96 s
 *
 *     - <b>Default</b> : 10 ms
 *
 *   A variable delay occurs when switching channels at the end of each scanning
 *   interval, which is included in the scanning interval time. During the
 *   switch time, advertising packets are not received by the device. The switch
 *   time variation is use case dependent. For example, if scanning while
 *   keeping active connections, the channel switch time might be longer than
 *   when scanning without any active connections. Increasing the scanning
 *   interval reduces the amount of time in which the device can't receive
 *   advertising packets because it switches channels less often.
 *
 *   After every scan interval, the scanner changes the frequency at which it
 *   operates. It cycles through all three advertising channels in a round robin
 *   fashion. According to the specification, all three channels must be used by
 *   a scanner.
 *   @endparblock
 * @param[in] window @parblock
 *   The scan window, i.e., the duration of the scan, which must be less than or
 *   equal to the @p interval
 *     - <b>Range:</b> 0x0004 to 0xFFFF
 *
 *     - Time = Value x 0.625 ms
 *     - Time Range: 2.5 ms to 40.96 s
 *
 *     - <b>Default</b> : 10 ms
 *
 *   Note that the packet reception is aborted if it's started just before the
 *   scan window ends.
 *   @endparblock
 * @param[in] flags Additional scanner options. Value: 0 or bitmask of @ref
 *   sl_bt_scanner_option_flags.
 * @param[in] filter_policy @parblock
 *   Enum @ref sl_bt_scanner_filter_policy_t.
 *
 *   The scanning filter policy to use when scanning is started. The filter
 *   policy determines which advertisements and scan responses are delivered to
 *   the application. Values:
 *     - <b>sl_bt_scanner_filter_policy_basic_unfiltered (0x0):</b> Advertising
 *       and scan response PDUs are processed from all devices. For directed
 *       advertising, the target address must additionally match the identity
 *       address of the local device or be a Resolvable Private Address that is
 *       resolved to the local device by the Bluetooth controller.
 *     - <b>sl_bt_scanner_filter_policy_basic_filtered (0x1):</b> Advertising
 *       and scan response PDUs are processed only from devices that the
 *       application has added to the Filter Accept List. For directed
 *       advertising, the target address must additionally match the identity
 *       address of the local device or be a Resolvable Private Address that is
 *       resolved to the local device by the Bluetooth controller.
 *     - <b>sl_bt_scanner_filter_policy_extended_unfiltered (0x2):</b>
 *       Advertising and scan response PDUs are processed from all devices. For
 *       directed advertising, the target address must additionally match the
 *       identity address of the local device or be any Resolvable Private
 *       Address.
 *     - <b>sl_bt_scanner_filter_policy_extended_filtered (0x3):</b> Advertising
 *       and scan response PDUs are processed only from devices that the
 *       application has added to the Filter Accept List. For directed
 *       advertising, the target address must additionally match the identity
 *       address of the local device or be any Resolvable Private Address.
 *
 *     - <b>Default</b> : @ref sl_bt_scanner_filter_policy_basic_unfiltered
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_scanner_set_parameters_and_filter(uint8_t mode,
                                                    uint16_t interval,
                                                    uint16_t window,
                                                    uint32_t flags,
                                                    uint8_t filter_policy);

/***************************************************************************//**
 *
 * Start the GAP discovery procedure to scan for advertising devices that use
 * legacy or extended advertising PDUs. To cancel an ongoing discovery
 * procedure, use the @ref sl_bt_scanner_stop command.
 *
 * The invalid parameter error will be returned if the value of scanning PHYs is
 * invalid or the device does not support a PHY.
 *
 * Received advertising packets are not filtered in any way, so multiple events
 * will be received for every advertising device in range.
 *
 * @param[in] scanning_phy @parblock
 *   Enum @ref sl_bt_scanner_scan_phy_t.
 *
 *   The scanning PHY(s).
 *
 *   In simultaneous scanning, the stack alternates the scanning on two PHYs by
 *   switching the PHY at every scan interval. When a timing parameter is set
 *   differently on 1M and Coded PHY, the stack chooses the most relaxed value
 *   for both PHYs during simultaneous scanning, i.e., the largest scan interval
 *   or the smallest scan window. If one PHY is set to passive scanning and the
 *   other to active scanning, passive scanning is chosen for simultaneous
 *   scanning. Values:
 *     - <b>sl_bt_scanner_scan_phy_1m (0x1):</b> Initiate the scanning on the 1M
 *       PHY
 *     - <b>sl_bt_scanner_scan_phy_coded (0x4):</b> Initiate the scanning on the
 *       Coded PHY
 *     - <b>sl_bt_scanner_scan_phy_1m_and_coded (0x5):</b> Simultaneous scanning
 *       by initiating the scanning on the 1M and Coded PHY alternatively
 *   @endparblock
 * @param[in] discover_mode Enum @ref sl_bt_scanner_discover_mode_t. Bluetooth
 *   discovery Mode. Values:
 *     - <b>sl_bt_scanner_discover_limited (0x0):</b> Discover only limited
 *       discoverable devices.
 *     - <b>sl_bt_scanner_discover_generic (0x1):</b> Discover limited and
 *       general discoverable devices.
 *     - <b>sl_bt_scanner_discover_observation (0x2):</b> Discover
 *       non-discoverable, limited and general discoverable devices.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_scanner_legacy_advertisement_report - This event is
 *     triggered for reporting a received advertisement that uses legacy
 *     advertising PDUs if the application includes the
 *     bluetooth_feature_legacy_scanner or bluetooth_feature_extended_scanner
 *     component.
 *   - @ref sl_bt_evt_scanner_extended_advertisement_report - This event is
 *     triggered for reporting a received advertisement that uses extended
 *     advertising PDUs if the application includes the
 *     bluetooth_feature_extended_scanner component.
 *
 ******************************************************************************/
sl_status_t sl_bt_scanner_start(uint8_t scanning_phy, uint8_t discover_mode);

/***************************************************************************//**
 *
 * Stop scanning for advertising devices. For more information about the
 * discovery, see the @ref sl_bt_scanner_start command.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_scanner_stop();

/** @} */ // end addtogroup sl_bt_scanner

/**
 * @addtogroup sl_bt_sync Synchronization
 * @{
 *
 * @brief Synchronization
 *
 * Provides the base functionality of periodic advertising synchronization. Use
 * bluetooth_feature_sync_scanner and/or bluetooth_feature_past_receiver
 * components to include the synchronization mechanisms that the application
 * requires. Use bluetooth_feature_periodic_sync to include support for trains
 * that do not have subevents or response slots, and/or
 * bluetooth_feature_pawr_sync to include support for Periodic Advertising with
 * Responses (PAwR) trains.
 */

/* Command and Response IDs */
#define sl_bt_cmd_sync_set_reporting_mode_id                         0x03420020
#define sl_bt_cmd_sync_update_sync_parameters_id                     0x04420020
#define sl_bt_cmd_sync_close_id                                      0x01420020
#define sl_bt_rsp_sync_set_reporting_mode_id                         0x03420020
#define sl_bt_rsp_sync_update_sync_parameters_id                     0x04420020
#define sl_bt_rsp_sync_close_id                                      0x01420020

/**
 * @brief Specifies the mode for periodic advertising reports.
 */
typedef enum
{
  sl_bt_sync_report_none = 0x0, /**< (0x0) Data received in periodic advertising
                                     trains is not reported to the application. */
  sl_bt_sync_report_all  = 0x1  /**< (0x1) Data received in periodic advertising
                                     trains is reported to the application. */
} sl_bt_sync_reporting_mode_t;

/**
 * @brief These values indicate the advertiser clock accuracy in a periodic
 * advertising synchronization.
 */
typedef enum
{
  sl_bt_sync_clock_accuracy_500 = 0x1f4, /**< (0x1f4) Clock accuracy 500 ppm */
  sl_bt_sync_clock_accuracy_250 = 0xfa,  /**< (0xfa) Clock accuracy 250 ppm */
  sl_bt_sync_clock_accuracy_150 = 0x96,  /**< (0x96) Clock accuracy 150 ppm */
  sl_bt_sync_clock_accuracy_100 = 0x64,  /**< (0x64) Clock accuracy 100 ppm */
  sl_bt_sync_clock_accuracy_75  = 0x4b,  /**< (0x4b) Clock accuracy 75 ppm */
  sl_bt_sync_clock_accuracy_50  = 0x32,  /**< (0x32) Clock accuracy 50 ppm */
  sl_bt_sync_clock_accuracy_30  = 0x1e,  /**< (0x1e) Clock accuracy 30 ppm */
  sl_bt_sync_clock_accuracy_20  = 0x14   /**< (0x14) Clock accuracy 20 ppm */
} sl_bt_sync_advertiser_clock_accuracy_t;

/**
 * @addtogroup sl_bt_evt_sync_closed sl_bt_evt_sync_closed
 * @{
 * @brief Indicates that periodic advertising synchronization was lost or a
 * synchronization establishment procedure was canceled
 *
 * The synchronization establishment procedure can be canceled explicitly by the
 * application by issuing command @ref sl_bt_sync_close, or internally due to
 * synchronization failing. Synchronization can fail for example due to
 * incompatible sync CTE type.
 */

/** @brief Identifier of the closed event */
#define sl_bt_evt_sync_closed_id                                     0x014200a0

/***************************************************************************//**
 * @brief Data structure of the closed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sync_closed_s
{
  uint16_t reason; /**< SL_STATUS_OK if successful. Error code otherwise. */
  uint16_t sync;   /**< Periodic advertising synchronization handle */
});

typedef struct sl_bt_evt_sync_closed_s sl_bt_evt_sync_closed_t;

/** @} */ // end addtogroup sl_bt_evt_sync_closed

/***************************************************************************//**
 *
 * Set data reporting mode of the periodic advertising synchronization.
 *
 * @param[in] sync Periodic advertising synchronization handle
 * @param[in] reporting_mode Enum @ref sl_bt_sync_reporting_mode_t. Specifies
 *   the mode for reporting data received in the periodic advertising train.
 *   Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_set_reporting_mode(uint16_t sync,
                                          uint8_t reporting_mode);

/***************************************************************************//**
 *
 * Update synchronization parameters for a periodic sync that was already
 * established.
 *
 * When a sync is established by scanning (see @ref sl_bt_sync_scanner) or by
 * receiving Periodic Advertising Synchronization Transfer (see @ref
 * sl_bt_past_receiver), the sync gets the @p skip and @p timeout parameters
 * that were configured in the corresponding class. The application can use this
 * command @ref sl_bt_sync_update_sync_parameters to update the values of a sync
 * that has been established. The application can for example update the values
 * to better match the actual interval of the periodic advertising train, or to
 * increase the @p skip value to minimize wakeups when power saving is
 * prioritized over receiving every periodic advertisement.
 *
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout.
 *
 * @param[in] sync Periodic advertising synchronization handle
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0x0000 to 0x01F3
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 0x000A to 0x4000
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_update_sync_parameters(uint16_t sync,
                                              uint16_t skip,
                                              uint16_t timeout);

/***************************************************************************//**
 *
 * Close a periodic advertising synchronization or cancel an ongoing attempt of
 * establishing a synchronization.
 *
 * @param[in] sync Periodic advertising synchronization handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_sync_closed - Triggered after a periodic advertising
 *     synchronization has been closed or canceled.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_close(uint16_t sync);

/** @} */ // end addtogroup sl_bt_sync

/**
 * @addtogroup sl_bt_sync_scanner Periodic Advertising Sync Scanner
 * @{
 *
 * @brief Periodic Advertising Sync Scanner
 *
 * Synchronize to periodic advertising trains by scanning for extended
 * advertisements that provide the synchronization information.
 */

/* Command and Response IDs */
#define sl_bt_cmd_sync_scanner_set_sync_parameters_id                0x00500020
#define sl_bt_cmd_sync_scanner_open_id                               0x01500020
#define sl_bt_rsp_sync_scanner_set_sync_parameters_id                0x00500020
#define sl_bt_rsp_sync_scanner_open_id                               0x01500020

/***************************************************************************//**
 *
 * Configure synchronization parameters for synchronizing to periodic
 * advertising trains. The specified parameters take effect immediately for all
 * periodic advertising trains that have not already established
 * synchronization.
 *
 * The application should determine skip and timeout values based on the
 * periodic advertising interval provided by the advertiser. Use a long enough
 * timeout to allow multiple receives. If @p skip and @p timeout are used,
 * select appropriate values so that they allow a few receiving attempts.
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout. Periodic advertising
 * intervals are reported in @ref
 * sl_bt_evt_scanner_extended_advertisement_report event.
 *
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0x0000 to 0x01F3
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 0x000A to 0x4000
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] reporting_mode @parblock
 *   Enum @ref sl_bt_sync_reporting_mode_t. Specifies the initial mode for
 *   reporting data received in the periodic advertising train after it has
 *   achieved synchronization. Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 *   Default: @ref sl_bt_sync_report_all (Data received in periodic advertising
 *   trains is reported to the application)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_scanner_set_sync_parameters(uint16_t skip,
                                                   uint16_t timeout,
                                                   uint8_t reporting_mode);

/***************************************************************************//**
 *
 * Start establishing synchronization with the specified periodic advertiser in
 * parallel with other advertisers given in previous invocations of this
 * command.
 *
 * Periodic advertisers are typically discovered by scanning for extended
 * advertisements and observing the received @ref
 * sl_bt_evt_scanner_extended_advertisement_report events. See the
 * bluetooth_feature_extended_scanner component and the @ref sl_bt_scanner_start
 * command. When an advertiser has periodic advertising, the @p
 * periodic_interval field of @ref
 * sl_bt_evt_scanner_extended_advertisement_report event will be non-zero. To
 * synchronize to that periodic advertising train, use the @p address, @p
 * address_type, and @p adv_sid fields from the @ref
 * sl_bt_evt_scanner_extended_advertisement_report when issuing this command.
 *
 * If the application has not already started scanning with the @ref
 * sl_bt_scanner_start command, the stack will internally enable scanning so
 * that synchronizations can occur. The internal scanning uses the PHY that was
 * most recently used with @ref sl_bt_scanner_start and the parameters that have
 * been configured with @ref sl_bt_scanner_set_parameters. The internal scanning
 * is automatically stopped when all requested synchronizations have occurred.
 *
 * The scanning responses from the internal scanning are not passed to the
 * application unless the application starts scanning with the @ref
 * sl_bt_scanner_start command. If the application starts scanning while
 * synchronizations are being established, the scanning PHY and settings set by
 * the application take effect immediately and scanning for synchronizations
 * continues with the new settings. When the application has started scanning
 * with the @ref sl_bt_scanner_start command, scanning continues until the
 * application stops scanning with the @ref sl_bt_scanner_stop command.
 *
 * Advertisers that have not already synced before the invocation of this
 * command will be synced using the @p skip and @p timeout values configured in
 * the most recent invocation of command @ref
 * sl_bt_evt_scanner_extended_advertisement_report.
 *
 * @param[in] address Address of the advertiser
 * @param[in] address_type @parblock
 *   Enum @ref sl_bt_gap_address_type_t.
 *
 *   Advertiser address type.
 *
 *   If the application does not include the
 *   bluetooth_feature_use_accurate_api_address_types component, @p address_type
 *   uses the following values:
 *     - <b>0:</b> Public address
 *     - <b>1:</b> Random address
 *
 *   If the application includes the
 *   bluetooth_feature_use_accurate_api_address_types component, @p address_type
 *   uses enum @ref sl_bt_gap_address_type_t values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 *   @endparblock
 * @param[in] adv_sid The identifier of the advertising set to synchronize to
 * @param[out] sync A handle that will be assigned to the periodic advertising
 *   synchronization after the synchronization is established. This handle is
 *   valid only if the result code of this response is SL_STATUS_OK.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_periodic_sync_opened - Triggered after synchronization is
 *     established to a periodic advertising train that does not have subevents
 *     or response slots.
 *   - @ref sl_bt_evt_pawr_sync_opened - Triggered after synchronization is
 *     established to a Periodic Advertising with Responses (PAwR) train.
 *   - @ref sl_bt_evt_periodic_sync_report - Triggered when data for periodic
 *     advertising train that does not have subevents or response slots is
 *     received and accepted by the reporting mode currently set to the train.
 *   - @ref sl_bt_evt_pawr_sync_subevent_report - Triggered when subevent data
 *     for Periodic Advertising with Responses (PAwR) train is received and
 *     accepted by the reporting mode currently set to the train.
 *   - @ref sl_bt_evt_sync_closed - Triggered after periodic advertising
 *     synchronization was lost or explicitly closed, or a synchronization
 *     establishment procedure was canceled.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_scanner_open(bd_addr address,
                                    uint8_t address_type,
                                    uint8_t adv_sid,
                                    uint16_t *sync);

/** @} */ // end addtogroup sl_bt_sync_scanner

/**
 * @addtogroup sl_bt_past_receiver PAST Receiver
 * @{
 *
 * @brief PAST Receiver
 *
 * Synchronize to periodic advertising trains by receiving Periodic Advertising
 * Synchronization Transfer over a connection.
 */

/* Command and Response IDs */
#define sl_bt_cmd_past_receiver_set_default_sync_receive_parameters_id 0x00510020
#define sl_bt_cmd_past_receiver_set_sync_receive_parameters_id       0x01510020
#define sl_bt_rsp_past_receiver_set_default_sync_receive_parameters_id 0x00510020
#define sl_bt_rsp_past_receiver_set_sync_receive_parameters_id       0x01510020

/**
 * @brief Specifies the mode for receiving synchronization transfers
 */
typedef enum
{
  sl_bt_past_receiver_mode_ignore      = 0x0, /**< (0x0) No attempt is made to
                                                   synchronize to a periodic
                                                   advertising train for which
                                                   the synchronization
                                                   information was received. No
                                                   event will be triggered
                                                   towards the application. */
  sl_bt_past_receiver_mode_synchronize = 0x1  /**< (0x1) Attempt to synchronize
                                                   to a periodic advertising
                                                   train for which the
                                                   synchronization information
                                                   was received. When the
                                                   information is received, an
                                                   event will be triggered to
                                                   indicate success or failure
                                                   and to provide the
                                                   application with the periodic
                                                   advertising synchronization
                                                   handle. */
} sl_bt_past_receiver_mode_t;

/***************************************************************************//**
 *
 * Set the default parameters for receiving Periodic Advertising Synchronization
 * Transfers (PAST) over connections. The default parameters will be in effect
 * for all subsequent connections, unless overridden by command @ref
 * sl_bt_past_receiver_set_sync_receive_parameters after the connection is
 * opened.
 *
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout.
 *
 * This command sets parameters that do not limit the synchronization based on
 * the CTE type. If the application includes bluetooth_feature_aoa_receiver or
 * bluetooth_feature_aod_receiver component and wants to specify a particular
 * CTE limitation, the application should use the command @ref
 * sl_bt_cte_receiver_set_default_sync_receive_parameters to set the default
 * parameters.
 *
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_past_receiver_mode_t. The mode to specify how the Bluetooth
 *   stack reacts when synchronization information is received. Values:
 *     - <b>sl_bt_past_receiver_mode_ignore (0x0):</b> No attempt is made to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. No event will be triggered
 *       towards the application.
 *     - <b>sl_bt_past_receiver_mode_synchronize (0x1):</b> Attempt to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. When the information is
 *       received, an event will be triggered to indicate success or failure and
 *       to provide the application with the periodic advertising
 *       synchronization handle.
 *
 *   Default: @ref sl_bt_past_receiver_mode_ignore (No attempt is made to
 *   synchronize)
 *   @endparblock
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0x0000 to 0x01F3
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 0x000A to 0x4000
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] reporting_mode @parblock
 *   Enum @ref sl_bt_sync_reporting_mode_t. Specifies the initial mode for
 *   reporting data received in the periodic advertising train after it has
 *   achieved synchronization. Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 *   Default: @ref sl_bt_sync_report_all (Data received in periodic advertising
 *   trains is reported to the application)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_periodic_sync_transfer_received - Triggered after
 *     synchronization transfer is received for a periodic advertising train
 *     that does not have subevents or response slots.
 *   - @ref sl_bt_evt_pawr_sync_transfer_received - If the application includes
 *     the bluetooth_feature_pawr_sync component, triggered after
 *     synchronization transfer is received for a Periodic Advertising with
 *     Responses (PAwR) train.
 *
 ******************************************************************************/
sl_status_t sl_bt_past_receiver_set_default_sync_receive_parameters(uint8_t mode,
                                                                    uint16_t skip,
                                                                    uint16_t timeout,
                                                                    uint8_t reporting_mode);

/***************************************************************************//**
 *
 * Set the parameters for receiving Periodic Advertising Synchronization
 * Transfers (PAST) over the specified connection. The parameters do not affect
 * periodic advertising trains that the device has already synchronized to.
 *
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout.
 *
 * This command sets parameters that do not limit the synchronization based on
 * the CTE type. If the application includes bluetooth_feature_aoa_receiver or
 * bluetooth_feature_aod_receiver component and wants to specify a particular
 * CTE limitation, the application should use the command @ref
 * sl_bt_cte_receiver_set_sync_receive_parameters to set the parameters.
 *
 * @param[in] connection Connection handle of the connection used to receive the
 *   sync transfer
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_past_receiver_mode_t. The mode to specify how the Bluetooth
 *   stack reacts when synchronization information is received. Values:
 *     - <b>sl_bt_past_receiver_mode_ignore (0x0):</b> No attempt is made to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. No event will be triggered
 *       towards the application.
 *     - <b>sl_bt_past_receiver_mode_synchronize (0x1):</b> Attempt to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. When the information is
 *       received, an event will be triggered to indicate success or failure and
 *       to provide the application with the periodic advertising
 *       synchronization handle.
 *
 *   Default: @ref sl_bt_past_receiver_mode_ignore (Do not attempt to
 *   synchronize)
 *   @endparblock
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0x0000 to 0x01F3
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 0x000A to 0x4000
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] reporting_mode @parblock
 *   Enum @ref sl_bt_sync_reporting_mode_t. Specifies the initial mode for
 *   reporting data received in the periodic advertising train after it has
 *   achieved synchronization. Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 *   Default: @ref sl_bt_sync_report_all (Data received in periodic advertising
 *   trains is reported to the application)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_periodic_sync_transfer_received - Triggered after
 *     synchronization transfer is received for a periodic advertising train
 *     that does not have subevents or response slots.
 *   - @ref sl_bt_evt_pawr_sync_transfer_received - If the application includes
 *     the bluetooth_feature_pawr_sync component, triggered after
 *     synchronization transfer is received for a Periodic Advertising with
 *     Responses (PAwR) train.
 *
 ******************************************************************************/
sl_status_t sl_bt_past_receiver_set_sync_receive_parameters(uint8_t connection,
                                                            uint8_t mode,
                                                            uint16_t skip,
                                                            uint16_t timeout,
                                                            uint8_t reporting_mode);

/** @} */ // end addtogroup sl_bt_past_receiver

/**
 * @addtogroup sl_bt_advertiser_past Advertiser PAST
 * @{
 *
 * @brief Advertiser PAST
 *
 * Transfer the synchronization information of a local active periodic
 * advertising set to a remote device using Periodic Advertising Synchronization
 * Transfer (PAST) over a connection.
 */

/* Command and Response IDs */
#define sl_bt_cmd_advertiser_past_transfer_id                        0x00520020
#define sl_bt_rsp_advertiser_past_transfer_id                        0x00520020

/***************************************************************************//**
 *
 * Transfer the synchronization information of an advertising set that is
 * actively performing periodic advertising.
 *
 * @param[in] connection Connection handle of the connection used to transmit
 *   the sync transfer
 * @param[in] service_data A value provided by the application for use by the
 *   peer device.
 * @param[in] advertising_set Handle of the periodic advertising set to transfer
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_advertiser_past_transfer(uint8_t connection,
                                           uint16_t service_data,
                                           uint8_t advertising_set);

/** @} */ // end addtogroup sl_bt_advertiser_past

/**
 * @addtogroup sl_bt_sync_past Sync PAST
 * @{
 *
 * @brief Sync PAST
 *
 * Transfer the synchronization information of a synchronized periodic
 * advertising train to a remote device using Periodic Advertising
 * Synchronization Transfer (PAST) over a connection.
 */

/* Command and Response IDs */
#define sl_bt_cmd_sync_past_transfer_id                              0x005b0020
#define sl_bt_rsp_sync_past_transfer_id                              0x005b0020

/***************************************************************************//**
 *
 * Transfer the synchronization information of a periodic advertising train for
 * which a sync has been established.
 *
 * @param[in] connection Connection handle of the connection used to transmit
 *   the sync transfer
 * @param[in] service_data A value provided by the application for use by the
 *   peer device.
 * @param[in] sync Handle of the periodic advertising synchronization to
 *   transfer
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sync_past_transfer(uint8_t connection,
                                     uint16_t service_data,
                                     uint16_t sync);

/** @} */ // end addtogroup sl_bt_sync_past

/**
 * @addtogroup sl_bt_periodic_sync Periodic Advertising without responses Synchronization
 * @{
 *
 * @brief Periodic Advertising without responses Synchronization
 *
 * Provides events and control for synchronized periodic advertising trains that
 * do not have subevents or response slots. Synchronization is achieved by
 * scanning (see @ref sl_bt_sync_scanner) or by receiving Periodic Advertising
 * Synchronization Transfer (see @ref sl_bt_past_receiver).
 */

/* Command and Response IDs */

/**
 * @addtogroup sl_bt_evt_periodic_sync_opened sl_bt_evt_periodic_sync_opened
 * @{
 * @brief Indicates that synchronization to a periodic advertising train that
 * does not have subevents or response slots has been opened by scanning
 *
 * See command @ref sl_bt_sync_scanner_open.
 */

/** @brief Identifier of the opened event */
#define sl_bt_evt_periodic_sync_opened_id                            0x005300a0

/***************************************************************************//**
 * @brief Data structure of the opened event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_periodic_sync_opened_s
{
  uint16_t sync;           /**< Periodic Advertising synchronization handle */
  uint8_t  adv_sid;        /**< Advertising set identifier */
  bd_addr  address;        /**< Address of the advertiser */
  uint8_t  address_type;   /**< Enum @ref sl_bt_gap_address_type_t.

                                Advertiser address type.

                                If the application does not include the
                                bluetooth_feature_use_accurate_api_address_types
                                component, @p address_type uses the following
                                values:
                                  - <b>0:</b> Public address
                                  - <b>1:</b> Random address

                                If the application includes the
                                bluetooth_feature_use_accurate_api_address_types
                                component, @p address_type uses enum @ref
                                sl_bt_gap_address_type_t values:
                                  - <b>sl_bt_gap_public_address (0x0):</b>
                                    Public device address
                                  - <b>sl_bt_gap_static_address (0x1):</b>
                                    Static device address
                                  - <b>sl_bt_gap_random_resolvable_address
                                    (0x2):</b> Resolvable private random address
                                  - <b>sl_bt_gap_random_nonresolvable_address
                                    (0x3):</b> Non-resolvable private random
                                    address
                                  - <b>sl_bt_gap_public_address_resolved_from_rpa
                                    (0x4):</b> Public identity address resolved
                                    from a resolvable private address (RPA)
                                  - <b>sl_bt_gap_static_address_resolved_from_rpa
                                    (0x5):</b> Static identity address resolved
                                    from a resolvable private address (RPA) */
  uint8_t  adv_phy;        /**< Enum @ref sl_bt_gap_phy_t. The advertiser PHY.
                                Values:
                                  - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                  - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
                                  - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY,
                                    125k (S=8) or 500k (S=2) */
  uint16_t adv_interval;   /**< The Periodic Advertising interval. Value in
                                units of 1.25 ms
                                  - <b>Range:</b> 0x06 to 0xFFFF

                                  - Time range: 7.5 ms to 81.92 s */
  uint16_t clock_accuracy; /**< Enum @ref
                                sl_bt_sync_advertiser_clock_accuracy_t. The
                                advertiser clock accuracy. */
  uint8_t  bonding;        /**< Bonding handle. Values:
                                  - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b>
                                    No bonding
                                  - <b>Other:</b> Bonding handle */
});

typedef struct sl_bt_evt_periodic_sync_opened_s sl_bt_evt_periodic_sync_opened_t;

/** @} */ // end addtogroup sl_bt_evt_periodic_sync_opened

/**
 * @addtogroup sl_bt_evt_periodic_sync_transfer_received sl_bt_evt_periodic_sync_transfer_received
 * @{
 * @brief Indicates that synchronization information for a periodic advertising
 * train that does not have subevents or response slots has been received
 *
 * See @ref sl_bt_past_receiver.
 */

/** @brief Identifier of the transfer_received event */
#define sl_bt_evt_periodic_sync_transfer_received_id                 0x015300a0

/***************************************************************************//**
 * @brief Data structure of the transfer_received event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_periodic_sync_transfer_received_s
{
  uint16_t status;         /**< SL_STATUS_OK if synchronization was established.
                                Other values indicate that the sync failed to
                                get established. */
  uint16_t sync;           /**< Periodic advertising synchronization handle */
  uint16_t service_data;   /**< A value provided by the peer device */
  uint8_t  connection;     /**< Connection handle of the connection that
                                transferred the sync info */
  uint8_t  adv_sid;        /**< Advertising set identifier */
  bd_addr  address;        /**< Address of the advertiser */
  uint8_t  address_type;   /**< Enum @ref sl_bt_gap_address_type_t.

                                Advertiser address type.

                                If the application does not include the
                                bluetooth_feature_use_accurate_api_address_types
                                component, @p address_type uses the following
                                values:
                                  - <b>0:</b> Public address
                                  - <b>1:</b> Random address

                                If the application includes the
                                bluetooth_feature_use_accurate_api_address_types
                                component, @p address_type uses enum @ref
                                sl_bt_gap_address_type_t values:
                                  - <b>sl_bt_gap_public_address (0x0):</b>
                                    Public device address
                                  - <b>sl_bt_gap_static_address (0x1):</b>
                                    Static device address
                                  - <b>sl_bt_gap_random_resolvable_address
                                    (0x2):</b> Resolvable private random address
                                  - <b>sl_bt_gap_random_nonresolvable_address
                                    (0x3):</b> Non-resolvable private random
                                    address
                                  - <b>sl_bt_gap_public_address_resolved_from_rpa
                                    (0x4):</b> Public identity address resolved
                                    from a resolvable private address (RPA)
                                  - <b>sl_bt_gap_static_address_resolved_from_rpa
                                    (0x5):</b> Static identity address resolved
                                    from a resolvable private address (RPA) */
  uint8_t  adv_phy;        /**< Enum @ref sl_bt_gap_phy_t. The advertiser PHY.
                                Values:
                                  - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                  - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
                                  - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY,
                                    125k (S=8) or 500k (S=2) */
  uint16_t adv_interval;   /**< The periodic advertising interval. Value in
                                units of 1.25 ms
                                  - <b>Range:</b> 0x06 to 0xFFFF

                                  - Time range: 7.5 ms to 81.92 s */
  uint16_t clock_accuracy; /**< Enum @ref
                                sl_bt_sync_advertiser_clock_accuracy_t. The
                                advertiser clock accuracy. */
  uint8_t  bonding;        /**< Bonding handle. Values:
                                  - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b>
                                    No bonding
                                  - <b>Other:</b> Bonding handle */
});

typedef struct sl_bt_evt_periodic_sync_transfer_received_s sl_bt_evt_periodic_sync_transfer_received_t;

/** @} */ // end addtogroup sl_bt_evt_periodic_sync_transfer_received

/**
 * @addtogroup sl_bt_evt_periodic_sync_report sl_bt_evt_periodic_sync_report
 * @{
 * @brief Reports a periodic advertising packet for periodic advertising train
 * that does not have subevents or response slots
 */

/** @brief Identifier of the report event */
#define sl_bt_evt_periodic_sync_report_id                            0x025300a0

/***************************************************************************//**
 * @brief Data structure of the report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_periodic_sync_report_s
{
  uint16_t   sync;        /**< Periodic advertising synchronization handle */
  int8_t     tx_power;    /**< TX power value in the received packet header.
                               Units: dBm
                                 - <b>Range:</b> -127 to 126

                                 - Value 127: information unavailable */
  int8_t     rssi;        /**< Signal strength indicator (RSSI) in the latest
                               received packet. Units: dBm
                                 - <b>Range:</b> -127 to +20 */
  uint8_t    cte_type;    /**< The CTE type
                                 - <b>0x00:</b> AoA CTE
                                 - <b>0x01:</b> AoD CTE with 1us slots
                                 - <b>0x02:</b> AoD CTE with 2us slots
                                 - <b>0xFF:</b> No CTE */
  uint8_t    data_status; /**< Data completeness:
                                 - <b>0:</b> Complete
                                 - <b>1:</b> Incomplete, more data to come in
                                   new events
                                 - <b>2:</b> Incomplete, data truncated, no more
                                   to come */
  uint8_t    counter;     /**< The sequence number of this @ref
                               sl_bt_evt_periodic_sync_report event as a
                               monotonically increasing counter that wraps from
                               255 to 0. There is a single counter for this
                               event type that starts from value 0 when the
                               Bluetooth stack is started and is never reset
                               while the stack is running. This counter can be
                               used to detect if one or more @ref
                               sl_bt_evt_periodic_sync_report events have been
                               created by the stack but dropped due to problems
                               such as temporarily running out of resources
                               before the event reached the application. */
  uint8array data;        /**< Periodic advertising data */
});

typedef struct sl_bt_evt_periodic_sync_report_s sl_bt_evt_periodic_sync_report_t;

/** @} */ // end addtogroup sl_bt_evt_periodic_sync_report

/** @} */ // end addtogroup sl_bt_periodic_sync

/**
 * @addtogroup sl_bt_pawr_sync Periodic Advertising with responses Synchronization
 * @{
 *
 * @brief Periodic Advertising with responses Synchronization
 *
 * Provides events and control for synchronized Periodic Advertising with
 * Responses (PAwR) trains. Synchronization is achieved by scanning (see @ref
 * sl_bt_sync_scanner) or by receiving Periodic Advertising Synchronization
 * Transfer (see @ref sl_bt_past_receiver).
 */

/* Command and Response IDs */
#define sl_bt_cmd_pawr_sync_set_sync_subevents_id                    0x02540020
#define sl_bt_cmd_pawr_sync_set_response_data_id                     0x03540020
#define sl_bt_rsp_pawr_sync_set_sync_subevents_id                    0x02540020
#define sl_bt_rsp_pawr_sync_set_response_data_id                     0x03540020

/**
 * @addtogroup sl_bt_evt_pawr_sync_opened sl_bt_evt_pawr_sync_opened
 * @{
 * @brief Indicates that synchronization to a Periodic Advertising with
 * Responses (PAwR) train has been opened by scanning
 *
 * See command @ref sl_bt_sync_scanner_open.
 */

/** @brief Identifier of the opened event */
#define sl_bt_evt_pawr_sync_opened_id                                0x005400a0

/***************************************************************************//**
 * @brief Data structure of the opened event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_sync_opened_s
{
  uint16_t sync;                  /**< PAwR synchronization handle */
  uint8_t  adv_sid;               /**< Advertising set identifier */
  bd_addr  address;               /**< Address of the advertiser */
  uint8_t  address_type;          /**< Enum @ref sl_bt_gap_address_type_t.

                                       Advertiser address type.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses the
                                       following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses enum @ref
                                       sl_bt_gap_address_type_t values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8_t  adv_phy;               /**< Enum @ref sl_bt_gap_phy_t. The advertiser
                                       PHY. Values:
                                         - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                         - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
                                         - <b>sl_bt_gap_phy_coded (0x4):</b>
                                           Coded PHY, 125k (S=8) or 500k (S=2) */
  uint16_t adv_interval;          /**< The periodic advertising interval. Value
                                       in units of 1.25 ms
                                         - <b>Range:</b> 0x06 to 0xFFFF

                                         - Time range: 7.5 ms to 81.92 s */
  uint16_t clock_accuracy;        /**< Enum @ref
                                       sl_bt_sync_advertiser_clock_accuracy_t.
                                       The advertiser clock accuracy. */
  uint8_t  num_subevents;         /**< The number of subevents.
                                         - <b>Range:</b> 0x01 to 0x80 */
  uint8_t  subevent_interval;     /**< Subevent interval. Value in units of 1.25
                                       ms.
                                         - <b>Range:</b> 0x06 to 0xFF

                                         - Time range: 7.5 ms to 318.75 ms */
  uint8_t  response_slot_delay;   /**< Time between the advertising packet in a
                                       subevent and the first response slot.
                                       Value in units of 1.25 ms.
                                         - <b>Range:</b> 0x01 to 0xFE

                                         - Time range: 1.25 ms to 317.5 ms */
  uint8_t  response_slot_spacing; /**< Time between the start of response slots
                                       in a subevent. Value in units of 0.125
                                       ms.
                                         - <b>Range:</b> 0x02 to 0xFF

                                         - Time range: 0.25 ms to 31.875 ms */
  uint8_t  bonding;               /**< Bonding handle. Values:
                                         - <b>SL_BT_INVALID_BONDING_HANDLE
                                           (0xff):</b> No bonding
                                         - <b>Other:</b> Bonding handle */
});

typedef struct sl_bt_evt_pawr_sync_opened_s sl_bt_evt_pawr_sync_opened_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_sync_opened

/**
 * @addtogroup sl_bt_evt_pawr_sync_transfer_received sl_bt_evt_pawr_sync_transfer_received
 * @{
 * @brief Indicates that synchronization information for Periodic Advertising
 * with Responses (PAwR) train has been received
 *
 * See @ref sl_bt_past_receiver.
 */

/** @brief Identifier of the transfer_received event */
#define sl_bt_evt_pawr_sync_transfer_received_id                     0x015400a0

/***************************************************************************//**
 * @brief Data structure of the transfer_received event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_sync_transfer_received_s
{
  uint16_t status;                /**< SL_STATUS_OK if synchronization was
                                       established. Other values indicate that
                                       the sync failed to get established. */
  uint16_t sync;                  /**< PAwR synchronization handle */
  uint16_t service_data;          /**< A value provided by the peer device. */
  uint8_t  connection;            /**< Connection handle of the connection that
                                       transferred the sync info */
  uint8_t  adv_sid;               /**< Advertising set identifier */
  bd_addr  address;               /**< Address of the advertiser */
  uint8_t  address_type;          /**< Enum @ref sl_bt_gap_address_type_t.

                                       Advertiser address type.

                                       If the application does not include the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses the
                                       following values:
                                         - <b>0:</b> Public address
                                         - <b>1:</b> Random address

                                       If the application includes the
                                       bluetooth_feature_use_accurate_api_address_types
                                       component, @p address_type uses enum @ref
                                       sl_bt_gap_address_type_t values:
                                         - <b>sl_bt_gap_public_address
                                           (0x0):</b> Public device address
                                         - <b>sl_bt_gap_static_address
                                           (0x1):</b> Static device address
                                         - <b>sl_bt_gap_random_resolvable_address
                                           (0x2):</b> Resolvable private random
                                           address
                                         - <b>sl_bt_gap_random_nonresolvable_address
                                           (0x3):</b> Non-resolvable private
                                           random address
                                         - <b>sl_bt_gap_public_address_resolved_from_rpa
                                           (0x4):</b> Public identity address
                                           resolved from a resolvable private
                                           address (RPA)
                                         - <b>sl_bt_gap_static_address_resolved_from_rpa
                                           (0x5):</b> Static identity address
                                           resolved from a resolvable private
                                           address (RPA) */
  uint8_t  adv_phy;               /**< Enum @ref sl_bt_gap_phy_t. The advertiser
                                       PHY. Values:
                                         - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
                                         - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
                                         - <b>sl_bt_gap_phy_coded (0x4):</b>
                                           Coded PHY, 125k (S=8) or 500k (S=2) */
  uint16_t adv_interval;          /**< The periodic advertising interval. Value
                                       in units of 1.25 ms
                                         - <b>Range:</b> 0x06 to 0xFFFF

                                         - Time range: 7.5 ms to 81.92 s */
  uint16_t clock_accuracy;        /**< Enum @ref
                                       sl_bt_sync_advertiser_clock_accuracy_t.
                                       The advertiser clock accuracy. */
  uint8_t  num_subevents;         /**< The number of subevents.
                                         - <b>Range:</b> 0x01 to 0x80 */
  uint8_t  subevent_interval;     /**< Subevent interval. Value in units of 1.25
                                       ms.
                                         - <b>Range:</b> 0x06 to 0xFF

                                         - Time range: 7.5 ms to 318.75 ms */
  uint8_t  response_slot_delay;   /**< Time between the advertising packet in a
                                       subevent and the first response slot.
                                       Value in units of 1.25 ms.
                                         - <b>Range:</b> 0x01 to 0xFE

                                         - Time range: 1.25 ms to 317.5 ms */
  uint8_t  response_slot_spacing; /**< Time between response slots. Value in
                                       units of 0.125 ms.
                                         - <b>Range:</b> 0x02 to 0xFF

                                         - Time range: 0.25 ms to 31.875 ms */
  uint8_t  bonding;               /**< Bonding handle. Values:
                                         - <b>SL_BT_INVALID_BONDING_HANDLE
                                           (0xff):</b> No bonding
                                         - <b>Other:</b> Bonding handle */
});

typedef struct sl_bt_evt_pawr_sync_transfer_received_s sl_bt_evt_pawr_sync_transfer_received_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_sync_transfer_received

/**
 * @addtogroup sl_bt_evt_pawr_sync_subevent_report sl_bt_evt_pawr_sync_subevent_report
 * @{
 * @brief Reports that the device's radio has received a periodic advertisement
 * in a subevent of an active PAwR train
 *
 * This event reports also empty advertisements, i.e., advertisements that
 * contained no payload data. In this case the @p data parameter has zero
 * length.
 */

/** @brief Identifier of the subevent_report event */
#define sl_bt_evt_pawr_sync_subevent_report_id                       0x025400a0

/***************************************************************************//**
 * @brief Data structure of the subevent_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_sync_subevent_report_s
{
  uint16_t   sync;          /**< PAwR synchronization handle */
  int8_t     tx_power;      /**< TX power value in the received packet header.
                                 Units: dBm
                                   - <b>Range:</b> -127 to 126

                                   - Value 127: information unavailable */
  int8_t     rssi;          /**< Signal strength indicator (RSSI) of the
                                 received packet. Units: dBm
                                   - <b>Range:</b> -127 to +20 */
  uint8_t    cte_type;      /**< The CTE type
                                   - <b>0x00:</b> AoA CTE
                                   - <b>0x01:</b> AoD CTE with 1us slots
                                   - <b>0x02:</b> AoD CTE with 2us slots
                                   - <b>0xFF:</b> No CTE */
  uint16_t   event_counter; /**< The value of the periodic advertising event
                                 counter (paEventCounter) of the event in which
                                 the advertisement was received. If the
                                 application responds to this advertisement, use
                                 this field as the value of @p request_event
                                 parameter for the @ref
                                 sl_bt_pawr_sync_set_response_data command. */
  uint8_t    subevent;      /**< The subevent in which the advertisement was
                                 received. If the application responds to this
                                 advertisement, use this field as the value of
                                 @p request_subevent parameter for the @ref
                                 sl_bt_pawr_sync_set_response_data command. */
  uint8_t    data_status;   /**< Data completeness:
                                   - <b>0:</b> Complete
                                   - <b>1:</b> Incomplete, more data to come in
                                     new events
                                   - <b>2:</b> Incomplete, data truncated, no
                                     more to come
                                   - <b>255:</b> Failed to receive subevent data
                                     in this subevent */
  uint8_t    counter;       /**< The sequence number of this @ref
                                 sl_bt_evt_pawr_sync_subevent_report event as a
                                 monotonically increasing counter that wraps
                                 from 255 to 0. There is a single counter for
                                 this event type that starts from value 0 when
                                 the Bluetooth stack is started and is never
                                 reset while the stack is running. This counter
                                 can be used to detect if one or more @ref
                                 sl_bt_evt_pawr_sync_subevent_report events have
                                 been created by the stack but dropped due to
                                 problems such as temporarily running out of
                                 resources before the event reached the
                                 application. */
  uint8array data;          /**< The advertisement data that was received */
});

typedef struct sl_bt_evt_pawr_sync_subevent_report_s sl_bt_evt_pawr_sync_subevent_report_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_sync_subevent_report

/***************************************************************************//**
 *
 * Specify the subevents that this device will synchronize to on the specified
 * PAwR train.
 *
 * @param[in] sync PAwR synchronization handle
 * @param[in] subevents_len Length of data in @p subevents
 * @param[in] subevents Array of subevent indexes to synchronize to
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_sync_set_sync_subevents(uint16_t sync,
                                               size_t subevents_len,
                                               const uint8_t* subevents);

/***************************************************************************//**
 *
 * Set the data to be sent in the specified response slot of a subevent of an
 * active PAwR train.
 *
 * Use this command when the application receives the @ref
 * sl_bt_evt_pawr_sync_subevent_report event and the application needs to send a
 * response. The application needs to set the response data within the timing
 * requirements implied by the parameters of the PAwR train that the device is
 * synchronized to. If the response data is set too late, this command will fail
 * with result SL_STATUS_BT_CTRL_ADVERTISING_TIMEOUT.
 *
 * @param[in] sync PAwR synchronization handle
 * @param[in] request_event The periodic advertising event counter
 *   (paEventCounter) of the request. Set this parameter to the value of the @p
 *   event_counter field of the @ref sl_bt_evt_pawr_sync_subevent_report_id
 *   event that the application is responding to.
 * @param[in] request_subevent The subevent of the request. Set this parameter
 *   to the value of the @p subevent field of the @ref
 *   sl_bt_evt_pawr_sync_subevent_report_id event that the application is
 *   responding to.
 * @param[in] response_subevent The subevent in which the response is to be sent
 * @param[in] response_slot The response slot in which the response is to be
 *   sent
 * @param[in] response_data_len Length of data in @p response_data
 * @param[in] response_data Data to be sent in the specified response slot.
 *   Maximum of 248 bytes of data can be set with this command.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_sync_set_response_data(uint16_t sync,
                                              uint16_t request_event,
                                              uint8_t request_subevent,
                                              uint8_t response_subevent,
                                              uint8_t response_slot,
                                              size_t response_data_len,
                                              const uint8_t* response_data);

/** @} */ // end addtogroup sl_bt_pawr_sync

/**
 * @addtogroup sl_bt_pawr_advertiser PAwR Advertiser
 * @{
 *
 * @brief PAwR Advertiser
 *
 * Provides support for advertising with Periodic Advertising with Responses
 * (PAwR) trains.
 */

/* Command and Response IDs */
#define sl_bt_cmd_pawr_advertiser_start_id                           0x00550020
#define sl_bt_cmd_pawr_advertiser_set_subevent_data_id               0x01550020
#define sl_bt_cmd_pawr_advertiser_create_connection_id               0x02550020
#define sl_bt_cmd_pawr_advertiser_stop_id                            0x03550020
#define sl_bt_rsp_pawr_advertiser_start_id                           0x00550020
#define sl_bt_rsp_pawr_advertiser_set_subevent_data_id               0x01550020
#define sl_bt_rsp_pawr_advertiser_create_connection_id               0x02550020
#define sl_bt_rsp_pawr_advertiser_stop_id                            0x03550020

/**
 * @addtogroup sl_bt_evt_pawr_advertiser_subevent_data_request sl_bt_evt_pawr_advertiser_subevent_data_request
 * @{
 * @brief This event is triggered to indicate that the Bluetooth stack is ready
 * to accept data for the specified subevents of the PAwR train.
 *
 * To ensure effective use of the available memory, applications are encouraged
 * to observe these events and set data using @ref
 * sl_bt_pawr_advertiser_set_subevent_data for the subevents that are being
 * requested and for which the application has data to send
 *
 * If the application has no data to send, it does not need to call @ref
 * sl_bt_pawr_advertiser_set_subevent_data for that subevent. The application
 * can attempt to set data for that subevent later when the application has new
 * data to send.
 */

/** @brief Identifier of the subevent_data_request event */
#define sl_bt_evt_pawr_advertiser_subevent_data_request_id           0x005500a0

/***************************************************************************//**
 * @brief Data structure of the subevent_data_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_advertiser_subevent_data_request_s
{
  uint8_t advertising_set;     /**< The PAwR advertising set handle */
  uint8_t subevent_start;      /**< The first subevent that data is requested
                                    for */
  uint8_t subevent_data_count; /**< The number of subevents that data is
                                    requested for */
});

typedef struct sl_bt_evt_pawr_advertiser_subevent_data_request_s sl_bt_evt_pawr_advertiser_subevent_data_request_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_advertiser_subevent_data_request

/**
 * @addtogroup sl_bt_evt_pawr_advertiser_subevent_tx_failed sl_bt_evt_pawr_advertiser_subevent_tx_failed
 * @{
 * @brief This event is triggered if subevent data was successfully set using
 * @ref sl_bt_pawr_advertiser_set_subevent_data command but the attempt to
 * transmit the subevent data has failed
 *
 * This error is not fatal. The event is provided to the application so that it
 * knows not to expect response reports for this subevent and can set subevent
 * data again for retransmission without needing to timeout waiting for response
 * reports that will not be received because the transmission failed.
 */

/** @brief Identifier of the subevent_tx_failed event */
#define sl_bt_evt_pawr_advertiser_subevent_tx_failed_id              0x025500a0

/***************************************************************************//**
 * @brief Data structure of the subevent_tx_failed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_advertiser_subevent_tx_failed_s
{
  uint8_t advertising_set; /**< The PAwR advertising set handle for the PAwR
                                train that failed to transmit subevent data */
  uint8_t subevent;        /**< The subevent that failed to transmit subevent
                                data */
});

typedef struct sl_bt_evt_pawr_advertiser_subevent_tx_failed_s sl_bt_evt_pawr_advertiser_subevent_tx_failed_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_advertiser_subevent_tx_failed

/**
 * @addtogroup sl_bt_evt_pawr_advertiser_response_report sl_bt_evt_pawr_advertiser_response_report
 * @{
 * @brief Reports the status and data of a used response slot of an active PAwR
 * train
 *
 * The report is given for every response slot that was marked as used in the
 * @ref sl_bt_pawr_advertiser_set_subevent_data command.
 */

/** @brief Identifier of the response_report event */
#define sl_bt_evt_pawr_advertiser_response_report_id                 0x015500a0

/***************************************************************************//**
 * @brief Data structure of the response_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_pawr_advertiser_response_report_s
{
  uint8_t    advertising_set; /**< The PAwR advertising set handle */
  uint8_t    subevent;        /**< The subevent that this report corresponds to */
  int8_t     tx_power;        /**< TX power value in the received packet header.
                                   Units: dBm
                                     - <b>Range:</b> -127 to 126

                                     - Value 127: information unavailable */
  int8_t     rssi;            /**< Signal strength indicator (RSSI) of the
                                   received packet. Units: dBm
                                     - <b>Range:</b> -127 to +20

                                     - Value 127: information unavailable */
  uint8_t    cte_type;        /**< The CTE type
                                     - <b>0x00:</b> AoA CTE
                                     - <b>0x01:</b> AoD CTE with 1us slots
                                     - <b>0x02:</b> AoD CTE with 2us slots
                                     - <b>0xFF:</b> No CTE */
  uint8_t    response_slot;   /**< The response slot that this report
                                   corresponds to */
  uint8_t    data_status;     /**< Data completeness:
                                     - <b>0:</b> Complete
                                     - <b>1:</b> Incomplete, more data to come
                                       in new events
                                     - <b>2:</b> Incomplete, data truncated, no
                                       more to come
                                     - <b>255:</b> Failed to receive subevent
                                       response in this response slot */
  uint8_t    counter;         /**< The sequence number of this @ref
                                   sl_bt_evt_pawr_advertiser_response_report
                                   event as a monotonically increasing counter
                                   that wraps from 255 to 0. There is a single
                                   counter for this event type that starts from
                                   value 0 when the Bluetooth stack is started
                                   and is never reset while the stack is
                                   running. This counter can be used to detect
                                   if one or more @ref
                                   sl_bt_evt_pawr_advertiser_response_report
                                   events have been created by the stack but
                                   dropped due to problems such as temporarily
                                   running out of resources before the event
                                   reached the application. */
  uint8array data;            /**< The response data that was received, if any */
});

typedef struct sl_bt_evt_pawr_advertiser_response_report_s sl_bt_evt_pawr_advertiser_response_report_t;

/** @} */ // end addtogroup sl_bt_evt_pawr_advertiser_response_report

/***************************************************************************//**
 *
 * Start PAwR advertising on the specified advertising set.
 *
 * According to the Bluetooth Core specification, PAwR advertising PDUs cannot
 * be transmitted until at least one extended advertising event has been
 * completed. If the application needs exact control over the extended
 * advertising data and parameters, use the @ref sl_bt_advertiser class to
 * configure the parameters of the advertising set and the @ref
 * sl_bt_extended_advertiser class to set or generate the desired extended
 * advertising data payload. If the application does not configure the
 * parameters or set the data, the default parameters and empty advertising data
 * are used for the extended advertising.
 *
 * If the application has not already started extended advertising and the flag
 * @ref SL_BT_PERIODIC_ADVERTISER_AUTO_START_EXTENDED_ADVERTISING is set in @p
 * flags, the stack will automatically start extended advertising with the
 * parameters and extended advertising data currently configured to the
 * advertising set. The application may stop the automatically started extended
 * advertising using the @ref sl_bt_advertiser_stop command.
 *
 * If the application has not already started extended advertising and the flag
 * @ref SL_BT_PERIODIC_ADVERTISER_AUTO_START_EXTENDED_ADVERTISING is not set in
 * @p flags, the stack will momentarily start extended advertising with the
 * parameters and extended advertising data currently configured to the
 * advertising set. Unless the application starts extended advertising before
 * the first extended advertising event has completed, the stack will
 * automatically stop the momentary extended advertising after the first
 * extended advertising event.
 *
 * PAwR advertising PDUs are transmitted on the secondary PHY configured for the
 * advertising set with the @ref sl_bt_extended_advertiser_set_phy command.
 *
 * To stop PAwR advertising, use @ref sl_bt_pawr_advertiser_stop command.
 *
 * @param[in] advertising_set The PAwR advertising set handle
 * @param[in] interval_min @parblock
 *   Minimum periodic advertising interval. Value in units of 1.25 ms.
 *     - <b>Range:</b> 0x06 to 0xFFFF
 *
 *     - Time range: 7.5 ms to 81.92 s
 *
 *     - <b>Default</b> : 100 ms
 *   @endparblock
 * @param[in] interval_max Maximum periodic advertising interval. Value in units
 *   of 1.25 ms.
 *     - Time range: 7.5 ms to 81.92 s
 *     - Note: interval_max should be bigger than interval_min
 * @param[in] flags Additional periodic advertising options. Value: 0 or bitmask
 *   of @ref sl_bt_periodic_advertiser_flags
 * @param[in] num_subevents The number of subevents.
 *     - <b>Range:</b> 0x01 to 0x80
 * @param[in] subevent_interval @parblock
 *   Subevent interval. Value in units of 1.25 ms.
 *     - <b>Range:</b> 0x06 to 0xFF
 *
 *     - Time range: 7.5 ms to 318.75 ms
 *   @endparblock
 * @param[in] response_slot_delay @parblock
 *   Time between the advertising packet in a subevent and the first response
 *   slot. Value in units of 1.25 ms.
 *     - <b>Range:</b> 0x01 to 0xFE
 *
 *     - Time range: 1.25 ms to 317.5 ms
 *   @endparblock
 * @param[in] response_slot_spacing @parblock
 *   Time between response slots. Value in units of 0.125 ms.
 *     - <b>Range:</b> 0x02 to 0xFF
 *
 *     - Time range: 0.25 ms to 31.875 ms
 *   @endparblock
 * @param[in] response_slots Number of subevent response slots.
 *     - <b>Range:</b> 0x01 to 0xFF
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_advertiser_start(uint8_t advertising_set,
                                        uint16_t interval_min,
                                        uint16_t interval_max,
                                        uint32_t flags,
                                        uint8_t num_subevents,
                                        uint8_t subevent_interval,
                                        uint8_t response_slot_delay,
                                        uint8_t response_slot_spacing,
                                        uint8_t response_slots);

/***************************************************************************//**
 *
 * Set data to be sent in the specified subevent of an active PAwR train. Data
 * is transmitted only once and is discarded after it has been transmitted.
 *
 * Data given to this command is passed to the Bluetooth controller, which will
 * queue data and transmit it at the correct time. The application may always
 * opportunistically try to queue more data with this command, but the
 * controller may reject data and return an error if the queuing capacity is
 * exceeded. In this case, the Bluetooth stack will trigger the @ref
 * sl_bt_evt_pawr_advertiser_subevent_data_request event later when the
 * controller is ready to accept more data.
 *
 * To ensure effective use of the available memory, applications are encouraged
 * to observe the @ref sl_bt_evt_pawr_advertiser_subevent_data_request events
 * and set data for the subevents that are being requested and for which the
 * application has data to send. Applications should also note that PAwR is an
 * unreliable transport and cannot guarantee delivery. If reliability is
 * required, the application must implement an acknowledgment mechanism using
 * response slots of the PAwR train and set subevent data again for a
 * re-transmission if it was not successfully delivered.
 *
 * @param[in] advertising_set The PAwR advertising set handle
 * @param[in] subevent The subevent in which the data is to be sent
 * @param[in] response_slot_start The first response slot to be used in this
 *   subevent
 * @param[in] response_slot_count The number of response slots to be used
 * @param[in] adv_data_len Length of data in @p adv_data
 * @param[in] adv_data Data to be sent in the specified subevent
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_pawr_advertiser_subevent_data_request - This event is
 *     triggered when the Bluetooth stack is ready to accept more subevent data.
 *   - @ref sl_bt_evt_pawr_advertiser_subevent_tx_failed - This event is
 *     triggered if transmitting the subevent data has failed.
 *   - @ref sl_bt_evt_pawr_advertiser_response_report - If the subevent data was
 *     successfully transmitted, this event is triggered for each response slot
 *     that was marked as used in this subevent.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_advertiser_set_subevent_data(uint8_t advertising_set,
                                                    uint8_t subevent,
                                                    uint8_t response_slot_start,
                                                    uint8_t response_slot_count,
                                                    size_t adv_data_len,
                                                    const uint8_t* adv_data);

/***************************************************************************//**
 *
 * Initiate a connection request to a device that is synchronized to the
 * specified active PAwR train. The connection is established on the secondary
 * PHY configured for the advertising set with the @ref
 * sl_bt_extended_advertiser_set_phy command. The connection uses the parameters
 * configured with command @ref sl_bt_connection_set_default_parameters.
 *
 * @param[in] advertising_set The PAwR advertising set handle
 * @param[in] subevent The subevent in which the connection request is to be
 *   sent
 * @param[in] address Address of the device to connect to
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. Address type of
 *   the device to connect to. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 * @param[out] connection Handle that will be assigned to the connection after
 *   the connection is established. This handle is valid only if the result code
 *   of this command is SL_STATUS_OK.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_closed - This event is triggered if the
 *     connection failed to be created.
 *   - @ref sl_bt_evt_connection_opened - This event is triggered after the
 *     connection is opened and indicates whether the devices are already bonded
 *     and the role of the device in this connection.
 *   - @ref sl_bt_evt_connection_parameters - This event indicates the
 *     connection parameters and security mode of the connection.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_advertiser_create_connection(uint8_t advertising_set,
                                                    uint8_t subevent,
                                                    bd_addr address,
                                                    uint8_t address_type,
                                                    uint8_t *connection);

/***************************************************************************//**
 *
 * Stop PAwR advertising on an advertising set. Counterpart to @ref
 * sl_bt_pawr_advertiser_start.
 *
 * This command does not affect the enable state of the advertising set, i.e.,
 * legacy or extended advertising is not stopped.
 *
 * @param[in] advertising_set The PAwR advertising set handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_pawr_advertiser_stop(uint8_t advertising_set);

/** @} */ // end addtogroup sl_bt_pawr_advertiser

/**
 * @addtogroup sl_bt_connection Connection
 * @{
 *
 * @brief Connection
 *
 * The commands and events in this class are related to managing connection
 * establishment, parameter setting, and disconnection procedures.
 */

/* Command and Response IDs */
#define sl_bt_cmd_connection_set_default_parameters_id               0x00060020
#define sl_bt_cmd_connection_set_default_preferred_phy_id            0x01060020
#define sl_bt_cmd_connection_set_default_data_length_id              0x10060020
#define sl_bt_cmd_connection_set_default_acceptable_subrate_id       0x19060020
#define sl_bt_cmd_connection_open_id                                 0x04060020
#define sl_bt_cmd_connection_open_with_accept_list_id                0x16060020
#define sl_bt_cmd_connection_set_parameters_id                       0x06060020
#define sl_bt_cmd_connection_set_preferred_phy_id                    0x08060020
#define sl_bt_cmd_connection_disable_slave_latency_id                0x03060020
#define sl_bt_cmd_connection_get_median_rssi_id                      0x15060020
#define sl_bt_cmd_connection_read_channel_map_id                     0x07060020
#define sl_bt_cmd_connection_set_power_reporting_id                  0x09060020
#define sl_bt_cmd_connection_set_remote_power_reporting_id           0x0a060020
#define sl_bt_cmd_connection_get_tx_power_id                         0x0b060020
#define sl_bt_cmd_connection_get_remote_tx_power_id                  0x0c060020
#define sl_bt_cmd_connection_set_tx_power_id                         0x12060020
#define sl_bt_cmd_connection_read_remote_used_features_id            0x0d060020
#define sl_bt_cmd_connection_get_security_status_id                  0x0e060020
#define sl_bt_cmd_connection_set_data_length_id                      0x11060020
#define sl_bt_cmd_connection_read_statistics_id                      0x13060020
#define sl_bt_cmd_connection_get_scheduling_details_id               0x14060020
#define sl_bt_cmd_connection_get_remote_address_id                   0x17060020
#define sl_bt_cmd_connection_request_subrate_id                      0x1a060020
#define sl_bt_cmd_connection_get_state_id                            0x18060020
#define sl_bt_cmd_connection_close_id                                0x05060020
#define sl_bt_cmd_connection_forcefully_close_id                     0x0f060020
#define sl_bt_rsp_connection_set_default_parameters_id               0x00060020
#define sl_bt_rsp_connection_set_default_preferred_phy_id            0x01060020
#define sl_bt_rsp_connection_set_default_data_length_id              0x10060020
#define sl_bt_rsp_connection_set_default_acceptable_subrate_id       0x19060020
#define sl_bt_rsp_connection_open_id                                 0x04060020
#define sl_bt_rsp_connection_open_with_accept_list_id                0x16060020
#define sl_bt_rsp_connection_set_parameters_id                       0x06060020
#define sl_bt_rsp_connection_set_preferred_phy_id                    0x08060020
#define sl_bt_rsp_connection_disable_slave_latency_id                0x03060020
#define sl_bt_rsp_connection_get_median_rssi_id                      0x15060020
#define sl_bt_rsp_connection_read_channel_map_id                     0x07060020
#define sl_bt_rsp_connection_set_power_reporting_id                  0x09060020
#define sl_bt_rsp_connection_set_remote_power_reporting_id           0x0a060020
#define sl_bt_rsp_connection_get_tx_power_id                         0x0b060020
#define sl_bt_rsp_connection_get_remote_tx_power_id                  0x0c060020
#define sl_bt_rsp_connection_set_tx_power_id                         0x12060020
#define sl_bt_rsp_connection_read_remote_used_features_id            0x0d060020
#define sl_bt_rsp_connection_get_security_status_id                  0x0e060020
#define sl_bt_rsp_connection_set_data_length_id                      0x11060020
#define sl_bt_rsp_connection_read_statistics_id                      0x13060020
#define sl_bt_rsp_connection_get_scheduling_details_id               0x14060020
#define sl_bt_rsp_connection_get_remote_address_id                   0x17060020
#define sl_bt_rsp_connection_request_subrate_id                      0x1a060020
#define sl_bt_rsp_connection_get_state_id                            0x18060020
#define sl_bt_rsp_connection_close_id                                0x05060020
#define sl_bt_rsp_connection_forcefully_close_id                     0x0f060020

/**
 * @brief Types of device roles in a Bluetooth connection
 */
typedef enum
{
  sl_bt_connection_role_peripheral = 0x0, /**< (0x0) The Peripheral role */
  sl_bt_connection_role_central    = 0x1  /**< (0x1) The Central role */
} sl_bt_connection_role_t;

/**
 * @brief Connection states
 */
typedef enum
{
  sl_bt_connection_state_closed  = 0x0, /**< (0x0) Connection closed */
  sl_bt_connection_state_closing = 0x1, /**< (0x1) Connection is being closed */
  sl_bt_connection_state_open    = 0x2, /**< (0x2) Connection open */
  sl_bt_connection_state_opening = 0x3  /**< (0x3) Connection is being opened */
} sl_bt_connection_state_t;

/**
 * @brief Indicate the Bluetooth Security Mode.
 */
typedef enum
{
  sl_bt_connection_mode1_level1 = 0x0, /**< (0x0) No security */
  sl_bt_connection_mode1_level2 = 0x1, /**< (0x1) Unauthenticated pairing with
                                            encryption */
  sl_bt_connection_mode1_level3 = 0x2, /**< (0x2) Authenticated pairing with
                                            encryption */
  sl_bt_connection_mode1_level4 = 0x3  /**< (0x3) Authenticated Secure
                                            Connections pairing with encryption
                                            using a 128-bit strength encryption
                                            key */
} sl_bt_connection_security_t;

/**
 * @brief These values define transmit power reporting modes in LE power
 * control.
 */
typedef enum
{
  sl_bt_connection_power_reporting_disable = 0x0, /**< (0x0) Disable transmit
                                                       power reporting */
  sl_bt_connection_power_reporting_enable  = 0x1  /**< (0x1) Enable transmit
                                                       power reporting */
} sl_bt_connection_power_reporting_mode_t;

/**
 * @brief This enum defines the flag values for a reported transmit power level.
 */
typedef enum
{
  sl_bt_connection_tx_power_flag_none  = 0x0, /**< (0x0) No flag is defined for
                                                   the reported TX power level */
  sl_bt_connection_tx_power_at_minimum = 0x1, /**< (0x1) Transmit power level is
                                                   at minimum level. */
  sl_bt_connection_tx_power_at_maximum = 0x2  /**< (0x2) Transmit power level is
                                                   at maximum level. */
} sl_bt_connection_tx_power_flag_t;

/**
 * @addtogroup sl_bt_connection_rssi_const RSSI Value Constants
 * @{
 *
 * Constants for RSSI value
 */

/** RSSI value is unavailable */
#define SL_BT_CONNECTION_RSSI_UNAVAILABLE 0x7f      

/** @} */ // end RSSI Value Constants

/**
 * @addtogroup sl_bt_connection_tx_power_const Transmit Power Reporting Constants
 * @{
 *
 * Constants in transmit power reporting
 */

/** Remote device is not managing power levels. */
#define SL_BT_CONNECTION_TX_POWER_UNMANAGED          0x7e      

/** Transmit power level is not available. */
#define SL_BT_CONNECTION_TX_POWER_UNAVAILABLE        0x7f      

/** Change is not available or is out of range. */
#define SL_BT_CONNECTION_TX_POWER_CHANGE_UNAVAILABLE 0x7f      

/** @} */ // end Transmit Power Reporting Constants

/**
 * @addtogroup sl_bt_evt_connection_opened sl_bt_evt_connection_opened
 * @{
 * @brief Indicates that a new connection was opened
 *
 * The specific semantics depend on the role of the device:
 *   - On the central device (parameter @p role set to @ref
 *     sl_bt_connection_role_central) the event is triggered when the Link Layer
 *     has sent a connect request to the remote device.
 *   - On the peripheral device (parameter @p role set to @ref
 *     sl_bt_connection_role_peripheral) the event is triggered when the Link
 *     Layer has responded to the connect request of the remote device.
 *
 * This event does not indicate the connection has been established (i.e., that
 * a data packet was received within 6 connection intervals). If the connection
 * is not established, an @ref sl_bt_evt_connection_closed event will
 * immediately follow.
 *
 * An open connection can be closed with @ref sl_bt_connection_close command by
 * giving the connection handle obtained from this event.
 */

/** @brief Identifier of the opened event */
#define sl_bt_evt_connection_opened_id                               0x000600a0

/***************************************************************************//**
 * @brief Data structure of the opened event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_opened_s
{
  bd_addr  address;      /**< Remote device address */
  uint8_t  address_type; /**< Enum @ref sl_bt_gap_address_type_t. Remote device
                              address type. Values:
                                - <b>sl_bt_gap_public_address (0x0):</b> Public
                                  device address
                                - <b>sl_bt_gap_static_address (0x1):</b> Static
                                  device address
                                - <b>sl_bt_gap_random_resolvable_address
                                  (0x2):</b> Resolvable private random address
                                - <b>sl_bt_gap_random_nonresolvable_address
                                  (0x3):</b> Non-resolvable private random
                                  address
                                - <b>sl_bt_gap_public_address_resolved_from_rpa
                                  (0x4):</b> Public identity address resolved
                                  from a resolvable private address (RPA)
                                - <b>sl_bt_gap_static_address_resolved_from_rpa
                                  (0x5):</b> Static identity address resolved
                                  from a resolvable private address (RPA) */
  uint8_t  role;         /**< Enum @ref sl_bt_connection_role_t. The role this
                              device operates in the connection. Values:
                                - <b>sl_bt_connection_role_peripheral (0x0):</b>
                                  The Peripheral role
                                - <b>sl_bt_connection_role_central (0x1):</b>
                                  The Central role */
  uint8_t  connection;   /**< Handle for new connection */
  uint8_t  bonding;      /**< Bonding handle. Values:
                                - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b> No
                                  bonding
                                - <b>Other:</b> Bonding handle */
  uint8_t  advertiser;   /**< The local advertising set that this connection was
                              opened to. Values:
                                - <b>SL_BT_INVALID_ADVERTISING_SET_HANDLE
                                  (0xff):</b> Invalid value or not applicable.
                                  Ignore this field
                                - <b>Other:</b> The advertising set handle */
  uint16_t sync;         /**< The sync handle if the local device is in the
                              peripheral-role and the connection was opened via
                              a Periodic Advertising with Responses (PAwR) train
                              that the local device has synchronized to. Values:
                                - <b>SL_BT_INVALID_SYNC_HANDLE (0xffff):</b> The
                                  connection was not opened over a PAwR train
                                  that the local device is synchronized to
                                - <b>Other:</b> The sync handle for the PAwR
                                  train that was used to open the connection */
});

typedef struct sl_bt_evt_connection_opened_s sl_bt_evt_connection_opened_t;

/** @} */ // end addtogroup sl_bt_evt_connection_opened

/**
 * @addtogroup sl_bt_evt_connection_parameters sl_bt_evt_connection_parameters
 * @{
 * @brief Triggered whenever the connection parameters are changed and at any
 * time a connection is established
 */

/** @brief Identifier of the parameters event */
#define sl_bt_evt_connection_parameters_id                           0x020600a0

/***************************************************************************//**
 * @brief Data structure of the parameters event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_parameters_s
{
  uint8_t  connection;    /**< Connection handle */
  uint16_t interval;      /**< Connection interval. Time = Value x 1.25 ms */
  uint16_t latency;       /**< Peripheral latency (how many connection intervals
                               the peripheral can skip) */
  uint16_t timeout;       /**< Supervision timeout. Time = Value x 10 ms */
  uint8_t  security_mode; /**< Enum @ref sl_bt_connection_security_t. Connection
                               security mode. Values:
                                 - <b>sl_bt_connection_mode1_level1 (0x0):</b>
                                   No security
                                 - <b>sl_bt_connection_mode1_level2 (0x1):</b>
                                   Unauthenticated pairing with encryption
                                 - <b>sl_bt_connection_mode1_level3 (0x2):</b>
                                   Authenticated pairing with encryption
                                 - <b>sl_bt_connection_mode1_level4 (0x3):</b>
                                   Authenticated Secure Connections pairing with
                                   encryption using a 128-bit strength
                                   encryption key */
});

typedef struct sl_bt_evt_connection_parameters_s sl_bt_evt_connection_parameters_t;

/** @} */ // end addtogroup sl_bt_evt_connection_parameters

/**
 * @addtogroup sl_bt_evt_connection_set_parameters_failed sl_bt_evt_connection_set_parameters_failed
 * @{
 * @brief Triggered when the peer device rejected an L2CAP connection parameter
 * update request as a result of calling command @ref
 * sl_bt_connection_set_parameters to change connection parameters on the @ref
 * sl_bt_connection_role_peripheral role
 */

/** @brief Identifier of the set_parameters_failed event */
#define sl_bt_evt_connection_set_parameters_failed_id                0x100600a0

/***************************************************************************//**
 * @brief Data structure of the set_parameters_failed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_set_parameters_failed_s
{
  uint8_t connection; /**< Connection handle */
});

typedef struct sl_bt_evt_connection_set_parameters_failed_s sl_bt_evt_connection_set_parameters_failed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_set_parameters_failed

/**
 * @addtogroup sl_bt_evt_connection_phy_status sl_bt_evt_connection_phy_status
 * @{
 * @brief Indicates that PHY update procedure is completed.
 */

/** @brief Identifier of the phy_status event */
#define sl_bt_evt_connection_phy_status_id                           0x040600a0

/***************************************************************************//**
 * @brief Data structure of the phy_status event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_phy_status_s
{
  uint8_t connection; /**< Connection handle */
  uint8_t phy;        /**< Current active PHY. See values from @ref
                           sl_bt_connection_set_preferred_phy command. */
});

typedef struct sl_bt_evt_connection_phy_status_s sl_bt_evt_connection_phy_status_t;

/** @} */ // end addtogroup sl_bt_evt_connection_phy_status

/**
 * @addtogroup sl_bt_evt_connection_get_remote_tx_power_completed sl_bt_evt_connection_get_remote_tx_power_completed
 * @{
 * @brief Indicates that reading remote transmit power initiated by @ref
 * sl_bt_connection_get_remote_tx_power command has completed
 */

/** @brief Identifier of the get_remote_tx_power_completed event */
#define sl_bt_evt_connection_get_remote_tx_power_completed_id        0x050600a0

/***************************************************************************//**
 * @brief Data structure of the get_remote_tx_power_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_get_remote_tx_power_completed_s
{
  uint16_t status;      /**< SL_STATUS_OK or another error code indicating the
                             reading remote transmit power operation failed. */
  uint8_t  connection;  /**< Handle of the connection */
  uint8_t  phy;         /**< Enum @ref sl_bt_gap_phy_coding_t. The PHY for which
                             the transmit power is reported. Values:
                               - <b>sl_bt_gap_phy_coding_1m_uncoded (0x1):</b>
                                 1M PHY
                               - <b>sl_bt_gap_phy_coding_2m_uncoded (0x2):</b>
                                 2M PHY
                               - <b>sl_bt_gap_phy_coding_125k_coded (0x4):</b>
                                 125k Coded PHY (S=8)
                               - <b>sl_bt_gap_phy_coding_500k_coded (0x8):</b>
                                 500k Coded PHY (S=2) */
  int8_t   power_level; /**< Transmit power level in dBm. Values:
                               - <b>Range:</b> -127 to +20

                               - <b>@ref SL_BT_CONNECTION_TX_POWER_UNMANAGED
                                 (0x7E):</b> Remote device is not managing power
                                 levels on this PHY.
                               - <b>@ref SL_BT_CONNECTION_TX_POWER_UNAVAILABLE
                                 (0x7F):</b> Transmit power level is not
                                 available. */
  uint8_t  flags;       /**< Enum @ref sl_bt_connection_tx_power_flag_t.
                             Transmit power level flags. Ignore this field if @p
                             power_level is set to 0x7E or 0x7F. Values:
                               - <b>sl_bt_connection_tx_power_flag_none
                                 (0x0):</b> No flag is defined for the reported
                                 TX power level
                               - <b>sl_bt_connection_tx_power_at_minimum
                                 (0x1):</b> Transmit power level is at minimum
                                 level.
                               - <b>sl_bt_connection_tx_power_at_maximum
                                 (0x2):</b> Transmit power level is at maximum
                                 level. */
  int8_t   delta;       /**< Change in transmit power level. Values:
                               - <b>@ref
                                 SL_BT_CONNECTION_TX_POWER_CHANGE_UNAVAILABLE
                                 (0x7F):</b> Change is not available or is out
                                 of range.
                               - <b>Other values:</b> positive indicates
                                 increased power, negative indicates decreased
                                 power, and zero indicates unchanged. Units: dB */
});

typedef struct sl_bt_evt_connection_get_remote_tx_power_completed_s sl_bt_evt_connection_get_remote_tx_power_completed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_get_remote_tx_power_completed

/**
 * @addtogroup sl_bt_evt_connection_tx_power sl_bt_evt_connection_tx_power
 * @{
 * @brief Reports a transmit power change on the local device of a connection if
 * transmit power reporting has been enabled
 *
 * Enable or disable transmit power reporting using @ref
 * sl_bt_connection_set_power_reporting command. Local transmit power reporting
 * is enabled by default for new connections.
 *
 * When enabled, local transmit power change events may get triggered in the
 * following situations:
 *   - When the connection has just opened and the initial transmit power is set
 *   - When the application explicitly changes the system TX power using the
 *     @ref sl_bt_system_set_tx_power command
 *   - When the bluetooth_feature_power_control component is included and
 *     enabled in the application and the local transmit power is adjusted by
 *     the power control feature
 *   - When the bluetooth_feature_afh component for Adaptive Frequency Hopping
 *     is included and enabled in the application and the local transmit power
 *     is adjusted due to changes in the availability of good channels.
 */

/** @brief Identifier of the tx_power event */
#define sl_bt_evt_connection_tx_power_id                             0x060600a0

/***************************************************************************//**
 * @brief Data structure of the tx_power event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_tx_power_s
{
  uint8_t connection;  /**< Handle of the connection */
  uint8_t phy;         /**< Enum @ref sl_bt_gap_phy_coding_t. The PHY which the
                            transmit power is reported for. Values:
                              - <b>sl_bt_gap_phy_coding_1m_uncoded (0x1):</b> 1M
                                PHY
                              - <b>sl_bt_gap_phy_coding_2m_uncoded (0x2):</b> 2M
                                PHY
                              - <b>sl_bt_gap_phy_coding_125k_coded (0x4):</b>
                                125k Coded PHY (S=8)
                              - <b>sl_bt_gap_phy_coding_500k_coded (0x8):</b>
                                500k Coded PHY (S=2) */
  int8_t  power_level; /**< Transmit power level in dBm. Values:
                              - <b>@ref SL_BT_CONNECTION_TX_POWER_UNAVAILABLE
                                (0x7F):</b> Transmit power level is not
                                available. */
  uint8_t flags;       /**< Enum @ref sl_bt_connection_tx_power_flag_t. Transmit
                            power level flags. Ignore this field if @p
                            power_level is set to 0x7E or 0x7F. Values:
                              - <b>sl_bt_connection_tx_power_flag_none
                                (0x0):</b> No flag is defined for the reported
                                TX power level
                              - <b>sl_bt_connection_tx_power_at_minimum
                                (0x1):</b> Transmit power level is at minimum
                                level.
                              - <b>sl_bt_connection_tx_power_at_maximum
                                (0x2):</b> Transmit power level is at maximum
                                level. */
  int8_t  delta;       /**< Change in transmit power level. Values:
                              - <b>@ref
                                SL_BT_CONNECTION_TX_POWER_CHANGE_UNAVAILABLE
                                (0x7F):</b> Change is not available or is out of
                                range.
                              - <b>Other values:</b> positive indicates
                                increased power, negative indicates decreased
                                power, and zero indicates unchanged. Units: dB */
});

typedef struct sl_bt_evt_connection_tx_power_s sl_bt_evt_connection_tx_power_t;

/** @} */ // end addtogroup sl_bt_evt_connection_tx_power

/**
 * @addtogroup sl_bt_evt_connection_remote_tx_power sl_bt_evt_connection_remote_tx_power
 * @{
 * @brief Reports a transmit power change of the connection on the remote device
 * that supports the LE Power Control for the connection
 *
 * This event is enabled if reporting the remote transmit power change has been
 * enabled with the @ref sl_bt_connection_set_remote_power_reporting command.
 */

/** @brief Identifier of the remote_tx_power event */
#define sl_bt_evt_connection_remote_tx_power_id                      0x070600a0

/***************************************************************************//**
 * @brief Data structure of the remote_tx_power event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_remote_tx_power_s
{
  uint8_t connection;  /**< Handle of the connection */
  uint8_t phy;         /**< Enum @ref sl_bt_gap_phy_coding_t. The PHY which the
                            transmit power is reported for. Values:
                              - <b>sl_bt_gap_phy_coding_1m_uncoded (0x1):</b> 1M
                                PHY
                              - <b>sl_bt_gap_phy_coding_2m_uncoded (0x2):</b> 2M
                                PHY
                              - <b>sl_bt_gap_phy_coding_125k_coded (0x4):</b>
                                125k Coded PHY (S=8)
                              - <b>sl_bt_gap_phy_coding_500k_coded (0x8):</b>
                                500k Coded PHY (S=2) */
  int8_t  power_level; /**< Transmit power level in dBm. Values:
                              - <b>Range:</b> -127 to +20

                              - <b>@ref SL_BT_CONNECTION_TX_POWER_UNMANAGED
                                (0x7E):</b> Remote device is not managing power
                                levels on this PHY.
                              - <b>@ref SL_BT_CONNECTION_TX_POWER_UNAVAILABLE
                                (0x7F):</b> Transmit power level is not
                                available. */
  uint8_t flags;       /**< Enum @ref sl_bt_connection_tx_power_flag_t. Transmit
                            power level flags. Ignore this field if @p
                            power_level is set to 0x7E or 0x7F. Values:
                              - <b>sl_bt_connection_tx_power_flag_none
                                (0x0):</b> No flag is defined for the reported
                                TX power level
                              - <b>sl_bt_connection_tx_power_at_minimum
                                (0x1):</b> Transmit power level is at minimum
                                level.
                              - <b>sl_bt_connection_tx_power_at_maximum
                                (0x2):</b> Transmit power level is at maximum
                                level. */
  int8_t  delta;       /**< Change in transmit power level. Values:
                              - <b>@ref
                                SL_BT_CONNECTION_TX_POWER_CHANGE_UNAVAILABLE
                                (0x7F):</b> Change is not available or is out of
                                range.
                              - <b>Other values:</b> positive indicates
                                increased power, negative indicates decreased
                                power, and zero indicates unchanged. Units: dB */
});

typedef struct sl_bt_evt_connection_remote_tx_power_s sl_bt_evt_connection_remote_tx_power_t;

/** @} */ // end addtogroup sl_bt_evt_connection_remote_tx_power

/**
 * @addtogroup sl_bt_evt_connection_remote_used_features sl_bt_evt_connection_remote_used_features
 * @{
 * @brief List of link layer features supported by the remote device.
 */

/** @brief Identifier of the remote_used_features event */
#define sl_bt_evt_connection_remote_used_features_id                 0x080600a0

/***************************************************************************//**
 * @brief Data structure of the remote_used_features event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_remote_used_features_s
{
  uint8_t    connection; /**< Connection handle */
  uint8array features;   /**< This parameter is 8 bytes in little endian format
                              and contains bit fields describing the supported
                              link layer features of the remote device. Bit
                              value 1 means that the feature is supported.

                              The bits are explained in Bluetooth specification
                              Vol 6, Part B, 4.6. */
});

typedef struct sl_bt_evt_connection_remote_used_features_s sl_bt_evt_connection_remote_used_features_t;

/** @} */ // end addtogroup sl_bt_evt_connection_remote_used_features

/**
 * @addtogroup sl_bt_evt_connection_data_length sl_bt_evt_connection_data_length
 * @{
 * @brief Reports a change to the maximum payload length or maximum TX time in
 * either direction of a connection.
 */

/** @brief Identifier of the data_length event */
#define sl_bt_evt_connection_data_length_id                          0x090600a0

/***************************************************************************//**
 * @brief Data structure of the data_length event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_data_length_s
{
  uint8_t  connection;  /**< Connection handle */
  uint16_t tx_data_len; /**< The maximum payload octets of a packet that the
                             local Controller will send */
  uint16_t tx_time_us;  /**< The maximum time in microseconds that the local
                             Controller will take to send a data packet */
  uint16_t rx_data_len; /**< The maximum payload octets of a packet that the
                             local Controller expects to receive */
  uint16_t rx_time_us;  /**< The maximum time in microseconds that the local
                             Controller expects to take to receive a data packet */
});

typedef struct sl_bt_evt_connection_data_length_s sl_bt_evt_connection_data_length_t;

/** @} */ // end addtogroup sl_bt_evt_connection_data_length

/**
 * @addtogroup sl_bt_evt_connection_statistics sl_bt_evt_connection_statistics
 * @{
 * @brief Provides the connection statistic values when the application includes
 * the component bluetooth_feature_connection_statistics
 *
 * When the component is included by the application, this event is
 * automatically triggered to provide the final statistics for the connection
 * just before the @ref sl_bt_evt_connection_closed event indicates that a
 * connection has been closed. This event is also triggered when the application
 * has requested reading the statistics using the command @ref
 * sl_bt_connection_read_statistics.
 */

/** @brief Identifier of the statistics event */
#define sl_bt_evt_connection_statistics_id                           0x0a0600a0

/***************************************************************************//**
 * @brief Data structure of the statistics event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_statistics_s
{
  uint8_t  connection;                       /**< Handle of the connection */
  int8_t   rssi_min;                         /**< The minimum Received Signal
                                                  Strength Indicator (RSSI)
                                                  value measured for packets
                                                  that have been successfully
                                                  received on this connection.
                                                  Units: dBm
                                                    - <b>Range:</b> -127 to +20

                                                    - Value 127: information
                                                      unavailable */
  int8_t   rssi_max;                         /**< The maximum Received Signal
                                                  Strength Indicator (RSSI)
                                                  value measured for packets
                                                  that have been successfully
                                                  received on this connection.
                                                  Units: dBm
                                                    - <b>Range:</b> -127 to +20

                                                    - Value 127: information
                                                      unavailable */
  uint32_t num_total_connection_events;      /**< The total number of connection
                                                  events that would have
                                                  occurred on this connection if
                                                  no packets were lost. This
                                                  count therefore includes also
                                                  connection events that were
                                                  missed due to errors (see @p
                                                  num_missed_connection_events).
                                                  In the Peripheral device this
                                                  count does not include
                                                  connection events where the
                                                  Peripheral deliberately did
                                                  not listen due to Peripheral
                                                  latency. */
  uint32_t num_missed_connection_events;     /**< The number of connection
                                                  events that have been missed
                                                  on this connection. This
                                                  includes connection events
                                                  where the first packet of a
                                                  connection event was either
                                                  not received at all or was
                                                  corrupted so that it was not
                                                  recognized as belonging to
                                                  this connection. */
  uint32_t num_successful_connection_events; /**< The number of connection
                                                  events that have occured on
                                                  this connection without
                                                  errors. */
  uint32_t num_crc_errors;                   /**< The number of packets that
                                                  have been received on this
                                                  connection with a Cyclic
                                                  Redundancy Check (CRC) error. */
});

typedef struct sl_bt_evt_connection_statistics_s sl_bt_evt_connection_statistics_t;

/** @} */ // end addtogroup sl_bt_evt_connection_statistics

/**
 * @addtogroup sl_bt_evt_connection_request_subrate_failed sl_bt_evt_connection_request_subrate_failed
 * @{
 * @brief This event is triggered when the command @ref
 * sl_bt_connection_request_subrate has failed and returns the error code for
 * that failure
 */

/** @brief Identifier of the request_subrate_failed event */
#define sl_bt_evt_connection_request_subrate_failed_id               0x0d0600a0

/***************************************************************************//**
 * @brief Data structure of the request_subrate_failed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_request_subrate_failed_s
{
  uint8_t  connection; /**< Connection handle */
  uint16_t result;     /**< The error code provided by the failed procedure */
});

typedef struct sl_bt_evt_connection_request_subrate_failed_s sl_bt_evt_connection_request_subrate_failed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_request_subrate_failed

/**
 * @addtogroup sl_bt_evt_connection_subrate_changed sl_bt_evt_connection_subrate_changed
 * @{
 * @brief This event is triggered when the procedure started by @ref
 * sl_bt_connection_request_subrate has completed successfully or when the
 * subrate parameters have changed due to a peer request
 */

/** @brief Identifier of the subrate_changed event */
#define sl_bt_evt_connection_subrate_changed_id                      0x0e0600a0

/***************************************************************************//**
 * @brief Data structure of the subrate_changed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_subrate_changed_s
{
  uint8_t  connection;          /**< Connection handle */
  uint16_t subrate_factor;      /**< Subrate factor applied to the specified
                                     underlying connection interval. */
  uint16_t latency;             /**< Peripheral latency for the connection in
                                     number of subrated connection events. */
  uint16_t continuation_number; /**< Number of underlying connection events to
                                     remain active after a packet containing a
                                     Link Layer PDU with a non-zero Length field
                                     is sent or received. */
  uint16_t timeout;             /**< Supervision timeout. Time = Value x 10 ms */
});

typedef struct sl_bt_evt_connection_subrate_changed_s sl_bt_evt_connection_subrate_changed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_subrate_changed

/**
 * @addtogroup sl_bt_evt_connection_closed sl_bt_evt_connection_closed
 * @{
 * @brief Indicates that a connection was either closed or that no connection
 * was established from a high duty cycle directed advertising.
 *
 * If the application includes the component
 * bluetooth_feature_connection_statistics and the connection was successfully
 * opened, the event @ref sl_bt_evt_connection_statistics is automatically
 * triggered just before the connection closed event to provide the application
 * with the final statistic values of the connection.
 */

/** @brief Identifier of the closed event */
#define sl_bt_evt_connection_closed_id                               0x010600a0

/***************************************************************************//**
 * @brief Data structure of the closed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_closed_s
{
  uint16_t reason;     /**< Reason of connection close.

                            Error code SL_STATUS_BT_CTRL_ADVERTISING_TIMEOUT
                            indicates that the high duty cycle directed
                            advertising timed out and no connection was
                            established. Ignore the value of @p connection in
                            this case. */
  uint8_t  connection; /**< Handle of the closed connection */
});

typedef struct sl_bt_evt_connection_closed_s sl_bt_evt_connection_closed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_closed

/***************************************************************************//**
 *
 * Set default Bluetooth connection parameters. The values are valid for all
 * subsequent connections initiated by this device.
 *
 * @p min_ce_length and @p max_ce_length specify the preference of the
 * connection event length so that the Link Layer can prioritize tasks
 * accordingly in simultaneous connections, or scanning and so on. A connection
 * event starts at an anchor point of a connection interval and lasts until the
 * lesser of @p max_ce_length and the actual connection interval. Packets that
 * do not fit into the connection event will be sent in the next connection
 * interval.
 *
 * To change parameters of an already established connection, use the command
 * @ref sl_bt_connection_set_parameters.
 *
 * @param[in] min_interval @parblock
 *   Minimum value for the connection event interval. This must be set less than
 *   or equal to @p max_interval.
 *     - <b>Range:</b> 0x0006 to 0x0c80
 *
 *     - Time = Value x 1.25 ms
 *     - Time Range: 7.5 ms to 4 s
 *
 *     - <b>Default</b> : 20 ms
 *   @endparblock
 * @param[in] max_interval @parblock
 *   Maximum value for the connection event interval. This must be set greater
 *   than or equal to @p min_interval.
 *     - <b>Range:</b> 0x0006 to 0x0c80
 *
 *     - Time = Value x 1.25 ms
 *     - Time Range: 7.5 ms to 4 s
 *
 *     - <b>Default</b> : 50 ms
 *   @endparblock
 * @param[in] latency @parblock
 *   Peripheral latency, which defines how many connection intervals the
 *   peripheral can skip if it has no data to send
 *     - <b>Range:</b> 0x0000 to 0x01f3
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   Supervision timeout, which defines the time that the connection is
 *   maintained although the devices can't communicate at the currently
 *   configured connection intervals.
 *     - <b>Range:</b> 0x000a to 0x0c80
 *
 *     - Time = Value x 10 ms
 *     - Time Range: 100 ms to 32 s
 *     - The value in milliseconds must be larger than (1 + @p latency) * @p
 *       max_interval * 2, where @p max_interval is given in milliseconds
 *
 *   Set the supervision timeout at a value which allows communication attempts
 *   over at least a few connection intervals.
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] min_ce_length @parblock
 *   Minimum length of the connection event. It must be less than or equal to @p
 *   max_ce_length.
 *
 *   This value defines the minimum time that should be given to the connection
 *   event in a situation where other tasks need to run immediately after the
 *   connection event. When the value is very small, the connection event still
 *   has at least one TX/RX operation. If this value is increased, more time is
 *   reserved for the connection event so it can transmit and receive more
 *   packets in a connection interval.
 *
 *   Use the default value if the application doesn't care about the connection
 *   event length or doesn't want to do fine tuning.
 *
 *     - Time = Value x 0.625 ms
 *
 *     - <b>Range:</b> 0x0000 to 0xffff
 *
 *     - <b>Default</b> : 0x0000
 *   @endparblock
 * @param[in] max_ce_length @parblock
 *   Maximum length of the connection event. It must be greater than or equal to
 *   @p min_ce_length.
 *
 *   This value is used for limiting the connection event length so that a
 *   connection that has large amounts of data to transmit or receive doesn't
 *   block other tasks. Limiting the connection event is a hard stop. If there
 *   is no enough time to send or receive a packet, the connection event will be
 *   closed.
 *
 *   If the value is set to 0, the connection event still has at least one TX/RX
 *   operation. This is useful to limit power consumption or leave more time to
 *   other tasks.
 *
 *   Use the default value if the application doesn't care about the connection
 *   event length or doesn't want to do fine tuning.
 *     - Time = Value x 0.625 ms
 *
 *     - <b>Range:</b> 0x0000 to 0xffff
 *
 *     - <b>Default</b> : 0xffff
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_default_parameters(uint16_t min_interval,
                                                    uint16_t max_interval,
                                                    uint16_t latency,
                                                    uint16_t timeout,
                                                    uint16_t min_ce_length,
                                                    uint16_t max_ce_length);

/***************************************************************************//**
 *
 * Set default preferred and accepted PHYs. PHY settings will be used for all
 * subsequent connections. Non-preferred PHY can also be set if the remote
 * device does not accept any of the preferred PHYs.
 *
 * The parameter @p accepted_phy is used to specify PHYs that the stack can
 * accept in a remotely-initiated PHY update request. A PHY update will not
 * happen if none of the accepted PHYs are present in the request.
 *
 * <b>NOTE:</b> 2M and Coded PHYs are not supported by all devices.
 *
 * @param[in] preferred_phy @parblock
 *   Preferred PHYs. This parameter is a bitfield and multiple PHYs can be set.
 *     - <b>0x01:</b> 1M PHY
 *     - <b>0x02:</b> 2M PHY
 *     - <b>0x04:</b> Coded PHY
 *     - <b>0xff:</b> Any PHYs
 *
 *   Default: 0xff (no preference)
 *   @endparblock
 * @param[in] accepted_phy @parblock
 *   Accepted PHYs in remotely-initiated PHY update request. This parameter is a
 *   bitfield and multiple PHYs can be set.
 *     - <b>0x01:</b> 1M PHY
 *     - <b>0x02:</b> 2M PHY
 *     - <b>0x04:</b> Coded PHY
 *     - <b>0xff:</b> Any PHYs
 *
 *   Default: 0xff (all PHYs accepted)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_default_preferred_phy(uint8_t preferred_phy,
                                                       uint8_t accepted_phy);

/***************************************************************************//**
 *
 * Set the default preferred maximum TX payload length to be used for new
 * connections.
 *
 * When a connection is open, the maximum TX payload length is 27. Either device
 * could initiate a data length update procedure and event @ref
 * sl_bt_evt_connection_data_length is generated when the data length has been
 * changed on the connection.
 *
 * @param[in] tx_data_len @parblock
 *   Preferred maximum payload octets of a packet that the local Controller will
 *   send
 *
 *     - <b>Range:</b> 0x001B to 0x00FB
 *
 *     - <b>Default</b> : 251
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_default_data_length(uint16_t tx_data_len);

/***************************************************************************//**
 *
 * Set the default acceptable parameters for subrating requests made by the
 * peripheral on future ACL connections where this device is the central. The
 * application must include the Connection Subrating feature
 * (bluetooth_feature_connection_subrating) in order to use this command.
 *
 * When a connection is established, central will store these values to
 * determine if future subrate requests from the peripheral are acceptable.
 * Peripheral may store it's own set of parameters independently but they will
 * not be used in that role. Running this command has no effect on connections
 * that are already active, instead to change the stored acceptable values, use
 * the command @ref sl_bt_connection_request_subrate on the central device.
 *
 * Subrating is not automatically enabled on new a connection. Use the command
 * @ref sl_bt_connection_request_subrate to set the subrate parameters and
 * enable subrating of a specific connection.
 *
 * @param[in] min_subrate @parblock
 *   Minimum subrate factor allowed in requests by a peripheral. This parameter
 *   must be set to less or equal to @p max_subrate.
 *     - <b>Range:</b> 1 to 500
 *
 *     - <b>Default</b> : 1
 *   @endparblock
 * @param[in] max_subrate @parblock
 *   Maximum subrate factor allowed in requests by a peripheral. This parameter
 *   must be set greater or equal to @p min_subrate.
 *     - <b>Range:</b> 1 to 500
 *
 *     - <b>Default</b> : 1
 *   @endparblock
 * @param[in] max_latency @parblock
 *   Maximum Peripheral latency allowed in requests, in units of subrated
 *   connection intervals. The result of (@p max_latency + 1) x @p max_subrate
 *   must be less than or equal to 500.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] continuation_number @parblock
 *   Minimum number of underlying connection events to remain active after a
 *   packet containing a Controller PDU with a non-zero Length field is sent or
 *   received in requests by a peripheral. This number must be set to be smaller
 *   than @p max_subrate.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] max_timeout @parblock
 *   Maximum supervision timeout allowed in requests by a peripheral.
 *     - <b>Range:</b> 10 (100 ms) to 3200 (32 s)
 *
 *     - Time = Value x 10 ms
 *     - Time Range: 100 ms to 32 s
 *
 *     - <b>Default</b> : 10 (100 ms)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_default_acceptable_subrate(uint16_t min_subrate,
                                                            uint16_t max_subrate,
                                                            uint16_t max_latency,
                                                            uint16_t continuation_number,
                                                            uint16_t max_timeout);

/***************************************************************************//**
 *
 * Connect to an advertising device with the specified initiating PHY on which
 * connectable advertisements on primary advertising channels are received. The
 * Bluetooth stack will enter a state where it continuously scans for the
 * connectable advertising packets from the remote device, which matches the
 * Bluetooth address given as a parameter. Scan parameters set in @ref
 * sl_bt_scanner_set_parameters are used in this operation. Upon receiving the
 * advertising packet, the module will send a connection request packet to the
 * target device to initiate a Bluetooth connection. To cancel an ongoing
 * connection process, use @ref sl_bt_connection_close command with the handle
 * received in response from this command.
 *
 * A connection is opened in no-security mode. If the GATT client needs to read
 * or write the attributes on GATT server requiring encryption or
 * authentication, it must first encrypt the connection using an appropriate
 * authentication method.
 *
 * If a connection can't be established, for example, the remote device has gone
 * out of range, has entered into deep sleep, or is not advertising, the stack
 * will try to connect forever. In this case, the application will not get an
 * event related to the connection request. To recover from this situation, the
 * application can implement a timeout and call @ref sl_bt_connection_close to
 * cancel the connection request.
 *
 * This command fails with the connection limit exceeded error if the number of
 * connections attempted exceeds the configured MAX_CONNECTIONS value.
 *
 * This command fails with the invalid parameter error if the initiating PHY
 * value is invalid or the device does not support PHY.
 *
 * Subsequent calls of this command have to wait for the ongoing command to
 * complete. A received event @ref sl_bt_evt_connection_opened indicates that
 * the connection opened successfully and a received event @ref
 * sl_bt_evt_connection_closed indicates that connection failures have occurred.
 *
 * @param[in] address Address of the device to connect to
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. Address type of
 *   the device to connect to. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 * @param[in] initiating_phy Enum @ref sl_bt_gap_phy_t. The initiating PHY.
 *   Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[out] connection Handle that will be assigned to the connection after
 *   the connection is established. This handle is valid only if the result code
 *   of this response is 0 (zero).
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_opened - This event is triggered after the
 *     connection is opened and indicates whether the devices are already bonded
 *     and the role of the device in this connection.
 *   - @ref sl_bt_evt_connection_parameters - This event indicates the
 *     connection parameters and security mode of the connection.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_open(bd_addr address,
                                  uint8_t address_type,
                                  uint8_t initiating_phy,
                                  uint8_t *connection);

/***************************************************************************//**
 *
 * Connect to any device in the Filter Accept List, which enables the Auto
 * Connection Establishment procedure on the local controller. The behavior of
 * this command is the same as @ref sl_bt_connection_open. The only difference
 * is that @ref sl_bt_connection_open opens a connection to a given peer device
 * address.
 *
 * See @ref sl_bt_connection_open for more details.
 *
 * @param[in] initiating_phy Enum @ref sl_bt_gap_phy_t. The initiating PHY.
 *   Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[out] connection Handle that will be assigned to the connection after
 *   the connection is established. This handle is valid only if the result code
 *   of this response is 0 (zero).
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_opened - This event is triggered after the
 *     connection is opened and indicates whether the devices are already bonded
 *     and the role of the device in this connection.
 *   - @ref sl_bt_evt_connection_parameters - This event indicates the
 *     connection parameters and security mode of the connection.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_open_with_accept_list(uint8_t initiating_phy,
                                                   uint8_t *connection);

/***************************************************************************//**
 *
 * Request a change in the connection parameters of a Bluetooth connection.
 *   - On the central device the HCI LE Connection Update command is used to
 *     update the parameters.
 *   - On the peripheral device an L2CAP connection parameter update request is
 *     sent to the peer device to update the parameters.
 *
 * @p min_ce_length and @p max_ce_length specify the preference of the
 * connection event length so that the Link Layer can prioritize tasks
 * accordingly in simultaneous connections, or scanning and so on. A connection
 * event starts at an anchor point of a connection interval and lasts until the
 * lesser of @p max_ce_length and the actual connection interval. Packets that
 * do not fit into the connection event will be sent in the next connection
 * interval.
 *
 * @param[in] connection Connection Handle
 * @param[in] min_interval @parblock
 *   Minimum value for the connection event interval. This must be set less than
 *   or equal to @p max_interval.
 *     - <b>Range:</b> 0x0006 to 0x0c80
 *
 *     - Time = Value x 1.25 ms
 *     - Time Range: 7.5 ms to 4 s
 *   @endparblock
 * @param[in] max_interval @parblock
 *   Maximum value for the connection event interval. This must be set greater
 *   than or equal to @p min_interval.
 *     - <b>Range:</b> 0x0006 to 0x0c80
 *
 *     - Time = Value x 1.25 ms
 *     - Time Range: 7.5 ms to 4 s
 *   @endparblock
 * @param[in] latency Peripheral latency, which defines how many connection
 *   intervals the peripheral can skip if it has no data to send
 *     - <b>Range:</b> 0x0000 to 0x01f3
 * @param[in] timeout @parblock
 *   Supervision timeout, which defines the time that the connection is
 *   maintained although the devices can't communicate at the currently
 *   configured connection intervals.
 *     - <b>Range:</b> 0x000a to 0x0c80
 *
 *     - Time = Value x 10 ms
 *     - Time Range: 100 ms to 32 s
 *     - The value in milliseconds must be larger than (1 + @p latency) * @p
 *       max_interval * 2, where @p max_interval is given in milliseconds
 *
 *   Set the supervision timeout at a value which allows communication attempts
 *   over at least a few connection intervals.
 *   @endparblock
 * @param[in] min_ce_length @parblock
 *   Minimum length of the connection event. It must be less than or equal to @p
 *   max_ce_length.
 *
 *   This value defines the minimum time that should be given to the connection
 *   event in a situation where other tasks need to run immediately after the
 *   connection event. When the value is very small, the connection event still
 *   has at least one TX/RX operation. If this value is increased, more time is
 *   reserved for the connection event so it can transmit and receive more
 *   packets in a connection interval.
 *
 *   Use value 0x0000 if the application doesn't care about the connection event
 *   length or doesn't want to do fine tuning.
 *
 *     - Time = Value x 0.625 ms
 *
 *     - <b>Range:</b> 0x0000 to 0xffff
 *   @endparblock
 * @param[in] max_ce_length @parblock
 *   Maximum length of the connection event. It must be greater than or equal to
 *   @p min_ce_length.
 *
 *   This value is used for limiting the connection event length so that a
 *   connection that has large amounts of data to transmit or receive doesn't
 *   block other tasks. Limiting the connection event is a hard stop. If there
 *   is no enough time to send or receive a packet, the connection event will be
 *   closed.
 *
 *   If the value is set to 0, the connection event still has at least one TX/RX
 *   operation. This is useful to limit power consumption or leave more time to
 *   other tasks.
 *
 *   Use value 0xffff if the application doesn't care about the connection event
 *   length or doesn't want to do fine tuning.
 *     - Time = Value x 0.625 ms
 *
 *     - <b>Range:</b> 0x0000 to 0xffff
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_parameters - Triggered after new connection
 *     parameters are applied on the connection.
 *   - @ref sl_bt_evt_connection_set_parameters_failed - Triggered when the peer
 *     device rejected an L2CAP connection parameter update request.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_parameters(uint8_t connection,
                                            uint16_t min_interval,
                                            uint16_t max_interval,
                                            uint16_t latency,
                                            uint16_t timeout,
                                            uint16_t min_ce_length,
                                            uint16_t max_ce_length);

/***************************************************************************//**
 *
 * Set preferred and accepted PHYs for a given connection. Event @ref
 * sl_bt_evt_connection_phy_status is received when PHY update procedure is
 * completed. Non-preferred PHY can also be set if remote device does not accept
 * any of the preferred PHYs.
 *
 * The parameter @p accepted_phy is used for specifying the PHYs that the stack
 * can accept in a remote initiated PHY update request. A PHY update will not
 * occur if none of the accepted PHYs presents in the request.
 *
 * <b>NOTE:</b> 2M and Coded PHYs are not supported by all devices.
 *
 * @param[in] connection Connection handle
 * @param[in] preferred_phy @parblock
 *   Preferred PHYs. This parameter is a bitfield and multiple PHYs can be set.
 *     - <b>0x01:</b> 1M PHY
 *     - <b>0x02:</b> 2M PHY
 *     - <b>0x04:</b> 125k Coded PHY (S=8)
 *     - <b>0x08:</b> 500k Coded PHY (S=2)
 *
 *   Default: 0xff (no preference)
 *   @endparblock
 * @param[in] accepted_phy @parblock
 *   Accepted PHYs in remotely-initiated PHY update requests. This parameter is
 *   a bitfield and multiple PHYs can be set.
 *     - <b>0x01:</b> 1M PHY
 *     - <b>0x02:</b> 2M PHY
 *     - <b>0x04:</b> Coded PHY
 *     - <b>0xff:</b> Any PHYs
 *
 *   Default: 0xff (all PHYs accepted)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_phy_status
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_preferred_phy(uint8_t connection,
                                               uint8_t preferred_phy,
                                               uint8_t accepted_phy);

/***************************************************************************//**
 *
 * Temporarily enable or disable peripheral latency. Used only when Bluetooth
 * device is acting as a peripheral. When peripheral latency is disabled, the
 * peripheral latency connection parameter is not set to 0 but the device will
 * wake up on every connection interval to receive and send packets.
 *
 * @param[in] connection Connection Handle
 * @param[in] disable 0 enable, 1 disable peripheral latency. Default: 0
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_disable_slave_latency(uint8_t connection,
                                                   uint8_t disable);

/***************************************************************************//**
 *
 * Get the RSSI value measured on a connection.
 *
 * @param[in] connection Connection handle
 * @param[out] rssi The median of the last seven measured RSSI values. Units:
 *   dBm.
 *     - <b>Range:</b> -127 to +20
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_median_rssi(uint8_t connection, int8_t *rssi);

/***************************************************************************//**
 *
 * Read channel map for a specified connection.
 *
 * @param[in] connection Connection Handle
 * @param[in] max_channel_map_size Size of output buffer passed in @p
 *   channel_map
 * @param[out] channel_map_len On return, set to the length of output data
 *   written to @p channel_map
 * @param[out] channel_map @parblock
 *   This parameter is 5 bytes and contains 37 1-bit fields.
 *
 *   The nth field (in the range 0 to 36) contains the value for the link layer
 *   channel index n.
 *
 *     - <b>0:</b> Channel n is unused.
 *     - <b>1:</b> Channel n is used.
 *
 *   The most significant bits are reserved for future use.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_read_channel_map(uint8_t connection,
                                              size_t max_channel_map_size,
                                              size_t *channel_map_len,
                                              uint8_t *channel_map);

/***************************************************************************//**
 *
 * Enable or disable the transmit power reporting for the local device on a
 * connection. When transmit power reporting is enabled, event @ref
 * sl_bt_evt_connection_tx_power is generated when transmit power on the local
 * device changes.
 *
 * The command is a built-in feature in the stack and is supported regardless of
 * whether the LE Power Control feature is used. By default, power reporting for
 * local device is enabled.
 *
 * @param[in] connection Handle of the connection
 * @param[in] mode Enum @ref sl_bt_connection_power_reporting_mode_t. Transmit
 *   power reporting mode. Values:
 *     - <b>sl_bt_connection_power_reporting_disable (0x0):</b> Disable transmit
 *       power reporting
 *     - <b>sl_bt_connection_power_reporting_enable (0x1):</b> Enable transmit
 *       power reporting
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_tx_power
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_power_reporting(uint8_t connection,
                                                 uint8_t mode);

/***************************************************************************//**
 *
 * Enable or disable reporting the transmit power change on the remote device.
 * The application must include the LE Power Control feature
 * (bluetooth_feature_power_control) in order to use this command.
 *
 * When the remote transmit power reporting is enabled, event @ref
 * sl_bt_evt_connection_remote_tx_power is generated when transmit power on the
 * remote device changes.
 *
 * By default, power reporting for the remote device is disabled.
 *
 * @param[in] connection Handle of the connection
 * @param[in] mode Enum @ref sl_bt_connection_power_reporting_mode_t. Transmit
 *   power reporting mode. Values:
 *     - <b>sl_bt_connection_power_reporting_disable (0x0):</b> Disable transmit
 *       power reporting
 *     - <b>sl_bt_connection_power_reporting_enable (0x1):</b> Enable transmit
 *       power reporting
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_remote_tx_power
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_remote_power_reporting(uint8_t connection,
                                                        uint8_t mode);

/***************************************************************************//**
 *
 * Get the transmit power of the local device on the given connection and PHY.
 * The application must include the LE Power Control feature
 * (bluetooth_feature_power_control) in order to use this command.
 *
 * @param[in] connection Handle of the connection
 * @param[in] phy Enum @ref sl_bt_gap_phy_coding_t. The PHY. Values:
 *     - <b>sl_bt_gap_phy_coding_1m_uncoded (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_coding_2m_uncoded (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coding_125k_coded (0x4):</b> 125k Coded PHY (S=8)
 *     - <b>sl_bt_gap_phy_coding_500k_coded (0x8):</b> 500k Coded PHY (S=2)
 * @param[out] current_level @parblock
 *   The current transmit power level in dBm of the PHY on the connection.
 *   Values:
 *     - <b>Range:</b> -127 to +20
 *
 *     - <b>@ref SL_BT_CONNECTION_TX_POWER_UNAVAILABLE (0x7F):</b> Transmit
 *       power level is not available.
 *   @endparblock
 * @param[out] max_level The maximum transmit power level in dBm of the PHY on
 *   the connection.
 *     - <b>Range:</b> -127 to +20
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_tx_power(uint8_t connection,
                                          uint8_t phy,
                                          int8_t *current_level,
                                          int8_t *max_level);

/***************************************************************************//**
 *
 * Get the transmit power of the remote device on the given connection and PHY.
 * The application must include the LE Power Control feature
 * (bluetooth_feature_power_control) in order to use this command. Transmit
 * power levels are returned in event @ref
 * sl_bt_evt_connection_get_remote_tx_power_completed after the operation
 * completed.
 *
 * @param[in] connection Handle of the connection
 * @param[in] phy Enum @ref sl_bt_gap_phy_coding_t. The PHY. Values:
 *     - <b>sl_bt_gap_phy_coding_1m_uncoded (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_coding_2m_uncoded (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coding_125k_coded (0x4):</b> 125k Coded PHY (S=8)
 *     - <b>sl_bt_gap_phy_coding_500k_coded (0x8):</b> 500k Coded PHY (S=2)
 *
 * @return SL_STATUS_OK if the command is accepted. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_get_remote_tx_power_completed
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_remote_tx_power(uint8_t connection,
                                                 uint8_t phy);

/***************************************************************************//**
 *
 * Set the transmit power of a connection. The application must include
 * component bluetooth_feature_user_power_control in order to use this command
 * for controlling the transmit power of the connection at application level.
 * This command is unavailable if the standard Bluetooth feature LE power
 * control (component bluetooth_feature_power_control) is used by the
 * application.
 *
 * The actual selected power level is returned from this command. The value may
 * be different than the requested one because of Bluetooth feature restrictions
 * or radio characteristics.
 *
 * @param[in] connection The connection handle
 * @param[in] tx_power The requested TX power. Unit: 0.1 dBm
 * @param[out] tx_power_out The selected TX power. Unit: 0.1 dBm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_tx_power(uint8_t connection,
                                          int16_t tx_power,
                                          int16_t *tx_power_out);

/***************************************************************************//**
 *
 * Read link layer features supported by the remote device.
 *
 * @param[in] connection Connection Handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_remote_used_features
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_read_remote_used_features(uint8_t connection);

/***************************************************************************//**
 *
 * Get the security status of the connection.
 *
 * @param[in] connection Connection handle
 * @param[out] security_mode Enum @ref sl_bt_connection_security_t. Connection
 *   security mode. Values:
 *     - <b>sl_bt_connection_mode1_level1 (0x0):</b> No security
 *     - <b>sl_bt_connection_mode1_level2 (0x1):</b> Unauthenticated pairing
 *       with encryption
 *     - <b>sl_bt_connection_mode1_level3 (0x2):</b> Authenticated pairing with
 *       encryption
 *     - <b>sl_bt_connection_mode1_level4 (0x3):</b> Authenticated Secure
 *       Connections pairing with encryption using a 128-bit strength encryption
 *       key
 * @param[out] key_size The size of encryption key
 * @param[out] bonding_handle Bonding handle. Values:
 *     - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b> No bonding
 *     - <b>Other:</b> Bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_security_status(uint8_t connection,
                                                 uint8_t *security_mode,
                                                 uint8_t *key_size,
                                                 uint8_t *bonding_handle);

/***************************************************************************//**
 *
 * Request to update the maximum TX payload length and maximum packet TX time of
 * a Bluetooth connection.
 *
 * Event @ref sl_bt_evt_connection_data_length is generated when the data length
 * has been changed on the connection.
 *
 * @param[in] connection Connection handle
 * @param[in] tx_data_len @parblock
 *   Preferred maximum payload octets of a packet that the local Controller will
 *   send
 *
 *     - <b>Range:</b> 20 (0x1B) to 251 (0XFB)
 *   @endparblock
 * @param[in] tx_time_us @parblock
 *   Preferred maximum TX time in microseconds that the local Controller will
 *   take to send a packet
 *
 *     - <b>Range:</b> 328 (0x0148) to 17040 (0x4290)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_data_length
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_set_data_length(uint8_t connection,
                                             uint16_t tx_data_len,
                                             uint16_t tx_time_us);

/***************************************************************************//**
 *
 * Read the statistic values collected on the specified connection. The
 * application must include component bluetooth_feature_connection_statistics in
 * order to use this command. If the component is not included in the
 * application this command returns the error SL_STATUS_NOT_AVAILABLE. If this
 * command is called before the connection has opened, the command returns the
 * error SL_STATUS_NOT_READY.
 *
 * @param[in] connection The connection handle
 * @param[in] reset Reset statistics if parameter value is not zero
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_statistics - Triggered to deliver the statistic
 *     values of the connection at the time the command was issued.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_read_statistics(uint8_t connection, uint8_t reset);

/***************************************************************************//**
 *
 * Get parameters and next connection event scheduling details of a connection.
 * Returned values from this command provide the necessary information for
 * following the packet transmissions of the connection on an external device.
 * Note that the stack cannot ensure a determined timing latency from the time
 * when issuing this command to the time when another device starts to follow
 * the connection, and the stack cannot guarantee that a scheduled connection
 * event will always happen.
 *
 * @param[in] connection Connection handle
 * @param[out] access_address Access address of the connection
 * @param[out] role Enum @ref sl_bt_connection_role_t. The role the local device
 *   operates in the connection. Values:
 *     - <b>sl_bt_connection_role_peripheral (0x0):</b> The Peripheral role
 *     - <b>sl_bt_connection_role_central (0x1):</b> The Central role
 * @param[out] crc_init The CRC initialization value
 * @param[out] interval The connection interval. Time = Value x 1.25 ms
 * @param[out] supervision_timeout The connection supervision time. Time = Value
 *   x 10 ms
 * @param[out] central_clock_accuracy The central device's clock accuracy index
 *   value
 * @param[out] central_phy Enum @ref sl_bt_gap_phy_t. The PHY that the Central
 *   device is transmitting on. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[out] peripheral_phy Enum @ref sl_bt_gap_phy_t. The PHY that the
 *   Peripheral device is transmitting on. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[out] channel_selection_algorithm Enum @ref
 *   sl_bt_gap_channel_selection_algorithm_t. The channel selection algorithm.
 *   Values:
 *     - <b>sl_bt_gap_channel_selection_algorithm_1 (0x0):</b> Channel selection
 *       algorithm #1
 *     - <b>sl_bt_gap_channel_selection_algorithm_2 (0x1):</b> Channel selection
 *       algorithm #2
 * @param[out] hop The hop increment when the channel selection algorithm #1 is
 *   used on the connection. Ignore if @p channel_selection_algorithm indicates
 *   that the connection uses channel selection algorithm #2.
 * @param[out] channel_map @parblock
 *   5 byte bit field in little endian format. Only the first 37 bits are used.
 *   Bit 0 of the first byte is channel 0, bit 0 of the second byte is channel
 *   8, etc. Ignore bits 37-39 that are reserved for future use.
 *
 *   A channel is unused when its bit is 0. A channel is used when its bit is 1.
 *   @endparblock
 * @param[out] channel The data channel number that transmissions will use in
 *   the next connection event
 * @param[out] event_counter The event counter of the next connection event
 * @param[out] start_time_us Time in microseconds from the PROTIMER tick when
 *   the next connection event is scheduled to happen
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_scheduling_details(uint8_t connection,
                                                    uint32_t *access_address,
                                                    uint8_t *role,
                                                    uint32_t *crc_init,
                                                    uint16_t *interval,
                                                    uint16_t *supervision_timeout,
                                                    uint8_t *central_clock_accuracy,
                                                    uint8_t *central_phy,
                                                    uint8_t *peripheral_phy,
                                                    uint8_t *channel_selection_algorithm,
                                                    uint8_t *hop,
                                                    sl_bt_connection_channel_map_t *channel_map,
                                                    uint8_t *channel,
                                                    uint16_t *event_counter,
                                                    uint32_t *start_time_us);

/***************************************************************************//**
 *
 * Get the Bluetooth address of the remote device on the given connection.
 *
 * @param[in] connection Connection handle
 * @param[out] address Bluetooth address of the remote device
 * @param[out] address_type Enum @ref sl_bt_gap_address_type_t. Remote device
 *   address type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_public_address_resolved_from_rpa (0x4):</b> Public
 *       identity address resolved from a resolvable private address (RPA)
 *     - <b>sl_bt_gap_static_address_resolved_from_rpa (0x5):</b> Static
 *       identity address resolved from a resolvable private address (RPA)
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_remote_address(uint8_t connection,
                                                bd_addr *address,
                                                uint8_t *address_type);

/***************************************************************************//**
 *
 * Request a change to the subrating factor and other parameters applied to an
 * existing connection. The application must include the Connection Subrating
 * feature (bluetooth_feature_connection_subrating) in order to use this
 * command.
 *
 * If the local device is in the central role on this connection, this command
 * overrides the default acceptable subrating parameters set by @ref
 * sl_bt_connection_set_default_acceptable_subrate and uses the specified
 * subrate parameters for this connection. If the local device is in the
 * peripheral role on this connection, this command requests a change to the
 * parameters but they may not be accepted by the peer central device.
 *
 * If the connection parameters are changed (see command @ref
 * sl_bt_connection_set_parameters and event @ref
 * sl_bt_evt_connection_parameters) after subrate parameters were successfully
 * applied, the @p latency and @p timeout from @ref
 * sl_bt_evt_connection_parameters take effect and the @p subrate_factor is
 * reset to 1 and the @p continuation_number to 0 for the connection. Use this
 * command @ref sl_bt_connection_request_subrate again after the connection
 * parameter change to re-enable subrating.
 *
 * This command returns SL_STATUS_BT_CTRL_UNSUPPORTED_REMOTE_FEATURE when the
 * device is in central role on the connection and the peripheral does not
 * support the subrate feature.
 *
 * @param[in] connection Connection handle
 * @param[in] min_subrate @parblock
 *   Minimum subrate factor to be applied to the underlying connection interval.
 *   This parameter must be set to less or equal to @p max_subrate.
 *     - <b>Range:</b> 1 to 500
 *
 *     - <b>Default</b> : 1
 *   @endparblock
 * @param[in] max_subrate @parblock
 *   Maximum subrate factor to be applied to the underlying connection interval.
 *   This parameter must be set greater or equal to @p min_subrate.
 *     - <b>Range:</b> 1 to 500
 *
 *     - <b>Default</b> : 1
 *   @endparblock
 * @param[in] max_latency @parblock
 *   Maximum Peripheral latency for the connection in units of subrated
 *   connection intervals. The result of (@p max_latency + 1) x @p max_subrate
 *   must be less than or equal to 500.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] continuation_number @parblock
 *   Minimum number of underlying connection events to remain active after a
 *   packet containing a Link Layer PDU with a non-zero Length field is sent or
 *   received. This number must be set to be smaller than @p max_subrate.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   Supervision timeout, which defines the time that the connection is
 *   maintained although the devices can't communicate at the currently
 *   configured connection intervals.
 *     - <b>Range:</b> 10 (100 ms) to 3200 (32 s)
 *
 *     - <b>Default</b> : 10 (100 ms)
 *
 *     - Time = Value x 10 ms
 *     - Time Range: 100 ms to 32 s
 *     - The value in milliseconds must be larger than (1 + @p max_latency) * @p
 *       max_interval * 2, where @p max_interval is given in milliseconds
 *
 *   Set the supervision timeout at a value which allows communication attempts
 *   over at least a few connection intervals.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_subrate_changed - Triggered when the request to
 *     change the subrate parameters has been successfully completed.
 *   - @ref sl_bt_evt_connection_request_subrate_failed - Triggered when the
 *     request to change the subrate parameters has failed.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_request_subrate(uint8_t connection,
                                             uint16_t min_subrate,
                                             uint16_t max_subrate,
                                             uint16_t max_latency,
                                             uint16_t continuation_number,
                                             uint16_t timeout);

/***************************************************************************//**
 *
 * Get the state of the given connection.
 *
 * @param[in] connection Connection handle
 * @param[out] state Enum @ref sl_bt_connection_state_t. State of the given
 *   connection. Values:
 *     - <b>sl_bt_connection_state_closed (0x0):</b> Connection closed
 *     - <b>sl_bt_connection_state_closing (0x1):</b> Connection is being closed
 *     - <b>sl_bt_connection_state_open (0x2):</b> Connection open
 *     - <b>sl_bt_connection_state_opening (0x3):</b> Connection is being opened
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_get_state(uint8_t connection, uint8_t *state);

/***************************************************************************//**
 *
 * Close a Bluetooth connection gracefully by performing the ACL Termination
 * procedure or cancel an ongoing connection establishment process. The
 * parameter is a connection handle which is reported in @ref
 * sl_bt_evt_connection_opened event or @ref sl_bt_connection_open command
 * response.
 *
 * Disconnecting a connection is an asynchronous operation. The disconnection is
 * completed when a @ref sl_bt_evt_connection_closed event for the given
 * connection handle is received. To open a new connection to the same remote
 * device, wait for the @ref sl_bt_evt_connection_closed event and then initiate
 * the connection.
 *
 * @param[in] connection Handle of the connection to be closed
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_closed
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_close(uint8_t connection);

/***************************************************************************//**
 *
 * Forcefully close a Bluetooth connection without performing the ACL
 * Termination procedure. The parameter is a connection handle which is reported
 * in @ref sl_bt_evt_connection_opened event or @ref sl_bt_connection_open
 * command response.
 *
 * Closing a connection using this command could result in the observation of
 * connection loss or supervision timeout on the remote device. Only use this
 * command for special cases, for example, when disconnecting a connection with
 * @ref sl_bt_connection_close did not complete in expected time period.
 *
 * @param[in] connection Handle of the connection to be closed
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_closed
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_forcefully_close(uint8_t connection);

/** @} */ // end addtogroup sl_bt_connection

/**
 * @addtogroup sl_bt_gatt GATT Client
 * @{
 *
 * @brief GATT Client
 *
 * The commands and events in this class are used to browse and manage
 * attributes in a remote GATT server.
 */

/* Command and Response IDs */
#define sl_bt_cmd_gatt_set_max_mtu_id                                0x00090020
#define sl_bt_cmd_gatt_discover_primary_services_id                  0x01090020
#define sl_bt_cmd_gatt_discover_primary_services_by_uuid_id          0x02090020
#define sl_bt_cmd_gatt_find_included_services_id                     0x10090020
#define sl_bt_cmd_gatt_discover_characteristics_id                   0x03090020
#define sl_bt_cmd_gatt_discover_characteristics_by_uuid_id           0x04090020
#define sl_bt_cmd_gatt_discover_descriptors_id                       0x06090020
#define sl_bt_cmd_gatt_discover_characteristic_descriptors_id        0x14090020
#define sl_bt_cmd_gatt_set_characteristic_notification_id            0x05090020
#define sl_bt_cmd_gatt_send_characteristic_confirmation_id           0x0d090020
#define sl_bt_cmd_gatt_read_characteristic_value_id                  0x07090020
#define sl_bt_cmd_gatt_read_characteristic_value_from_offset_id      0x12090020
#define sl_bt_cmd_gatt_read_multiple_characteristic_values_id        0x11090020
#define sl_bt_cmd_gatt_read_variable_length_characteristic_values_id 0x16090020
#define sl_bt_cmd_gatt_read_characteristic_value_by_uuid_id          0x08090020
#define sl_bt_cmd_gatt_write_characteristic_value_id                 0x09090020
#define sl_bt_cmd_gatt_write_characteristic_value_without_response_id 0x0a090020
#define sl_bt_cmd_gatt_prepare_characteristic_value_write_id         0x0b090020
#define sl_bt_cmd_gatt_prepare_characteristic_value_reliable_write_id 0x13090020
#define sl_bt_cmd_gatt_execute_characteristic_value_write_id         0x0c090020
#define sl_bt_cmd_gatt_read_descriptor_value_id                      0x0e090020
#define sl_bt_cmd_gatt_write_descriptor_value_id                     0x0f090020
#define sl_bt_cmd_gatt_get_mtu_id                                    0x15090020
#define sl_bt_rsp_gatt_set_max_mtu_id                                0x00090020
#define sl_bt_rsp_gatt_discover_primary_services_id                  0x01090020
#define sl_bt_rsp_gatt_discover_primary_services_by_uuid_id          0x02090020
#define sl_bt_rsp_gatt_find_included_services_id                     0x10090020
#define sl_bt_rsp_gatt_discover_characteristics_id                   0x03090020
#define sl_bt_rsp_gatt_discover_characteristics_by_uuid_id           0x04090020
#define sl_bt_rsp_gatt_discover_descriptors_id                       0x06090020
#define sl_bt_rsp_gatt_discover_characteristic_descriptors_id        0x14090020
#define sl_bt_rsp_gatt_set_characteristic_notification_id            0x05090020
#define sl_bt_rsp_gatt_send_characteristic_confirmation_id           0x0d090020
#define sl_bt_rsp_gatt_read_characteristic_value_id                  0x07090020
#define sl_bt_rsp_gatt_read_characteristic_value_from_offset_id      0x12090020
#define sl_bt_rsp_gatt_read_multiple_characteristic_values_id        0x11090020
#define sl_bt_rsp_gatt_read_variable_length_characteristic_values_id 0x16090020
#define sl_bt_rsp_gatt_read_characteristic_value_by_uuid_id          0x08090020
#define sl_bt_rsp_gatt_write_characteristic_value_id                 0x09090020
#define sl_bt_rsp_gatt_write_characteristic_value_without_response_id 0x0a090020
#define sl_bt_rsp_gatt_prepare_characteristic_value_write_id         0x0b090020
#define sl_bt_rsp_gatt_prepare_characteristic_value_reliable_write_id 0x13090020
#define sl_bt_rsp_gatt_execute_characteristic_value_write_id         0x0c090020
#define sl_bt_rsp_gatt_read_descriptor_value_id                      0x0e090020
#define sl_bt_rsp_gatt_write_descriptor_value_id                     0x0f090020
#define sl_bt_rsp_gatt_get_mtu_id                                    0x15090020

/**
 * @brief These values indicate which attribute request or response has caused
 * the event.
 */
typedef enum
{
  sl_bt_gatt_read_by_type_request      = 0x8,  /**< (0x8) Read by type request */
  sl_bt_gatt_read_by_type_response     = 0x9,  /**< (0x9) Read by type response */
  sl_bt_gatt_read_request              = 0xa,  /**< (0xa) Read request */
  sl_bt_gatt_read_response             = 0xb,  /**< (0xb) Read response */
  sl_bt_gatt_read_blob_request         = 0xc,  /**< (0xc) Read blob request */
  sl_bt_gatt_read_blob_response        = 0xd,  /**< (0xd) Read blob response */
  sl_bt_gatt_read_multiple_request     = 0xe,  /**< (0xe) Read multiple request */
  sl_bt_gatt_read_multiple_response    = 0xf,  /**< (0xf) Read multiple response */
  sl_bt_gatt_write_request             = 0x12, /**< (0x12) Write request */
  sl_bt_gatt_write_response            = 0x13, /**< (0x13) Write response */
  sl_bt_gatt_write_command             = 0x52, /**< (0x52) Write command */
  sl_bt_gatt_prepare_write_request     = 0x16, /**< (0x16) Prepare write request */
  sl_bt_gatt_prepare_write_response    = 0x17, /**< (0x17) Prepare write
                                                    response */
  sl_bt_gatt_execute_write_request     = 0x18, /**< (0x18) Execute write request */
  sl_bt_gatt_execute_write_response    = 0x19, /**< (0x19) Execute write
                                                    response */
  sl_bt_gatt_handle_value_notification = 0x1b, /**< (0x1b) Notification */
  sl_bt_gatt_handle_value_indication   = 0x1d  /**< (0x1d) Indication */
} sl_bt_gatt_att_opcode_t;

/**
 * @brief These values define whether the client is to receive notifications or
 * indications from a remote GATT server.
 */
typedef enum
{
  sl_bt_gatt_disable      = 0x0, /**< (0x0) Disable notifications and
                                      indications */
  sl_bt_gatt_notification = 0x1, /**< (0x1) Notification */
  sl_bt_gatt_indication   = 0x2  /**< (0x2) Indication */
} sl_bt_gatt_client_config_flag_t;

/**
 * @brief These values define whether the GATT server is to cancel all queued
 * writes or commit all queued writes to a remote database.
 */
typedef enum
{
  sl_bt_gatt_cancel = 0x0, /**< (0x0) Cancel all queued writes */
  sl_bt_gatt_commit = 0x1  /**< (0x1) Commit all queued writes */
} sl_bt_gatt_execute_write_flag_t;

/**
 * @addtogroup sl_bt_evt_gatt_mtu_exchanged sl_bt_evt_gatt_mtu_exchanged
 * @{
 * @brief Indicates that an ATT_MTU exchange procedure is completed
 *
 * The @p mtu parameter describes new MTU size. MTU size 23 is used before this
 * event is received.
 */

/** @brief Identifier of the mtu_exchanged event */
#define sl_bt_evt_gatt_mtu_exchanged_id                              0x000900a0

/***************************************************************************//**
 * @brief Data structure of the mtu_exchanged event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_mtu_exchanged_s
{
  uint8_t  connection; /**< Connection handle */
  uint16_t mtu;        /**< Exchanged ATT_MTU */
});

typedef struct sl_bt_evt_gatt_mtu_exchanged_s sl_bt_evt_gatt_mtu_exchanged_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_mtu_exchanged

/**
 * @addtogroup sl_bt_evt_gatt_service sl_bt_evt_gatt_service
 * @{
 * @brief Indicates that a GATT service in the remote GATT database was
 * discovered
 *
 * This event is generated after issuing either the @ref
 * sl_bt_gatt_discover_primary_services or @ref
 * sl_bt_gatt_discover_primary_services_by_uuid command.
 */

/** @brief Identifier of the service event */
#define sl_bt_evt_gatt_service_id                                    0x010900a0

/***************************************************************************//**
 * @brief Data structure of the service event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_service_s
{
  uint8_t    connection; /**< Connection handle */
  uint32_t   service;    /**< GATT service handle */
  uint8array uuid;       /**< Service UUID in little endian format */
});

typedef struct sl_bt_evt_gatt_service_s sl_bt_evt_gatt_service_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_service

/**
 * @addtogroup sl_bt_evt_gatt_characteristic sl_bt_evt_gatt_characteristic
 * @{
 * @brief Indicates that a GATT characteristic in the remote GATT database was
 * discovered
 *
 * This event is generated after issuing either the @ref
 * sl_bt_gatt_discover_characteristics or @ref
 * sl_bt_gatt_discover_characteristics_by_uuid command.
 */

/** @brief Identifier of the characteristic event */
#define sl_bt_evt_gatt_characteristic_id                             0x020900a0

/***************************************************************************//**
 * @brief Data structure of the characteristic event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_characteristic_s
{
  uint8_t    connection;     /**< Connection handle */
  uint16_t   characteristic; /**< GATT characteristic handle */
  uint8_t    properties;     /**< Characteristic properties */
  uint8array uuid;           /**< Characteristic UUID in little endian format */
});

typedef struct sl_bt_evt_gatt_characteristic_s sl_bt_evt_gatt_characteristic_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_characteristic

/**
 * @addtogroup sl_bt_evt_gatt_descriptor sl_bt_evt_gatt_descriptor
 * @{
 * @brief Indicates that a GATT characteristic descriptor in the remote GATT
 * database was discovered
 *
 * It is generated after issuing the @ref sl_bt_gatt_discover_descriptors or
 * @ref sl_bt_gatt_discover_characteristic_descriptors command.
 */

/** @brief Identifier of the descriptor event */
#define sl_bt_evt_gatt_descriptor_id                                 0x030900a0

/***************************************************************************//**
 * @brief Data structure of the descriptor event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_descriptor_s
{
  uint8_t    connection; /**< Connection handle */
  uint16_t   descriptor; /**< GATT characteristic descriptor handle */
  uint8array uuid;       /**< Descriptor UUID in little endian format */
});

typedef struct sl_bt_evt_gatt_descriptor_s sl_bt_evt_gatt_descriptor_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_descriptor

/**
 * @addtogroup sl_bt_evt_gatt_characteristic_value sl_bt_evt_gatt_characteristic_value
 * @{
 * @brief Indicates that the value of one or several characteristics in the
 * remote GATT server was received
 *
 * It is triggered by several commands: @ref
 * sl_bt_gatt_read_characteristic_value, @ref
 * sl_bt_gatt_read_characteristic_value_from_offset, @ref
 * sl_bt_gatt_read_characteristic_value_by_uuid, @ref
 * sl_bt_gatt_read_multiple_characteristic_values and @ref
 * sl_bt_gatt_read_variable_length_characteristic_values. When the remote GATT
 * server sends indications or notifications after enabling notifications with
 * @ref sl_bt_gatt_set_characteristic_notification. The parameter @p att_opcode
 * indicates which type of GATT transaction triggered this event. In particular,
 * if the @p att_opcode type is @ref sl_bt_gatt_handle_value_indication (0x1d),
 * the application needs to confirm the indication with @ref
 * sl_bt_gatt_send_characteristic_confirmation.
 */

/** @brief Identifier of the characteristic_value event */
#define sl_bt_evt_gatt_characteristic_value_id                       0x040900a0

/***************************************************************************//**
 * @brief Data structure of the characteristic_value event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_characteristic_value_s
{
  uint8_t    connection;     /**< Connection handle */
  uint16_t   characteristic; /**< GATT characteristic handle. This value is
                                  normally received from the gatt_characteristic
                                  event. */
  uint8_t    att_opcode;     /**< Enum @ref sl_bt_gatt_att_opcode_t. Attribute
                                  opcode, which indicates the GATT transaction
                                  used. */
  uint16_t   offset;         /**< Value offset */
  uint8array value;          /**< Characteristic value */
});

typedef struct sl_bt_evt_gatt_characteristic_value_s sl_bt_evt_gatt_characteristic_value_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_characteristic_value

/**
 * @addtogroup sl_bt_evt_gatt_descriptor_value sl_bt_evt_gatt_descriptor_value
 * @{
 * @brief Indicates that the value of a descriptor in the remote GATT server was
 * received
 *
 * This event is generated by the @ref sl_bt_gatt_read_descriptor_value command.
 */

/** @brief Identifier of the descriptor_value event */
#define sl_bt_evt_gatt_descriptor_value_id                           0x050900a0

/***************************************************************************//**
 * @brief Data structure of the descriptor_value event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_descriptor_value_s
{
  uint8_t    connection; /**< Connection handle */
  uint16_t   descriptor; /**< GATT characteristic descriptor handle */
  uint16_t   offset;     /**< Value offset */
  uint8array value;      /**< Descriptor value */
});

typedef struct sl_bt_evt_gatt_descriptor_value_s sl_bt_evt_gatt_descriptor_value_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_descriptor_value

/**
 * @addtogroup sl_bt_evt_gatt_procedure_completed sl_bt_evt_gatt_procedure_completed
 * @{
 * @brief Indicates that the current GATT procedure was completed successfully
 * or that it failed with an error
 *
 * All GATT commands excluding @ref
 * sl_bt_gatt_write_characteristic_value_without_response and @ref
 * sl_bt_gatt_send_characteristic_confirmation will trigger this event. As a
 * result, the application must wait for this event before issuing another GATT
 * command (excluding the two aforementioned exceptions).
 *
 * <b>Note:</b> After a failed GATT procedure with SL_STATUS_TIMEOUT error,
 * further GATT transactions over this connection are not allowed by the stack.
 */

/** @brief Identifier of the procedure_completed event */
#define sl_bt_evt_gatt_procedure_completed_id                        0x060900a0

/***************************************************************************//**
 * @brief Data structure of the procedure_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_procedure_completed_s
{
  uint8_t  connection; /**< Connection handle */
  uint16_t result;     /**< SL_STATUS_OK if successful. Error code otherwise. */
});

typedef struct sl_bt_evt_gatt_procedure_completed_s sl_bt_evt_gatt_procedure_completed_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_procedure_completed

/***************************************************************************//**
 *
 * Set the maximum size of ATT Message Transfer Units (MTU) in the host stack.
 * The functionality is the same as @ref sl_bt_gatt_server_set_max_mtu and the
 * setting applies to both GATT client and server.
 *
 * The value will be used as the maximum receive MTU size of the GATT client in
 * ATT_EXCHANGE_MTU_REQ and maximum receive MTU size of the GATT server in
 * ATT_EXCHANGE_MTU_RSP. The ATT_MTU on a connection is 23 when the connection
 * is opened. The GATT client will automatically send an ATT_EXCHANGE_MTU_REQ
 * after a connection is opened if the setting value is larger than 23.
 *
 * If the given value is too large according to the maximum BGAPI payload size,
 * the host stack will select the maximum possible value.
 *
 * @param[in] max_mtu @parblock
 *   Maximum size of MTU in the host stack for GATT client and server
 *     - <b>Range:</b> 23 to 250
 *
 *     - <b>Default</b> : 247
 *   @endparblock
 * @param[out] max_mtu_out The maximum size of MTU selected by the host stack
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_set_max_mtu(uint16_t max_mtu, uint16_t *max_mtu_out);

/***************************************************************************//**
 *
 * Discover all primary services of a remote GATT database. This command
 * generates a unique gatt_service event for every discovered primary service.
 * Received @ref sl_bt_evt_gatt_procedure_completed event indicates that this
 * GATT procedure has successfully completed or failed with an error.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_service - Discovered service from remote GATT
 *     database
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_primary_services(uint8_t connection);

/***************************************************************************//**
 *
 * Discover primary services with the specified UUID in a remote GATT database.
 * This command generates unique gatt_service event for every discovered primary
 * service. Received @ref sl_bt_evt_gatt_procedure_completed event indicates
 * that this GATT procedure was successfully completed or failed with an error.
 *
 * @param[in] connection Connection handle
 * @param[in] uuid_len Length of data in @p uuid
 * @param[in] uuid Service UUID in little endian format
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_service - Discovered service from remote GATT
 *     database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_primary_services_by_uuid(uint8_t connection,
                                                         size_t uuid_len,
                                                         const uint8_t* uuid);

/***************************************************************************//**
 *
 * Find the services that are included by a service in a remote GATT database.
 * This command generates a unique gatt_service event for each included service.
 * The received @ref sl_bt_evt_gatt_procedure_completed event indicates that
 * this GATT procedure was successfully completed or failed with an error.
 *
 * @param[in] connection Connection handle
 * @param[in] service GATT service handle. This value is normally received from
 *   the gatt_service event.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_service - Discovered service from remote GATT
 *     database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_find_included_services(uint8_t connection,
                                              uint32_t service);

/***************************************************************************//**
 *
 * Discover all characteristics of a GATT service from a remote GATT database.
 * This command generates a unique gatt_characteristic event for every
 * discovered characteristic. Received @ref sl_bt_evt_gatt_procedure_completed
 * event indicates that this GATT procedure was successfully completed or failed
 * with an error.
 *
 * @param[in] connection Connection handle
 * @param[in] service GATT service handle. This value is normally received from
 *   the gatt_service event.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic - Discovered characteristic from
 *     remote GATT database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_characteristics(uint8_t connection,
                                                uint32_t service);

/***************************************************************************//**
 *
 * Discover all characteristics of a GATT service in a remote GATT database
 * having the specified UUID. This command generates a unique
 * gatt_characteristic event for every discovered characteristic having the
 * specified UUID. Received @ref sl_bt_evt_gatt_procedure_completed event
 * indicates that this GATT procedure was successfully completed or failed with
 * an error.
 *
 * @param[in] connection Connection handle
 * @param[in] service GATT service handle. This value is normally received from
 *   the gatt_service event.
 * @param[in] uuid_len Length of data in @p uuid
 * @param[in] uuid Characteristic UUID in little endian format
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic - Discovered characteristic from
 *     remote GATT database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_characteristics_by_uuid(uint8_t connection,
                                                        uint32_t service,
                                                        size_t uuid_len,
                                                        const uint8_t* uuid);

/***************************************************************************//**
 *
 * Discover all descriptors in a remote GATT database starting from handle. It
 * generates a unique gatt_descriptor event for every discovered descriptor.
 * Received @ref sl_bt_evt_gatt_procedure_completed event indicates that this
 * GATT procedure has successfully completed or failed with an error.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_descriptor - Discovered descriptor from remote GATT
 *     database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_descriptors(uint8_t connection,
                                            uint16_t characteristic);

/***************************************************************************//**
 *
 * Discover all descriptors of a GATT characteristic in a remote GATT database.
 * It generates a unique gatt_descriptor event for every discovered descriptor.
 * Received @ref sl_bt_evt_gatt_procedure_completed event indicates that this
 * GATT procedure has successfully completed or failed with an error.
 *
 * @param[in] connection Connection handle
 * @param[in] start GATT characteristic handle. This value is normally received
 *   from the gatt_characteristic event.
 * @param[in] end GATT characteristic handle. This value is normally received
 *   from the gatt_characteristic event.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_descriptor - Discovered descriptor from remote GATT
 *     database.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_discover_characteristic_descriptors(uint8_t connection,
                                                           uint16_t start,
                                                           uint16_t end);

/***************************************************************************//**
 *
 * Enable or disable the notifications and indications sent from a remote GATT
 * server. This procedure discovers a characteristic client configuration
 * descriptor and writes the related configuration flags to a remote GATT
 * database. A received @ref sl_bt_evt_gatt_procedure_completed event indicates
 * that this GATT procedure was successfully completed or that it failed with an
 * error.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] flags Enum @ref sl_bt_gatt_client_config_flag_t. Characteristic
 *   client configuration flags. Values:
 *     - <b>sl_bt_gatt_disable (0x0):</b> Disable notifications and indications
 *     - <b>sl_bt_gatt_notification (0x1):</b> Notification
 *     - <b>sl_bt_gatt_indication (0x2):</b> Indication
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *   - @ref sl_bt_evt_gatt_characteristic_value - If an indication or
 *     notification has been enabled for a characteristic, this event is
 *     triggered whenever an indication or notification is sent by the remote
 *     GATT server. The triggering conditions of the GATT server are defined by
 *     an upper level, for example by a profile. <b>As a result, it is possible
 *     that no values are ever received, or that it may take time, depending on
 *     how the server is configured.</b>
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_set_characteristic_notification(uint8_t connection,
                                                       uint16_t characteristic,
                                                       uint8_t flags);

/***************************************************************************//**
 *
 * Send a confirmation to a remote GATT server after receiving a characteristic
 * indication. The @ref sl_bt_evt_gatt_characteristic_value event carries the @p
 * att_opcode containing @ref sl_bt_gatt_handle_value_indication (0x1d), which
 * reveals that an indication has been received and must be confirmed with this
 * command. The confirmation needs to be sent within 30 seconds, otherwise
 * further GATT transactions are not allowed by the remote side.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_send_characteristic_confirmation(uint8_t connection);

/***************************************************************************//**
 *
 * Read the value of a characteristic from a remote GATT database. A single @ref
 * sl_bt_evt_gatt_characteristic_value is generated if the characteristic value
 * fits in one ATT PDU. Otherwise, more than one @ref
 * sl_bt_evt_gatt_characteristic_value event is generated because the firmware
 * will automatically use the Read Long Characteristic Values procedure. A
 * received @ref sl_bt_evt_gatt_procedure_completed event indicates that all
 * data was read successfully or that an error response was received.
 *
 * Note that the GATT client does not verify if the requested attribute is a
 * characteristic value. Therefore, before calling this command, ensure that the
 * attribute handle is for a characteristic value, for example, by performing
 * characteristic discovery.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic_value - Contains the data of a
 *     characteristic sent by the GATT Server.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_characteristic_value(uint8_t connection,
                                                 uint16_t characteristic);

/***************************************************************************//**
 *
 * Read a partial characteristic value with a specified offset and maximum
 * length from a remote GATT database. It is equivalent to @ref
 * sl_bt_gatt_read_characteristic_value if both the offset and maximum length
 * parameters are 0. A single @ref sl_bt_evt_gatt_characteristic_value event is
 * generated if the value to read fits in one ATT PDU. Otherwise, more than one
 * @ref sl_bt_evt_gatt_characteristic_value events are generated because the
 * firmware will automatically use the Read Long Characteristic Values
 * procedure. A received @ref sl_bt_evt_gatt_procedure_completed event indicates
 * that all data was read successfully or that an error response was received.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] offset Offset of the characteristic value
 * @param[in] maxlen Maximum bytes to read. If this parameter is 0, all
 *   characteristic values starting at a given offset will be read.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic_value - Contains data of a
 *     characteristic sent by the GATT Server.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_characteristic_value_from_offset(uint8_t connection,
                                                             uint16_t characteristic,
                                                             uint16_t offset,
                                                             uint16_t maxlen);

/***************************************************************************//**
 *
 * Read values of multiple characteristics from a remote GATT database at once.
 * The GATT server returns values in one ATT PDU as the response. If the total
 * set of values is greater than (ATT_MTU - 1) bytes in length, only the first
 * (ATT_MTU - 1) bytes are included. A single @ref
 * sl_bt_evt_gatt_characteristic_value event is generated, in which the
 * characteristic is set to 0 and data in the value parameter is a concatenation
 * of characteristic values in the order they were requested. The received @ref
 * sl_bt_evt_gatt_procedure_completed event indicates either that this GATT
 * procedure was successfully completed or failed with an error.
 *
 * Use this command only for characteristics values that have a known fixed
 * size, except the last one that could have variable length.
 *
 * When the remote GATT server is from Silicon Labs Bluetooth stack, the server
 * returns ATT Invalid PDU (0x04) if this command only reads one characteristic
 * value. The server returns ATT Application Error 0x80 if this command reads
 * the value of a user-type characteristic.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic_list_len Length of data in @p characteristic_list
 * @param[in] characteristic_list List of uint16 characteristic handles each in
 *   little endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic_value - A concatenation of
 *     characteristic values in the order they were requested
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was either
 *     successfully completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_multiple_characteristic_values(uint8_t connection,
                                                           size_t characteristic_list_len,
                                                           const uint8_t* characteristic_list);

/***************************************************************************//**
 *
 * Read multiple variable-length characteristic values from a remote GATT
 * database at once. The remote GATT server returns values in the order they
 * were requested in one ATT PDU as the response. When the GATT client receives
 * the response, it generates an @ref sl_bt_evt_gatt_characteristic_value event
 * for each length-value tuple in the response that contains a characteristic
 * value in full length. If data in the last tuple is truncated, it will be
 * discarded by the host stack. A @ref sl_bt_evt_gatt_procedure_completed event
 * indicates either that this GATT procedure was successfully completed or
 * failed with an error.
 *
 * When the remote GATT server is from Silicon Labs Bluetooth stack, the server
 * returns ATT Invalid PDU (0x04) if this command only reads one characteristic
 * value.
 *
 * This command has following limitations in current SDK,
 *   - It cannot read all the requested characteristic values if the total
 *     length of length-value tuples is greater than the response PDU can take.
 *     The GATT client will discard a value in the response if the value is
 *     truncated.
 *   - The characteristic handle is not correctly reported in an @ref
 *     sl_bt_evt_gatt_characteristic_value event generated from this procedure.
 *   - When the remote GATT server is from Silicon Labs Bluetooth stack, the
 *     server returns ATT Application Error 0x80 if this command tries to read
 *     the value of a user-type characteristic.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic_list_len Length of data in @p characteristic_list
 * @param[in] characteristic_list List of uint16 characteristic handles each in
 *   little endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic_value - Generated for each
 *     length-value tuple in the response that contains a characteristic value
 *     in full length. The characteristic handle is set to 0 in this event.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was either
 *     successfully completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_variable_length_characteristic_values(uint8_t connection,
                                                                  size_t characteristic_list_len,
                                                                  const uint8_t* characteristic_list);

/***************************************************************************//**
 *
 * Read characteristic values of a service from a remote GATT database by giving
 * the UUID of the characteristic and the handle of the service containing this
 * characteristic. If multiple characteristic values are received in one ATT
 * PDU, one @ref sl_bt_evt_gatt_characteristic_value event is generated for each
 * value. If the first characteristic value does not fit in one ATT PDU, the
 * firmware automatically uses the Read Long Characteristic Values procedure and
 * generate more @ref sl_bt_evt_gatt_characteristic_value events until the value
 * has been completely read. A received @ref sl_bt_evt_gatt_procedure_completed
 * event indicates that all data was read successfully or that an error response
 * was received.
 *
 * @param[in] connection Connection handle
 * @param[in] service GATT service handle. This value is normally received from
 *   the gatt_service event.
 * @param[in] uuid_len Length of data in @p uuid
 * @param[in] uuid Characteristic UUID in little endian format
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_characteristic_value - Contains the data of a
 *     characteristic sent by the GATT Server.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_characteristic_value_by_uuid(uint8_t connection,
                                                         uint32_t service,
                                                         size_t uuid_len,
                                                         const uint8_t* uuid);

/***************************************************************************//**
 *
 * Write the value of a characteristic in a remote GATT database. If the value
 * length is greater than (ATT_MTU - 3) and does not fit in one ATT PDU, "write
 * long" GATT procedure is used automatically. Received @ref
 * sl_bt_evt_gatt_procedure_completed event indicates that all data was written
 * successfully or that an error response was received.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] value_len Length of data in @p value
 * @param[in] value Characteristic value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_write_characteristic_value(uint8_t connection,
                                                  uint16_t characteristic,
                                                  size_t value_len,
                                                  const uint8_t* value);

/***************************************************************************//**
 *
 * Write the value of a characteristic in a remote GATT server. It does not
 * generate an event. All failures on the server are ignored silently. For
 * example, if an error is generated in the remote GATT server and the given
 * value is not written into the database, no error message will be reported to
 * the local GATT client. Note that this command can't be used to write long
 * values. At most ATT_MTU - 3 number of bytes can be sent once.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] value_len Length of data in @p value
 * @param[in] value Characteristic value
 * @param[out] sent_len The length of data sent to the remote GATT server
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_write_characteristic_value_without_response(uint8_t connection,
                                                                   uint16_t characteristic,
                                                                   size_t value_len,
                                                                   const uint8_t* value,
                                                                   uint16_t *sent_len);

/***************************************************************************//**
 *
 * Add a characteristic value to the write queue of a remote GATT server. It can
 * be used when long attributes need to be written or a set of values needs to
 * be written atomically. At most ATT_MTU - 5 number of bytes can be sent at one
 * time. Writes are executed or canceled with the @ref
 * sl_bt_gatt_execute_characteristic_value_write command. Whether the writes
 * succeed or not is indicated in the response of the @ref
 * sl_bt_gatt_execute_characteristic_value_write command.
 *
 * In all use cases where the amount of data to transfer fits into the BGAPI
 * payload, use the command @ref sl_bt_gatt_write_characteristic_value to write
 * long values because it transparently performs the prepare_write and
 * execute_write commands.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] offset Offset of the characteristic value
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to write into the specified characteristic of the
 *   remote GATT database
 * @param[out] sent_len The length of data sent to the remote GATT server
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_prepare_characteristic_value_write(uint8_t connection,
                                                          uint16_t characteristic,
                                                          uint16_t offset,
                                                          size_t value_len,
                                                          const uint8_t* value,
                                                          uint16_t *sent_len);

/***************************************************************************//**
 *
 * Add a characteristic value to the write queue of a remote GATT server and
 * verify whether the value was correctly received by the server. Received @ref
 * sl_bt_evt_gatt_procedure_completed event indicates that this GATT procedure
 * was successfully completed or failed with an error. Specifically, error code
 * 0x0194 (data_corrupted) will be returned if the value received from the GATT
 * server's response fails to pass the reliable write verification. At most,
 * ATT_MTU - 5 number of bytes can be sent at one time. Writes are executed or
 * canceled with the @ref sl_bt_gatt_execute_characteristic_value_write command.
 * Whether the writes succeed or not is indicated in the response of the @ref
 * sl_bt_gatt_execute_characteristic_value_write command.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] offset Offset of the characteristic value
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to write into the specified characteristic of the
 *   remote GATT database
 * @param[out] sent_len The length of data sent to the remote GATT server
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_prepare_characteristic_value_reliable_write(uint8_t connection,
                                                                   uint16_t characteristic,
                                                                   uint16_t offset,
                                                                   size_t value_len,
                                                                   const uint8_t* value,
                                                                   uint16_t *sent_len);

/***************************************************************************//**
 *
 * Commit or cancel previously queued writes to a long characteristic of a
 * remote GATT server. Writes are sent to the queue with @ref
 * sl_bt_gatt_prepare_characteristic_value_write command. Content, offset, and
 * length of queued values are validated by this procedure. A received @ref
 * sl_bt_evt_gatt_procedure_completed event indicates that all data was written
 * successfully or that an error response was received.
 *
 * @param[in] connection Connection handle
 * @param[in] flags Enum @ref sl_bt_gatt_execute_write_flag_t. Execute write
 *   flag. Values:
 *     - <b>sl_bt_gatt_cancel (0x0):</b> Cancel all queued writes
 *     - <b>sl_bt_gatt_commit (0x1):</b> Commit all queued writes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_execute_characteristic_value_write(uint8_t connection,
                                                          uint8_t flags);

/***************************************************************************//**
 *
 * Read the descriptor value of a characteristic in a remote GATT database. A
 * single @ref sl_bt_evt_gatt_descriptor_value event is generated if the
 * descriptor value fits in one ATT PDU. Otherwise, more than one @ref
 * sl_bt_evt_gatt_descriptor_value events are generated because the firmware
 * automatically uses the Read Long Characteristic Values procedure. A received
 * @ref sl_bt_evt_gatt_procedure_completed event indicates that all data was
 * read successfully or that an error response was received.
 *
 * @param[in] connection Connection handle
 * @param[in] descriptor GATT characteristic descriptor handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_descriptor_value - Descriptor value received from the
 *     remote GATT server.
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_read_descriptor_value(uint8_t connection,
                                             uint16_t descriptor);

/***************************************************************************//**
 *
 * Write the value of a characteristic descriptor in a remote GATT database. If
 * the value length is greater than ATT_MTU - 3 and does not fit in one ATT PDU,
 * "write long" GATT procedure is used automatically. Received @ref
 * sl_bt_evt_gatt_procedure_completed event indicates either that all data was
 * written successfully or that an error response was received.
 *
 * @param[in] connection Connection handle
 * @param[in] descriptor GATT characteristic descriptor handle
 * @param[in] value_len Length of data in @p value
 * @param[in] value Descriptor value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_procedure_completed - Procedure was successfully
 *     completed or failed with an error.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_write_descriptor_value(uint8_t connection,
                                              uint16_t descriptor,
                                              size_t value_len,
                                              const uint8_t* value);

/***************************************************************************//**
 *
 * Get the size of ATT Message Transfer Units (MTU) for a connection.
 *
 * @param[in] connection Connection handle
 * @param[out] mtu The maximum ATT_MTU used by the connection
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_get_mtu(uint8_t connection, uint16_t *mtu);

/** @} */ // end addtogroup sl_bt_gatt

/**
 * @addtogroup sl_bt_gattdb GATT Database
 * @{
 *
 * @brief GATT Database
 *
 * These commands and events are used for managing the local GATT database.
 *
 * Many commands in this class return the handles of created attributes. These
 * handles may change during commit if attributes are not created in the order
 * they present in the database. For example, when creating a new service using
 * the @ref sl_bt_gattdb_add_service command, the service declaration handle
 * returned from this command becomes invalid later when an attribute is added
 * or removed in front of this service. If the user requires that the attribute
 * handles returned from these commands must remain valid after the database
 * update has been committed, attributes must be created in the order they
 * present in the database.
 */

/* Command and Response IDs */
#define sl_bt_cmd_gattdb_new_session_id                              0x00460020
#define sl_bt_cmd_gattdb_add_service_id                              0x01460020
#define sl_bt_cmd_gattdb_remove_service_id                           0x02460020
#define sl_bt_cmd_gattdb_add_included_service_id                     0x03460020
#define sl_bt_cmd_gattdb_remove_included_service_id                  0x04460020
#define sl_bt_cmd_gattdb_add_uuid16_characteristic_id                0x05460020
#define sl_bt_cmd_gattdb_add_uuid128_characteristic_id               0x06460020
#define sl_bt_cmd_gattdb_remove_characteristic_id                    0x07460020
#define sl_bt_cmd_gattdb_add_uuid16_descriptor_id                    0x08460020
#define sl_bt_cmd_gattdb_add_uuid128_descriptor_id                   0x09460020
#define sl_bt_cmd_gattdb_remove_descriptor_id                        0x0a460020
#define sl_bt_cmd_gattdb_start_service_id                            0x0b460020
#define sl_bt_cmd_gattdb_stop_service_id                             0x0c460020
#define sl_bt_cmd_gattdb_start_characteristic_id                     0x0d460020
#define sl_bt_cmd_gattdb_stop_characteristic_id                      0x0e460020
#define sl_bt_cmd_gattdb_commit_id                                   0x0f460020
#define sl_bt_cmd_gattdb_abort_id                                    0x10460020
#define sl_bt_cmd_gattdb_get_attribute_state_id                      0x11460020
#define sl_bt_rsp_gattdb_new_session_id                              0x00460020
#define sl_bt_rsp_gattdb_add_service_id                              0x01460020
#define sl_bt_rsp_gattdb_remove_service_id                           0x02460020
#define sl_bt_rsp_gattdb_add_included_service_id                     0x03460020
#define sl_bt_rsp_gattdb_remove_included_service_id                  0x04460020
#define sl_bt_rsp_gattdb_add_uuid16_characteristic_id                0x05460020
#define sl_bt_rsp_gattdb_add_uuid128_characteristic_id               0x06460020
#define sl_bt_rsp_gattdb_remove_characteristic_id                    0x07460020
#define sl_bt_rsp_gattdb_add_uuid16_descriptor_id                    0x08460020
#define sl_bt_rsp_gattdb_add_uuid128_descriptor_id                   0x09460020
#define sl_bt_rsp_gattdb_remove_descriptor_id                        0x0a460020
#define sl_bt_rsp_gattdb_start_service_id                            0x0b460020
#define sl_bt_rsp_gattdb_stop_service_id                             0x0c460020
#define sl_bt_rsp_gattdb_start_characteristic_id                     0x0d460020
#define sl_bt_rsp_gattdb_stop_characteristic_id                      0x0e460020
#define sl_bt_rsp_gattdb_commit_id                                   0x0f460020
#define sl_bt_rsp_gattdb_abort_id                                    0x10460020
#define sl_bt_rsp_gattdb_get_attribute_state_id                      0x11460020

/**
 * @brief This enum defines GATT service types.
 */
typedef enum
{
  sl_bt_gattdb_primary_service   = 0x0, /**< (0x0) Primary service */
  sl_bt_gattdb_secondary_service = 0x1  /**< (0x1) Secondary service */
} sl_bt_gattdb_service_type_t;

/**
 * @brief This enum defines the high-level category of an attribute in the local
 * GATT database.
 */
typedef enum
{
  sl_bt_gattdb_category_service                    = 0x1, /**< (0x1) The
                                                               attribute is a
                                                               service
                                                               declaration */
  sl_bt_gattdb_category_include                    = 0x2, /**< (0x2) The
                                                               attribute is an
                                                               include
                                                               declaration */
  sl_bt_gattdb_category_characteristic_declaration = 0x3, /**< (0x3) The
                                                               attribute is a
                                                               characteristic
                                                               declaration */
  sl_bt_gattdb_category_characteristic_value       = 0x4, /**< (0x4) The
                                                               attribute is a
                                                               characteristic
                                                               value */
  sl_bt_gattdb_category_descriptor                 = 0x5  /**< (0x5) The
                                                               attribute is a
                                                               descriptor */
} sl_bt_gattdb_attribute_category_t;

/**
 * @brief This enum defines characteristic and descriptor value types.
 */
typedef enum
{
  sl_bt_gattdb_fixed_length_value    = 0x1, /**< (0x1) A fixed-length value
                                                 managed by the local GATT
                                                 server for responding the read
                                                 and write requests of remote
                                                 GATT clients */
  sl_bt_gattdb_variable_length_value = 0x2, /**< (0x2) A variable-length value
                                                 managed by the local GATT
                                                 server for responding the read
                                                 and write requests of remote
                                                 GATT clients */
  sl_bt_gattdb_user_managed_value    = 0x3  /**< (0x3) A value managed by the
                                                 user application for responding
                                                 the read and write requests of
                                                 remote GATT clients. */
} sl_bt_gattdb_value_type_t;

/**
 * @addtogroup sl_bt_gattdb_service_property_flags GATT Service Property Flags
 * @{
 *
 * This enum defines GATT service property flags.
 */

/** The service should be advertised. */
#define SL_BT_GATTDB_ADVERTISED_SERVICE 0x1       

/** @} */ // end GATT Service Property Flags

/**
 * @addtogroup sl_bt_gattdb_security_requirements GATT Attribute Security Requirement Flags
 * @{
 *
 * This enum defines the security requirement flags for GATT characteristic
 * value properties.
 */

/** The read property requires pairing and encrypted connection. */
#define SL_BT_GATTDB_ENCRYPTED_READ       0x1       

/** The read property requires bonding and encrypted connection. */
#define SL_BT_GATTDB_BONDED_READ          0x2       

/** The read property requires authenticated pairing and encrypted connection.
 * */
#define SL_BT_GATTDB_AUTHENTICATED_READ   0x4       

/** The write property requires pairing and encrypted connection. */
#define SL_BT_GATTDB_ENCRYPTED_WRITE      0x8       

/** The write property requires bonding and encrypted connection. */
#define SL_BT_GATTDB_BONDED_WRITE         0x10      

/** The write property requires authenticated pairing and encrypted connection.
 * */
#define SL_BT_GATTDB_AUTHENTICATED_WRITE  0x20      

/** The notification and indication properties require pairing and encrypted
 * connection. */
#define SL_BT_GATTDB_ENCRYPTED_NOTIFY     0x40      

/** The notification and indication properties require bonding and encrypted
 * connection. */
#define SL_BT_GATTDB_BONDED_NOTIFY        0x80      

/** The notification and indication properties require authenticated pairing and
 * encrypted connection. */
#define SL_BT_GATTDB_AUTHENTICATED_NOTIFY 0x100     

/** @} */ // end GATT Attribute Security Requirement Flags

/**
 * @addtogroup sl_bt_gattdb_flags GATT Database Flags
 * @{
 *
 * This enum defines the options of GATT attribute management.
 */

/** Do not automatically create a Client Characteristic Configuration descriptor
 * when adding a characteristic that has the notify or indicate property. */
#define SL_BT_GATTDB_NO_AUTO_CCCD 0x1       

/** @} */ // end GATT Database Flags

/**
 * @addtogroup sl_bt_gattdb_characteristic_properties GATT Characteristic Property Flags
 * @{
 *
 * This enum defines the property flags for GATT characteristic values. Lower
 * byte is Characteristic Properties and higher byte is Characteristic Extended
 * Properties.
 */

/** A GATT client can read the characteristic value. */
#define SL_BT_GATTDB_CHARACTERISTIC_READ              0x2       

/** A GATT client can write the characteristic value without a response. */
#define SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE 0x4       

/** A GATT client can write the characteristic value. */
#define SL_BT_GATTDB_CHARACTERISTIC_WRITE             0x8       

/** The characteristic value can be notified without acknowledgment. */
#define SL_BT_GATTDB_CHARACTERISTIC_NOTIFY            0x10      

/** The characteristic value can be notified with acknowledgment. */
#define SL_BT_GATTDB_CHARACTERISTIC_INDICATE          0x20      

/** The additional characteristic properties are defined. */
#define SL_BT_GATTDB_CHARACTERISTIC_EXTENDED_PROPS    0x80      

/** The characteristic value supports reliable write. */
#define SL_BT_GATTDB_CHARACTERISTIC_RELIABLE_WRITE    0x101     

/** @} */ // end GATT Characteristic Property Flags

/**
 * @addtogroup sl_bt_gattdb_descriptor_properties GATT Descriptor Property Flags
 * @{
 *
 * This enum defines the property flags for GATT characteristic descriptors.
 */

/** A GATT client can read the descriptor value. */
#define SL_BT_GATTDB_DESCRIPTOR_READ       0x1       

/** A GATT client can write the descriptor value. */
#define SL_BT_GATTDB_DESCRIPTOR_WRITE      0x2       

/** The descriptor is local only and should be invisible to GATT clients. */
#define SL_BT_GATTDB_DESCRIPTOR_LOCAL_ONLY 0x200     

/** @} */ // end GATT Descriptor Property Flags

/**
 * @addtogroup sl_bt_gattdb_attribute_state GATT Attribute State Flags
 * @{
 *
 * Defines the attribute state flags for GATT attributes.
 */

/** The attribute is visible to remote GATT clients. */
#define SL_BT_GATTDB_ATTRIBUTE_STATE_FLAG_ACTIVE  0x1       

/** The attribute has been marked to be activated when the changes are
 * committed. */
#define SL_BT_GATTDB_ATTRIBUTE_STATE_FLAG_STARTED 0x2       

/** The attribute has been marked to be inactivated when the changes are
 * committed. */
#define SL_BT_GATTDB_ATTRIBUTE_STATE_FLAG_STOPPED 0x4       

/** The attribute has been marked to be added when the changes are committed. */
#define SL_BT_GATTDB_ATTRIBUTE_STATE_FLAG_ADDED   0x8       

/** The attribute has been marked to be deleted when the changes are committed.
 * */
#define SL_BT_GATTDB_ATTRIBUTE_STATE_FLAG_DELETED 0x10      

/** @} */ // end GATT Attribute State Flags

/***************************************************************************//**
 *
 * Start a new GATT database update session. If the operation is successful, the
 * Bluetooth stack returns a session ID, with which the GATT database can be
 * updated by calling other database management APIs of this class. Changes in
 * the database are not immediately saved. Unsaved changes are invisible to a
 * connected remote GATT client.
 *
 * After all changes were performed successfully, commit the changes using the
 * @ref sl_bt_gattdb_commit command. The Bluetooth stack will save the changes
 * and handle GATT caching as needed. Unsaved database changes can also be
 * canceled by calling the @ref sl_bt_gattdb_abort command. In either case,
 * after a commit or abort command is called, the current session is closed and
 * the session ID becomes invalid.
 *
 * Only one session is allowed at a time. Error SL_STATUS_ALREADY_EXISTS is
 * returned if another session has been started already.
 *
 * @param[out] session The database update session ID
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_new_session(uint16_t *session);

/***************************************************************************//**
 *
 * Add a service into the local GATT database. When successful, the service is
 * appended to the service list and is in stopped state. Use @ref
 * sl_bt_gattdb_start_service command to set it visible to remote GATT clients.
 *
 * The user application is not allowed to add the Generic Attribute Profile
 * service. If the application needs GATT caching, enable the feature in the
 * configuration of this component and the GATT server will handle GATT caching
 * according to the procedures specified by the Bluetooth core specification.
 *
 * @param[in] session The database update session ID
 * @param[in] type Enum @ref sl_bt_gattdb_service_type_t. The service type.
 *   Values:
 *     - <b>sl_bt_gattdb_primary_service (0x0):</b> Primary service
 *     - <b>sl_bt_gattdb_secondary_service (0x1):</b> Secondary service
 * @param[in] property Service properties. Value: 0 or bit flag @ref
 *   SL_BT_GATTDB_ADVERTISED_SERVICE
 * @param[in] uuid_len Length of data in @p uuid
 * @param[in] uuid The service UUID in little endian format
 * @param[out] service The service declaration attribute handle. This handle is
 *   ensured valid in current session. It may change after the session if
 *   attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_service(uint16_t session,
                                     uint8_t type,
                                     uint8_t property,
                                     size_t uuid_len,
                                     const uint8_t* uuid,
                                     uint16_t *service);

/***************************************************************************//**
 *
 * Remove a service and its characteristics from the local GATT database.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_remove_service(uint16_t session, uint16_t service);

/***************************************************************************//**
 *
 * Add an included-service attribute to a service.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *   which the included-service attribute is added to
 * @param[in] included_service The service declaration attribute handle of the
 *   service to be included
 * @param[out] attribute The included-service attribute handle. This handle is
 *   ensured valid in current session. It may change after the session if
 *   attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_included_service(uint16_t session,
                                              uint16_t service,
                                              uint16_t included_service,
                                              uint16_t *attribute);

/***************************************************************************//**
 *
 * Remove an included-service attribute from a service.
 *
 * @param[in] session The database update session ID
 * @param[in] attribute The included-service attribute handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_remove_included_service(uint16_t session,
                                                 uint16_t attribute);

/***************************************************************************//**
 *
 * Add a 16-bits UUID characteristic to a service. On success, the
 * characteristic is appended to the characteristic list of the service and it
 * inherits the started or stopped state of the service. In addition, it can be
 * started and stopped separately with the @ref
 * sl_bt_gattdb_start_characteristic and @ref sl_bt_gattdb_stop_characteristic
 * commands.
 *
 * If the @p flag parameter does not set @ref SL_BT_GATTDB_NO_AUTO_CCCD, the
 * stack will automatically add a Client Characteristic Configuration descriptor
 * to this characteristic when it has the notify or indicate property. If @ref
 * SL_BT_GATTDB_NO_AUTO_CCCD is set, the user application should add the
 * descriptor separately as needed.
 *
 * A Characteristic Extended Properties descriptor is automatically added if the
 * reliable write property is set.
 *
 * Use the @ref sl_bt_gattdb_add_uuid128_characteristic command to add a
 * 128-bits UUID characteristic.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *   which the characteristic is added to
 * @param[in] property Characteristic value properties. Value: bitmask of @ref
 *   sl_bt_gattdb_characteristic_properties
 * @param[in] security Security requirement. Value: 0 or bitmask of @ref
 *   sl_bt_gattdb_security_requirements. A security requirement flag for a
 *   property is ignored if the property is not set for the characteristic
 *   value.
 * @param[in] flag Option flags. Value: 0 or bitmask of @ref sl_bt_gattdb_flags.
 * @param[in] uuid The 16-bits UUID in little endian format
 * @param[in] value_type Enum @ref sl_bt_gattdb_value_type_t. The value type.
 *   Values:
 *     - <b>sl_bt_gattdb_fixed_length_value (0x1):</b> A fixed-length value
 *       managed by the local GATT server for responding the read and write
 *       requests of remote GATT clients
 *     - <b>sl_bt_gattdb_variable_length_value (0x2):</b> A variable-length
 *       value managed by the local GATT server for responding the read and
 *       write requests of remote GATT clients
 *     - <b>sl_bt_gattdb_user_managed_value (0x3):</b> A value managed by the
 *       user application for responding the read and write requests of remote
 *       GATT clients.
 * @param[in] maxlen The maximum length of the characteristic value. Ignored if
 *   @p value_type is @ref sl_bt_gattdb_user_managed_value.
 * @param[in] value_len Length of data in @p value
 * @param[in] value The initial characteristic value. Length of this value must
 *   be less than or equal to @p maxlen. Ignored if @p value_type is @ref
 *   sl_bt_gattdb_user_managed_value.
 * @param[out] characteristic The characteristic value attribute handle. This
 *   handle is ensured valid in current session. It may change after the session
 *   if attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_uuid16_characteristic(uint16_t session,
                                                   uint16_t service,
                                                   uint16_t property,
                                                   uint16_t security,
                                                   uint8_t flag,
                                                   sl_bt_uuid_16_t uuid,
                                                   uint8_t value_type,
                                                   uint16_t maxlen,
                                                   size_t value_len,
                                                   const uint8_t* value,
                                                   uint16_t *characteristic);

/***************************************************************************//**
 *
 * Add a 128-bits UUID characteristic to a service. When successful, the
 * characteristic is appended to the characteristic list of the service and
 * inherits the started or stopped state of the service. Additionally, it can be
 * started and stopped separately with the @ref
 * sl_bt_gattdb_start_characteristic and @ref sl_bt_gattdb_stop_characteristic
 * commands.
 *
 * If the @p flag parameter does not set @ref SL_BT_GATTDB_NO_AUTO_CCCD, the
 * stack will automatically add a Client Characteristic Configuration descriptor
 * to this characteristic when it has the notify or indicate property. If @ref
 * SL_BT_GATTDB_NO_AUTO_CCCD is set, the user application should add the
 * descriptor separately as needed.
 *
 * A Characteristic Extended Properties descriptor is automatically added if the
 * reliable write property is set.
 *
 * Use the @ref sl_bt_gattdb_add_uuid16_characteristic command to add a 16-bits
 * UUID characteristic.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *   which the characteristic is added to
 * @param[in] property Characteristic value properties. Value: bitmask of @ref
 *   sl_bt_gattdb_characteristic_properties
 * @param[in] security Security requirement. Value: 0 or bitmask of @ref
 *   sl_bt_gattdb_security_requirements. A security requirement flag for a
 *   property is ignored if the property is not set for the characteristic
 *   value.
 * @param[in] flag Option flags. Value: 0 or bitmask of @ref sl_bt_gattdb_flags.
 * @param[in] uuid The 128-bits UUID in little endian format
 * @param[in] value_type Enum @ref sl_bt_gattdb_value_type_t. The value type.
 *   Values:
 *     - <b>sl_bt_gattdb_fixed_length_value (0x1):</b> A fixed-length value
 *       managed by the local GATT server for responding the read and write
 *       requests of remote GATT clients
 *     - <b>sl_bt_gattdb_variable_length_value (0x2):</b> A variable-length
 *       value managed by the local GATT server for responding the read and
 *       write requests of remote GATT clients
 *     - <b>sl_bt_gattdb_user_managed_value (0x3):</b> A value managed by the
 *       user application for responding the read and write requests of remote
 *       GATT clients.
 * @param[in] maxlen The maximum length of the characteristic value. Ignored if
 *   @p value_type is @ref sl_bt_gattdb_user_managed_value.
 * @param[in] value_len Length of data in @p value
 * @param[in] value The initial characteristic value. Length of this value must
 *   be less than or equal to @p maxlen. Ignored if @p value_type is @ref
 *   sl_bt_gattdb_user_managed_value.
 * @param[out] characteristic The characteristic value attribute handle. This
 *   handle is ensured valid in current session. It may change after the session
 *   if attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_uuid128_characteristic(uint16_t session,
                                                    uint16_t service,
                                                    uint16_t property,
                                                    uint16_t security,
                                                    uint8_t flag,
                                                    uuid_128 uuid,
                                                    uint8_t value_type,
                                                    uint16_t maxlen,
                                                    size_t value_len,
                                                    const uint8_t* value,
                                                    uint16_t *characteristic);

/***************************************************************************//**
 *
 * Remove a characteristic and its descriptors from a service.
 *
 * @param[in] session The database update session ID
 * @param[in] characteristic The characteristic value attribute handle of the
 *   characteristic
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_remove_characteristic(uint16_t session,
                                               uint16_t characteristic);

/***************************************************************************//**
 *
 * Add a 16-bits UUID descriptor to a characteristic. When successful, the
 * descriptor is appended to the descriptor list of the characteristic and
 * inherits the started or stopped state of the characteristic.
 *
 * This command does not support adding Characteristic Extended Properties
 * descriptors. This descriptor is automatically added if the characteristic
 * value has the reliable-write property or when a Characteristic User
 * Description descriptor is added and the user description has the write
 * property.
 *
 * Use the @ref sl_bt_gattdb_add_uuid128_descriptor command to add a 128-bits
 * UUID descriptor.
 *
 * @param[in] session The database update session ID
 * @param[in] characteristic The characteristic value attribute handle of the
 *   characteristic the descriptor is added to
 * @param[in] property The descriptor properties. Value: bitmask of @ref
 *   sl_bt_gattdb_descriptor_properties
 * @param[in] security Security requirement. Value: 0 or bitmask of @ref
 *   sl_bt_gattdb_security_requirements. A security requirement flag for a
 *   property is ignored if the property is not set for the descriptor.
 * @param[in] uuid The 16-bits UUID in little endian format
 * @param[in] value_type Enum @ref sl_bt_gattdb_value_type_t. The value type.
 *   Ignored if this is a Client Characteristic Configuration descriptor.
 *   Values:
 *     - <b>sl_bt_gattdb_fixed_length_value (0x1):</b> A fixed-length value
 *       managed by the local GATT server for responding the read and write
 *       requests of remote GATT clients
 *     - <b>sl_bt_gattdb_variable_length_value (0x2):</b> A variable-length
 *       value managed by the local GATT server for responding the read and
 *       write requests of remote GATT clients
 *     - <b>sl_bt_gattdb_user_managed_value (0x3):</b> A value managed by the
 *       user application for responding the read and write requests of remote
 *       GATT clients.
 * @param[in] maxlen The maximum length of the descriptor value. Ignored if @p
 *   value_type is sl_bt_gattdb_user_managed_value, or if this is a Client
 *   Characteristic Configuration descriptor.
 * @param[in] value_len Length of data in @p value
 * @param[in] value The initial descriptor value. Length of this value must be
 *   less than or equal to @p maxlen. Ingored if value type is @ref
 *   sl_bt_gattdb_user_managed_value, or if this is a Client Characteristic
 *   Configuration descriptor.
 * @param[out] descriptor The descriptor attribute handle. This handle is
 *   ensured valid in current session. It may change after the session if
 *   attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_uuid16_descriptor(uint16_t session,
                                               uint16_t characteristic,
                                               uint16_t property,
                                               uint16_t security,
                                               sl_bt_uuid_16_t uuid,
                                               uint8_t value_type,
                                               uint16_t maxlen,
                                               size_t value_len,
                                               const uint8_t* value,
                                               uint16_t *descriptor);

/***************************************************************************//**
 *
 * Add a 128-bits UUID descriptor to a characteristic. When successful, the
 * descriptor is appended to the descriptor list of the characteristic and
 * inherits the started or stopped state of the characteristic.
 *
 * This command does not support adding Characteristic Extended Properties
 * descriptors. This descriptor is automatically added if the characteristic
 * value has the reliable-write property or when a Characteristic User
 * Description descriptor is added and the user description has the write
 * property.
 *
 * Use the @ref sl_bt_gattdb_add_uuid16_descriptor command to add a 16-bits UUID
 * descriptor.
 *
 * @param[in] session The database update session ID
 * @param[in] characteristic The characteristic value attribute handle of the
 *   characteristic the descriptor is added to
 * @param[in] property The descriptor properties. Value: bitmask of @ref
 *   sl_bt_gattdb_descriptor_properties
 * @param[in] security Security requirement. Value: 0 or bitmask of @ref
 *   sl_bt_gattdb_security_requirements. A security requirement flag for a
 *   property is ignored if the property is not set for the descriptor.
 * @param[in] uuid The 128-bits UUID in little endian format
 * @param[in] value_type Enum @ref sl_bt_gattdb_value_type_t. The value type.
 *   Ignored if this is a Client Characteristic Configuration descriptor.
 *   Values:
 *     - <b>sl_bt_gattdb_fixed_length_value (0x1):</b> A fixed-length value
 *       managed by the local GATT server for responding the read and write
 *       requests of remote GATT clients
 *     - <b>sl_bt_gattdb_variable_length_value (0x2):</b> A variable-length
 *       value managed by the local GATT server for responding the read and
 *       write requests of remote GATT clients
 *     - <b>sl_bt_gattdb_user_managed_value (0x3):</b> A value managed by the
 *       user application for responding the read and write requests of remote
 *       GATT clients.
 * @param[in] maxlen The maximum length of the descriptor value. Ignored if @p
 *   value_type is sl_bt_gattdb_user_managed_value, or if this is a Client
 *   Characteristic Configuration descriptor.
 * @param[in] value_len Length of data in @p value
 * @param[in] value The initial descriptor value. Length of this value must be
 *   less than or equal to @p maxlen. Ignored if value type is @ref
 *   sl_bt_gattdb_user_managed_value, or if this is a Client Characteristic
 *   Configuration descriptor.
 * @param[out] descriptor The descriptor attribute handle. This handle is
 *   ensured valid in current session. It may change after the session if
 *   attributes have been inserted or deleted in front of it.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_add_uuid128_descriptor(uint16_t session,
                                                uint16_t characteristic,
                                                uint16_t property,
                                                uint16_t security,
                                                uuid_128 uuid,
                                                uint8_t value_type,
                                                uint16_t maxlen,
                                                size_t value_len,
                                                const uint8_t* value,
                                                uint16_t *descriptor);

/***************************************************************************//**
 *
 * Remove a descriptor from a characteristic.
 *
 * @param[in] session The database update session ID
 * @param[in] descriptor The descriptor handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_remove_descriptor(uint16_t session,
                                           uint16_t descriptor);

/***************************************************************************//**
 *
 * Start a service, so that the service and its attributes including
 * characteristics and descriptors become visible to remote GATT clients after
 * this change has been committed.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_start_service(uint16_t session, uint16_t service);

/***************************************************************************//**
 *
 * Stop a service, so that the service and its attributes including
 * characteristics and descriptors become invisible to remote GATT clients after
 * this change has been committed.
 *
 * @param[in] session The database update session ID
 * @param[in] service The service declaration attribute handle of the service
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_stop_service(uint16_t session, uint16_t service);

/***************************************************************************//**
 *
 * Start a characteristic, so that the characteristic and its attributes become
 * visible to remote GATT clients after this change has been committed.
 * SL_STATUS_INVALID_STATE error is returned if the parent service is not
 * started.
 *
 * @param[in] session The database update session ID
 * @param[in] characteristic The characteristic value attribute handle of the
 *   characteristic
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_start_characteristic(uint16_t session,
                                              uint16_t characteristic);

/***************************************************************************//**
 *
 * Stop a characteristic, so that the characteristic and its attributes become
 * invisible to remote GATT clients after this change has been committed.
 *
 * @param[in] session The database update session ID
 * @param[in] characteristic The characteristic value attribute handle of the
 *   characteristic
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_stop_characteristic(uint16_t session,
                                             uint16_t characteristic);

/***************************************************************************//**
 *
 * Save all changes performed in the current session and close the session. The
 * stack will assign final handles to new and affected attributes and handle
 * GATT caching as needed. The stack removes the client characteristic
 * configurations of non-connected GATT clients except the service-changed
 * configuration. For connected GATT clients during this database change, the
 * stack removes the configurations to the removed characteristics. The session
 * ID, temporary attribute handles returned during this session, and other
 * existing attribute handles that are after newly added or removed attributes
 * are invalidated.
 *
 * Some attribute handles returned in this session may become invalid if
 * attributes are not created in the order they present in the database. In this
 * case, attribute handle cache of the database in the user application must be
 * refreshed to avoid accidentally using an invalidated handle in subsequent
 * operations.
 *
 * @param[in] session The database update session ID
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_commit(uint16_t session);

/***************************************************************************//**
 *
 * Cancel all changes performed in the current session and close the session.
 * The database remains in the same state it was in just before the session was
 * started. The session ID and all temporary attribute handles returned during
 * this session are invalidated.
 *
 * @param[in] session The database update session ID
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_abort(uint16_t session);

/***************************************************************************//**
 *
 * Get the state of the given attribute.
 *
 * @param[in] attribute Attribute handle
 * @param[out] state State of the given attribute. Value: bitmask of @ref
 *   sl_bt_gattdb_attribute_state
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gattdb_get_attribute_state(uint16_t attribute,
                                             uint8_t *state);

/** @} */ // end addtogroup sl_bt_gattdb

/**
 * @addtogroup sl_bt_gatt_server GATT Server
 * @{
 *
 * @brief GATT Server
 *
 * These commands and events are used for accessing to the local GATT server and
 * database.
 */

/* Command and Response IDs */
#define sl_bt_cmd_gatt_server_set_max_mtu_id                         0x0a0a0020
#define sl_bt_cmd_gatt_server_get_mtu_id                             0x0b0a0020
#define sl_bt_cmd_gatt_server_find_attribute_id                      0x060a0020
#define sl_bt_cmd_gatt_server_find_primary_service_id                0x090a0020
#define sl_bt_cmd_gatt_server_read_attribute_value_id                0x000a0020
#define sl_bt_cmd_gatt_server_read_attribute_type_id                 0x010a0020
#define sl_bt_cmd_gatt_server_read_attribute_properties_id           0x050a0020
#define sl_bt_cmd_gatt_server_write_attribute_value_id               0x020a0020
#define sl_bt_cmd_gatt_server_send_user_read_response_id             0x030a0020
#define sl_bt_cmd_gatt_server_send_user_write_response_id            0x040a0020
#define sl_bt_cmd_gatt_server_send_notification_id                   0x0f0a0020
#define sl_bt_cmd_gatt_server_send_indication_id                     0x100a0020
#define sl_bt_cmd_gatt_server_notify_all_id                          0x110a0020
#define sl_bt_cmd_gatt_server_read_client_configuration_id           0x120a0020
#define sl_bt_cmd_gatt_server_send_user_prepare_write_response_id    0x140a0020
#define sl_bt_cmd_gatt_server_set_capabilities_id                    0x080a0020
#define sl_bt_cmd_gatt_server_enable_capabilities_id                 0x0c0a0020
#define sl_bt_cmd_gatt_server_disable_capabilities_id                0x0d0a0020
#define sl_bt_cmd_gatt_server_get_enabled_capabilities_id            0x0e0a0020
#define sl_bt_cmd_gatt_server_read_client_supported_features_id      0x150a0020
#define sl_bt_rsp_gatt_server_set_max_mtu_id                         0x0a0a0020
#define sl_bt_rsp_gatt_server_get_mtu_id                             0x0b0a0020
#define sl_bt_rsp_gatt_server_find_attribute_id                      0x060a0020
#define sl_bt_rsp_gatt_server_find_primary_service_id                0x090a0020
#define sl_bt_rsp_gatt_server_read_attribute_value_id                0x000a0020
#define sl_bt_rsp_gatt_server_read_attribute_type_id                 0x010a0020
#define sl_bt_rsp_gatt_server_read_attribute_properties_id           0x050a0020
#define sl_bt_rsp_gatt_server_write_attribute_value_id               0x020a0020
#define sl_bt_rsp_gatt_server_send_user_read_response_id             0x030a0020
#define sl_bt_rsp_gatt_server_send_user_write_response_id            0x040a0020
#define sl_bt_rsp_gatt_server_send_notification_id                   0x0f0a0020
#define sl_bt_rsp_gatt_server_send_indication_id                     0x100a0020
#define sl_bt_rsp_gatt_server_notify_all_id                          0x110a0020
#define sl_bt_rsp_gatt_server_read_client_configuration_id           0x120a0020
#define sl_bt_rsp_gatt_server_send_user_prepare_write_response_id    0x140a0020
#define sl_bt_rsp_gatt_server_set_capabilities_id                    0x080a0020
#define sl_bt_rsp_gatt_server_enable_capabilities_id                 0x0c0a0020
#define sl_bt_rsp_gatt_server_disable_capabilities_id                0x0d0a0020
#define sl_bt_rsp_gatt_server_get_enabled_capabilities_id            0x0e0a0020
#define sl_bt_rsp_gatt_server_read_client_supported_features_id      0x150a0020

/**
 * @brief These values define whether the server is to sent notifications or
 * indications to a remote GATT server.
 */
typedef enum
{
  sl_bt_gatt_server_disable                     = 0x0, /**< (0x0) Disable
                                                            notifications and
                                                            indications. */
  sl_bt_gatt_server_notification                = 0x1, /**< (0x1) The
                                                            characteristic value
                                                            shall be notified. */
  sl_bt_gatt_server_indication                  = 0x2, /**< (0x2) The
                                                            characteristic value
                                                            shall be indicated. */
  sl_bt_gatt_server_notification_and_indication = 0x3  /**< (0x3) The
                                                            characteristic value
                                                            notification and
                                                            indication are
                                                            enabled, application
                                                            decides which one to
                                                            send. */
} sl_bt_gatt_server_client_configuration_t;

/**
 * @brief These values describe whether the characteristic client configuration
 * was changed or whether a characteristic confirmation was received.
 */
typedef enum
{
  sl_bt_gatt_server_client_config = 0x1, /**< (0x1) Characteristic client
                                              configuration has been changed. */
  sl_bt_gatt_server_confirmation  = 0x2  /**< (0x2) Characteristic confirmation
                                              has been received. */
} sl_bt_gatt_server_characteristic_status_flag_t;

/**
 * @addtogroup sl_bt_evt_gatt_server_attribute_value sl_bt_evt_gatt_server_attribute_value
 * @{
 * @brief Indicates that the value of an attribute in the local GATT database
 * was changed by a remote GATT client
 *
 * The parameter @p att_opcode describes which GATT procedure was used to change
 * the value.
 */

/** @brief Identifier of the attribute_value event */
#define sl_bt_evt_gatt_server_attribute_value_id                     0x000a00a0

/***************************************************************************//**
 * @brief Data structure of the attribute_value event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_attribute_value_s
{
  uint8_t    connection; /**< Connection handle */
  uint16_t   attribute;  /**< Attribute Handle */
  uint8_t    att_opcode; /**< Enum @ref sl_bt_gatt_att_opcode_t. Attribute
                              opcode that informs the procedure from which the
                              value was received. */
  uint16_t   offset;     /**< Value offset */
  uint8array value;      /**< Value */
});

typedef struct sl_bt_evt_gatt_server_attribute_value_s sl_bt_evt_gatt_server_attribute_value_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_attribute_value

/**
 * @addtogroup sl_bt_evt_gatt_server_user_read_request sl_bt_evt_gatt_server_user_read_request
 * @{
 * @brief Indicates that a remote GATT client is attempting to read a value of
 * an attribute from the local GATT database, where the attribute was defined in
 * the GATT database XML file to have the type="user"
 *
 * The parameter @p att_opcode informs which GATT procedure was used to read the
 * value. The application needs to respond to this request by using the @ref
 * sl_bt_gatt_server_send_user_read_response command within 30 seconds,
 * otherwise further GATT transactions are not allowed by the remote side.
 */

/** @brief Identifier of the user_read_request event */
#define sl_bt_evt_gatt_server_user_read_request_id                   0x010a00a0

/***************************************************************************//**
 * @brief Data structure of the user_read_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_user_read_request_s
{
  uint8_t  connection;     /**< Connection handle */
  uint16_t characteristic; /**< GATT characteristic handle. This value is
                                normally received from the gatt_characteristic
                                event. */
  uint8_t  att_opcode;     /**< Enum @ref sl_bt_gatt_att_opcode_t. Attribute
                                opcode that informs the procedure from which the
                                value was received. */
  uint16_t offset;         /**< Value offset */
});

typedef struct sl_bt_evt_gatt_server_user_read_request_s sl_bt_evt_gatt_server_user_read_request_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_user_read_request

/**
 * @addtogroup sl_bt_evt_gatt_server_user_write_request sl_bt_evt_gatt_server_user_write_request
 * @{
 * @brief Indicates that a remote GATT client is attempting to write a value of
 * an attribute into the local GATT database, where the attribute was defined in
 * the GATT database XML file to have the type="user"
 *
 * The parameter @p att_opcode informs which attribute procedure was used to
 * write the value. If the @p att_opcode is @ref sl_bt_gatt_write_request (see
 * @ref sl_bt_gatt_att_opcode_t), the application needs to respond to this
 * request by using the @ref sl_bt_gatt_server_send_user_write_response command
 * within 30 seconds, otherwise further GATT transactions are not allowed by the
 * remote side. If the @p att_opcode is @ref sl_bt_gatt_prepare_write_request,
 * the application needs to respond to this request by using the @ref
 * sl_bt_gatt_server_send_user_prepare_write_response command within 30 seconds,
 * otherwise further GATT transactions are not allowed by the remote side. If
 * the value of @p att_opcode is @ref sl_bt_gatt_execute_write_request, it
 * indicates that there was one or more prepare writes earlier and now the GATT
 * server is processing the execute write, the value of @p characteristic is set
 * to 0 and should be ignored. The event @ref
 * sl_bt_evt_gatt_server_execute_write_completed will be emitted after
 * responding to @ref sl_bt_gatt_execute_write_request by using @ref
 * sl_bt_gatt_server_send_user_write_response.
 */

/** @brief Identifier of the user_write_request event */
#define sl_bt_evt_gatt_server_user_write_request_id                  0x020a00a0

/***************************************************************************//**
 * @brief Data structure of the user_write_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_user_write_request_s
{
  uint8_t    connection;     /**< Connection handle */
  uint16_t   characteristic; /**< GATT characteristic handle. This value is
                                  normally received from the gatt_characteristic
                                  event. */
  uint8_t    att_opcode;     /**< Enum @ref sl_bt_gatt_att_opcode_t. Attribute
                                  opcode that informs the procedure from which
                                  the value was received. */
  uint16_t   offset;         /**< Value offset */
  uint8array value;          /**< Value */
});

typedef struct sl_bt_evt_gatt_server_user_write_request_s sl_bt_evt_gatt_server_user_write_request_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_user_write_request

/**
 * @addtogroup sl_bt_evt_gatt_server_characteristic_status sl_bt_evt_gatt_server_characteristic_status
 * @{
 * @brief Indicates either that a local Client Characteristic Configuration
 * descriptor was changed by the remote GATT client, or that a confirmation from
 * the remote GATT client was received upon a successful reception of the
 * indication
 *
 * A confirmation by the remote GATT client should be received within 30 seconds
 * after an indication was sent with the @ref sl_bt_gatt_server_send_indication
 * command, otherwise further GATT transactions over this connection are not
 * allowed by the stack.
 */

/** @brief Identifier of the characteristic_status event */
#define sl_bt_evt_gatt_server_characteristic_status_id               0x030a00a0

/***************************************************************************//**
 * @brief Data structure of the characteristic_status event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_characteristic_status_s
{
  uint8_t  connection;          /**< Connection handle */
  uint16_t characteristic;      /**< GATT characteristic handle. This value is
                                     normally received from the
                                     gatt_characteristic event. */
  uint8_t  status_flags;        /**< Enum @ref
                                     sl_bt_gatt_server_characteristic_status_flag_t.
                                     Describes whether Client Characteristic
                                     Configuration was changed or if a
                                     confirmation was received. Values:
                                       - <b>sl_bt_gatt_server_client_config
                                         (0x1):</b> Characteristic client
                                         configuration has been changed.
                                       - <b>sl_bt_gatt_server_confirmation
                                         (0x2):</b> Characteristic confirmation
                                         has been received. */
  uint16_t client_config_flags; /**< Enum @ref
                                     sl_bt_gatt_server_client_configuration_t.
                                     This field carries the new value of the
                                     Client Characteristic Configuration. If the
                                     status_flags is 0x2 (confirmation
                                     received), the value of this field can be
                                     ignored. */
  uint16_t client_config;       /**< The handle of client-config descriptor. */
});

typedef struct sl_bt_evt_gatt_server_characteristic_status_s sl_bt_evt_gatt_server_characteristic_status_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_characteristic_status

/**
 * @addtogroup sl_bt_evt_gatt_server_execute_write_completed sl_bt_evt_gatt_server_execute_write_completed
 * @{
 * @brief Indicates that the execute write command from a remote GATT client has
 * completed with the given result
 */

/** @brief Identifier of the execute_write_completed event */
#define sl_bt_evt_gatt_server_execute_write_completed_id             0x040a00a0

/***************************************************************************//**
 * @brief Data structure of the execute_write_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_execute_write_completed_s
{
  uint8_t  connection; /**< Connection handle */
  uint16_t result;     /**< Execute write result */
});

typedef struct sl_bt_evt_gatt_server_execute_write_completed_s sl_bt_evt_gatt_server_execute_write_completed_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_execute_write_completed

/**
 * @addtogroup sl_bt_evt_gatt_server_indication_timeout sl_bt_evt_gatt_server_indication_timeout
 * @{
 * @brief Indicates confirmation from the remote GATT client has not been
 * received within 30 seconds after an indication was sent
 *
 * Furthermore, the stack does not allow GATT transactions over this connection.
 */

/** @brief Identifier of the indication_timeout event */
#define sl_bt_evt_gatt_server_indication_timeout_id                  0x050a00a0

/***************************************************************************//**
 * @brief Data structure of the indication_timeout event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_indication_timeout_s
{
  uint8_t connection; /**< Connection handle */
});

typedef struct sl_bt_evt_gatt_server_indication_timeout_s sl_bt_evt_gatt_server_indication_timeout_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_indication_timeout

/**
 * @addtogroup sl_bt_evt_gatt_server_notification_tx_completed sl_bt_evt_gatt_server_notification_tx_completed
 * @{
 * @brief Indicates that one or more GATT notifications have been transmitted.
 *
 * By default, this event is not enabled on Bluetooth connections due to
 * additional resource usages
 *
 * Following enablers are required to enable the Bluetooth stack's functionality
 * for supporting this event,
 *   - Include feature bluetooth_feature_resource_report in the application
 *     project.
 *   - Use command @ref sl_bt_resource_enable_connection_tx_report to enable
 *     data packet TX status reports before this Bluetooth connection is
 *     established.
 */

/** @brief Identifier of the notification_tx_completed event */
#define sl_bt_evt_gatt_server_notification_tx_completed_id           0x060a00a0

/***************************************************************************//**
 * @brief Data structure of the notification_tx_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_gatt_server_notification_tx_completed_s
{
  uint8_t connection; /**< Connection handle */
  uint8_t count;      /**< Number of notifications that have been transmitted */
});

typedef struct sl_bt_evt_gatt_server_notification_tx_completed_s sl_bt_evt_gatt_server_notification_tx_completed_t;

/** @} */ // end addtogroup sl_bt_evt_gatt_server_notification_tx_completed

/***************************************************************************//**
 *
 * Set the maximum size of ATT Message Transfer Units (MTU) in the host stack.
 * The functionality is the same as @ref sl_bt_gatt_set_max_mtu and the setting
 * applies to both GATT client and server.
 *
 * The value will be used as maximum receive MTU size of the GATT server in
 * ATT_EXCHANGE_MTU_RSP. If the GATT client component presents in the
 * application, the value will also be used as the maximum receive MTU size in
 * ATT_EXCHANGE_MTU_REQ. The ATT_MTU on a connection is 23 when the connection
 * is opened. Note that only the GATT Client role can initiate an ATT MTU
 * Exchange request.
 *
 * If the given value is too large according to the maximum BGAPI payload size,
 * the host stack will select the maximum possible value.
 *
 * @param[in] max_mtu @parblock
 *   Maximum size of MTU in the host stack for GATT client and server
 *     - <b>Range:</b> 23 to 250
 *
 *     - <b>Default</b> : 247
 *   @endparblock
 * @param[out] max_mtu_out The maximum size of MTU selected by the host stack
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_set_max_mtu(uint16_t max_mtu,
                                          uint16_t *max_mtu_out);

/***************************************************************************//**
 *
 * Get the size of ATT Message Transfer Units (MTU) for a connection.
 *
 * @param[in] connection Connection handle
 * @param[out] mtu The maximum ATT_MTU used by the connection
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_get_mtu(uint8_t connection, uint16_t *mtu);

/***************************************************************************//**
 *
 * Find attributes of a certain type from a local GATT database. The type is
 * usually given as a 16-bit or 128-bit UUID in little endian format.
 *
 * @param[in] start Search start handle
 * @param[in] type_len Length of data in @p type
 * @param[in] type The attribute type UUID
 * @param[out] attribute Attribute handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_find_attribute(uint16_t start,
                                             size_t type_len,
                                             const uint8_t* type,
                                             uint16_t *attribute);

/***************************************************************************//**
 *
 * Find a primary service from local GATT database.
 *
 * @param[in] start Search start index
 * @param[in] uuid_len Length of data in @p uuid
 * @param[in] uuid Variable-length byte array. The first byte defines the length
 *   of data that follows, 0 - 255 bytes.
 * @param[out] start_out Group start handle if this command succeeds
 * @param[out] end_out Group end handle if this command succeeds
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_find_primary_service(uint16_t start,
                                                   size_t uuid_len,
                                                   const uint8_t* uuid,
                                                   uint16_t *start_out,
                                                   uint16_t *end_out);

/***************************************************************************//**
 *
 * Read the value of an attribute from a local GATT database. Only (maximum
 * BGAPI payload size - 3) amount of data can be read at once. The application
 * can continue reading with increased offset value if it receives (maximum
 * BGAPI payload size - 3) number of bytes.
 *
 * @param[in] attribute Attribute handle
 * @param[in] offset Value offset
 * @param[in] max_value_size Size of output buffer passed in @p value
 * @param[out] value_len On return, set to the length of output data written to
 *   @p value
 * @param[out] value The attribute value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_read_attribute_value(uint16_t attribute,
                                                   uint16_t offset,
                                                   size_t max_value_size,
                                                   size_t *value_len,
                                                   uint8_t *value);

/***************************************************************************//**
 *
 * Read the type of an attribute from a local GATT database. The type is a UUID,
 * usually 16 or 128 bits long in little endian format.
 *
 * @param[in] attribute Attribute handle
 * @param[in] max_type_size Size of output buffer passed in @p type
 * @param[out] type_len On return, set to the length of output data written to
 *   @p type
 * @param[out] type The attribute type UUID
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_read_attribute_type(uint16_t attribute,
                                                  size_t max_type_size,
                                                  size_t *type_len,
                                                  uint8_t *type);

/***************************************************************************//**
 *
 * Read the properties of an attribute from the local GATT database.
 *
 * @param[in] attribute Attribute handle
 * @param[out] category Enum @ref sl_bt_gattdb_attribute_category_t. The
 *   high-level attribute category. Values:
 *     - <b>sl_bt_gattdb_category_service (0x1):</b> The attribute is a service
 *       declaration
 *     - <b>sl_bt_gattdb_category_include (0x2):</b> The attribute is an include
 *       declaration
 *     - <b>sl_bt_gattdb_category_characteristic_declaration (0x3):</b> The
 *       attribute is a characteristic declaration
 *     - <b>sl_bt_gattdb_category_characteristic_value (0x4):</b> The attribute
 *       is a characteristic value
 *     - <b>sl_bt_gattdb_category_descriptor (0x5):</b> The attribute is a
 *       descriptor
 * @param[out] security Security requirement for accessing the attribute. The
 *   value is a bitmask of @ref sl_bt_gattdb_security_requirements.
 * @param[out] properties The property flags of the attribute. Different flags
 *   are used depending on the attribute category specified by @p category:
 *     - When @p category is @ref sl_bt_gattdb_category_service, the value is a
 *       bitmask of @ref sl_bt_gattdb_service_property_flags
 *     - When @p category is @ref sl_bt_gattdb_category_include, the value is
 *       set to 0
 *     - When @p category is @ref
 *       sl_bt_gattdb_category_characteristic_declaration, the value is set to 0
 *     - When @p category is @ref sl_bt_gattdb_category_characteristic_value,
 *       the value is a bitmask of @ref sl_bt_gattdb_characteristic_properties
 *     - When @p category is @ref sl_bt_gattdb_category_descriptor, the value is
 *       a bitmask of @ref sl_bt_gattdb_descriptor_properties
 * @param[out] value_type Enum @ref sl_bt_gattdb_value_type_t. The value type.
 *   Values:
 *     - <b>sl_bt_gattdb_fixed_length_value (0x1):</b> A fixed-length value
 *       managed by the local GATT server for responding the read and write
 *       requests of remote GATT clients
 *     - <b>sl_bt_gattdb_variable_length_value (0x2):</b> A variable-length
 *       value managed by the local GATT server for responding the read and
 *       write requests of remote GATT clients
 *     - <b>sl_bt_gattdb_user_managed_value (0x3):</b> A value managed by the
 *       user application for responding the read and write requests of remote
 *       GATT clients.
 * @param[out] len The current length of the attribute data stored in the local
 *   GATT database. This value is set to 0 for attributes that are managed by
 *   the user application (@p value_type is @ref
 *   sl_bt_gattdb_user_managed_value).
 * @param[out] max_writable_len The maximum writable length of the attribute
 *   data stored in the local GATT database. This is independent of the
 *   attribute permissions, as even read-only attributes can be writable by the
 *   local GATT server. This value is set to 0 for attributes that are not
 *   writable or are managed by the user application (@p value_type is @ref
 *   sl_bt_gattdb_user_managed_value).
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_read_attribute_properties(uint16_t attribute,
                                                        uint8_t *category,
                                                        uint16_t *security,
                                                        uint16_t *properties,
                                                        uint8_t *value_type,
                                                        uint16_t *len,
                                                        uint16_t *max_writable_len);

/***************************************************************************//**
 *
 * Write the value of an attribute in the local GATT database. Writing the value
 * of a characteristic of the local GATT database will not trigger notifications
 * or indications to the remote GATT client if the characteristic has a property
 * to indicate or notify and the client has enabled notification or indication.
 * Notifications and indications are sent to the remote GATT client using @ref
 * sl_bt_gatt_server_send_notification or @ref sl_bt_gatt_server_send_indication
 * commands.
 *
 * @param[in] attribute Attribute handle
 * @param[in] offset Value offset
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_write_attribute_value(uint16_t attribute,
                                                    uint16_t offset,
                                                    size_t value_len,
                                                    const uint8_t* value);

/***************************************************************************//**
 *
 * Send a response to a @ref sl_bt_evt_gatt_server_user_read_request event. The
 * response needs to be sent within 30 seconds, otherwise no more GATT
 * transactions are allowed by the remote side. If attr_errorcode is set to 0,
 * the characteristic value is sent to the remote GATT client in the standard
 * way. Other attr_errorcode values will cause the local GATT server to send an
 * attribute protocol error response instead of the actual data. Maximum number
 * of bytes this command can send depends on the value of parameter att_opcode
 * in @ref sl_bt_evt_gatt_server_user_read_request event:
 *   - ATT_MTU - 1 if the opcode is @ref sl_bt_gatt_read_request or @ref
 *     sl_bt_gatt_read_blob_request
 *   - ATT_MTU - 4 if the opcode is @ref sl_bt_gatt_read_by_type_request
 *
 * If the data length in @p value exceeds the limit, the first maximum number of
 * bytes will be sent and rest data is ignored. The actual number of bytes that
 * was sent is retruned in the response of this command.
 *
 * The client will continue reading by sending a new read blob request with an
 * increased offset value if it receives the maximum amount of attribute data
 * the read respond packet can contain.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle received in the @ref
 *   sl_bt_evt_gatt_server_user_read_request event.
 * @param[in] att_errorcode Attribute protocol error code
 *     - <b>0:</b> No error
 *     - <b>Non-zero:</b> See Bluetooth specification, Host volume, Attribute
 *       Protocol, Error Codes table.
 * @param[in] value_len Length of data in @p value
 * @param[in] value Characteristic value to send to the GATT client. Ignored if
 *   att_errorcode is not 0.
 * @param[out] sent_len The length of data sent to the remote GATT client
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_send_user_read_response(uint8_t connection,
                                                      uint16_t characteristic,
                                                      uint8_t att_errorcode,
                                                      size_t value_len,
                                                      const uint8_t* value,
                                                      uint16_t *sent_len);

/***************************************************************************//**
 *
 * Send a response to a @ref sl_bt_evt_gatt_server_user_write_request event when
 * parameter @p att_opcode in the event is @ref sl_bt_gatt_write_request or @ref
 * sl_bt_gatt_execute_write_request (see @ref sl_bt_gatt_att_opcode_t). The
 * response needs to be sent within 30 seconds, otherwise no more GATT
 * transactions are allowed by the remote side. When responding to @ref
 * sl_bt_gatt_execute_write_request, the value of parameter @p characteristic is
 * ignored. If attr_errorcode is set to 0, the ATT protocol's write response is
 * sent to indicate to the remote GATT client that the write operation was
 * processed successfully. Other values will cause the local GATT server to send
 * an ATT protocol error response.
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle received in the @ref
 *   sl_bt_evt_gatt_server_user_write_request event
 * @param[in] att_errorcode Attribute protocol error code
 *     - <b>0:</b> No error
 *     - <b>Non-zero:</b> See Bluetooth specification, Host volume, Attribute
 *       Protocol, Error Codes table.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_send_user_write_response(uint8_t connection,
                                                       uint16_t characteristic,
                                                       uint8_t att_errorcode);

/***************************************************************************//**
 *
 * Send a notification to a remote GATT client. At most, ATT_MTU - 3 number of
 * bytes can be sent in a notification. An error SL_STATUS_COMMAND_TOO_LONG is
 * returned if the value length exceeds ATT_MTU - 3.
 *
 * A notification is sent only if the client has enabled it by setting the
 * corresponding flag to the Client Characteristic Configuration descriptor. The
 * error SL_STATUS_INVALID_PARAMETER is returned if the characteristic does not
 * have the notification property. The error SL_STATUS_INVALID_STATE is returned
 * if the client has not enabled the notification.
 *
 * @param[in] connection A handle of the connection over which the notification
 *   is sent.
 * @param[in] characteristic Characteristic handle
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to be notified
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_send_notification(uint8_t connection,
                                                uint16_t characteristic,
                                                size_t value_len,
                                                const uint8_t* value);

/***************************************************************************//**
 *
 * Send an indication to a remote GATT client. At most, ATT_MTU - 3 number of
 * bytes can be sent in an indication. An error SL_STATUS_COMMAND_TOO_LONG is
 * returned if the value length exceeds ATT_MTU - 3.
 *
 * An indication is sent only if the client has enabled it by setting the
 * corresponding flag to the Client Characteristic Configuration descriptor. The
 * error SL_STATUS_INVALID_PARAMETER is returned if the characteristic does not
 * have the indication property. The error SL_STATUS_INVALID_STATE is returned
 * if the client has not enabled the indication.
 *
 * A new indication to a GATT client can't be sent until an outstanding
 * indication procedure with the same client has completed. The procedure is
 * completed when a confirmation from the client is received. The confirmation
 * is indicated by @ref sl_bt_evt_gatt_server_characteristic_status.
 *
 * The error SL_STATUS_IN_PROGRESS is returned if an indication procedure with
 * the same client is outstanding. Always wait for confirmation for previous
 * indication before sending a new indication.
 *
 * @param[in] connection A handle of the connection over which the indication is
 *   sent.
 * @param[in] characteristic Characteristic handle
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to be indicated
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_gatt_server_characteristic_status - This event is
 *     triggered after the confirmation from the client is received.
 *   - @ref sl_bt_evt_gatt_server_indication_timeout - This event indicates
 *     confirmation from the remote GATT client has not been received within 30
 *     seconds after an indication was sent. Further GATT transactions over this
 *     connection are not allowed by the stack.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_send_indication(uint8_t connection,
                                              uint16_t characteristic,
                                              size_t value_len,
                                              const uint8_t* value);

/***************************************************************************//**
 *
 * Send notifications or indications to all connected remote GATT clients. At
 * most, ATT_MTU - 3 number of bytes can be sent in a notification or
 * indication. If the value length exceeds the limit on a connection, the first
 * ATT_MTU - 3 bytes will be sent and rest of data is ignored.
 *
 * A notification or indication is sent only if the client has enabled it by
 * setting the corresponding flag to the Client Characteristic Configuration
 * descriptor. If the Client Characteristic Configuration descriptor supports
 * both notifications and indications, the stack will always send a notification
 * even when the client has enabled both. This command will not return an error
 * if some clients fail to be notified. Iterate @ref
 * sl_bt_gatt_server_send_notification or @ref sl_bt_gatt_server_send_indication
 * on each connection if the application needs to check that the notification or
 * indication was sent to each client.
 *
 * A new indication to a GATT client can't be sent until an outstanding
 * indication procedure with the same client has completed, and the operation
 * will continue for the next client. The procedure is completed when a
 * confirmation from the client is received. The confirmation is indicated by
 * @ref sl_bt_evt_gatt_server_characteristic_status.
 *
 * @param[in] characteristic Characteristic handle
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to be notified or indicated
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_notify_all(uint16_t characteristic,
                                         size_t value_len,
                                         const uint8_t* value);

/***************************************************************************//**
 *
 * Read client characteristic configuration of a remote GATT client.
 *
 * @param[in] connection A handle of the connection to a remote client.
 * @param[in] characteristic Characteristic handle
 * @param[out] client_config_flags Enum @ref
 *   sl_bt_gatt_server_client_configuration_t. Client characteristic
 *   configuration of a remote client.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_read_client_configuration(uint8_t connection,
                                                        uint16_t characteristic,
                                                        uint16_t *client_config_flags);

/***************************************************************************//**
 *
 * Send a response to a @ref sl_bt_evt_gatt_server_user_write_request event when
 * parameter @p att_opcode in the event is @ref sl_bt_gatt_prepare_write_request
 * (see @ref sl_bt_gatt_att_opcode_t). The response needs to be sent within 30
 * seconds, otherwise no more GATT transactions are allowed by the remote side.
 * If @p att_errorcode is set to 0, the ATT protocol's prepare write response is
 * sent to indicate to the remote GATT client that the write operation was
 * processed successfully. Other values will cause the local GATT server to send
 * an ATT protocol error response. The application should set values of
 * parameters @p offset and @p value to identical values from the @ref
 * sl_bt_evt_gatt_server_user_write_request event. The values will be verified
 * on the client side in case the request is a reliable write (by Bluetooth Core
 * Specification Volume 3, Part G, 4.9.5).
 *
 * @param[in] connection Connection handle
 * @param[in] characteristic GATT characteristic handle. This value is normally
 *   received from the gatt_characteristic event.
 * @param[in] att_errorcode Attribute protocol error code
 *     - <b>0:</b> No error
 *     - <b>Non-zero:</b> See Bluetooth specification, Host volume, Attribute
 *       Protocol, Error Codes table.
 * @param[in] offset Value offset
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_send_user_prepare_write_response(uint8_t connection,
                                                               uint16_t characteristic,
                                                               uint8_t att_errorcode,
                                                               uint16_t offset,
                                                               size_t value_len,
                                                               const uint8_t* value);

/***************************************************************************//**
 *
 * Reset capabilities that should be enabled by the GATT database. A service is
 * visible to remote GATT clients if at least one of its capabilities is
 * enabled. The same applies to a characteristic and its attributes. Capability
 * identifiers and their corresponding bit flag values are in the auto-generated
 * database header file. See UG118: Blue Gecko Bluetooth Profile Toolkit
 * Developer's Guide for how to declare capabilities in the GATT database.
 *
 * Changing the capabilities of a database effectively causes a database change
 * (attributes being added or removed) from a remote GATT client point of view.
 * If the database has a Generic Attribute service and Service Changed
 * characteristic, the stack will monitor the local database change status and
 * manage service changed indications for a GATT client that has enabled the
 * indication configuration of the Service Changed characteristic.
 *
 * @param[in] caps Bit flags of capabilities to reset. Value 0 sets the default
 *   database capabilities.
 * @param[in] reserved Use the value 0 on this reserved field. Do not use
 *   none-zero values because they are reserved for future use.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_set_capabilities(uint32_t caps,
                                               uint32_t reserved);

/***************************************************************************//**
 *
 * Enable additional capabilities in the local GATT database. Already enabled
 * capabilities keep unchanged after this command. See @ref
 * sl_bt_gatt_server_set_capabilities for more information.
 *
 * @param[in] caps Capabilities to enable
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_enable_capabilities(uint32_t caps);

/***************************************************************************//**
 *
 * Disable the given capabilities in the local GATT database. See @ref
 * sl_bt_gatt_server_set_capabilities for more information.
 *
 * @param[in] caps Capabilities to disable
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_disable_capabilities(uint32_t caps);

/***************************************************************************//**
 *
 * Get capabilities currently enabled in the local GATT database.
 *
 * @param[out] caps Enabled capabilities
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_get_enabled_capabilities(uint32_t *caps);

/***************************************************************************//**
 *
 * Read client supported features of a remote GATT client.
 *
 * @param[in] connection A handle of the connection to a remote client.
 * @param[out] client_features Bit field describing client supported features of
 *   a remote client. See Bluetooth specification Vol 3, Part G, 7.2 for the
 *   values.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_gatt_server_read_client_supported_features(uint8_t connection,
                                                             uint8_t *client_features);

/** @} */ // end addtogroup sl_bt_gatt_server

/**
 * @addtogroup sl_bt_nvm NVM3 Access in NCP mode
 * @{
 *
 * @brief NVM3 Access in NCP mode
 *
 * Provide convenient methods for applications in NCP mode to manage user data
 * in the Bluetooth region of NVM3. User data stored in NVM3 is persistent
 * across reset and power cycling of the device. Because Bluetooth bondings are
 * also stored in NVM3, the space available for user data also depends on the
 * number of bondings the device has at the time.
 *
 * It is recommended to use NVM3 APIs directly whenever it is possible,
 * especially for applications in SoC mode.
 *
 * In NVM3, the flash store size is configurable and the minimum is 3 flash
 * pages. The maximum data object size is configurable up to 4096 bytes. A
 * Bluetooth bonding uses maximum 110 bytes for secure connections and 138 bytes
 * for legacy pairing. For more details, see AN1135 "Using Third Generation
 * NonVolatile Memory (NVM3) Data Storage".
 */

/* Command and Response IDs */
#define sl_bt_cmd_nvm_save_id                                        0x020d0020
#define sl_bt_cmd_nvm_load_id                                        0x030d0020
#define sl_bt_cmd_nvm_erase_id                                       0x040d0020
#define sl_bt_cmd_nvm_erase_all_id                                   0x010d0020
#define sl_bt_rsp_nvm_save_id                                        0x020d0020
#define sl_bt_rsp_nvm_load_id                                        0x030d0020
#define sl_bt_rsp_nvm_erase_id                                       0x040d0020
#define sl_bt_rsp_nvm_erase_all_id                                   0x010d0020

/**
 * @addtogroup sl_bt_nvm_key_range Defined key ranges
 * @{
 *
 * Define key ranges in the Bluetooth region of NVM3
 */

/** The low bound of the key range for storing user data in the Bluetooth region
 * of NVM3 */
#define SL_BT_NVM_KEY_RANGE_USER_MIN 0x4000    

/** The high bound of the key range for storing user data in the Bluetooth
 * region of NVM3 */
#define SL_BT_NVM_KEY_RANGE_USER_MAX 0x5fff    

/** @} */ // end Defined key ranges

/***************************************************************************//**
 *
 * Store a value into the specified NVM3 key in the Bluetooth region. Allowed
 * keys are in the range from SL_BT_NVM_KEY_RANGE_USER_MIN (0x4000) to
 * SL_BT_NVM_KEY_RANGE_USER_MAX (0x5FFF). The maximum value length is the lesser
 * of NVM3_DEFAULT_MAX_OBJECT_SIZE configuration in the application and 253
 * bytes which is the maximum amount this command can accept.
 *
 * @param[in] key The NVM3 key in the Bluetooth region
 * @param[in] value_len Length of data in @p value
 * @param[in] value Value to store into the NVM3 key
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_nvm_save(uint16_t key,
                           size_t value_len,
                           const uint8_t* value);

/***************************************************************************//**
 *
 * Retrieve the value of the specified NVM3 key in the Bluetooth region. Allowed
 * keys are in the range from SL_BT_NVM_KEY_RANGE_USER_MIN (0x4000) to
 * SL_BT_NVM_KEY_RANGE_USER_MAX (0x5FFF).
 *
 * @param[in] key The NVM3 key in the Bluetooth region
 * @param[in] max_value_size Size of output buffer passed in @p value
 * @param[out] value_len On return, set to the length of output data written to
 *   @p value
 * @param[out] value The value of the NVM3 key
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_nvm_load(uint16_t key,
                           size_t max_value_size,
                           size_t *value_len,
                           uint8_t *value);

/***************************************************************************//**
 *
 * Delete a single NVM3 key and its value from the Bluetooth region. Allowed
 * keys are in the range from SL_BT_NVM_KEY_RANGE_USER_MIN (0x4000) to
 * SL_BT_NVM_KEY_RANGE_USER_MAX (0x5FFF).
 *
 * @param[in] key The NVM3 key to delete
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_nvm_erase(uint16_t key);

/***************************************************************************//**
 *
 * Delete all NVMs keys and their corresponding values in the Bluetooth region
 * of NVM3. All bluetooth bondings and associated data in NVM3 are deleted too.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_nvm_erase_all();

/** @} */ // end addtogroup sl_bt_nvm

/**
 * @addtogroup sl_bt_test Testing Commands
 * @{
 *
 * @brief Testing Commands
 */

/* Command and Response IDs */
#define sl_bt_cmd_test_dtm_tx_v4_id                                  0x030e0020
#define sl_bt_cmd_test_dtm_tx_cw_id                                  0x040e0020
#define sl_bt_cmd_test_dtm_rx_id                                     0x010e0020
#define sl_bt_cmd_test_dtm_end_id                                    0x020e0020
#define sl_bt_rsp_test_dtm_tx_v4_id                                  0x030e0020
#define sl_bt_rsp_test_dtm_tx_cw_id                                  0x040e0020
#define sl_bt_rsp_test_dtm_rx_id                                     0x010e0020
#define sl_bt_rsp_test_dtm_end_id                                    0x020e0020

/**
 * @brief Test packet types supported by the stack
 */
typedef enum
{
  sl_bt_test_pkt_prbs9    = 0x0,  /**< (0x0) PRBS9 packet payload */
  sl_bt_test_pkt_11110000 = 0x1,  /**< (0x1) 11110000 packet payload */
  sl_bt_test_pkt_10101010 = 0x2,  /**< (0x2) 10101010 packet payload */
  sl_bt_test_pkt_11111111 = 0x4,  /**< (0x4) 11111111 packet payload */
  sl_bt_test_pkt_00000000 = 0x5,  /**< (0x5) 00000000 packet payload */
  sl_bt_test_pkt_00001111 = 0x6,  /**< (0x6) 00001111 packet payload */
  sl_bt_test_pkt_01010101 = 0x7,  /**< (0x7) 01010101 packet payload */
  sl_bt_test_pkt_pn9      = 0xfd, /**< (0xfd) PN9 continuously modulated output */
  sl_bt_test_pkt_carrier  = 0xfe  /**< (0xfe) Unmodulated carrier */
} sl_bt_test_packet_type_t;

/**
 * @brief Test PHY types
 */
typedef enum
{
  sl_bt_test_phy_1m   = 0x1, /**< (0x1) 1M PHY */
  sl_bt_test_phy_2m   = 0x2, /**< (0x2) 2M PHY */
  sl_bt_test_phy_125k = 0x3, /**< (0x3) 125k Coded PHY */
  sl_bt_test_phy_500k = 0x4  /**< (0x4) 500k Coded PHY */
} sl_bt_test_phy_t;

/**
 * @addtogroup sl_bt_evt_test_dtm_completed sl_bt_evt_test_dtm_completed
 * @{
 * @brief Indicates that the radio has processed a test start or end command
 *
 * The @p result parameter indicates the success of the command.
 *
 * After the receiver or transmitter test is stopped, the @p number_of_packets
 * parameter in this event indicates the number of received or transmitted
 * packets.
 */

/** @brief Identifier of the dtm_completed event */
#define sl_bt_evt_test_dtm_completed_id                              0x000e00a0

/***************************************************************************//**
 * @brief Data structure of the dtm_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_test_dtm_completed_s
{
  uint16_t result;            /**< Command result */
  uint16_t number_of_packets; /**< Number of packets

                                   Only valid for @ref sl_bt_test_dtm_end
                                   command. */
});

typedef struct sl_bt_evt_test_dtm_completed_s sl_bt_evt_test_dtm_completed_t;

/** @} */ // end addtogroup sl_bt_evt_test_dtm_completed

/***************************************************************************//**
 *
 * Start a transmitter test against a separate Bluetooth tester device. When the
 * command is processed by the radio, a @ref sl_bt_evt_test_dtm_completed event
 * is triggered. This event indicates whether the test started successfully.
 *
 * In the transmitter test, the device sends packets continuously with a fixed
 * interval. The type and length of each packet is set by @p packet_type and @p
 * length parameters. The parameter @p phy specifies which PHY is used to
 * transmit the packets. All devices support at least 1M PHY.
 *
 * Stop the test using the @ref sl_bt_test_dtm_end command.
 *
 * For transmitter test with packet type @ref sl_bt_test_pkt_pn9 or @ref
 * sl_bt_test_pkt_carrier, use the @ref sl_bt_test_dtm_tx_cw command.
 *
 * Starting a transmitter test while other Bluetooth activities are active will
 * either result in an error response or cause functionality issues, hence make
 * sure other Bluetooth activities are stopped when performing the test.
 *
 * @param[in] packet_type Enum @ref sl_bt_test_packet_type_t. Packet type to
 *   transmit. Values:
 *     - <b>sl_bt_test_pkt_prbs9 (0x0):</b> PRBS9 packet payload
 *     - <b>sl_bt_test_pkt_11110000 (0x1):</b> 11110000 packet payload
 *     - <b>sl_bt_test_pkt_10101010 (0x2):</b> 10101010 packet payload
 *     - <b>sl_bt_test_pkt_11111111 (0x4):</b> 11111111 packet payload
 *     - <b>sl_bt_test_pkt_00000000 (0x5):</b> 00000000 packet payload
 *     - <b>sl_bt_test_pkt_00001111 (0x6):</b> 00001111 packet payload
 *     - <b>sl_bt_test_pkt_01010101 (0x7):</b> 01010101 packet payload
 * @param[in] length Packet length in bytes
 *     - <b>Range:</b> 0 to 255
 * @param[in] channel Bluetooth channel. Channel is (F - 2402) / 2, where F is
 *   frequency in MHz.
 *     - <b>Range:</b> 0 (2402 MHz) to 39 (2480 MHz)
 * @param[in] phy Enum @ref sl_bt_test_phy_t. PHY to use. Values:
 *     - <b>sl_bt_test_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_test_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_test_phy_125k (0x3):</b> 125k Coded PHY
 *     - <b>sl_bt_test_phy_500k (0x4):</b> 500k Coded PHY
 * @param[in] power_level @parblock
 *   TX power level in unit dBm. Use specified or the nearest TX power level.
 *   The minimum -127 dBm is specified in the Bluetooth specification. However,
 *   a device may not support this low TX power. In addition, only some devices
 *   support 20 dBm TX power. Effective TX power will be limited by the global
 *   system TX power that can be set with the @ref sl_bt_system_set_tx_power
 *   command. Values:
 *     - <b>Range:</b> -127 to +20
 *
 *     - <b>0x7E:</b> Use minimum TX power level the device supports.
 *     - <b>0x7F:</b> Use the smallest of the maximum TX power level the device
 *       supports and the global maximum TX power setting in stack.
 *   @endparblock
 *
 * @return Command result
 *
 * @b Events
 *   - @ref sl_bt_evt_test_dtm_completed - This event is received when the
 *     command is processed.
 *
 ******************************************************************************/
sl_status_t sl_bt_test_dtm_tx_v4(uint8_t packet_type,
                                 uint8_t length,
                                 uint8_t channel,
                                 uint8_t phy,
                                 int8_t power_level);

/***************************************************************************//**
 *
 * Start a transmitter test for a custom wave. When the command is processed by
 * the radio, a @ref sl_bt_evt_test_dtm_completed event is triggered. This event
 * indicates whether the test started successfully.
 *
 * In the custom wave transmitter test, the device continuously transmits the
 * career. The parameter @p packet_type specifies the packet type. The parameter
 * @p phy specifies which PHY is used to transmit the packets. All devices
 * support at least 1M PHY.
 *
 * Stop the test using the @ref sl_bt_test_dtm_end command.
 *
 * Starting a transmitter test while other Bluetooth activities are active will
 * either result in an error response or cause functionality issues, hence make
 * sure other Bluetooth activities are stopped when performing the test.
 *
 * @param[in] packet_type Enum @ref sl_bt_test_packet_type_t. Packet type to
 *   transmit. Values:
 *     - <b>sl_bt_test_pkt_pn9 (0xfd):</b> PN9 continuously modulated output
 *     - <b>sl_bt_test_pkt_carrier (0xfe):</b> Unmodulated carrier
 * @param[in] channel Bluetooth channel. Channel is (F - 2402) / 2,where F is
 *   frequency in MHz.
 *     - <b>Range:</b> 0 to 39
 * @param[in] phy Enum @ref sl_bt_test_phy_t. PHY to use. Values:
 *     - <b>sl_bt_test_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_test_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_test_phy_125k (0x3):</b> 125k Coded PHY
 *     - <b>sl_bt_test_phy_500k (0x4):</b> 500k Coded PHY
 * @param[in] power_level @parblock
 *   TX power level. Unit: 0.1 dBm.
 *
 *   If the value exceeds the range of power level value, allowed by the device,
 *   the command will adjust the power level to the closest minimum or maximum
 *   value.
 *   @endparblock
 *
 * @return Command result
 *
 * @b Events
 *   - @ref sl_bt_evt_test_dtm_completed - This event is received when the
 *     command is processed.
 *
 ******************************************************************************/
sl_status_t sl_bt_test_dtm_tx_cw(uint8_t packet_type,
                                 uint8_t channel,
                                 uint8_t phy,
                                 int16_t power_level);

/***************************************************************************//**
 *
 * Start a receiver test against a separate Bluetooth tester device. When the
 * command is processed by the radio, a @ref sl_bt_evt_test_dtm_completed event
 * is triggered. This event indicates whether the test started successfully.
 *
 * Parameter @p phy specifies which PHY is used to receive the packets. All
 * devices support at least 1M PHY.
 *
 * The test may be stopped using @ref sl_bt_test_dtm_end command. This will
 * trigger another @ref sl_bt_evt_test_dtm_completed event, which carries the
 * number of packets received during the test.
 *
 * Starting a receiver test while other Bluetooth activities are active will
 * either result in an error response or cause functionality issues, hence make
 * sure other Bluetooth activities are stopped when performing the test.
 *
 * @param[in] channel Bluetooth channel. Channel is (F - 2402) / 2, where F is
 *   frequency in MHz.
 *     - <b>Range:</b> 0 to 39
 * @param[in] phy Enum @ref sl_bt_test_phy_t. PHY to use. Values:
 *     - <b>sl_bt_test_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_test_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_test_phy_125k (0x3):</b> 125k Coded PHY
 *     - <b>sl_bt_test_phy_500k (0x4):</b> 500k Coded PHY
 *
 * @return Command result
 *
 * @b Events
 *   - @ref sl_bt_evt_test_dtm_completed - This event is received when the
 *     command is processed.
 *
 ******************************************************************************/
sl_status_t sl_bt_test_dtm_rx(uint8_t channel, uint8_t phy);

/***************************************************************************//**
 *
 * End a transmitter or a receiver test. When the command is processed by the
 * radio and the test has ended, a @ref sl_bt_evt_test_dtm_completed event is
 * triggered.
 *
 *
 * @return Command result
 *
 * @b Events
 *   - @ref sl_bt_evt_test_dtm_completed - Received when the command is
 *     processed by the radio and the test has ended.
 *
 ******************************************************************************/
sl_status_t sl_bt_test_dtm_end();

/** @} */ // end addtogroup sl_bt_test

/**
 * @addtogroup sl_bt_sm Security Manager
 * @{
 *
 * @brief Security Manager
 *
 * The commands in this class manage Bluetooth security, including commands for
 * starting and stopping encryption and commands for management of all bonding
 * operations.
 *
 * Use the following procedure to bond with a remote device:
 *   - Use the command @ref sl_bt_sm_configure to configure security
 *     requirements and I/O capabilities of this device.
 *   - Use the command @ref sl_bt_sm_set_bondable_mode to set this device into
 *     bondable mode.
 *   - Use the command @ref sl_bt_connection_open to open a connection to the
 *     remote device.
 *   - After the connection is open, use command @ref sl_bt_sm_increase_security
 *     to encrypt the connection. This will also start the bonding process.
 *
 * Use the following procedure to respond to the bonding initiated by a remote
 * device:
 *   - Use the command @ref sl_bt_sm_configure to configure security
 *     requirements and I/O capabilities of this device.
 *   - Use the command @ref sl_bt_sm_set_bondable_mode to set this device into
 *     bondable mode.
 *   - Start connectable advertising, e.g., using the command @ref
 *     sl_bt_legacy_advertiser_start.
 *   - Open a connection to this device from the remote device.
 *   - After the connection is open, start the bonding process on the remote
 *     device.
 *
 * If MITM is required, the application needs to display or ask the user to
 * enter a passkey during the process. See events @ref
 * sl_bt_evt_sm_passkey_display and @ref sl_bt_evt_sm_passkey_request for more
 * information.
 */

/* Command and Response IDs */
#define sl_bt_cmd_sm_configure_id                                    0x010f0020
#define sl_bt_cmd_sm_set_minimum_key_size_id                         0x140f0020
#define sl_bt_cmd_sm_set_debug_mode_id                               0x0f0f0020
#define sl_bt_cmd_sm_store_bonding_configuration_id                  0x020f0020
#define sl_bt_cmd_sm_set_bondable_mode_id                            0x000f0020
#define sl_bt_cmd_sm_set_passkey_id                                  0x100f0020
#define sl_bt_cmd_sm_increase_security_id                            0x040f0020
#define sl_bt_cmd_sm_enter_passkey_id                                0x080f0020
#define sl_bt_cmd_sm_passkey_confirm_id                              0x090f0020
#define sl_bt_cmd_sm_bonding_confirm_id                              0x0e0f0020
#define sl_bt_cmd_sm_delete_bonding_id                               0x060f0020
#define sl_bt_cmd_sm_delete_bondings_id                              0x070f0020
#define sl_bt_cmd_sm_get_bonding_handles_id                          0x150f0020
#define sl_bt_cmd_sm_get_bonding_details_id                          0x160f0020
#define sl_bt_cmd_sm_find_bonding_by_address_id                      0x170f0020
#define sl_bt_cmd_sm_resolve_rpa_id                                  0x1d0f0020
#define sl_bt_cmd_sm_set_bonding_key_id                              0x180f0020
#define sl_bt_cmd_sm_set_legacy_oob_id                               0x190f0020
#define sl_bt_cmd_sm_set_oob_id                                      0x1a0f0020
#define sl_bt_cmd_sm_set_remote_oob_id                               0x1b0f0020
#define sl_bt_rsp_sm_configure_id                                    0x010f0020
#define sl_bt_rsp_sm_set_minimum_key_size_id                         0x140f0020
#define sl_bt_rsp_sm_set_debug_mode_id                               0x0f0f0020
#define sl_bt_rsp_sm_store_bonding_configuration_id                  0x020f0020
#define sl_bt_rsp_sm_set_bondable_mode_id                            0x000f0020
#define sl_bt_rsp_sm_set_passkey_id                                  0x100f0020
#define sl_bt_rsp_sm_increase_security_id                            0x040f0020
#define sl_bt_rsp_sm_enter_passkey_id                                0x080f0020
#define sl_bt_rsp_sm_passkey_confirm_id                              0x090f0020
#define sl_bt_rsp_sm_bonding_confirm_id                              0x0e0f0020
#define sl_bt_rsp_sm_delete_bonding_id                               0x060f0020
#define sl_bt_rsp_sm_delete_bondings_id                              0x070f0020
#define sl_bt_rsp_sm_get_bonding_handles_id                          0x150f0020
#define sl_bt_rsp_sm_get_bonding_details_id                          0x160f0020
#define sl_bt_rsp_sm_find_bonding_by_address_id                      0x170f0020
#define sl_bt_rsp_sm_resolve_rpa_id                                  0x1d0f0020
#define sl_bt_rsp_sm_set_bonding_key_id                              0x180f0020
#define sl_bt_rsp_sm_set_legacy_oob_id                               0x190f0020
#define sl_bt_rsp_sm_set_oob_id                                      0x1a0f0020
#define sl_bt_rsp_sm_set_remote_oob_id                               0x1b0f0020

/**
 * @cond RESTRICTED
 *
 * Restricted/experimental API. Contact Silicon Labs sales for more information.
 * @brief These values define the bonding keys of the bonded device stored in
 * the persistent store.
 */
typedef enum
{
  sl_bt_sm_bonding_key_remote_ltk = 0x1, /**< (0x1) LTK used as central device,
                                              always used when paired using
                                              secure connections and local LTK
                                              does not exist */
  sl_bt_sm_bonding_key_local_ltk  = 0x2, /**< (0x2) LTK used as peripheral
                                              device when using legacy pairing,
                                              only used with secure connections
                                              pairing if key exists */
  sl_bt_sm_bonding_key_irk        = 0x3  /**< (0x3) Identity resolving key for
                                              resolvable private addresses */
} sl_bt_sm_bonding_key_t;
/** @endcond */ // end restricted enum type

/**
 * @cond RESTRICTED
 *
 * Restricted/experimental API. Contact Silicon Labs sales for more information.
 * @brief These values define the bonding data types, which are stored in the
 * persistent store.
 */
typedef enum
{
  sl_bt_sm_bonding_data_remote_address       = 0x0, /**< (0x0) Identity address
                                                         of the remote device */
  sl_bt_sm_bonding_data_remote_ltk           = 0x1, /**< (0x1) LTK used as
                                                         central device */
  sl_bt_sm_bonding_data_local_ltk            = 0x2, /**< (0x2) LTK used as
                                                         peripheral device */
  sl_bt_sm_bonding_data_remote_master_inf    = 0x3, /**< (0x3) Idenfication info
                                                         used as central device */
  sl_bt_sm_bonding_data_local_master_inf     = 0x4, /**< (0x4) Idenfication info
                                                         used as central device */
  sl_bt_sm_bonding_data_irk                  = 0x5, /**< (0x5) IRK of the remote
                                                         device */
  sl_bt_sm_bonding_data_meta                 = 0x6, /**< (0x6) Metadata about
                                                         the bonding */
  sl_bt_sm_bonding_data_gatt_client_config   = 0x7, /**< (0x7) GATT database
                                                         client configuration */
  sl_bt_sm_bonding_data_gatt_client_features = 0x8, /**< (0x8) GATT client
                                                         supported features */
  sl_bt_sm_bonding_data_gatt_db_hash         = 0x9  /**< (0x9) GATT database
                                                         hash */
} sl_bt_sm_bonding_data_t;
/** @endcond */ // end restricted enum type

/**
 * @brief These values define the security management related I/O capabilities
 * supported by the device.
 */
typedef enum
{
  sl_bt_sm_io_capability_displayonly     = 0x0, /**< (0x0) Display Only */
  sl_bt_sm_io_capability_displayyesno    = 0x1, /**< (0x1) Display with
                                                     Yes/No-buttons */
  sl_bt_sm_io_capability_keyboardonly    = 0x2, /**< (0x2) Keyboard Only */
  sl_bt_sm_io_capability_noinputnooutput = 0x3, /**< (0x3) No Input and No
                                                     Output */
  sl_bt_sm_io_capability_keyboarddisplay = 0x4  /**< (0x4) Display with Keyboard */
} sl_bt_sm_io_capability_t;

/**
 * @addtogroup sl_bt_sm_configuration Security Manager configuration flags
 * @{
 *
 * These values are used to configure security requirements.
 */

/** Bonding requires authentication (Man-in-the-Middle protection). */
#define SL_BT_SM_CONFIGURATION_MITM_REQUIRED                        0x1       

/** Encryption requires bonding. Note that this setting will also enable
 * bonding. */
#define SL_BT_SM_CONFIGURATION_BONDING_REQUIRED                     0x2       

/** Require secure connections pairing. */
#define SL_BT_SM_CONFIGURATION_SC_ONLY                              0x4       

/** Bonding requests need to be confirmed. Received bonding requests are
 * notified by @ref sl_bt_evt_sm_confirm_bonding. */
#define SL_BT_SM_CONFIGURATION_BONDING_REQUEST_REQUIRED             0x8       

/** Allow connections only from bonded devices. This option is ignored when the
 * application includes the bluetooth_feature_external_bonding_database feature.
 * */
#define SL_BT_SM_CONFIGURATION_CONNECTIONS_FROM_BONDED_DEVICES_ONLY 0x10      

/** Prefer authenticated pairing when both options are possible based on the
 * settings. Otherwise just works pairing is preferred. */
#define SL_BT_SM_CONFIGURATION_PREFER_MITM                          0x20      

/** Require secure connections OOB data from both devices. */
#define SL_BT_SM_CONFIGURATION_OOB_FROM_BOTH_DEVICES_REQUIRED       0x40      

/** Reject pairing if remote device uses debug keys. */
#define SL_BT_SM_CONFIGURATION_REJECT_DEBUG_KEYS                    0x80      

/** @} */ // end Security Manager configuration flags

/**
 * @addtogroup sl_bt_evt_sm_passkey_display sl_bt_evt_sm_passkey_display
 * @{
 * @brief Indicates a request to display the passkey to the user.
 */

/** @brief Identifier of the passkey_display event */
#define sl_bt_evt_sm_passkey_display_id                              0x000f00a0

/***************************************************************************//**
 * @brief Data structure of the passkey_display event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_passkey_display_s
{
  uint8_t  connection; /**< Connection handle */
  uint32_t passkey;    /**< Passkey.
                              - <b>Range:</b> 0 to 999999

                              - NOTE! When displaying the passkey to the user,
                                prefix the number with zeros to obtain a 6 digit
                                number
                              - Example: Passkey value is 42
                              - Number to display to the user is 000042 */
});

typedef struct sl_bt_evt_sm_passkey_display_s sl_bt_evt_sm_passkey_display_t;

/** @} */ // end addtogroup sl_bt_evt_sm_passkey_display

/**
 * @addtogroup sl_bt_evt_sm_passkey_request sl_bt_evt_sm_passkey_request
 * @{
 * @brief Indicates a request for the passkey prompt displayed on the remote
 * device
 *
 * Use the command @ref sl_bt_sm_enter_passkey to input the passkey value.
 */

/** @brief Identifier of the passkey_request event */
#define sl_bt_evt_sm_passkey_request_id                              0x010f00a0

/***************************************************************************//**
 * @brief Data structure of the passkey_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_passkey_request_s
{
  uint8_t connection; /**< Connection handle */
});

typedef struct sl_bt_evt_sm_passkey_request_s sl_bt_evt_sm_passkey_request_t;

/** @} */ // end addtogroup sl_bt_evt_sm_passkey_request

/**
 * @addtogroup sl_bt_evt_sm_confirm_passkey sl_bt_evt_sm_confirm_passkey
 * @{
 * @brief Indicates a request for passkey display and confirmation by the user
 *
 * Use the command @ref sl_bt_sm_passkey_confirm to accept or reject the
 * displayed passkey.
 */

/** @brief Identifier of the confirm_passkey event */
#define sl_bt_evt_sm_confirm_passkey_id                              0x020f00a0

/***************************************************************************//**
 * @brief Data structure of the confirm_passkey event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_confirm_passkey_s
{
  uint8_t  connection; /**< Connection handle */
  uint32_t passkey;    /**< Passkey.
                              - <b>Range:</b> 0 to 999999

                              - NOTE! When displaying the passkey to the user,
                                prefix the number with zeros to obtain a 6 digit
                                number
                              - Example: Passkey value is 42
                              - Number to display to the user is 000042 */
});

typedef struct sl_bt_evt_sm_confirm_passkey_s sl_bt_evt_sm_confirm_passkey_t;

/** @} */ // end addtogroup sl_bt_evt_sm_confirm_passkey

/**
 * @addtogroup sl_bt_evt_sm_bonded sl_bt_evt_sm_bonded
 * @{
 * @brief Triggered when the pairing or bonding procedure is successfully
 * completed.
 */

/** @brief Identifier of the bonded event */
#define sl_bt_evt_sm_bonded_id                                       0x030f00a0

/***************************************************************************//**
 * @brief Data structure of the bonded event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_bonded_s
{
  uint8_t connection;    /**< Connection handle */
  uint8_t bonding;       /**< Bonding handle. Values:
                                - <b>SL_BT_INVALID_BONDING_HANDLE (0xff):</b>
                                  Pairing completed without bonding - the
                                  pairing key will be discarded after
                                  disconnection.
                                - <b>Other:</b> Procedure completed, pairing key
                                  stored with given bonding handle */
  uint8_t security_mode; /**< Enum @ref sl_bt_connection_security_t. Connection
                              security mode. Values:
                                - <b>sl_bt_connection_mode1_level2 (0x1):</b>
                                  Unauthenticated pairing with encryption
                                - <b>sl_bt_connection_mode1_level3 (0x2):</b>
                                  Authenticated pairing with encryption
                                - <b>sl_bt_connection_mode1_level4 (0x3):</b>
                                  Authenticated Secure Connections pairing with
                                  encryption using a 128-bit strength encryption
                                  key */
});

typedef struct sl_bt_evt_sm_bonded_s sl_bt_evt_sm_bonded_t;

/** @} */ // end addtogroup sl_bt_evt_sm_bonded

/**
 * @addtogroup sl_bt_evt_sm_bonding_failed sl_bt_evt_sm_bonding_failed
 * @{
 * @brief This event is triggered if the pairing or bonding procedure fails.
 */

/** @brief Identifier of the bonding_failed event */
#define sl_bt_evt_sm_bonding_failed_id                               0x040f00a0

/***************************************************************************//**
 * @brief Data structure of the bonding_failed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_bonding_failed_s
{
  uint8_t  connection; /**< Connection handle */
  uint16_t reason;     /**< An sl_status_t code describing the error that
                            occurred */
});

typedef struct sl_bt_evt_sm_bonding_failed_s sl_bt_evt_sm_bonding_failed_t;

/** @} */ // end addtogroup sl_bt_evt_sm_bonding_failed

/**
 * @addtogroup sl_bt_evt_sm_confirm_bonding sl_bt_evt_sm_confirm_bonding
 * @{
 * @brief Indicates a user request to display that the new bonding request is
 * received and for the user to confirm the request
 *
 * Use the command @ref sl_bt_sm_bonding_confirm to accept or reject the bonding
 * request.
 */

/** @brief Identifier of the confirm_bonding event */
#define sl_bt_evt_sm_confirm_bonding_id                              0x090f00a0

/***************************************************************************//**
 * @brief Data structure of the confirm_bonding event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_sm_confirm_bonding_s
{
  uint8_t connection;     /**< Connection handle */
  uint8_t bonding_handle; /**< Bonding handle for the request.
                                 - <b>Range:</b> 0 to 31

                                 - 0xFF: SL_BT_INVALID_BONDING_HANDLE
                                 - NOTE! When the bonding handle is anything
                                   other than SL_BT_INVALID_BONDING_HANDLE
                                   (0xff), a bonding already exists for this
                                   connection. Overwriting the existing bonding
                                   is a potential security risk. */
});

typedef struct sl_bt_evt_sm_confirm_bonding_s sl_bt_evt_sm_confirm_bonding_t;

/** @} */ // end addtogroup sl_bt_evt_sm_confirm_bonding

/***************************************************************************//**
 *
 * Configure security requirements and I/O capabilities of the system.
 *
 * @param[in] flags @parblock
 *   Security requirement flags. This value can be a bitmask of multiple flags
 *   from @ref sl_bt_sm_configuration
 *
 *   Bit 0:
 *     - <b>0:</b> Allow bonding without authentication
 *     - <b>1:</b> Bonding requires authentication (Man-in-the-Middle
 *       protection)
 *
 *   Bit 1:
 *     - <b>0:</b> Allow encryption without bonding
 *     - <b>1:</b> Encryption requires bonding. Note that this setting will also
 *       enable bonding.
 *
 *   Bit 2:
 *     - <b>0:</b> Allow bonding with legacy pairing
 *     - <b>1:</b> Secure connections only
 *
 *   Bit 3:
 *     - <b>0:</b> Bonding request does not need to be confirmed
 *     - <b>1:</b> Bonding requests need to be confirmed. Received bonding
 *       requests are notified by @ref sl_bt_evt_sm_confirm_bonding
 *
 *   Bit 4: This option is ignored when the application includes the
 *   bluetooth_feature_external_bonding_database feature.
 *     - <b>0:</b> Allow all connections
 *     - <b>1:</b> Allow connections only from bonded devices
 *
 *   Bit 5:
 *     - <b>0:</b> Prefer just works pairing when both options are possible
 *       based on the settings.
 *     - <b>1:</b> Prefer authenticated pairing when both options are possible
 *       based on the settings.
 *
 *   Bit 6:
 *     - <b>0:</b> Allow secure connections OOB pairing with OOB data from only
 *       one device.
 *     - <b>1:</b> Require secure connections OOB data from both devices.
 *
 *   Bit 7:
 *     - <b>0:</b> Allow debug keys from remote device.
 *     - <b>1:</b> Reject pairing if remote device uses debug keys.
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] io_capabilities Enum @ref sl_bt_sm_io_capability_t. I/O
 *   Capabilities. The default I/O Capability used by the stack is No Input and
 *   No Output. Values:
 *     - <b>sl_bt_sm_io_capability_displayonly (0x0):</b> Display Only
 *     - <b>sl_bt_sm_io_capability_displayyesno (0x1):</b> Display with
 *       Yes/No-buttons
 *     - <b>sl_bt_sm_io_capability_keyboardonly (0x2):</b> Keyboard Only
 *     - <b>sl_bt_sm_io_capability_noinputnooutput (0x3):</b> No Input and No
 *       Output
 *     - <b>sl_bt_sm_io_capability_keyboarddisplay (0x4):</b> Display with
 *       Keyboard
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_configure(uint8_t flags, uint8_t io_capabilities);

/***************************************************************************//**
 *
 * Set the minimum allowed key size used for bonding. The default value is 16
 * bytes.
 *
 * @param[in] minimum_key_size Minimum allowed key size for bonding.
 *     - <b>Range:</b> 7 to 16
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_minimum_key_size(uint8_t minimum_key_size);

/***************************************************************************//**
 *
 * Set Security Manager in debug mode. In this mode, the secure connections
 * bonding uses known debug keys, so that the encrypted packet can be opened by
 * Bluetooth protocol analyzer. To disable the debug mode, restart the device.
 *
 * Bondings made in debug mode are unsecure.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_debug_mode();

/***************************************************************************//**
 *
 * Set the maximum allowed bonding count and bonding policy. The maximum number
 * of bondings that can be supported depends on how much user data is stored in
 * the NVM3 and the NVM3 size. When bond policy value 1 or 2 is selected, the
 * stack will automatically write the new bond, as per the policy, only if the
 * maximum allowed bonding count has been reached. If the stack can't write a
 * new bond for any other reason (e.g., NVM3 is full), an error will be thrown
 * through the bonding_failed event indicating why the bonding was not written.
 * The application has to manually release space from the NVM (e.g., by deleting
 * one of the existing bonds or application data) so that a new bond can be
 * saved. The default value is 13.
 *
 * @param[in] max_bonding_count Maximum allowed bonding count.
 *     - <b>Range:</b> 1 to 32
 * @param[in] policy_flags @parblock
 *   Bonding policy. Values:
 *     - <b>0:</b> If database is full, new bonding attempts will fail
 *     - <b>1:</b> New bonding will overwrite the oldest existing bonding
 *     - <b>2:</b> New bonding will overwrite the bonding that was used the
 *       longest time ago
 *
 *   Default: 0
 *
 *   With external bonding database the parameter is ignored.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_store_bonding_configuration(uint8_t max_bonding_count,
                                                 uint8_t policy_flags);

/***************************************************************************//**
 *
 * Set whether the device should accept new bondings. By default, the device
 * does not accept new bondings.
 *
 * @param[in] bondable @parblock
 *   Bondable mode. Values:
 *     - <b>0:</b> New bondings not accepted
 *     - <b>1:</b> Bondings allowed
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_bondable_mode(uint8_t bondable);

/***************************************************************************//**
 *
 * Enter a fixed passkey, which will be used in the @ref
 * sl_bt_evt_sm_passkey_display event.
 *
 * @param[in] passkey @parblock
 *   Passkey.
 *     - <b>Range:</b> 0 to 999999
 *
 *   . Set -1 to disable and start using random passkeys.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_passkey(int32_t passkey);

/***************************************************************************//**
 *
 * Enhance the security of a connection to current security requirements. On an
 * unencrypted connection, it will encrypt the connection and will also perform
 * bonding if requested by both devices. On an encrypted connection, it will
 * cause the connection to be re-encrypted.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_parameters - Triggered after increasing
 *     security has been completed successfully and indicates the latest
 *     security mode of the connection.
 *   - @ref sl_bt_evt_sm_bonded - Triggered if pairing or bonding was performed
 *     in this operation and the result is successful.
 *   - @ref sl_bt_evt_sm_bonding_failed - Triggered if pairing or bonding was
 *     performed in this operation and the result has failed.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_increase_security(uint8_t connection);

/***************************************************************************//**
 *
 * Enter a passkey after receiving a passkey request event.
 *
 * @param[in] connection Connection handle
 * @param[in] passkey @parblock
 *   Passkey.
 *     - <b>Range:</b> 0 to 999999
 *
 *   Set -1 to cancel pairing.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_enter_passkey(uint8_t connection, int32_t passkey);

/***************************************************************************//**
 *
 * Accept or reject the reported passkey confirm value.
 *
 * @param[in] connection Connection handle
 * @param[in] confirm Acceptance. Values:
 *     - <b>0:</b> Reject
 *     - <b>1:</b> Accept confirm value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_passkey_confirm(uint8_t connection, uint8_t confirm);

/***************************************************************************//**
 *
 * Accept or reject the bonding request.
 *
 * NOTE! When the bonding handle for the connection is anything other than
 * SL_BT_INVALID_BONDING_HANDLE (0xff), a bonding already exists for this
 * connection. Overwriting the existing bonding is a potential security risk.
 * The bonding handle is included in the @ref sl_bt_evt_sm_confirm_bonding
 * event.
 *
 * @param[in] connection Connection handle
 * @param[in] confirm Acceptance. Values:
 *     - <b>0:</b> Reject
 *     - <b>1:</b> Accept bonding request
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_bonding_confirm(uint8_t connection, uint8_t confirm);

/***************************************************************************//**
 *
 * Delete the specified bonding. The connection will be closed if the remote
 * device is connected currently.
 *
 * This commands deletes the information from the persistent bonding database
 * when the built-in bonding database
 * (bluetooth_feature_builtin_bonding_database) is used.
 *
 * When used with Filter Accept List (bluetooth_feature_accept_list) or
 * Resolving List (bluetooth_feature_resolving_list), this command removes the
 * deleted device from the lists.
 *
 * This command is unavailable if the external bonding database
 * (bluetooth_feature_external_bonding_database) is used.
 *
 * @param[in] bonding Bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_delete_bonding(uint8_t bonding);

/***************************************************************************//**
 *
 * Delete all bondings and device local identity resolving key (IRK). All
 * connections to affected devices are closed as well.
 *
 * This command empties the persistent bonding database when the built-in
 * bonding database (bluetooth_feature_builtin_bonding_database) is used.
 *
 * When used with Filter Accept List (bluetooth_feature_accept_list) or
 * Resolving List (bluetooth_feature_resolving_list), this command removes the
 * deleted devices from the lists.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_delete_bondings();

/***************************************************************************//**
 *
 * Get number of entries and bitmask of their handles saved in the bonding
 * database.
 *
 * To get the bonding information and peer device address of a bonding, use the
 * @ref sl_bt_sm_get_bonding_details command. The bonding handle can be
 * calculated from the handle bitmask returned by this command, or
 * alternatively, repeat calling the @ref sl_bt_sm_get_bonding_details command
 * to get the detailed information of all bondings.
 *
 * This command is unavailable if the external bonding database
 * (bluetooth_feature_external_bonding_database) is used.
 *
 * @param[in] reserved Use the value 0 on this reserved field. Do not use
 *   none-zero values because they are reserved for future use.
 * @param[out] num_bondings Total number of bondings stored in bonding database.
 * @param[in] max_bondings_size Size of output buffer passed in @p bondings
 * @param[out] bondings_len On return, set to the length of output data written
 *   to @p bondings
 * @param[out] bondings 4 byte bit field of used bonding handles in little
 *   endian format. Bit 0 of first byte is bonding handle 0, bit 0 of second
 *   byte is bonding handle 8 etc. If the bit is 1 that bonding handle exists in
 *   the bonding database.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_get_bonding_handles(uint32_t reserved,
                                         uint32_t *num_bondings,
                                         size_t max_bondings_size,
                                         size_t *bondings_len,
                                         uint8_t *bondings);

/***************************************************************************//**
 *
 * Get the detailed information for a bonding entry. Data includes remote device
 * address and address type as well as security mode for bonding and a used
 * encryption key length.
 *
 * To get the detailed information of all bondings, repeat calling this command
 * starting from 0 as the bonding handle value until the maximum number of
 * configured bondings are reached. Use 32 as the maximum number if the
 * configured number is unknown.
 *
 * This command is unavailable if the external bonding database
 * (bluetooth_feature_external_bonding_database) is used.
 *
 * @param[in] bonding Bonding handle
 * @param[out] address Bluetooth address of the remote device
 * @param[out] address_type Enum @ref sl_bt_gap_address_type_t. Address type.
 *   Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 * @param[out] security_mode Enum @ref sl_bt_connection_security_t. Connection
 *   security mode. Values:
 *     - <b>sl_bt_connection_mode1_level1 (0x0):</b> No security
 *     - <b>sl_bt_connection_mode1_level2 (0x1):</b> Unauthenticated pairing
 *       with encryption
 *     - <b>sl_bt_connection_mode1_level3 (0x2):</b> Authenticated pairing with
 *       encryption
 *     - <b>sl_bt_connection_mode1_level4 (0x3):</b> Authenticated Secure
 *       Connections pairing with encryption using a 128-bit strength encryption
 *       key
 * @param[out] key_size Key length in bytes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_get_bonding_details(uint32_t bonding,
                                         bd_addr *address,
                                         uint8_t *address_type,
                                         uint8_t *security_mode,
                                         uint8_t *key_size);

/***************************************************************************//**
 *
 * Find the bonding entry by using a Bluetooth device address.
 *
 * This command is unavailable if the external bonding database
 * (bluetooth_feature_external_bonding_database) is used.
 *
 * @param[in] address The Bluetooth device address
 * @param[out] bonding The bonding handle
 * @param[out] security_mode Enum @ref sl_bt_connection_security_t. Connection
 *   security mode. Values:
 *     - <b>sl_bt_connection_mode1_level1 (0x0):</b> No security
 *     - <b>sl_bt_connection_mode1_level2 (0x1):</b> Unauthenticated pairing
 *       with encryption
 *     - <b>sl_bt_connection_mode1_level3 (0x2):</b> Authenticated pairing with
 *       encryption
 *     - <b>sl_bt_connection_mode1_level4 (0x3):</b> Authenticated Secure
 *       Connections pairing with encryption using a 128-bit strength encryption
 *       key
 * @param[out] key_size Key length in bytes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_find_bonding_by_address(bd_addr address,
                                             uint32_t *bonding,
                                             uint8_t *security_mode,
                                             uint8_t *key_size);

/***************************************************************************//**
 *
 * Find the identity address of bonded device by using resolvable private
 * address (RPA).
 *
 * The error SL_STATUS_NOT_FOUND is returned if the RPA cannot be resolved.
 *
 * This command is unavailable if the external bonding database
 * (bluetooth_feature_external_bonding_database) is used.
 *
 * @param[in] rpa Resolvable private address to be resolved
 * @param[out] address The identity adderss of the bonded device
 * @param[out] address_type Enum @ref sl_bt_gap_address_type_t. Identity address
 *   type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 * @param[out] bonding The bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_resolve_rpa(bd_addr rpa,
                                 bd_addr *address,
                                 uint8_t *address_type,
                                 uint32_t *bonding);

/***************************************************************************//**
 * @cond RESTRICTED
 *
 * Restricted/experimental API. Contact Silicon Labs sales for more information.
 *
 * <b>Deprecated</b> . Use feature "External Bluetooth bonding database"
 * (bluetooth_feature_external_bonding_database) if the user case requires
 * overwriting bonding keys outside of the bonding process.
 *
 * Set the key for a bonding. This overwrites the value if the key exists
 * already. If there is error which leaves the bonding entry in inconsistent
 * state the bonding will be deleted.
 *
 * @param[in] bonding Bonding handle
 * @param[in] key_type Enum @ref sl_bt_sm_bonding_key_t. Key type. Values:
 *     - <b>sl_bt_sm_bonding_key_remote_ltk (0x1):</b> LTK used as central
 *       device, always used when paired using secure connections and local LTK
 *       does not exist
 *     - <b>sl_bt_sm_bonding_key_local_ltk (0x2):</b> LTK used as peripheral
 *       device when using legacy pairing, only used with secure connections
 *       pairing if key exists
 *     - <b>sl_bt_sm_bonding_key_irk (0x3):</b> Identity resolving key for
 *       resolvable private addresses
 * @param[in] key Bonding key in little endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @endcond
 ******************************************************************************/
SL_BGAPI_DEPRECATED sl_status_t sl_bt_sm_set_bonding_key(uint32_t bonding,
                                     uint8_t key_type,
                                     aes_key_128 key);

/***************************************************************************//**
 *
 * Set Out-Of-Band (OOB) encryption data for a legacy pairing of a device. OOB
 * data may be, for example, a PIN code exchanged over an alternate path, such
 * as NFC. The device will not allow any other bonding if OOB data is set. OOB
 * data can't be set simultaneously with secure connections OOB data.
 *
 * @param[in] enable Enable OOB with legacy pairing. Values:
 *     - <b>0:</b> disable
 *     - <b>1:</b> enable
 * @param[in] oob_data 16-byte legacy pairing OOB data in little endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_legacy_oob(uint8_t enable, aes_key_128 oob_data);

/***************************************************************************//**
 *
 * Enable the use of Out-Of-Band (OOB) encryption data for a device for secure
 * connections pairing. Enabling will generate new OOB data and confirm values,
 * which can be sent to the remote device. After enabling the secure connections
 * OOB data, the remote devices OOB data can be set with @ref
 * sl_bt_sm_set_remote_oob. Calling this function will erase any set remote
 * device OOB data and confirm values. The device will not allow any other
 * bonding if OOB data is set. The secure connections OOB data cannot be enabled
 * simultaneously with legacy pairing OOB data.
 *
 * @param[in] enable Enable OOB with secure connections pairing. Values:
 *     - <b>0:</b> disable
 *     - <b>1:</b> enable
 * @param[out] random 16-byte randomly-generated secure connections OOB data in
 *   little endian format.
 * @param[out] confirm 16-byte confirm value for the OOB random value in little
 *   endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_oob(uint8_t enable,
                             aes_key_128 *random,
                             aes_key_128 *confirm);

/***************************************************************************//**
 *
 * Set Out-Of-Band (OOB) data and confirm values received from the remote device
 * for secure connections pairing. OOB data must be enabled with @ref
 * sl_bt_sm_set_oob before setting the remote device OOB data.
 *
 * @param[in] enable Enable remote device OOB data with secure connections
 *   pairing. Values:
 *     - <b>0:</b> disable
 *     - <b>1:</b> enable
 * @param[in] random 16-byte remote device secure connections OOB data in little
 *   endian format.
 * @param[in] confirm 16-byte remote device confirm value for the OOB random
 *   value in little endian format.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_sm_set_remote_oob(uint8_t enable,
                                    aes_key_128 random,
                                    aes_key_128 confirm);

/** @} */ // end addtogroup sl_bt_sm

/**
 * @addtogroup sl_bt_external_bondingdb External Bonding Database
 * @{
 *
 * @brief External Bonding Database
 *
 * Receive and respond to requests related to managing an external bonding
 * database. When the application uses the
 * bluetooth_feature_external_bonding_database feature, the Bluetooth stack and
 * the application use the commands and events in this class to store and
 * retrieve data from the persistent bonding storage.
 *
 * When this feature is used, the built-in bonding database
 * (bluetooth_feature_builtin_bonding_database) and the address resolving in the
 * Bluetooth stack are excluded. The user application and external bonding
 * database are responsible for the following functionalities:
 *   - The external bonding database must be able to reliably and permanently
 *     store the bonding data and type tuples that are provided by the stack in
 *     the @ref sl_bt_evt_external_bondingdb_data_request event. Data types @ref
 *     sl_bt_external_bondingdb_data_remote_address and @ref
 *     sl_bt_external_bondingdb_data_remote_address_type can be used by the
 *     application to identify the device, but they are not used by the stack
 *     and hence the stack will not ask for their values. Other data types in
 *     @ref sl_bt_external_bondingdb_data_t are needed by the Bluetooth Security
 *     Manager or GATT Server. The Long Term Keys (LTK) and Identity Resolving
 *     Keys (IRK) must be stored securely. When the database is on a separate
 *     host device, they must be securely transmitted between the host and
 *     target over the NCP transport. The application also needs to manage the
 *     bondings in the storage, e.g., delete a bonding when requested by the
 *     business logic.
 *   - Manage a local RAM-based bookkeeping to keep track of open connections.
 *     At a minimum, the remote Bluetooth address address and address type must
 *     be stored in the bookkeeping when a @ref sl_bt_evt_connection_opened
 *     event is received. If the remote Bluetooth device address is a resolvable
 *     private random (RPA) address, the application may perform address
 *     resolving using the IRK keys of the remote devices stored in the bonding
 *     database. Address resolving is required for identifying a connected
 *     device using an RPA address. When the scanner or periodic advertising
 *     synchronization feature is used, RPA addresses in advertisement report or
 *     sync events may need to be resolved if required by the business
 *     requirements.
 *   - When the Bluetooth stack requires bonding data, it will send an @ref
 *     sl_bt_evt_external_bondingdb_data_request event. The application must
 *     respond to the request by sending the data value using the @ref
 *     sl_bt_external_bondingdb_set_data command. The GATT server will pend the
 *     requests from the remote GATT client and encryption or bonding is not
 *     possible until all requested bonding data has been sent to the stack. The
 *     stack will send @ref sl_bt_evt_external_bondingdb_data_ready when it has
 *     received all the necessary bonding data from the application.
 *
 * In this configuration, the value of @p bonding parameter in @ref
 * sl_bt_evt_advertiser_scan_request, @ref sl_bt_evt_connection_opened, @ref
 * sl_bt_evt_sm_bonded, @ref sl_bt_evt_scanner_legacy_advertisement_report, @ref
 * sl_bt_evt_scanner_extended_advertisement_report, @ref
 * sl_bt_evt_periodic_sync_opened, @ref sl_bt_evt_pawr_sync_opened, @ref
 * sl_bt_evt_periodic_sync_transfer_received, and @ref
 * sl_bt_evt_pawr_sync_transfer_received is always SL_BT_INVALID_BONDING_HANDLE
 * (0xff). Resolving the bonding handle is the responsibility of the external
 * bonding database. Additionally, the following BGAPI commands are not
 * available and will return the SL_STATUS_NOT_AVAILABLE error:
 *   - @ref sl_bt_sm_delete_bonding
 *   - @ref sl_bt_sm_get_bonding_handles
 *   - @ref sl_bt_sm_get_bonding_details
 *   - @ref sl_bt_sm_find_bonding_by_address
 *   - @ref sl_bt_sm_resolve_rpa
 */

/* Command and Response IDs */
#define sl_bt_cmd_external_bondingdb_set_data_id                     0x005c0020
#define sl_bt_rsp_external_bondingdb_set_data_id                     0x005c0020

/**
 * @brief These values define the bonding data types, which are stored in the
 * persistent store.
 */
typedef enum
{
  sl_bt_external_bondingdb_data_remote_address       = 0x0, /**< (0x0) Identity
                                                                 address of the
                                                                 remote device,
                                                                 which a
                                                                 resolvable
                                                                 random address
                                                                 can be resolved
                                                                 to */
  sl_bt_external_bondingdb_data_remote_address_type  = 0x1, /**< (0x1) Type of
                                                                 the remote
                                                                 device identity
                                                                 address. Value
                                                                 0 is public
                                                                 device address
                                                                 and 1 is static
                                                                 device address */
  sl_bt_external_bondingdb_data_remote_ltk           = 0x2, /**< (0x2) Long Term
                                                                 Key (LTK) used
                                                                 as central
                                                                 device. The
                                                                 value of this
                                                                 type must be
                                                                 stored
                                                                 securely. */
  sl_bt_external_bondingdb_data_local_ltk            = 0x3, /**< (0x3) Long Term
                                                                 Key (LTK) used
                                                                 as peripheral
                                                                 device. The
                                                                 value of this
                                                                 type must be
                                                                 stored
                                                                 securely. */
  sl_bt_external_bondingdb_data_remote_central_inf   = 0x4, /**< (0x4)
                                                                 Identification
                                                                 info used as a
                                                                 central device */
  sl_bt_external_bondingdb_data_local_central_inf    = 0x5, /**< (0x5)
                                                                 Identification
                                                                 info used as a
                                                                 central device */
  sl_bt_external_bondingdb_data_irk                  = 0x6, /**< (0x6) Identity
                                                                 Resolving Key
                                                                 (IRK) of the
                                                                 remote device
                                                                 that is used
                                                                 for resolving
                                                                 its RPA
                                                                 addresses when
                                                                 the device is
                                                                 in privacy
                                                                 mode. The value
                                                                 of this type
                                                                 must be stored
                                                                 securely. */
  sl_bt_external_bondingdb_data_meta                 = 0x7, /**< (0x7) Metadata
                                                                 about the
                                                                 bonding */
  sl_bt_external_bondingdb_data_gatt_client_config   = 0x8, /**< (0x8) The
                                                                 remote GATT
                                                                 client
                                                                 characteristic
                                                                 configurations */
  sl_bt_external_bondingdb_data_gatt_client_features = 0x9, /**< (0x9) The
                                                                 remote GATT
                                                                 client
                                                                 supported
                                                                 features */
  sl_bt_external_bondingdb_data_gatt_db_hash         = 0xa  /**< (0xa) Latest
                                                                 local GATT
                                                                 database hash
                                                                 value that the
                                                                 remote client
                                                                 has seen for
                                                                 GATT robust
                                                                 caching feature */
} sl_bt_external_bondingdb_data_t;

/**
 * @addtogroup sl_bt_evt_external_bondingdb_data_request sl_bt_evt_external_bondingdb_data_request
 * @{
 * @brief Indicates that the Bluetooth stack requests the bonding data of a
 * connection from the external bonding database
 *
 * The application must respond by setting bonding data using the @ref
 * sl_bt_external_bondingdb_set_data command.
 *
 * The Bluetooth stack will send this event one or more times after a connection
 * is open following the @ref sl_bt_evt_connection_opened event to get ready for
 * security related operations.
 */

/** @brief Identifier of the data_request event */
#define sl_bt_evt_external_bondingdb_data_request_id                 0x005c00a0

/***************************************************************************//**
 * @brief Data structure of the data_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_external_bondingdb_data_request_s
{
  uint8_t connection; /**< Connection handle */
  uint8_t type;       /**< Enum @ref sl_bt_external_bondingdb_data_t. The
                           requested bonding data type. Values:
                             - <b>sl_bt_external_bondingdb_data_remote_address
                               (0x0):</b> Identity address of the remote device,
                               which a resolvable random address can be resolved
                               to
                             - <b>sl_bt_external_bondingdb_data_remote_address_type
                               (0x1):</b> Type of the remote device identity
                               address. Value 0 is public device address and 1
                               is static device address
                             - <b>sl_bt_external_bondingdb_data_remote_ltk
                               (0x2):</b> Long Term Key (LTK) used as central
                               device. The value of this type must be stored
                               securely.
                             - <b>sl_bt_external_bondingdb_data_local_ltk
                               (0x3):</b> Long Term Key (LTK) used as peripheral
                               device. The value of this type must be stored
                               securely.
                             - <b>sl_bt_external_bondingdb_data_remote_central_inf
                               (0x4):</b> Identification info used as a central
                               device
                             - <b>sl_bt_external_bondingdb_data_local_central_inf
                               (0x5):</b> Identification info used as a central
                               device
                             - <b>sl_bt_external_bondingdb_data_irk (0x6):</b>
                               Identity Resolving Key (IRK) of the remote device
                               that is used for resolving its RPA addresses when
                               the device is in privacy mode. The value of this
                               type must be stored securely.
                             - <b>sl_bt_external_bondingdb_data_meta (0x7):</b>
                               Metadata about the bonding
                             - <b>sl_bt_external_bondingdb_data_gatt_client_config
                               (0x8):</b> The remote GATT client characteristic
                               configurations
                             - <b>sl_bt_external_bondingdb_data_gatt_client_features
                               (0x9):</b> The remote GATT client supported
                               features
                             - <b>sl_bt_external_bondingdb_data_gatt_db_hash
                               (0xa):</b> Latest local GATT database hash value
                               that the remote client has seen for GATT robust
                               caching feature */
});

typedef struct sl_bt_evt_external_bondingdb_data_request_s sl_bt_evt_external_bondingdb_data_request_t;

/** @} */ // end addtogroup sl_bt_evt_external_bondingdb_data_request

/**
 * @addtogroup sl_bt_evt_external_bondingdb_data sl_bt_evt_external_bondingdb_data
 * @{
 * @brief Indicates that updated bonding data of a connection is available
 *
 * The application must store it in the external bonding database permanently.
 */

/** @brief Identifier of the data event */
#define sl_bt_evt_external_bondingdb_data_id                         0x015c00a0

/***************************************************************************//**
 * @brief Data structure of the data event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_external_bondingdb_data_s
{
  uint8_t    connection; /**< Connection handle */
  uint8_t    type;       /**< Enum @ref sl_bt_external_bondingdb_data_t. The
                              bonding data type. Values:
                                - <b>sl_bt_external_bondingdb_data_remote_address
                                  (0x0):</b> Identity address of the remote
                                  device, which a resolvable random address can
                                  be resolved to
                                - <b>sl_bt_external_bondingdb_data_remote_address_type
                                  (0x1):</b> Type of the remote device identity
                                  address. Value 0 is public device address and
                                  1 is static device address
                                - <b>sl_bt_external_bondingdb_data_remote_ltk
                                  (0x2):</b> Long Term Key (LTK) used as central
                                  device. The value of this type must be stored
                                  securely.
                                - <b>sl_bt_external_bondingdb_data_local_ltk
                                  (0x3):</b> Long Term Key (LTK) used as
                                  peripheral device. The value of this type must
                                  be stored securely.
                                - <b>sl_bt_external_bondingdb_data_remote_central_inf
                                  (0x4):</b> Identification info used as a
                                  central device
                                - <b>sl_bt_external_bondingdb_data_local_central_inf
                                  (0x5):</b> Identification info used as a
                                  central device
                                - <b>sl_bt_external_bondingdb_data_irk
                                  (0x6):</b> Identity Resolving Key (IRK) of the
                                  remote device that is used for resolving its
                                  RPA addresses when the device is in privacy
                                  mode. The value of this type must be stored
                                  securely.
                                - <b>sl_bt_external_bondingdb_data_meta
                                  (0x7):</b> Metadata about the bonding
                                - <b>sl_bt_external_bondingdb_data_gatt_client_config
                                  (0x8):</b> The remote GATT client
                                  characteristic configurations
                                - <b>sl_bt_external_bondingdb_data_gatt_client_features
                                  (0x9):</b> The remote GATT client supported
                                  features
                                - <b>sl_bt_external_bondingdb_data_gatt_db_hash
                                  (0xa):</b> Latest local GATT database hash
                                  value that the remote client has seen for GATT
                                  robust caching feature */
  uint8array data;       /**< Bonding data */
});

typedef struct sl_bt_evt_external_bondingdb_data_s sl_bt_evt_external_bondingdb_data_t;

/** @} */ // end addtogroup sl_bt_evt_external_bondingdb_data

/**
 * @addtogroup sl_bt_evt_external_bondingdb_data_ready sl_bt_evt_external_bondingdb_data_ready
 * @{
 * @brief Indicates that stack has received all the necessary bonding data from
 * the application and is now fully functional
 */

/** @brief Identifier of the data_ready event */
#define sl_bt_evt_external_bondingdb_data_ready_id                   0x025c00a0

/***************************************************************************//**
 * @brief Data structure of the data_ready event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_external_bondingdb_data_ready_s
{
  uint8_t connection; /**< Connection handle */
});

typedef struct sl_bt_evt_external_bondingdb_data_ready_s sl_bt_evt_external_bondingdb_data_ready_t;

/** @} */ // end addtogroup sl_bt_evt_external_bondingdb_data_ready

/***************************************************************************//**
 *
 * Set bonding data of a connection from the external bonding database. When the
 * Bluetooth stack needs bonding data, it will send the request to user
 * application with a @ref sl_bt_evt_external_bondingdb_data_request event that
 * contains the requested data type. The application must respond to the request
 * by sending data using this command.
 *
 * Send 0 length data to the stack with this command if the requested data is
 * not available in the external bonding database.
 *
 * @param[in] connection Connection handle
 * @param[in] type Enum @ref sl_bt_external_bondingdb_data_t. Bonding data type.
 *   Values:
 *     - <b>sl_bt_external_bondingdb_data_remote_address (0x0):</b> Identity
 *       address of the remote device, which a resolvable random address can be
 *       resolved to
 *     - <b>sl_bt_external_bondingdb_data_remote_address_type (0x1):</b> Type of
 *       the remote device identity address. Value 0 is public device address
 *       and 1 is static device address
 *     - <b>sl_bt_external_bondingdb_data_remote_ltk (0x2):</b> Long Term Key
 *       (LTK) used as central device. The value of this type must be stored
 *       securely.
 *     - <b>sl_bt_external_bondingdb_data_local_ltk (0x3):</b> Long Term Key
 *       (LTK) used as peripheral device. The value of this type must be stored
 *       securely.
 *     - <b>sl_bt_external_bondingdb_data_remote_central_inf (0x4):</b>
 *       Identification info used as a central device
 *     - <b>sl_bt_external_bondingdb_data_local_central_inf (0x5):</b>
 *       Identification info used as a central device
 *     - <b>sl_bt_external_bondingdb_data_irk (0x6):</b> Identity Resolving Key
 *       (IRK) of the remote device that is used for resolving its RPA addresses
 *       when the device is in privacy mode. The value of this type must be
 *       stored securely.
 *     - <b>sl_bt_external_bondingdb_data_meta (0x7):</b> Metadata about the
 *       bonding
 *     - <b>sl_bt_external_bondingdb_data_gatt_client_config (0x8):</b> The
 *       remote GATT client characteristic configurations
 *     - <b>sl_bt_external_bondingdb_data_gatt_client_features (0x9):</b> The
 *       remote GATT client supported features
 *     - <b>sl_bt_external_bondingdb_data_gatt_db_hash (0xa):</b> Latest local
 *       GATT database hash value that the remote client has seen for GATT
 *       robust caching feature
 * @param[in] data_len Length of data in @p data
 * @param[in] data Bonding data
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_external_bondingdb_set_data(uint8_t connection,
                                              uint8_t type,
                                              size_t data_len,
                                              const uint8_t* data);

/** @} */ // end addtogroup sl_bt_external_bondingdb

/**
 * @addtogroup sl_bt_resolving_list Address Resolving List
 * @{
 *
 * @brief Address Resolving List
 *
 * Provides support for adding and removing devices from the Resolving List in
 * controller-based privacy.
 *
 * Adding a peer device to the Resolving List allows the Bluetooth controller to
 * resolve the address when the peer device is using privacy and is transmitting
 * with a Resolvable Private Address (RPA). When the controller has resolved an
 * address in a received Bluetooth packet such as an advertisement, the
 * corresponding event will report the peer device's identity address even if a
 * Resolvable Private Address was used over the air.
 *
 * When the application has added a peer device to the Resolving List, the
 * application may use the peer device's identity address in commands such as
 * @ref sl_bt_connection_open or @ref sl_bt_sync_scanner_open even if the peer
 * device is using privacy and is using a Resolvable Private Address over the
 * air.
 */

/* Command and Response IDs */
#define sl_bt_cmd_resolving_list_add_device_by_bonding_id            0x005d0020
#define sl_bt_cmd_resolving_list_add_device_by_address_id            0x015d0020
#define sl_bt_cmd_resolving_list_remove_device_by_bonding_id         0x025d0020
#define sl_bt_cmd_resolving_list_remove_device_by_address_id         0x035d0020
#define sl_bt_cmd_resolving_list_remove_all_devices_id               0x045d0020
#define sl_bt_rsp_resolving_list_add_device_by_bonding_id            0x005d0020
#define sl_bt_rsp_resolving_list_add_device_by_address_id            0x015d0020
#define sl_bt_rsp_resolving_list_remove_device_by_bonding_id         0x025d0020
#define sl_bt_rsp_resolving_list_remove_device_by_address_id         0x035d0020
#define sl_bt_rsp_resolving_list_remove_all_devices_id               0x045d0020

/**
 * @brief Specifies the Privacy Mode used for a peer device in the Resolving
 * List
 */
typedef enum
{
  sl_bt_resolving_list_privacy_mode_network = 0x0, /**< (0x0) Use Network
                                                        Privacy Mode for the
                                                        peer device */
  sl_bt_resolving_list_privacy_mode_device  = 0x1  /**< (0x1) Use Device Privacy
                                                        Mode for the peer device */
} sl_bt_resolving_list_privacy_mode_t;

/***************************************************************************//**
 *
 * Add a device to the Resolving List based on its bonding handle.
 *
 * This command is not available if the application uses the external bonding
 * database provided by the component
 * bluetooth_feature_external_bonding_database. In that configuration the
 * application can use the command @ref
 * sl_bt_resolving_list_add_device_by_address and provide the peer's identity
 * address and its Identity Resolving Key (IRK).
 *
 * @param[in] bonding The bonding handle
 * @param[in] privacy_mode @parblock
 *   Enum @ref sl_bt_resolving_list_privacy_mode_t. The Privacy Mode to use for
 *   the peer device. Values:
 *     - <b>sl_bt_resolving_list_privacy_mode_network (0x0):</b> Use Network
 *       Privacy Mode for the peer device
 *     - <b>sl_bt_resolving_list_privacy_mode_device (0x1):</b> Use Device
 *       Privacy Mode for the peer device
 *
 *   Default: @ref sl_bt_resolving_list_privacy_mode_network (Use Network
 *   Privacy Mode for the peer device)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resolving_list_add_device_by_bonding(uint32_t bonding,
                                                       uint8_t privacy_mode);

/***************************************************************************//**
 *
 * Add a device to the Resolving List based on its identity address and its
 * Identity Resolving Key (IRK).
 *
 * This command is typically only needed when the application uses the external
 * bonding database provided by the component
 * bluetooth_feature_external_bonding_database. When the application uses the
 * built-in bonding database, the command @ref
 * sl_bt_resolving_list_add_device_by_bonding is more convenient.
 *
 * @param[in] address Bluetooth address of the peer device
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. The peer device
 *   address type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 * @param[in] key Identity Resolving Key (IRK) of the peer device in little
 *   endian format.
 * @param[in] privacy_mode @parblock
 *   Enum @ref sl_bt_resolving_list_privacy_mode_t. The Privacy Mode to use for
 *   the peer device. Values:
 *     - <b>sl_bt_resolving_list_privacy_mode_network (0x0):</b> Use Network
 *       Privacy Mode for the peer device
 *     - <b>sl_bt_resolving_list_privacy_mode_device (0x1):</b> Use Device
 *       Privacy Mode for the peer device
 *
 *   Default: @ref sl_bt_resolving_list_privacy_mode_network (Use Network
 *   Privacy Mode for the peer device)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resolving_list_add_device_by_address(bd_addr address,
                                                       uint8_t address_type,
                                                       aes_key_128 key,
                                                       uint8_t privacy_mode);

/***************************************************************************//**
 *
 * Remove a device from the Resolving List based on its bonding handle.
 *
 * This command is not available if the application uses the external bonding
 * database provided by the component
 * bluetooth_feature_external_bonding_database. In that configuration the
 * application can use the command @ref
 * sl_bt_resolving_list_remove_device_by_address and provide the peer's identity
 * address.
 *
 * @param[in] bonding The bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resolving_list_remove_device_by_bonding(uint32_t bonding);

/***************************************************************************//**
 *
 * Remove a device from the Resolving List based on its identity address.
 *
 * @param[in] address Bluetooth address of the peer device
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. The peer device
 *   address type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resolving_list_remove_device_by_address(bd_addr address,
                                                          uint8_t address_type);

/***************************************************************************//**
 *
 * Remove all devices from the Resolving List.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_resolving_list_remove_all_devices();

/** @} */ // end addtogroup sl_bt_resolving_list

/**
 * @addtogroup sl_bt_accept_list Filter Accept List
 * @{
 *
 * @brief Filter Accept List
 *
 * Provides support for adding and removing devices from the Filter Accept List
 * in the Bluetooth controller.
 *
 * The Filter Accept List allows filtering transmissions from other Bluetooth
 * devices so that the local device only receives/accepts transmissions from the
 * devices that it's interested in or wants to communicate with. Operations that
 * support filtering provide a means to control how the Filter Accept List is
 * used:
 *   - For advertising, use the command @ref sl_bt_advertiser_configure and @p
 *     flags bits @ref SL_BT_ADVERTISER_USE_FILTER_FOR_SCAN_REQUESTS and @ref
 *     SL_BT_ADVERTISER_USE_FILTER_FOR_CONNECTION_REQUESTS to control the
 *     advertising filter policy
 *   - For scanning, use the command @ref
 *     sl_bt_scanner_set_parameters_and_filter to control the scanning filter
 *     policy
 */

/* Command and Response IDs */
#define sl_bt_cmd_accept_list_add_device_by_bonding_id               0x005e0020
#define sl_bt_cmd_accept_list_add_device_by_address_id               0x015e0020
#define sl_bt_cmd_accept_list_remove_device_by_bonding_id            0x025e0020
#define sl_bt_cmd_accept_list_remove_device_by_address_id            0x035e0020
#define sl_bt_cmd_accept_list_remove_all_devices_id                  0x045e0020
#define sl_bt_rsp_accept_list_add_device_by_bonding_id               0x005e0020
#define sl_bt_rsp_accept_list_add_device_by_address_id               0x015e0020
#define sl_bt_rsp_accept_list_remove_device_by_bonding_id            0x025e0020
#define sl_bt_rsp_accept_list_remove_device_by_address_id            0x035e0020
#define sl_bt_rsp_accept_list_remove_all_devices_id                  0x045e0020

/***************************************************************************//**
 *
 * Add a device to the Filter Accept List based on its bonding handle.
 *
 * This command returns the error SL_STATUS_BT_CTRL_CONTROLLER_BUSY if the
 * Filter Accept List is currently used by scanning filter policy, advertising
 * filter policy, or by initiator filter policy, i.e. initiating connection to
 * devices on the Filter Accept List. If the list is currently in use, the
 * application needs to first stop the operation that uses the Filter Accept
 * List, then add a device to the Filter Accept List, and then restart the
 * operation that uses the Filter Accept List.
 *
 * @param[in] bonding The bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_accept_list_add_device_by_bonding(uint32_t bonding);

/***************************************************************************//**
 *
 * Add a device to the Filter Accept List based on its identity address.
 *
 * Use the special address type @ref sl_bt_gap_anonymous_address to add an entry
 * that matches all advertisements sent with no address.
 *
 * This command returns the error SL_STATUS_BT_CTRL_CONTROLLER_BUSY if the
 * Filter Accept List is currently used by scanning filter policy, advertising
 * filter policy, or by initiator filter policy, i.e. initiating connection to
 * devices on the Filter Accept List. If the list is currently in use, the
 * application needs to first stop the operation that uses the Filter Accept
 * List, then add a device to the Filter Accept List, and then restart the
 * operation that uses the Filter Accept List.
 *
 * @param[in] address Bluetooth address of the peer device
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. The peer device
 *   address type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_anonymous_address (0xff):</b> Anonymous address. A Filter
 *       Accept List entry with this type matches all advertisements sent with
 *       no address.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_accept_list_add_device_by_address(bd_addr address,
                                                    uint8_t address_type);

/***************************************************************************//**
 *
 * Remove a device from the Filter Accept List based on its bonding handle.
 *
 * This command returns the error SL_STATUS_BT_CTRL_CONTROLLER_BUSY if the
 * Filter Accept List is currently used by scanning filter policy, advertising
 * filter policy, or by initiator filter policy, i.e. initiating connection to
 * devices on the Filter Accept List. If the list is currently in use, the
 * application needs to first stop the operation that uses the Filter Accept
 * List, then remove a device from the Filter Accept List, and then restart the
 * operation that uses the Filter Accept List.
 *
 * @param[in] bonding The bonding handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_accept_list_remove_device_by_bonding(uint32_t bonding);

/***************************************************************************//**
 *
 * Remove a device from the Filter Accept List based on its identity address.
 *
 * Use the special address type @ref sl_bt_gap_anonymous_address to remove an
 * entry that matches all advertisements sent with no address.
 *
 * This command returns the error SL_STATUS_BT_CTRL_CONTROLLER_BUSY if the
 * Filter Accept List is currently used by scanning filter policy, advertising
 * filter policy, or by initiator filter policy, i.e. initiating connection to
 * devices on the Filter Accept List. If the list is currently in use, the
 * application needs to first stop the operation that uses the Filter Accept
 * List, then remove a device from the Filter Accept List, and then restart the
 * operation that uses the Filter Accept List.
 *
 * @param[in] address Bluetooth address of the peer device
 * @param[in] address_type Enum @ref sl_bt_gap_address_type_t. The peer device
 *   address type. Values:
 *     - <b>sl_bt_gap_public_address (0x0):</b> Public device address
 *     - <b>sl_bt_gap_static_address (0x1):</b> Static device address
 *     - <b>sl_bt_gap_random_resolvable_address (0x2):</b> Resolvable private
 *       random address
 *     - <b>sl_bt_gap_random_nonresolvable_address (0x3):</b> Non-resolvable
 *       private random address
 *     - <b>sl_bt_gap_anonymous_address (0xff):</b> Anonymous address. A Filter
 *       Accept List entry with this type matches all advertisements sent with
 *       no address.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_accept_list_remove_device_by_address(bd_addr address,
                                                       uint8_t address_type);

/***************************************************************************//**
 *
 * Remove all devices from the Filter Accept List.
 *
 * This command returns the error SL_STATUS_BT_CTRL_CONTROLLER_BUSY if the
 * Filter Accept List is currently used by scanning filter policy, advertising
 * filter policy, or by initiator filter policy, i.e. initiating connection to
 * devices on the Filter Accept List. If the list is currently in use, the
 * application needs to first stop the operation that uses the Filter Accept
 * List, and then remove all devices from the Filter Accept List.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_accept_list_remove_all_devices();

/** @} */ // end addtogroup sl_bt_accept_list

/**
 * @addtogroup sl_bt_coex Coexistence
 * @{
 *
 * @brief Coexistence
 *
 * Coexistence BGAPI class. Coexistence interface is enabled and initialized
 * with sl_bt_init_coex_hal function.
 */

/* Command and Response IDs */
#define sl_bt_cmd_coex_set_options_id                                0x00200020
#define sl_bt_cmd_coex_set_parameters_id                             0x02200020
#define sl_bt_cmd_coex_set_directional_priority_pulse_id             0x03200020
#define sl_bt_cmd_coex_get_parameters_id                             0x04200020
#define sl_bt_cmd_coex_get_counters_id                               0x01200020
#define sl_bt_rsp_coex_set_options_id                                0x00200020
#define sl_bt_rsp_coex_set_parameters_id                             0x02200020
#define sl_bt_rsp_coex_set_directional_priority_pulse_id             0x03200020
#define sl_bt_rsp_coex_get_parameters_id                             0x04200020
#define sl_bt_rsp_coex_get_counters_id                               0x01200020

/**
 * @brief Coexistence configuration options
 */
typedef enum
{
  sl_bt_coex_option_enable        = 0x100, /**< (0x100) Enable coexistence
                                                feature */
  sl_bt_coex_option_tx_abort      = 0x400, /**< (0x400) Abort transmission if
                                                grant is denied */
  sl_bt_coex_option_high_priority = 0x800  /**< (0x800) Enable priority signal */
} sl_bt_coex_option_t;

/***************************************************************************//**
 *
 * Configure coexistence options at runtime.
 *
 * @param[in] mask Bitmask of following coexistence options to change
 *     - <b>@ref sl_bt_coex_option_enable</b> : (0x100) Enable coexistence
 *       feature
 *     - <b>@ref sl_bt_coex_option_tx_abort</b> : (0x400) Abort transmission if
 *       grant is denied
 *     - <b>@ref sl_bt_coex_option_high_priority</b> : (0x800) Enable priority
 *       signal
 * @param[in] options Bitmask of following coexistence option values to set
 *     - <b>@ref sl_bt_coex_option_enable</b> : (0x100) Enable coexistence
 *       feature
 *     - <b>@ref sl_bt_coex_option_tx_abort</b> : (0x400) Abort transmission if
 *       grant is denied
 *     - <b>@ref sl_bt_coex_option_high_priority</b> : (0x800) Enable priority
 *       signal
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_coex_set_options(uint32_t mask, uint32_t options);

/***************************************************************************//**
 *
 * Configure coexistence parameters.
 *
 * @param[in] priority Coexistence priority threshold. Coexistence priority is
 *   toggled if priority is below this value.
 * @param[in] request Coexistence request threshold. Coexistence request is
 *   toggled if priority is below this value.
 * @param[in] pwm_period PWM functionality period length in 1 ms units
 * @param[in] pwm_dutycycle PWM functionality duty cycle in percentage
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_coex_set_parameters(uint8_t priority,
                                      uint8_t request,
                                      uint8_t pwm_period,
                                      uint8_t pwm_dutycycle);

/***************************************************************************//**
 *
 * Set Directional Priority Pulse Width.
 *
 * @param[in] pulse Directional priority pulse width in us
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_coex_set_directional_priority_pulse(uint8_t pulse);

/***************************************************************************//**
 *
 * Get the coexistence parameters.
 *
 * @param[out] priority Coexistence priority threshold. Coexistence priority is
 *   toggled if priority is below this value.
 * @param[out] request Coexistence request threshold. Coexistence request is
 *   toggled if priority is below this value.
 * @param[out] pwm_period PWM functionality period length in 1 ms units
 * @param[out] pwm_dutycycle PWM functionality duty cycle in percentage
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_coex_get_parameters(uint8_t *priority,
                                      uint8_t *request,
                                      uint8_t *pwm_period,
                                      uint8_t *pwm_dutycycle);

/***************************************************************************//**
 *
 * Read coexistence statistic counters from the device. Response contains the
 * list of uint32 type counter values. Counters in the list are in following
 * order: low priority requested, high priority requested, low priority denied,
 * high priority denied, low-priority TX aborted, and high-priority TX aborted.
 * Passing a non-zero value also resets counters.
 *
 * @param[in] reset Reset counters if parameter value is not zero.
 * @param[in] max_counters_size Size of output buffer passed in @p counters
 * @param[out] counters_len On return, set to the length of output data written
 *   to @p counters
 * @param[out] counters Coexistence statistic counters
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_coex_get_counters(uint8_t reset,
                                    size_t max_counters_size,
                                    size_t *counters_len,
                                    uint8_t *counters);

/** @} */ // end addtogroup sl_bt_coex

/**
 * @addtogroup sl_bt_cs Channel Sounding
 * @{
 *
 * @brief Channel Sounding
 *
 * This class provides commands and events for Channel Sounding (CS) between
 * Bluetooth devices.
 *
 * Any CS command that involves a remote device requires that the connection
 * with the remote device is encrypted.
 */

/* Command and Response IDs */
#define sl_bt_cmd_cs_security_enable_id                              0x00590020
#define sl_bt_cmd_cs_set_default_settings_id                         0x01590020
#define sl_bt_cmd_cs_create_config_id                                0x02590020
#define sl_bt_cmd_cs_remove_config_id                                0x03590020
#define sl_bt_cmd_cs_set_channel_classification_id                   0x04590020
#define sl_bt_cmd_cs_set_procedure_parameters_id                     0x05590020
#define sl_bt_cmd_cs_procedure_enable_id                             0x06590020
#define sl_bt_cmd_cs_set_antenna_configuration_id                    0x07590020
#define sl_bt_cmd_cs_read_local_supported_capabilities_id            0x08590020
#define sl_bt_cmd_cs_read_remote_supported_capabilities_id           0x09590020
#define sl_bt_rsp_cs_security_enable_id                              0x00590020
#define sl_bt_rsp_cs_set_default_settings_id                         0x01590020
#define sl_bt_rsp_cs_create_config_id                                0x02590020
#define sl_bt_rsp_cs_remove_config_id                                0x03590020
#define sl_bt_rsp_cs_set_channel_classification_id                   0x04590020
#define sl_bt_rsp_cs_set_procedure_parameters_id                     0x05590020
#define sl_bt_rsp_cs_procedure_enable_id                             0x06590020
#define sl_bt_rsp_cs_set_antenna_configuration_id                    0x07590020
#define sl_bt_rsp_cs_read_local_supported_capabilities_id            0x08590020
#define sl_bt_rsp_cs_read_remote_supported_capabilities_id           0x09590020

/**
 * @brief Specifies the role for the device during CS procedure.
 */
typedef enum
{
  sl_bt_cs_role_initiator = 0x0, /**< (0x0) The device will initiate the
                                      procedure */
  sl_bt_cs_role_reflector = 0x1  /**< (0x1) The device will reciprocate
                                      transmission */
} sl_bt_cs_role_t;

/**
 * @brief Defines the status of a given role for an CS capable device.
 */
typedef enum
{
  sl_bt_cs_role_status_disable = 0x0, /**< (0x0) The given role is disabled */
  sl_bt_cs_role_status_enable  = 0x1  /**< (0x1) The given role is enabled */
} sl_bt_cs_role_status_t;

/**
 * @brief Defines the CS procedure state for the device.
 */
typedef enum
{
  sl_bt_cs_procedure_state_disabled = 0x0, /**< (0x0) CS procedures are disabled */
  sl_bt_cs_procedure_state_enabled  = 0x1  /**< (0x1) CS procedures are enabled */
} sl_bt_cs_procedure_state_t;

/**
 * @brief Defines the different modes for CS steps.
 */
typedef enum
{
  sl_bt_cs_mode_rtt         = 0x1,  /**< (0x1) Round Trip Time (RTT) measurement */
  sl_bt_cs_mode_pbr         = 0x2,  /**< (0x2) Phase-Based Ranging (PBR)
                                         measurement */
  sl_bt_cs_submode_disabled = 0xff  /**< (0xff) Submode disabled for the
                                         procedure. */
} sl_bt_cs_mode_t;

/**
 * @brief Defines the Round Trip Time (RTT) payload types used during the CS
 * sequence.
 */
typedef enum
{
  sl_bt_cs_rtt_type_aa_only                    = 0x0, /**< (0x0) RTT Access
                                                           Address (AA) only */
  sl_bt_cs_rtt_type_fractional_32_bit_sounding = 0x1, /**< (0x1) RTT Fractional
                                                           with 32-bit Sounding
                                                           Sequence */
  sl_bt_cs_rtt_type_fractional_96_bit_sounding = 0x2, /**< (0x2) RTT Fractional
                                                           with 96-bit Sounding
                                                           Sequence */
  sl_bt_cs_rtt_type_fractional_32_bit_random   = 0x3, /**< (0x3) RTT Fractional
                                                           with 32-bit Random
                                                           Sequence */
  sl_bt_cs_rtt_type_fractional_64_bit_random   = 0x4, /**< (0x4) RTT Fractional
                                                           with 64-bit Random
                                                           Sequence */
  sl_bt_cs_rtt_type_fractional_96_bit_random   = 0x5, /**< (0x5) RTT Fractional
                                                           with 96-bit Random
                                                           Sequence */
  sl_bt_cs_rtt_type_fractional_128_bit_random  = 0x6  /**< (0x6) RTT Fractional
                                                           with 128-bit Random
                                                           Sequence */
} sl_bt_cs_rtt_type_t;

/**
 * @brief Specifies the CS channel selection algorithms.
 */
typedef enum
{
  sl_bt_cs_channel_selection_algorithm_3b                     = 0x0, /**< (0x0)
                                                                          Use
                                                                          Channel
                                                                          Selection
                                                                          Algorithm
                                                                          #3b
                                                                          for
                                                                          non-mode
                                                                          0 CS
                                                                          steps */
  sl_bt_cs_channel_selection_algorithm_3c                     = 0x1, /**< (0x1)
                                                                          Use
                                                                          Channel
                                                                          Selection
                                                                          Algorithm
                                                                          #3c
                                                                          for
                                                                          non-mode
                                                                          0 CS
                                                                          steps */
  sl_bt_cs_channel_selection_algorithm_user_shape_interleaved = 0x2  /**< (0x2)
                                                                          Use
                                                                          Interleaved
                                                                          shape
                                                                          for
                                                                          user
                                                                          specified
                                                                          channel
                                                                          sequence */
} sl_bt_cs_channel_selection_algorithm_t;

/**
 * @brief Specifies the ch3c shapes.
 */
typedef enum
{
  sl_bt_cs_ch3c_shape_hat         = 0x0, /**< (0x0) Use Hat shape for user
                                              specified channel sequence */
  sl_bt_cs_chc3_shape_interleaved = 0x1  /**< (0x1) Use Interleaved shape for
                                              user specified channel sequence */
} sl_bt_cs_ch3c_shape_t;

/**
 * @brief Describes the current status of the procedure or subevents.
 */
typedef enum
{
  sl_bt_cs_done_status_complete                 = 0x0, /**< (0x0) All results
                                                            complete for the CS
                                                            procedure or
                                                            subevent */
  sl_bt_cs_done_status_partial_results_continue = 0x1, /**< (0x1) Partial
                                                            results with more to
                                                            follow */
  sl_bt_cs_done_status_aborted                  = 0xf  /**< (0xf) Current
                                                            procedure and all
                                                            subsequent subevents
                                                            in the procedure
                                                            aborted or current
                                                            subevent aborted. */
} sl_bt_cs_done_status_t;

/**
 * @brief Specifies the role for the device during CS procedure.
 */
typedef enum
{
  sl_bt_cs_config_state_removed = 0x0, /**< (0x0) The CS device configuration is
                                            removed */
  sl_bt_cs_config_state_created = 0x1  /**< (0x1) The CS device configuration is
                                            created */
} sl_bt_cs_config_state_t;

/**
 * @brief Specifies the Signal Noise Ration (SNR) used to control transmission
 * of CS_SYNC packets. Currently the SiSDK does not support the SNR control
 * adjustment.
 */
typedef enum
{
  sl_bt_cs_snr_control_adjustment_not_applied = 0xff  /**< (0xff) SNR control
                                                           adjustment not to be
                                                           applied */
} sl_bt_cs_snr_control_adjustment_t;

/**
 * @addtogroup sl_bt_evt_cs_security_enable_complete sl_bt_evt_cs_security_enable_complete
 * @{
 * @brief Indicates that a locally initiated CS security start procedure has
 * completed or the local controller has responded to a channel security request
 * from the remote controller
 */

/** @brief Identifier of the security_enable_complete event */
#define sl_bt_evt_cs_security_enable_complete_id                     0x005900a0

/***************************************************************************//**
 * @brief Data structure of the security_enable_complete event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_security_enable_complete_s
{
  uint8_t connection; /**< Connection handle */
});

typedef struct sl_bt_evt_cs_security_enable_complete_s sl_bt_evt_cs_security_enable_complete_t;

/** @} */ // end addtogroup sl_bt_evt_cs_security_enable_complete

/**
 * @addtogroup sl_bt_evt_cs_config_complete sl_bt_evt_cs_config_complete
 * @{
 * @brief Indicates that a locally initiated CS configuration procedure has
 * completed or the local controller has responded to an CS configuration
 * request from the remote controller
 */

/** @brief Identifier of the config_complete event */
#define sl_bt_evt_cs_config_complete_id                              0x015900a0

/***************************************************************************//**
 * @brief Data structure of the config_complete event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_config_complete_s
{
  uint8_t                connection;             /**< Connection handle */
  uint8_t                config_id;              /**< CS configuration
                                                      identifier.
                                                        - <b>Range:</b> 0 to 3 */
  uint16_t               status;                 /**< SL_STATUS_OK if
                                                      successful. Error code
                                                      otherwise. */
  uint8_t                config_state;           /**< Enum @ref
                                                      sl_bt_cs_config_state_t.
                                                      CS configuration state
                                                      Values:
                                                        - <b>sl_bt_cs_config_state_removed
                                                          (0x0):</b> The CS
                                                          device configuration
                                                          is removed
                                                        - <b>sl_bt_cs_config_state_created
                                                          (0x1):</b> The CS
                                                          device configuration
                                                          is created */
  uint8_t                main_mode_type;         /**< Enum @ref sl_bt_cs_mode_t.
                                                      Main mode type. Values:
                                                        - <b>sl_bt_cs_mode_rtt
                                                          (0x1):</b> Round Trip
                                                          Time (RTT) measurement
                                                        - <b>sl_bt_cs_mode_pbr
                                                          (0x2):</b> Phase-Based
                                                          Ranging (PBR)
                                                          measurement
                                                        - <b>sl_bt_cs_submode_disabled
                                                          (0xff):</b> Submode
                                                          disabled for the
                                                          procedure. */
  uint8_t                sub_mode_type;          /**< Enum @ref sl_bt_cs_mode_t.
                                                      Sub mode type. Values:
                                                        - <b>sl_bt_cs_mode_rtt
                                                          (0x1):</b> Round Trip
                                                          Time (RTT) measurement
                                                        - <b>sl_bt_cs_mode_pbr
                                                          (0x2):</b> Phase-Based
                                                          Ranging (PBR)
                                                          measurement
                                                        - <b>sl_bt_cs_submode_disabled
                                                          (0xff):</b> Submode
                                                          disabled for the
                                                          procedure. */
  uint8_t                min_main_mode_steps;    /**< Minimum number of CS main
                                                      mode steps to be executed
                                                      prior to a sub mode step.
                                                        - <b>Range:</b> 2 to 255 */
  uint8_t                max_main_mode_steps;    /**< Maximum number of CS main
                                                      mode steps to be executed
                                                      prior to a sub mode step.
                                                        - <b>Range:</b> 2 to 255 */
  uint8_t                main_mode_repetition;   /**< Number of main mode steps
                                                      taken from the end of the
                                                      last CS subevent to be
                                                      repeated at the beginning
                                                      of the current CS subevent
                                                      directly after the last
                                                      Mode 0 step of that event.
                                                        - <b>Range:</b> 0 to 3 */
  uint8_t                mode_calibration_steps; /**< Number of calibration mode
                                                      steps to be included at
                                                      the beginning of the test
                                                      CS subevent.
                                                        - <b>Range:</b> 1 to 3 */
  uint8_t                role;                   /**< Enum @ref sl_bt_cs_role_t.
                                                      Device role during the CS
                                                      procedure Values:
                                                        - <b>sl_bt_cs_role_initiator
                                                          (0x0):</b> The device
                                                          will initiate the
                                                          procedure
                                                        - <b>sl_bt_cs_role_reflector
                                                          (0x1):</b> The device
                                                          will reciprocate
                                                          transmission */
  uint8_t                rtt_type;               /**< Enum @ref
                                                      sl_bt_cs_rtt_type_t. RTT
                                                      payload type used in the
                                                      CS procedure Values:
                                                        - <b>sl_bt_cs_rtt_type_aa_only
                                                          (0x0):</b> RTT Access
                                                          Address (AA) only
                                                        - <b>sl_bt_cs_rtt_type_fractional_32_bit_sounding
                                                          (0x1):</b> RTT
                                                          Fractional with 32-bit
                                                          Sounding Sequence
                                                        - <b>sl_bt_cs_rtt_type_fractional_96_bit_sounding
                                                          (0x2):</b> RTT
                                                          Fractional with 96-bit
                                                          Sounding Sequence
                                                        - <b>sl_bt_cs_rtt_type_fractional_32_bit_random
                                                          (0x3):</b> RTT
                                                          Fractional with 32-bit
                                                          Random Sequence
                                                        - <b>sl_bt_cs_rtt_type_fractional_64_bit_random
                                                          (0x4):</b> RTT
                                                          Fractional with 64-bit
                                                          Random Sequence
                                                        - <b>sl_bt_cs_rtt_type_fractional_96_bit_random
                                                          (0x5):</b> RTT
                                                          Fractional with 96-bit
                                                          Random Sequence
                                                        - <b>sl_bt_cs_rtt_type_fractional_128_bit_random
                                                          (0x6):</b> RTT
                                                          Fractional with
                                                          128-bit Random
                                                          Sequence */
  uint8_t                cs_sync_phy;            /**< Enum @ref sl_bt_gap_phy_t.
                                                      Used PHY for CS_SYNC
                                                      exchanges during a
                                                      procedure Values:
                                                        - <b>sl_bt_gap_phy_1m
                                                          (0x1):</b> 1M PHY
                                                        - <b>sl_bt_gap_phy_2m
                                                          (0x2):</b> 2M PHY
                                                        - <b>sl_bt_gap_phy_coded
                                                          (0x4):</b> Coded PHY,
                                                          125k (S=8) or 500k
                                                          (S=2)
                                                        - <b>sl_bt_gap_phy_any
                                                          (0xff):</b> Any PHYs
                                                          the device supports */
  sl_bt_cs_channel_map_t channel_map;            /**< A fixed length byte array
                                                      of 10 bytes consisting of
                                                      80 1-bit fields.

                                                      The nth field (in the
                                                      range 0 to 78) contains
                                                      the value for the CS
                                                      channel index n.

                                                        - Bit value 0: Channel n
                                                          is disabled
                                                        - Bit value 1: Channel n
                                                          is enabled

                                                      The most significant bit
                                                      (79) is reserved for
                                                      future use and must be set
                                                      to 0. Channels n = 0, 1,
                                                      23, 24, 25, 77 and 78
                                                      shall be ignored and not
                                                      used for CS. At least 15
                                                      channels shall be marked
                                                      as used. */
  uint8_t                channel_map_repetition; /**< Number of times the
                                                      channel_map field will be
                                                      cycled through for
                                                      non-Mode 0 steps within a
                                                      CS procedure.
                                                        - <b>Range:</b> 1 to 255 */
  uint8_t                channel_selection_type; /**< Enum @ref
                                                      sl_bt_cs_channel_selection_algorithm_t.
                                                      CS algorithm to be used
                                                      during the procedure for
                                                      non-mode 0 steps Value:
                                                        - <b>sl_bt_cs_channel_selection_algorithm_3b
                                                          (0x0):</b> Use Channel
                                                          Selection Algorithm
                                                          #3b for non-mode 0 CS
                                                          steps
                                                        - <b>sl_bt_cs_channel_selection_algorithm_3c
                                                          (0x1):</b> Use Channel
                                                          Selection Algorithm
                                                          #3c for non-mode 0 CS
                                                          steps
                                                        - <b>sl_bt_cs_channel_selection_algorithm_user_shape_interleaved
                                                          (0x2):</b> Use
                                                          Interleaved shape for
                                                          user specified channel
                                                          sequence */
  uint8_t                ch3c_shape;             /**< Enum @ref
                                                      sl_bt_cs_ch3c_shape_t.
                                                      Shape for user-specified
                                                      channel sequence Values:
                                                        - <b>sl_bt_cs_ch3c_shape_hat
                                                          (0x0):</b> Use Hat
                                                          shape for user
                                                          specified channel
                                                          sequence
                                                        - <b>sl_bt_cs_chc3_shape_interleaved
                                                          (0x1):</b> Use
                                                          Interleaved shape for
                                                          user specified channel
                                                          sequence */
  uint8_t                ch3c_jump;              /**< Number of channels skipped
                                                      in each rising and falling
                                                      sequence
                                                        - <b>Range:</b> 3 to 8 */
  uint8_t                reserved;               /**< Reserved for future use. */
  uint8_t                t_ip1_time;             /**< Interlude time in
                                                      microseconds between the
                                                      RTT packets. Values: 0x0A,
                                                      0x14, 0x1E, 0x28, 0x32,
                                                      0x3C, 0x50, or 0x91. All
                                                      other values are reserved
                                                      for future use. */
  uint8_t                t_ip2_time;             /**< Interlude time in
                                                      microseconds between the
                                                      CS tones. Values: 0x0A,
                                                      0x14, 0x1E, 0x28, 0x32,
                                                      0x3C, 0x50, or 0x91. All
                                                      other values are reserved
                                                      for future use. */
  uint8_t                t_fcs_time;             /**< Time in microseconds for
                                                      frequency changes. Values:
                                                      0x0F, 0x14, 0x1E, 0x28,
                                                      0x32, 0x3C, 0x50, 0x64,
                                                      0x78, or 0x96. All other
                                                      values are reserved for
                                                      future use. */
  uint8_t                t_pm_time;              /**< Time in microseconds for
                                                      the phase measurement
                                                      period of the CS tones.
                                                      Values: 0x0A, 0x14, or
                                                      0x28. All other values are
                                                      reserved for future use. */
});

typedef struct sl_bt_evt_cs_config_complete_s sl_bt_evt_cs_config_complete_t;

/** @} */ // end addtogroup sl_bt_evt_cs_config_complete

/**
 * @addtogroup sl_bt_evt_cs_procedure_enable_complete sl_bt_evt_cs_procedure_enable_complete
 * @{
 * @brief Indicates the controller has scheduled a new CS procedure measurement,
 * as a result of @ref sl_bt_cs_procedure_enable command or disabled an ongoing,
 * as a result of @ref sl_bt_cs_procedure_enable command.
 */

/** @brief Identifier of the procedure_enable_complete event */
#define sl_bt_evt_cs_procedure_enable_complete_id                    0x025900a0

/***************************************************************************//**
 * @brief Data structure of the procedure_enable_complete event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_procedure_enable_complete_s
{
  uint8_t  connection;          /**< Connection handle */
  uint8_t  config_id;           /**< CS configuration identifier.
                                       - <b>Range:</b> 0 to 3 */
  uint16_t status;              /**< SL_STATUS_OK if successful. Error code
                                     otherwise. */
  uint8_t  state;               /**< Enum @ref sl_bt_cs_procedure_state_t. CS
                                     procedure enabled or disabled Values:
                                       - <b>sl_bt_cs_procedure_state_disabled
                                         (0x0):</b> CS procedures are disabled
                                       - <b>sl_bt_cs_procedure_state_enabled
                                         (0x1):</b> CS procedures are enabled */
  uint8_t  antenna_config;      /**< Antenna configuration index
                                       - <b>Range:</b> 0 to 7 */
  int8_t   tx_power;            /**< Transmit power level used in the
                                     transmission. Units: dBm.
                                       - <b>Range:</b> -127 to +20

                                       - Value: 0x07F. Transmit power level is
                                         unavailable */
  uint32_t subevent_len;        /**< Duration for each subevent in microseconds
                                       - <b>Range:</b> 1250 to 3999999

                                       - Time range: 1250 us to 4s */
  uint8_t  subevents_per_event; /**< Number of subevents anchored off the same
                                     ACL connection event
                                       - <b>Range:</b> 1 to 20 */
  uint16_t subevent_interval;   /**< Duration in microseconds between
                                     consecutive CS subevents anchored off the
                                     same ACL connection event. Units: 0.625 ms. */
  uint16_t event_interval;      /**< Number of ACL connection events between
                                     consecutive CS event anchor points */
  uint16_t procedure_interval;  /**< Number of ACL connection events between
                                     consecutive CS procedure anchor points */
  uint16_t procedure_count;     /**< Number of procedures to continue until
                                     disabled. Maximum number of procedures to
                                     be scheduled.
                                       - <b>Range:</b> 1 to 65535 */
  uint16_t max_procedure_len;   /**< Maximum duration for each measurement
                                     procedure. Value in units of 0.625 ms.
                                       - <b>Range:</b> 1 to 65535

                                       - Time: N x 0.625ms. <b>N</b> being the
                                         input.
                                       - Time range: 0.625 ms to 40.959375 s. */
});

typedef struct sl_bt_evt_cs_procedure_enable_complete_s sl_bt_evt_cs_procedure_enable_complete_t;

/** @} */ // end addtogroup sl_bt_evt_cs_procedure_enable_complete

/**
 * @addtogroup sl_bt_evt_cs_result sl_bt_evt_cs_result
 * @{
 * @brief Reports results of every CS subevent within the CS procedure
 *
 * When the number of steps exceeds the maximum HCI event size, the controller
 * may report further results for the CS subevent using the @ref
 * sl_bt_evt_cs_result_continue event.
 */

/** @brief Identifier of the result event */
#define sl_bt_evt_cs_result_id                                       0x035900a0

/***************************************************************************//**
 * @brief Data structure of the result event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_result_s
{
  uint8_t    connection;             /**< Connection handle. Returns a
                                          SL_BT_INVALID_CONNECTION_HANDLE (0xFF)
                                          when triggered as a result of @ref
                                          sl_bt_cs_test_start command */
  uint8_t    config_id;              /**< CS configuration identifier. This
                                          paremeter is ignored when triggered as
                                          a result of @ref sl_bt_cs_test_start
                                          command.
                                            - <b>Range:</b> 0 to 3 */
  uint16_t   start_acl_conn_event;   /**< Starting an ACL connection event count
                                          for the results reported in the event.
                                          This is reported only in the first
                                          subevent in the procedure. For
                                          subsequent subevents, this value is
                                          set to 0. This parameter is ignored
                                          when triggered as a result of @ref
                                          sl_bt_cs_test_start command. */
  uint16_t   procedure_counter;      /**< Indicates the associated CS procedure
                                          count for the results reported in this
                                          event. This parameter is ignored when
                                          triggered as a result of @ref
                                          sl_bt_cs_test_start command. */
  int16_t    frequency_compensation; /**< Frequency compensation value. Units:
                                          0.01 ppm (15-bit signed integer).
                                            - <b>Range:</b> -100ppm (227680) to
                                              +100ppm (10000)

                                            - Value: 0xC000. Frequency
                                              compensation value is not
                                              available or the role is not
                                              initiator. This is reported only
                                              in the first subevent in the
                                              procedure. For subsequent
                                              subevents, this value is set to 0. */
  int8_t     reference_power_level;  /**< Reference power level used by the
                                          transmission. Units: dBm.
                                            - <b>Range:</b> -127 to +20

                                            - Value: 0x07F. The reference power
                                              level is not applicable */
  uint8_t    procedure_done_status;  /**< Enum @ref sl_bt_cs_done_status_t.
                                          Current status of the CS procedure
                                          Values:
                                            - <b>sl_bt_cs_done_status_complete
                                              (0x0):</b> All results complete
                                              for the CS procedure or subevent
                                            - <b>sl_bt_cs_done_status_partial_results_continue
                                              (0x1):</b> Partial results with
                                              more to follow
                                            - <b>sl_bt_cs_done_status_aborted
                                              (0xf):</b> Current procedure and
                                              all subsequent subevents in the
                                              procedure aborted or current
                                              subevent aborted. */
  uint8_t    subevent_done_status;   /**< Enum @ref sl_bt_cs_done_status_t.
                                          Current status of the CS subevent
                                          Values:
                                            - <b>sl_bt_cs_done_status_complete
                                              (0x0):</b> All results complete
                                              for the CS procedure or subevent
                                            - <b>sl_bt_cs_done_status_partial_results_continue
                                              (0x1):</b> Partial results with
                                              more to follow
                                            - <b>sl_bt_cs_done_status_aborted
                                              (0xf):</b> Current procedure and
                                              all subsequent subevents in the
                                              procedure aborted or current
                                              subevent aborted. */
  uint8_t    abort_reason;           /**< Indicates the abort reason when the @p
                                          procedure_done_status or @p
                                          subevent_done_status is set to 0xF,
                                          otherwise the default value is set to
                                          zero.

                                          Bits 0-3 indicate the procedure abort
                                          reasons:

                                            - 0x0 = Report with no abort
                                            - 0x1 = Abort because of local Host
                                              or remote request
                                            - 0x2 = Abort because filtered
                                              channel map has less than 15
                                              channels
                                            - 0x3 = Abort because the channel
                                              map update instant has passed
                                            - 0xF = Abort because of unspecified
                                              reasons

                                          Bits 4-7 indicate the subevent done
                                          reasons:

                                            - 0x0 = Report with no abort
                                            - 0x1 = Abort because of local Host
                                              or remote request
                                            - 0x2 = Abort because no CS_SYNC
                                              (mode-0) received
                                            - 0x3 = Abort because of scheduling
                                              conflicts or limited resources
                                            - 0xF = Abort because of unspecified
                                              reasons */
  uint8_t    num_antenna_paths;      /**< Number of antenna paths supported by
                                          the local controller for the CS tone
                                          exchanges. The number of antenna paths
                                          used during the phase measurement
                                          stage of the CS step.
                                            - <b>Range:</b> 1 to 4

                                            - Value: 0. Phase measurement does
                                              not occur during the CS step,
                                              therefore ignored */
  uint8_t    num_steps;              /**< Number of steps in the CS subevent for
                                          which results are reported.
                                            - <b>Range:</b> 1 to 160 */
  uint8array data;                   /**< The result data is structured as
                                          follows:
                                            - step_status: 1 octet for each
                                              num_steps. 0x00 for step scheduled
                                              and reported. 0xFE for step
                                              aborted.
                                            - step_mode: 1 octet for each
                                              num_steps. Mode type. Range 0 to
                                              3.
                                            - step_channel: 1 octet for each
                                              num_steps. Channel index. Range 1
                                              to 78.
                                            - step_data_length: 1 octet for each
                                              num_steps. Length of mode and role
                                              specific information being
                                              reported. Range 0x00 to 0xFF.
                                            - step_data: step_data_length octet
                                              for each corresponding steps in
                                              num_steps. */
});

typedef struct sl_bt_evt_cs_result_s sl_bt_evt_cs_result_t;

/** @} */ // end addtogroup sl_bt_evt_cs_result

/**
 * @addtogroup sl_bt_evt_cs_result_continue sl_bt_evt_cs_result_continue
 * @{
 * @brief Reports results when the number of steps exceeds the maximum HCI event
 * size, of every CS subevent within the CS procedure
 *
 * This event is triggered after the @ref sl_bt_evt_cs_result event.
 */

/** @brief Identifier of the result_continue event */
#define sl_bt_evt_cs_result_continue_id                              0x055900a0

/***************************************************************************//**
 * @brief Data structure of the result_continue event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_result_continue_s
{
  uint8_t    connection;            /**< Connection handle. Returns a
                                         SL_BT_INVALID_CONNECTION_HANDLE (0xFF)
                                         when triggered as a result of @ref
                                         sl_bt_cs_test_start command */
  uint8_t    config_id;             /**< CS configuration identifier. This
                                         paremeter is ignored when the event is
                                         triggered as a result of @ref
                                         sl_bt_cs_test_start command.
                                           - <b>Range:</b> 0 to 3 */
  uint8_t    procedure_done_status; /**< Enum @ref sl_bt_cs_done_status_t.
                                         Current status of the CS procedure
                                         Values:
                                           - <b>sl_bt_cs_done_status_complete
                                             (0x0):</b> All results complete for
                                             the CS procedure or subevent
                                           - <b>sl_bt_cs_done_status_partial_results_continue
                                             (0x1):</b> Partial results with
                                             more to follow
                                           - <b>sl_bt_cs_done_status_aborted
                                             (0xf):</b> Current procedure and
                                             all subsequent subevents in the
                                             procedure aborted or current
                                             subevent aborted. */
  uint8_t    subevent_done_status;  /**< Enum @ref sl_bt_cs_done_status_t.
                                         Current status of the CS subevent
                                         Values:
                                           - <b>sl_bt_cs_done_status_complete
                                             (0x0):</b> All results complete for
                                             the CS procedure or subevent
                                           - <b>sl_bt_cs_done_status_partial_results_continue
                                             (0x1):</b> Partial results with
                                             more to follow
                                           - <b>sl_bt_cs_done_status_aborted
                                             (0xf):</b> Current procedure and
                                             all subsequent subevents in the
                                             procedure aborted or current
                                             subevent aborted. */
  uint8_t    abort_reason;          /**< Indicates the abort reason when the @p
                                         procedure_done_status or @p
                                         subevent_done_status is set to 0xF,
                                         otherwise the default value is set to
                                         zero.

                                         Bits 0-3 indicate the procedure abort
                                         reasons:

                                           - 0x0 = Report with no abort
                                           - 0x1 = Abort because of local Host
                                             or remote request
                                           - 0x2 = Abort because filtered
                                             channel map has less than 15
                                             channels
                                           - 0x3 = Abort because the channel map
                                             update instant has passed
                                           - 0xF = Abort because of unspecified
                                             reasons

                                         Bits 4-7 indicate the subevent done
                                         reasons:

                                           - 0x0 = Report with no abort
                                           - 0x1 = Abort because of local Host
                                             or remote request
                                           - 0x2 = Abort because no CS_SYNC
                                             (mode-0) received
                                           - 0x3 = Abort because of scheduling
                                             conflicts or limited resources
                                           - 0xF = Abort because of unspecified
                                             reasons */
  uint8_t    num_antenna_paths;     /**< Number of antenna paths supported by
                                         the local controller for the CS tone
                                         exchanges. The number of antenna paths
                                         used during the phase measurement stage
                                         of the CS step.
                                           - <b>Range:</b> 1 to 4

                                           - Value: 0. Phase measurement does
                                             not occur during the CS step,
                                             therefore ignored */
  uint8_t    num_steps;             /**< Number of steps in the CS subevent for
                                         which results are reported.
                                           - <b>Range:</b> 1 to 160 */
  uint8array data;                  /**< The result data is structured as
                                         follows:
                                           - step_mode: 1 octet for each
                                             num_steps. Mode type. Range 0 to 3.
                                           - step_channel: 1 octet for each
                                             num_steps. Channel index. Range 1
                                             to 78.
                                           - step_data_length: 1 octet for each
                                             num_steps. Length of mode and role
                                             specific information being
                                             reported. Range 0x00 to 0xFF.
                                           - step_data: step_data_length octet
                                             for each corresponding steps in
                                             num_steps. */
});

typedef struct sl_bt_evt_cs_result_continue_s sl_bt_evt_cs_result_continue_t;

/** @} */ // end addtogroup sl_bt_evt_cs_result_continue

/**
 * @addtogroup sl_bt_evt_cs_read_remote_supported_capabilities_complete sl_bt_evt_cs_read_remote_supported_capabilities_complete
 * @{
 * @brief Report the remote controller's supported capabilities as a result of
 * calling the @ref sl_bt_cs_read_remote_supported_capabilities command
 * previously
 */

/** @brief Identifier of the read_remote_supported_capabilities_complete event */
#define sl_bt_evt_cs_read_remote_supported_capabilities_complete_id  0x045900a0

/***************************************************************************//**
 * @brief Data structure of the read_remote_supported_capabilities_complete event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_read_remote_supported_capabilities_complete_s
{
  uint8_t  connection;                 /**< Connection handle */
  uint16_t status;                     /**< SL_STATUS_OK if successful. Error
                                            code otherwise. */
  uint8_t  num_config;                 /**< The number of CS configurations
                                            supported per connection.
                                              - <b>Range:</b> 1 to 4 */
  uint16_t max_consecutive_procedures; /**< The maximum number of consecutive CS
                                            procedures supported.
                                              - <b>Range:</b> 1 to 65535

                                              - <b>0x00:</b> Fixed number of
                                                consecutive procedures and for
                                                an indefinite number of
                                                procedures until termination */
  uint8_t  num_antennas;               /**< The number of antenna elements
                                            available for CS tone exchanges.
                                              - <b>Range:</b> 1 to 4 */
  uint8_t  max_antenna_paths;          /**< The maximum number of antenna paths
                                            supported.
                                              - <b>Range:</b> 1 to 4 */
  uint8_t  roles;                      /**< This value is a bitmask of flags to
                                            indicate which CS roles are
                                            supported by the controller. Flags:
                                              - <b>0x01, bit 0:</b> Initiator
                                                role
                                              - <b>0x02, bit 1:</b> Reflector
                                                role */
  uint8_t  modes;                      /**< This value is a bitmask of flags to
                                            indicate which optional CS modes are
                                            supported. Flags:
                                              - <b>0x02, bit 1:</b> Mode 3 is
                                                supported */
  uint8_t  rtt_capability;             /**< This value is a bitmask of flags to
                                            indicate which Round Trip Time (RTT)
                                            CS capabilities accuracy requirement
                                            in @p rtt_aa_only_n, @p
                                            rtt_sounding_n, and @p
                                            rtt_random_payload. Flags:
                                              - <b>0x01, bit 0:</b> 10 ns
                                                time-of-flight (ToF) precision
                                                requirement for @p rtt_aa_only
                                                if set; otherwise 150 ns ToF
                                              - <b>0x02, bit 1:</b> 10 ns ToF
                                                precision requirement for @p
                                                rtt_sounding if set; otherwise
                                                150 ns ToF
                                              - <b>0x04, bit 2:</b> 10 ns ToF
                                                precision requirement for @p
                                                rtt_random_payload if set;
                                                otherwise 150 ns ToF */
  uint8_t  rtt_aa_only;                /**< The support status of RTT Access
                                            Address(AA) on the remote
                                            controller. When supported it
                                            establishes the number of SYNC
                                            changes needed to satisfy the
                                            precision requirements.
                                              - <b>Range:</b> 1 to 255

                                              - 0x00: RTT AA Only is not
                                                supported */
  uint8_t  rtt_sounding;               /**< The support status of RTT Sounding
                                            is supported on the remote
                                            controller. When supported it
                                            establishes the number of SYNC
                                            changes needed to satisfy the
                                            precision requirements.
                                              - <b>Range:</b> 1 to 255

                                              - 0x00: RTT Sounding is not
                                                supported */
  uint8_t  rtt_random_payload;         /**< The support status of RTT Random
                                            Payload on the remote controller.
                                            When supported it establishes the
                                            number of SYNC changes needed to
                                            satisfy the precision requirements.
                                              - <b>Range:</b> 1 to 255

                                              - 0x00: RTT Random Payload is not
                                                supported */
  uint8_t  cs_sync_phys;               /**< This value is a bitmask of flags to
                                            indicate which CS SYNC packages
                                            supported in an specific PHY. Flags:
                                              - <b>0x01, bit 0:</b> LE 2M PHY CS
                                                SYNC packages are supported
                                              - <b>0x02, bit 1:</b> LE 2M PHY
                                                2BT CS SYNC packages are
                                                supported */
  uint16_t subfeatures;                /**< This value is a bitmask of flags to
                                            indicate which CS subfeatures are
                                            supported. Flags:
                                              - <b>0x01, bit 0:</b> CS with zero
                                                Frequency Actuation Error
                                                relative to Mode 0 transmissions
                                                in reflector role is supported
                                              - <b>0x02, bit 1:</b> CS Channel
                                                Selection Algorithm #3c is
                                                supported
                                              - <b>0x04, bit 2:</b> CS
                                                phase-based ranging from a
                                                sounding sequence is supported */
  uint16_t t_ip1_times;                /**< This value is a bitmask of flags to
                                            indicate which time durations of
                                            Time for Interlude Period 1 (IP1)
                                            are supported. Flags:
                                              - <b>0x0001, bit 0:</b> Time
                                                duration of 10 microseconds is
                                                supported
                                              - <b>0x0002, bit 1:</b> Time
                                                duration of 20 microseconds is
                                                supported
                                              - <b>0x0004, bit 2:</b> Time
                                                duration of 30 microseconds is
                                                supported
                                              - <b>0x0008, bit 3:</b> Time
                                                duration of 40 microseconds is
                                                supported
                                              - <b>0x0010, bit 4:</b> Time
                                                duration of 50 microseconds is
                                                supported
                                              - <b>0x0020, bit 5:</b> Time
                                                duration of 60 microseconds is
                                                supported
                                              - <b>0x0040, bit 6:</b> Time
                                                duration of 80 microseconds is
                                                supported */
  uint16_t t_ip2_times;                /**< This value is a bitmask of flags to
                                            indicate which time durations of
                                            Time for Interlude Period 2 (IP2)
                                            are supported. Flags:
                                              - <b>0x0001, bit 0:</b> Time
                                                duration of 10 microseconds is
                                                supported
                                              - <b>0x0002, bit 1:</b> Time
                                                duration of 20 microseconds is
                                                supported
                                              - <b>0x0004, bit 2:</b> Time
                                                duration of 30 microseconds is
                                                supported
                                              - <b>0x0008, bit 3:</b> Time
                                                duration of 40 microseconds is
                                                supported
                                              - <b>0x0010, bit 4:</b> Time
                                                duration of 50 microseconds is
                                                supported
                                              - <b>0x0020, bit 5:</b> Time
                                                duration of 60 microseconds is
                                                supported
                                              - <b>0x0040, bit 6:</b> Time
                                                duration of 80 microseconds is
                                                supported */
  uint16_t t_fcs_times;                /**< This value is a bitmask of flags to
                                            indicate which time durations of
                                            Time for Frequency Change Spacing
                                            (FCS) are supported. Flags:
                                              - <b>0x0001, bit 0:</b> Time
                                                duration of 10 microseconds is
                                                supported
                                              - <b>0x0002, bit 1:</b> Time
                                                duration of 20 microseconds is
                                                supported
                                              - <b>0x0004, bit 2:</b> Time
                                                duration of 30 microseconds is
                                                supported
                                              - <b>0x0008, bit 3:</b> Time
                                                duration of 40 microseconds is
                                                supported
                                              - <b>0x0010, bit 4:</b> Time
                                                duration of 50 microseconds is
                                                supported
                                              - <b>0x0020, bit 5:</b> Time
                                                duration of 60 microseconds is
                                                supported
                                              - <b>0x0040, bit 6:</b> Time
                                                duration of 80 microseconds is
                                                supported
                                              - <b>0x0080, bit 7:</b> Time
                                                duration of 100 microseconds is
                                                supported
                                              - <b>0x0100, bit 8:</b> Time
                                                duration of 120 microseconds is
                                                supported */
  uint16_t t_pm_times;                 /**< This value is a bitmask of flags to
                                            indicate which time durations of
                                            Time for Phase Measurement (PM) are
                                            supported. Flags:
                                              - <b>0x0001, bit 0:</b> Time
                                                duration of 10 microseconds is
                                                supported
                                              - <b>0x0002, bit 1:</b> Time
                                                duration of 20 microseconds is
                                                supported */
  uint8_t  t_sw_times;                 /**< Time in microseconds for the antenna
                                            switch period of the CS tones.
                                              - Values: 0x00, 0x01, 0x02, 0x04
                                                and 0x0A */
  uint8_t  tx_snr_capability;          /**< This value is a bitmask of flags to
                                            indicate the supported SNR levels
                                            used in RTT packets. Flags:
                                              - <b>0x01, bit 0:</b> 18 dB
                                                supported
                                              - <b>0x02, bit 1:</b> 21 dB
                                                supported
                                              - <b>0x04, bit 2:</b> 24 dB
                                                supported
                                              - <b>0x08, bit 3:</b> 27 dB
                                                supported
                                              - <b>0x10, bit 4:</b> 30 dB
                                                supported */
});

typedef struct sl_bt_evt_cs_read_remote_supported_capabilities_complete_s sl_bt_evt_cs_read_remote_supported_capabilities_complete_t;

/** @} */ // end addtogroup sl_bt_evt_cs_read_remote_supported_capabilities_complete

/***************************************************************************//**
 *
 * Start or restart CS security start procedure for the specified connection.
 * This command returns SL_STATUS_BT_CTRL_INSUFFICIENT_SECURITY if the
 * connection is not encrypted.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if the device is
 * not the central in the connection.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_security_enable_complete - Triggered when CS security
 *     start procedure has completed.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_security_enable(uint8_t connection);

/***************************************************************************//**
 *
 * Set the default CS settings for the specified connection. By default, all
 * roles are disabled and the antenna is set to 1.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if the
 * device is trying to disable a role for which a valid CS configuration is
 * present.
 *
 * @param[in] connection Connection handle
 * @param[in] initiator_status Enum @ref sl_bt_cs_role_status_t. Enable or
 *   disable status of the Initiator role. Values:
 *     - <b>sl_bt_cs_role_status_disable (0x0):</b> The given role is disabled
 *     - <b>sl_bt_cs_role_status_enable (0x1):</b> The given role is enabled
 * @param[in] reflector_status Enum @ref sl_bt_cs_role_status_t. Enable or
 *   disable status of the Reflector role. Values:
 *     - <b>sl_bt_cs_role_status_disable (0x0):</b> The given role is disabled
 *     - <b>sl_bt_cs_role_status_enable (0x1):</b> The given role is enabled
 * @param[in] antenna_identifier Antenna identifier to be used for CS sync
 *   packets.
 *     - <b>Range:</b> 1 to 4
 * @param[in] max_tx_power Maximum transmit power level to be used in all CS
 *   transmissions. Units: dBm.
 *     - <b>Range:</b> -127 to +20
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_set_default_settings(uint8_t connection,
                                          uint8_t initiator_status,
                                          uint8_t reflector_status,
                                          uint8_t antenna_identifier,
                                          int8_t max_tx_power);

/***************************************************************************//**
 *
 * Create a new CS configuration in the local and remote controller. The role
 * used in this command must be enabled prior to issuing this command using @ref
 * sl_bt_cs_set_default_settings command.
 *
 * This command returns SL_STATUS_BT_CTRL_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE
 * if the local or the remote controller does not support that CS feature.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if there is
 * no valid CS configuration previously created for the specified connection.
 * These are created with the @ref sl_bt_cs_set_default_settings.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if a previously
 * created CS configuration is enabled or ongoing for the specified connection,
 * by calling @ref sl_bt_cs_procedure_enable.
 *
 * @param[in] connection The connection handle
 * @param[in] config_id CS configuration identifier.
 *     - <b>Range:</b> 0 to 3
 * @param[in] create_context Defines in which device the created configuration
 *   will be written
 *     - Value: 0x00. Write CS configuration in the local controller only
 *     - Value: 0x01. Write CS configuration in both the local and remote
 *       controller using a configuration procedure
 * @param[in] main_mode_type Enum @ref sl_bt_cs_mode_t. Main mode type. Values:
 *     - <b>sl_bt_cs_mode_rtt (0x1):</b> Round Trip Time (RTT) measurement
 *     - <b>sl_bt_cs_mode_pbr (0x2):</b> Phase-Based Ranging (PBR) measurement
 *     - <b>sl_bt_cs_submode_disabled (0xff):</b> Submode disabled for the
 *       procedure.
 * @param[in] sub_mode_type Enum @ref sl_bt_cs_mode_t. Sub mode type. Values:
 *     - <b>sl_bt_cs_mode_rtt (0x1):</b> Round Trip Time (RTT) measurement
 *     - <b>sl_bt_cs_mode_pbr (0x2):</b> Phase-Based Ranging (PBR) measurement
 *     - <b>sl_bt_cs_submode_disabled (0xff):</b> Submode disabled for the
 *       procedure.
 * @param[in] min_main_mode_steps Minimum number of CS main mode steps to be
 *   executed prior to a sub mode step.
 *     - <b>Range:</b> 2 to 255
 * @param[in] max_main_mode_steps Maximum number of CS main mode steps to be
 *   executed prior to a sub mode step.
 *     - <b>Range:</b> 2 to 255
 * @param[in] main_mode_repetition Number of main mode steps taken from the end
 *   of the last CS subevent to be repeated at the beginning of the current CS
 *   subevent directly after the value r last Mode 0 step of that event.
 *     - <b>Range:</b> 0 to 3
 * @param[in] mode_calibration_steps Number of Mode 0 steps to be included at
 *   the beginning of the test CS subevent
 *     - <b>Range:</b> 1 to 3
 * @param[in] role Enum @ref sl_bt_cs_role_t. Device's role during the CS
 *   procedure Values:
 *     - <b>sl_bt_cs_role_initiator (0x0):</b> The device will initiate the
 *       procedure
 *     - <b>sl_bt_cs_role_reflector (0x1):</b> The device will reciprocate
 *       transmission
 * @param[in] rtt_type Enum @ref sl_bt_cs_rtt_type_t. RTT payload type used in
 *   the CS procedure Values:
 *     - <b>sl_bt_cs_rtt_type_aa_only (0x0):</b> RTT Access Address (AA) only
 *     - <b>sl_bt_cs_rtt_type_fractional_32_bit_sounding (0x1):</b> RTT
 *       Fractional with 32-bit Sounding Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_96_bit_sounding (0x2):</b> RTT
 *       Fractional with 96-bit Sounding Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_32_bit_random (0x3):</b> RTT Fractional
 *       with 32-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_64_bit_random (0x4):</b> RTT Fractional
 *       with 64-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_96_bit_random (0x5):</b> RTT Fractional
 *       with 96-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_128_bit_random (0x6):</b> RTT
 *       Fractional with 128-bit Random Sequence
 * @param[in] cs_sync_phy Enum @ref sl_bt_gap_phy_t. Used PHY for CS SYNC
 *   exchanges during a procedure Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 * @param[in] channel_map @parblock
 *   A fixed length byte array of 10 bytes consisting of 80 1-bit fields.
 *
 *   The nth such field (in the range 0 to 78) contains the value for the CS
 *   channel index n.
 *
 *     - Bit value 0: Channel n is disabled
 *     - Bit value 1: Channel n is enabled
 *
 *   The most significant bit (79) is reserved for future use and must be set to
 *   0. Channels n = 0, 1, 23, 24, 25, 77 and 78 shall be ignored and not used
 *   for CS. At least 15 channels shall be marked as used.
 *   @endparblock
 * @param[in] channel_map_repetition Number of times the channel_map field will
 *   be cycled through for non-Mode 0 steps within an CS procedure.
 *     - <b>Range:</b> 1 to 255
 * @param[in] channel_selection_type Enum @ref
 *   sl_bt_cs_channel_selection_algorithm_t. Channel selection algorithm Values:
 *     - <b>sl_bt_cs_channel_selection_algorithm_3b (0x0):</b> Use Channel
 *       Selection Algorithm #3b for non-mode 0 CS steps
 *     - <b>sl_bt_cs_channel_selection_algorithm_3c (0x1):</b> Use Channel
 *       Selection Algorithm #3c for non-mode 0 CS steps
 *     - <b>sl_bt_cs_channel_selection_algorithm_user_shape_interleaved
 *       (0x2):</b> Use Interleaved shape for user specified channel sequence
 * @param[in] ch3c_shape Enum @ref sl_bt_cs_ch3c_shape_t. Ch3c shape Values:
 *     - <b>sl_bt_cs_ch3c_shape_hat (0x0):</b> Use Hat shape for user specified
 *       channel sequence
 *     - <b>sl_bt_cs_chc3_shape_interleaved (0x1):</b> Use Interleaved shape for
 *       user specified channel sequence
 * @param[in] ch3c_jump Number of channels skipped in each rising and falling
 *   sequence.
 *     - <b>Range:</b> 2 to 8
 * @param[in] reserved Reserved for future use.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_config_complete - Triggered when an CS configuration
 *     procedure completed
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_create_config(uint8_t connection,
                                   uint8_t config_id,
                                   uint8_t create_context,
                                   uint8_t main_mode_type,
                                   uint8_t sub_mode_type,
                                   uint8_t min_main_mode_steps,
                                   uint8_t max_main_mode_steps,
                                   uint8_t main_mode_repetition,
                                   uint8_t mode_calibration_steps,
                                   uint8_t role,
                                   uint8_t rtt_type,
                                   uint8_t cs_sync_phy,
                                   const sl_bt_cs_channel_map_t *channel_map,
                                   uint8_t channel_map_repetition,
                                   uint8_t channel_selection_type,
                                   uint8_t ch3c_shape,
                                   uint8_t ch3c_jump,
                                   uint8_t reserved);

/***************************************************************************//**
 *
 * Remove an CS configuration from the local controller.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if the @p
 * config_id does not exist .
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if there is one or
 * more CS procedures enabled with the @ref sl_bt_cs_procedure_enable command.
 *
 * @param[in] connection Connection handle
 * @param[in] config_id CS configuration identifier
 *     - <b>Range:</b> 0 to 3
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_config_complete - Triggered when an CS configuration
 *     procedure is completed
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_remove_config(uint8_t connection, uint8_t config_id);

/***************************************************************************//**
 *
 * Update the channel classification for CS. This classification persists until
 * overwritten with a subsequent command or until the system is reset.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if this command was
 * issued after less than one second before.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if less
 * than 15 channels are enabled.
 *
 * @param[in] channel_map @parblock
 *   A fixed length byte array of 10 bytes consisting of 80 1-bit fields.
 *
 *   The nth field (in the range 0 to 78) contains the value for the link layer
 *   channel index n.
 *
 *     - Bit value 0: Channel n is disabled.
 *     - Bit value 1: Channel n is enabled.
 *
 *   The most significant bit (79) is reserved for future use and must be set to
 *   0. Channels n = 0, 1, 23, 24, 25, 77 and 78 shall be ignored and not used
 *   for CS.
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_set_channel_classification(const sl_bt_cs_channel_map_t *channel_map);

/***************************************************************************//**
 *
 * Set the parameters for scheduling CS procedures with the remote device. The
 * parameters set by this command become invalid after removing the @p config_id
 * with @ref sl_bt_cs_remove_config.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if the @p
 * config_id does not exists or was previously removed with @ref
 * sl_bt_cs_remove_config
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if less
 * than 15 channels are enabled.
 *
 * This command returns SL_STATUS_BT_CTRL_INSUFFICIENT_CHANNELS if there are
 * less than 15 channels available.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if the CS procedure
 * has been previously enabled.
 *
 * @param[in] connection The connection handle
 * @param[in] config_id CS configuration identifier.
 *     - <b>Range:</b> 0 to 3
 * @param[in] max_procedure_len @parblock
 *   Maximum duration for each measurement procedure. Value in units of 0.625
 *   ms.
 *     - <b>Range:</b> 1 to 65535
 *
 *     - Time: N x 0.625ms. <b>N</b> being the input.
 *     - Time range: 0.625 ms to 40.959375 s.
 *   @endparblock
 * @param[in] min_procedure_interval Minimum duration in number of connection
 *   events between consecutive measurement procedure.
 *     - <b>Range:</b> 1 to 65535
 * @param[in] max_procedure_interval Maximum duration in number of connection
 *   events between consecutive measurement procedure.
 *     - <b>Range:</b> 1 to 65535
 * @param[in] max_procedure_count @parblock
 *   Maximum number of CS procedures to be scheduled.
 *     - <b>Range:</b> 1 to 65535
 *
 *     - Value: 0x00. Procedures to continue until disabled.
 *   @endparblock
 * @param[in] min_subevent_len @parblock
 *   Minimum suggested duration for each CS subevent. Units: microseconds.
 *     - <b>Range:</b> 1250 to 3999999
 *
 *     - Time range: 1250 us to 4s
 *   @endparblock
 * @param[in] max_subevent_len @parblock
 *   Maximum suggested duration for each CS subevent. Units: microseconds.
 *     - <b>Range:</b> 1250 to 3999999
 *
 *     - Time range: 1250 us to 4s
 *   @endparblock
 * @param[in] tone_antenna_config_selection Antenna configuration index
 *     - <b>Range:</b> 0 to 7
 * @param[in] phy Enum @ref sl_bt_gap_phy_t. PHY used by the controller to
 *   determine the base transmit power for the CS procedure. The final transmit
 *   power level will be the lesser of the configured maximum transmission power
 *   for the PHY and the sum of the connection transmission power and the remote
 *   controller's power delta. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 *     - <b>sl_bt_gap_phy_any (0xff):</b> Any PHYs the device supports
 * @param[in] tx_pwr_delta Indicates the recommended difference between the
 *   remote controller's power level for the CS tones and RTT packets and the
 *   power level for the PHY indicated by the PHY parameter. Units: dB.
 *     - Value: 0x80. Host does not have a recommendation for transmit power
 *       delta
 * @param[in] preferred_peer_antenna Preferred peer-ordered antenna elements to
 *   be used by the remote device for the antenna configuration denoted by the
 *   tone antenna config selection.
 *     - 0x01, bit 0: Use first ordered antenna element
 *     - 0x02, bit 1: Use second ordered antenna element
 *     - 0x04, bit 2: Use third ordered antenna element
 *     - 0x08, bit 3: Use fourth ordered antenna element
 * @param[in] snr_control_initiator Enum @ref sl_bt_cs_snr_control_adjustment_t.
 *   The Signal Noise Ratio (SNR) control adjustment for the CS_SYNC
 *   transmissions of the initiator. Values:
 *     - <b>sl_bt_cs_snr_control_adjustment_not_applied (0xff):</b> SNR control
 *       adjustment not to be applied
 * @param[in] snr_control_reflector Enum @ref sl_bt_cs_snr_control_adjustment_t.
 *   The Signal Noise Ratio (SNR) control adjustment for the CS_SYNC
 *   transmissions of the reflector. Values:
 *     - <b>sl_bt_cs_snr_control_adjustment_not_applied (0xff):</b> SNR control
 *       adjustment not to be applied
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_set_procedure_parameters(uint8_t connection,
                                              uint8_t config_id,
                                              uint16_t max_procedure_len,
                                              uint16_t min_procedure_interval,
                                              uint16_t max_procedure_interval,
                                              uint16_t max_procedure_count,
                                              uint32_t min_subevent_len,
                                              uint32_t max_subevent_len,
                                              uint8_t tone_antenna_config_selection,
                                              uint8_t phy,
                                              int8_t tx_pwr_delta,
                                              uint8_t preferred_peer_antenna,
                                              uint8_t snr_control_initiator,
                                              uint8_t snr_control_reflector);

/***************************************************************************//**
 *
 * Enable or disable scheduling CS procedures with the remote device.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if the @ref
 * sl_bt_cs_set_procedure_parameters has not been called previosly for that @p
 * config_id.
 *
 * This command returns SL_STATUS_BT_CTRL_INVALID_COMMAND_PARAMETERS if the @p
 * config_id does not exists or was previously removed with @ref
 * sl_bt_cs_remove_config.
 *
 * This command returns SL_STATUS_BT_CTRL_COMMAND_DISALLOWED if the CS procedure
 * has been previously enabled by calling @ref sl_bt_cs_procedure_enable.
 *
 * This command returns SL_STATUS_BT_CTRL_INSUFFICIENT_CHANNELS if there are
 * less than 15 channels available.
 *
 * @param[in] connection The connection handle
 * @param[in] enable Enum @ref sl_bt_cs_procedure_state_t. Enabled or disabled
 *   CS procedure state. Values:
 *     - <b>sl_bt_cs_procedure_state_disabled (0x0):</b> CS procedures are
 *       disabled
 *     - <b>sl_bt_cs_procedure_state_enabled (0x1):</b> CS procedures are
 *       enabled
 * @param[in] config_id CS configuration identifier
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_procedure_enable_complete - Triggered when local
 *     controller has scheduled or disabled an CS procedure measurement
 *   - @ref sl_bt_evt_cs_result - Triggered when local controller has results to
 *     report for every CS event within the CS procedure
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_procedure_enable(uint8_t connection,
                                      uint8_t enable,
                                      uint8_t config_id);

/***************************************************************************//**
 *
 * Set the antenna configuration for the CS feature.
 *
 * @param[in] antenna_element_offset_len Length of data in @p
 *   antenna_element_offset
 * @param[in] antenna_element_offset Offset of each antenna, each element is a
 *   signed 16-bit integer stored in two consecutive bytes in little-endian
 *   order. Units: 1 cm.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_set_antenna_configuration(size_t antenna_element_offset_len,
                                               const uint8_t* antenna_element_offset);

/***************************************************************************//**
 *
 * Read the CS capabilities of the local controller.
 *
 * @param[out] num_config The number of CS configurations supported per
 *   connection.
 *     - <b>Range:</b> 1 to 4
 * @param[out] max_consecutive_procedures @parblock
 *   The maximum number of consecutive CS procedures supported.
 *     - <b>Range:</b> 1 to 65535
 *
 *     - <b>0x00:</b> Fixed number of consecutive procedures and for an
 *       indefinite number of procedures until termination
 *   @endparblock
 * @param[out] num_antennas The number of antenna elements available for CS tone
 *   exchanges.
 *     - <b>Range:</b> 1 to 4
 * @param[out] max_antenna_paths The maximum number of antenna paths supported.
 *     - <b>Range:</b> 1 to 4
 * @param[out] roles This value is a bitmask of flags to indicate which CS roles
 *   are supported by the controller. Flags:
 *     - <b>0x01, bit 0:</b> Initiator role
 *     - <b>0x02, bit 1:</b> Reflector role
 * @param[out] modes This value is a bitmask of flags to indicate which Optional
 *   CS modes are supported. Flags:
 *     - <b>0x01, bit 0:</b> Mode 3 is supported
 * @param[out] rtt_capability This value is a bitmask of flags to indicate which
 *   Round Trip Time (RTT) CS capabilities accuracy requirement in @p
 *   rtt_aa_only_n, @p rtt_sounding_n, and @p rtt_random_payload. Flags:
 *     - <b>0x01, bit 0:</b> 10 ns time-of-flight (ToF) precision requirement
 *       for @p rtt_aa_only if set; otherwise 150 ns ToF
 *     - <b>0x02, bit 1:</b> 10 ns ToF precision requirement for @p rtt_sounding
 *       if set; otherwise 150 ns ToF
 *     - <b>0x04, bit 2:</b> 10 ns ToF precision requirement for @p
 *       rtt_random_payload if set; otherwise 150 ns ToF
 * @param[out] rtt_aa_only @parblock
 *   The support status of RTT Access Address(AA) in the local controller. When
 *   supported it establishes the number of SYNC changes needed to satisfy the
 *   precision requirements.
 *     - <b>Range:</b> 1 to 255
 *
 *     - 0x00: RTT AA Only is not supported
 *   @endparblock
 * @param[out] rtt_sounding @parblock
 *   The support status of RTT Sounding is supported in the local controller.
 *   When supported it establishes the number of SYNC changes needed to satisfy
 *   the precision requirements.
 *     - <b>Range:</b> 1 to 255
 *
 *     - 0x00: RTT Sounding is not supported
 *   @endparblock
 * @param[out] rtt_random_payload @parblock
 *   The support status of RTT Random Payload in the local controller. When
 *   supported it establishes the number of SYNC changes needed to satisfy the
 *   precision requirements.
 *     - <b>Range:</b> 1 to 255
 *
 *     - 0x00: RTT Random Payload is not supported
 *   @endparblock
 * @param[out] cs_sync_phys This value is a bitmask of flags to indicate which
 *   CS_SYNC packages supported in an specific PHY. Flags:
 *     - <b>0x02, bit 1:</b> LE 2M PHY CS_SYNC packages are supported
 *     - <b>0x04, bit 2:</b> LE 2M 2BT PHY CS_SYNC packages are supported
 * @param[out] subfeatures This value is a bitmask of flags to indicate which
 *   optional CS subfeatures are supported. Flags:
 *     - <b>0x01, bit 0:</b> CS with zero Frequency Actuation Error relative to
 *       Mode 0 transmissions in reflector role is supported
 *     - <b>0x02, bit 1:</b> CS Channel Selection Algorithm #3c is supported
 *     - <b>0x04, bit 2:</b> CS phase-based ranging from a sounding sequence is
 *       supported
 * @param[out] t_ip1_times This value is a bitmask of flags to indicate which
 *   time durations of Time for Interlude Period 1 (IP1) are supported. Flags:
 *     - <b>0x0001, bit 0:</b> Time durantion of 10 microseconds is supported
 *     - <b>0x0002, bit 1:</b> Time durantion of 20 microseconds is supported
 *     - <b>0x0004, bit 2:</b> Time durantion of 30 microseconds is supported
 *     - <b>0x0008, bit 3:</b> Time durantion of 40 microseconds is supported
 *     - <b>0x0010, bit 4:</b> Time durantion of 50 microseconds is supported
 *     - <b>0x0020, bit 5:</b> Time durantion of 60 microseconds is supported
 *     - <b>0x0040, bit 6:</b> Time durantion of 80 microseconds is supported
 * @param[out] t_ip2_times This value is a bitmask of flags to indicate which
 *   time durations of Time for Interlude Period 2 (IP2) are supported. Flags:
 *     - <b>0x0001, bit 0:</b> Time duration of 10 microseconds is supported
 *     - <b>0x0002, bit 1:</b> Time duration of 20 microseconds is supported
 *     - <b>0x0004, bit 2:</b> Time duration of 30 microseconds is supported
 *     - <b>0x0008, bit 3:</b> Time duration of 40 microseconds is supported
 *     - <b>0x0010, bit 4:</b> Time duration of 50 microseconds is supported
 *     - <b>0x0020, bit 5:</b> Time duration of 60 microseconds is supported
 *     - <b>0x0040, bit 6:</b> Time duration of 80 microseconds is supported
 * @param[out] t_fcs_times This value is a bitmask of flags to indicate which
 *   durations of Time for Frequency Change Spacing (FCS) are supported. Flags:
 *     - <b>0x0001, bit 0:</b> Time duration of 10 microseconds is supported
 *     - <b>0x0002, bit 1:</b> Time duration of 20 microseconds is supported
 *     - <b>0x0004, bit 2:</b> Time duration of 30 microseconds is supported
 *     - <b>0x0008, bit 3:</b> Time duration of 40 microseconds is supported
 *     - <b>0x0010, bit 4:</b> Time duration of 50 microseconds is supported
 *     - <b>0x0020, bit 5:</b> Time duration of 60 microseconds is supported
 *     - <b>0x0040, bit 6:</b> Time duration of 80 microseconds is supported
 *     - <b>0x0100, bit 7:</b> Time duration of 100 microseconds is supported
 *     - <b>0x0200, bit 8:</b> Time duration of 120 microseconds is supported
 * @param[out] t_pm_times This value is a bitmask of flags to indicate which
 *   time durations of Time for Phase Measurement (PM) are supported. Flags:
 *     - <b>0x0001, bit 0:</b> Time duration of 10 microseconds is supported
 *     - <b>0x0002, bit 1:</b> Time duration of 20 microseconds is supported
 * @param[out] t_sw_times Time in microseconds for the antenna switch period of
 *   the CS tones.
 *     - Values: 0x00, 0x01, 0x02, 0x04 and 0x0A
 * @param[out] tx_snr_capability This value is a bitmask of flags to indicate
 *   the supported SNR levels used in RTT packets. Flags:
 *     - <b>0x01, bit 0:</b> 18 dB supported
 *     - <b>0x02, bit 1:</b> 21 dB supported
 *     - <b>0x04, bit 2:</b> 24 dB supported
 *     - <b>0x08, bit 3:</b> 27 dB supported
 *     - <b>0x10, bit 4:</b> 30 dB supported
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_read_local_supported_capabilities(uint8_t *num_config,
                                                       uint16_t *max_consecutive_procedures,
                                                       uint8_t *num_antennas,
                                                       uint8_t *max_antenna_paths,
                                                       uint8_t *roles,
                                                       uint8_t *modes,
                                                       uint8_t *rtt_capability,
                                                       uint8_t *rtt_aa_only,
                                                       uint8_t *rtt_sounding,
                                                       uint8_t *rtt_random_payload,
                                                       uint8_t *cs_sync_phys,
                                                       uint16_t *subfeatures,
                                                       uint16_t *t_ip1_times,
                                                       uint16_t *t_ip2_times,
                                                       uint16_t *t_fcs_times,
                                                       uint16_t *t_pm_times,
                                                       uint8_t *t_sw_times,
                                                       uint8_t *tx_snr_capability);

/***************************************************************************//**
 *
 * Read the Channel Sounding (CS) capabilities of the remote controller.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_read_remote_supported_capabilities_complete - Triggered
 *     when local controller has the requested supported capabilities of the
 *     remote controller.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_read_remote_supported_capabilities(uint8_t connection);

/** @} */ // end addtogroup sl_bt_cs

/**
 * @addtogroup sl_bt_cs_test Channel Sounding Test
 * @{
 *
 * @brief Channel Sounding Test
 *
 * This class provides optional test commands and events for CS between
 * Bluetooth devices.
 */

/* Command and Response IDs */
#define sl_bt_cmd_cs_test_start_id                                   0x005a0020
#define sl_bt_cmd_cs_test_end_id                                     0x015a0020
#define sl_bt_rsp_cs_test_start_id                                   0x005a0020
#define sl_bt_rsp_cs_test_end_id                                     0x015a0020

/**
 * @brief Defines tone extension for CS test
 */
typedef enum
{
  sl_bt_cs_test_tone_extension_both_without      = 0x0, /**< (0x0) Initiator and
                                                             Reflector tones
                                                             sent without tone
                                                             extension */
  sl_bt_cs_test_tone_extension_reflector_without = 0x1, /**< (0x1) Initiator
                                                             tone sent with
                                                             extension;
                                                             Reflector tone sent
                                                             without extension */
  sl_bt_cs_test_tone_extension_initiator_without = 0x2, /**< (0x2) Initiator
                                                             tone sent without
                                                             extension;
                                                             Reflector tone sent
                                                             with extension */
  sl_bt_cs_test_tone_extension_both_with         = 0x3, /**< (0x3) Initiator and
                                                             Reflector tones
                                                             sent with extension */
  sl_bt_cs_test_tone_extension_round_robin       = 0x4  /**< (0x4) Loop through
                                                             values 0x00 to 0x03 */
} sl_bt_cs_test_tone_extension_t;

/**
 * @brief This defines sounding sequence marker for CS test
 */
typedef enum
{
  sl_bt_cs_test_sounding_sequence_marker_1           = 0x0, /**< (0x0) Use
                                                                 0b0011 as the
                                                                 Sounding
                                                                 Sequence marker */
  sl_bt_cs_test_sounding_sequence_marker_2           = 0x1, /**< (0x1) Use
                                                                 0b1100 as the
                                                                 Sounding
                                                                 Sequence marker */
  sl_bt_cs_test_sounding_sequence_marker_round_robin = 0x2  /**< (0x2) Loop
                                                                 through 0b0011
                                                                 and 0b1100 */
} sl_bt_cs_test_sounding_sequence_marker_t;

/**
 * @addtogroup sl_bt_evt_cs_test_end_completed sl_bt_evt_cs_test_end_completed
 * @{
 * @brief This event is generated when the local controller stops the ongoing CS
 * test as the result of calling the @ref sl_bt_cs_test_end command
 */

/** @brief Identifier of the end_completed event */
#define sl_bt_evt_cs_test_end_completed_id                           0x005a00a0

/***************************************************************************//**
 * @brief Data structure of the end_completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cs_test_end_completed_s
{
  uint16_t status; /**< SL_STATUS_OK if successful. Error code otherwise. */
});

typedef struct sl_bt_evt_cs_test_end_completed_s sl_bt_evt_cs_test_end_completed_t;

/** @} */ // end addtogroup sl_bt_evt_cs_test_end_completed

/***************************************************************************//**
 *
 * Start a single CS procedure using the given configuration. The reflector must
 * be initialized before starting the initiator. To stop an ongoing test, use
 * the @ref sl_bt_test_dtm_end command.
 *
 * @param[in] main_mode_type Enum @ref sl_bt_cs_mode_t. Main mode type. Values :
 *     - <b>sl_bt_cs_mode_rtt (0x1):</b> Round Trip Time (RTT) measurement
 *     - <b>sl_bt_cs_mode_pbr (0x2):</b> Phase-Based Ranging (PBR) measurement
 *     - <b>sl_bt_cs_submode_disabled (0xff):</b> Submode disabled for the
 *       procedure.
 * @param[in] sub_mode_type Enum @ref sl_bt_cs_mode_t. Sub mode type. Values:
 *     - <b>sl_bt_cs_mode_rtt (0x1):</b> Round Trip Time (RTT) measurement
 *     - <b>sl_bt_cs_mode_pbr (0x2):</b> Phase-Based Ranging (PBR) measurement
 *     - <b>sl_bt_cs_submode_disabled (0xff):</b> Submode disabled for the
 *       procedure.
 * @param[in] main_mode_repetition Number of main mode steps taken from the end
 *   of the last CS subevent to be repeated at the beginning of the current CS
 *   subevent directly after the last Mode 0 step of that event.
 *     - <b>Range:</b> 0 to 3
 * @param[in] mode_calibration_steps Number of calibration mode steps to be
 *   included at the beginning of the test CS subevent
 *     - <b>Range:</b> 1 to 3
 * @param[in] role Enum @ref sl_bt_cs_role_t. Role during CS procedure Values:
 *     - <b>sl_bt_cs_role_initiator (0x0):</b> The device will initiate the
 *       procedure
 *     - <b>sl_bt_cs_role_reflector (0x1):</b> The device will reciprocate
 *       transmission
 * @param[in] rtt_type Enum @ref sl_bt_cs_rtt_type_t. RTT payload type used in
 *   the CS procedure Values:
 *     - <b>sl_bt_cs_rtt_type_aa_only (0x0):</b> RTT Access Address (AA) only
 *     - <b>sl_bt_cs_rtt_type_fractional_32_bit_sounding (0x1):</b> RTT
 *       Fractional with 32-bit Sounding Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_96_bit_sounding (0x2):</b> RTT
 *       Fractional with 96-bit Sounding Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_32_bit_random (0x3):</b> RTT Fractional
 *       with 32-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_64_bit_random (0x4):</b> RTT Fractional
 *       with 64-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_96_bit_random (0x5):</b> RTT Fractional
 *       with 96-bit Random Sequence
 *     - <b>sl_bt_cs_rtt_type_fractional_128_bit_random (0x6):</b> RTT
 *       Fractional with 128-bit Random Sequence
 * @param[in] cs_sync_phy Enum @ref sl_bt_gap_phy_t. Used PHY for CS_SYNC
 *   exchanges during a procedure Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 *     - <b>sl_bt_gap_phy_any (0xff):</b> Any PHYs the device supports
 * @param[in] antenna_selection @parblock
 *   Antenna Identifier to be used for RTT packets
 *     - <b>Range:</b> 1 to 4
 *
 *     - Value: 0xFE. Use antennas in repetitive order from 0x01 to 0x04
 *   @endparblock
 * @param[in] subevent_len @parblock
 *   CS subevent length in units of microseconds.
 *     - <b>Range:</b> 1250 to 3999999
 *
 *     - Time range: 1250 us to 4s
 *   @endparblock
 * @param[in] subevent_interval Interval between the start of two consecutive CS
 *   events. Units: 0.625 ms.
 *     - Value: 0x0000. Single CS subevent
 * @param[in] max_num_subevents @parblock
 *   The maximum allowed number of subevents in the procedure.
 *     - Value: 0x00. This parameter is ignored when determining the number of
 *       subevents in the procedure
 *
 *     - <b>Range:</b> 1 to 32
 *   @endparblock
 * @param[in] tx_power @parblock
 *   Transmit power level for the transmission. Units: dBm.
 *     - <b>Range:</b> -127 to +20
 *
 *     - Value: 0x7E. Set transmitter to minimum transmit power level
 *     - Value: 0x7F. Set transmitter to maximum transmit power level
 *   @endparblock
 * @param[in] t_ip1_time Idle time in microseconds between the RTT packets
 *     - Values: 10, 20, 30, 40, 50, 60, 80 or 145
 * @param[in] t_ip2_time Interlude time in microseconds between the CS tones
 *     - Values: 10, 20, 30, 40, 50, 60, 80 or 145
 * @param[in] t_fcs_time Time in microseconds for frequency changes
 *     - Values: 15, 20, 30, 40, 50, 60, 80, 100, 120 or 150
 * @param[in] t_pm_time Time in microseconds for the phase measurement period of
 *   the CS tones
 *     - Values: 10, 20 or 40
 * @param[in] t_sw_time Time in microseconds for the antenna switch period of
 *   the CS tones
 *     - Values: 0, 1, 2, 4 or 10
 * @param[in] tone_antenna_config Antenna Configuration Index used during
 *   antenna switching
 *     - <b>Range:</b> 0 to 7
 * @param[in] reserved Reserved for future use.
 * @param[in] snr_control_initiator Enum @ref sl_bt_cs_snr_control_adjustment_t.
 *   The Signal Noise Ratio (SNR) control adjustment for the CS_SYNC
 *   transmissions of the initiator. Values:
 *     - <b>sl_bt_cs_snr_control_adjustment_not_applied (0xff):</b> SNR control
 *       adjustment not to be applied
 * @param[in] snr_control_reflector Enum @ref sl_bt_cs_snr_control_adjustment_t.
 *   The Signal Noise Ratio (SNR) control adjustment for the CS_SYNC
 *   transmissions of the reflector. Values:
 *     - <b>sl_bt_cs_snr_control_adjustment_not_applied (0xff):</b> SNR control
 *       adjustment not to be applied
 * @param[in] drbg_nonce The DRBG_Nonce value determines octets 14 and 15 of the
 *   initial value of the DRBG nonce.
 * @param[in] channel_map_repetition Number of times the channel_map field will
 *   be cycled through for non-Mode 0 steps within an CS procedure.
 *     - <b>Range:</b> 1 to 255
 * @param[in] override_config Configuration of the parameters in
 *   override_parameters. Please refer to the Bluetooth Specification document
 *   on the HCI_CS_Test comamnd for more information on the override
 *   configuration details.
 * @param[in] override_parameters_len Length of data in @p override_parameters
 * @param[in] override_parameters Variable set of parameters. Please refer to
 *   the Bluetooth Specification document on the HCI_CS_Test comamnd for more
 *   information on the override parameters details.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_result - Triggered when local controller has results to
 *     report for every CS subevent within the CS procedure
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_test_start(uint8_t main_mode_type,
                                uint8_t sub_mode_type,
                                uint8_t main_mode_repetition,
                                uint8_t mode_calibration_steps,
                                uint8_t role,
                                uint8_t rtt_type,
                                uint8_t cs_sync_phy,
                                uint8_t antenna_selection,
                                const sl_bt_cs_subevent_length_t *subevent_len,
                                uint16_t subevent_interval,
                                uint8_t max_num_subevents,
                                int8_t tx_power,
                                uint8_t t_ip1_time,
                                uint8_t t_ip2_time,
                                uint8_t t_fcs_time,
                                uint8_t t_pm_time,
                                uint8_t t_sw_time,
                                uint8_t tone_antenna_config,
                                uint8_t reserved,
                                uint8_t snr_control_initiator,
                                uint8_t snr_control_reflector,
                                uint16_t drbg_nonce,
                                uint8_t channel_map_repetition,
                                uint16_t override_config,
                                size_t override_parameters_len,
                                const uint8_t* override_parameters);

/***************************************************************************//**
 *
 * Stops any ongoing CS test. When the local controller has sent all the
 * remaining subevents for the ongoing procedure it will generate an @ref
 * sl_bt_evt_cs_test_end_completed event.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cs_test_end_completed - Triggered when local controller
 *     has stopped the CS procedure started with the @ref sl_bt_cs_test_start
 *     command.
 *
 ******************************************************************************/
sl_status_t sl_bt_cs_test_end();

/** @} */ // end addtogroup sl_bt_cs_test

/**
 * @addtogroup sl_bt_l2cap L2CAP Connection Oriented Channels
 * @{
 *
 * @brief L2CAP Connection Oriented Channels
 *
 * The commands and events in this class provide Logical Link Control and
 * Adaptation Protocol (L2CAP) credit-based logical channels.
 *
 * An L2CAP credit-based logical channel is a logical link identified by a
 * channel identifier in the assigned name space 0x40-0x7F for LE-U logical
 * links (the @p cid parameter in the commands and events of this API class).
 * These channels use a credit-based flow control mechanism. The credit can be
 * configured at the channel opening and later dynamically updated on the
 * channel.
 *
 * The Simplified Protocol/Service Multiplexer (SPSM) of a channel specifies the
 * protocol or services the channel implements. It can be a value for a fixed
 * service assigned by the Bluetooth SIG or a dynamically-allocated value and
 * used with services defined in the GATT Server. The dynamically-assigned value
 * may be used to support multiple implementations of a particular protocol. See
 * the Bluetooth core specification for more details. The application specifies
 * the SPSM value in a channel open request in the @p spsm parameter of the @ref
 * sl_bt_l2cap_open_le_channel command.
 *
 * Each of the local and peer channel endpoints must specify the maximum Service
 * Data Unit (SDU) size (the 'MTU' field of the L2CAP packets in the Bluetooth
 * Core specification) that it can receive on the channel. The application
 * specifies the maximum Service Data Unit size (the @p max_sdu parameter in
 * commands and events) of the local channel endpoint.
 *
 * Each channel endpoint has a maximum PDU payload Size that the L2CAP layer can
 * receive in a single data packet on the channel. The maximum PDU payload size
 * supported by the stack for local channel endpoints is 252 bytes, which is the
 * maximum data length the @ref sl_bt_evt_l2cap_channel_data event can support.
 * The application specifies the max PDU payload size (the @p max_pdu parameter
 * in commands and events). Additionally, the application is responsible for
 * handling the segmentation from SDU to PDU and reassembly from PDU to SDU.
 */

/* Command and Response IDs */
#define sl_bt_cmd_l2cap_open_le_channel_id                           0x01430020
#define sl_bt_cmd_l2cap_send_le_channel_open_response_id             0x02430020
#define sl_bt_cmd_l2cap_channel_send_data_id                         0x03430020
#define sl_bt_cmd_l2cap_channel_send_credit_id                       0x04430020
#define sl_bt_cmd_l2cap_close_channel_id                             0x05430020
#define sl_bt_rsp_l2cap_open_le_channel_id                           0x01430020
#define sl_bt_rsp_l2cap_send_le_channel_open_response_id             0x02430020
#define sl_bt_rsp_l2cap_channel_send_data_id                         0x03430020
#define sl_bt_rsp_l2cap_channel_send_credit_id                       0x04430020
#define sl_bt_rsp_l2cap_close_channel_id                             0x05430020

/**
 * @brief Defines possible result values in the responses to credit based
 * channel connection requests.
 */
typedef enum
{
  sl_bt_l2cap_connection_result_successful                    = 0x0, /**< (0x0)
                                                                          Connection
                                                                          successful */
  sl_bt_l2cap_connection_result_spsm_not_supported            = 0x2, /**< (0x2)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          SPSM
                                                                          not
                                                                          supported */
  sl_bt_l2cap_connection_result_no_resources_available        = 0x4, /**< (0x4)
                                                                          Connection
                                                                          refused
                                                                          \- no
                                                                          resources
                                                                          available */
  sl_bt_l2cap_connection_result_insufficient_authentication   = 0x5, /**< (0x5)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          insufficient
                                                                          authentication */
  sl_bt_l2cap_connection_result_insufficient_authorization    = 0x6, /**< (0x6)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          insufficient
                                                                          authorization */
  sl_bt_l2cap_connection_result_encryption_key_size_too_short = 0x7, /**< (0x7)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          encryption
                                                                          key
                                                                          size
                                                                          too
                                                                          short */
  sl_bt_l2cap_connection_result_insufficient_encryption       = 0x8, /**< (0x8)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          insufficient
                                                                          encryption */
  sl_bt_l2cap_connection_result_invalid_source_cid            = 0x9, /**< (0x9)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          invalid
                                                                          Source
                                                                          CID */
  sl_bt_l2cap_connection_result_source_cid_already_allocated  = 0xa, /**< (0xa)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          Source
                                                                          CID
                                                                          already
                                                                          allocated */
  sl_bt_l2cap_connection_result_unacceptable_parameters       = 0xb  /**< (0xb)
                                                                          Connection
                                                                          refused
                                                                          \-
                                                                          unacceptable
                                                                          parameters */
} sl_bt_l2cap_connection_result_t;

/**
 * @brief Describes why a request command was rejected.
 */
typedef enum
{
  sl_bt_l2cap_command_not_understood = 0x0, /**< (0x0) Command not understood
                                                 e.g., unknown command code */
  sl_bt_l2cap_signaling_mtu_exceeded = 0x1, /**< (0x1) Command size has exceeded
                                                 the signaling MTU size */
  sl_bt_l2cap_invalid_cid_request    = 0x2  /**< (0x2) An invalid CID is
                                                 included in the command */
} sl_bt_l2cap_command_reject_reason_t;

/**
 * @brief Describes which of the request commands has been rejected.
 */
typedef enum
{
  sl_bt_l2cap_disconnection_request = 0x6,  /**< (0x6) A disconnection request */
  sl_bt_l2cap_le_connection_request = 0x14, /**< (0x14) An LE credit-based
                                                 connection request */
  sl_bt_l2cap_flow_control_credit   = 0x16  /**< (0x16) A Flow Control Credit
                                                 packet */
} sl_bt_l2cap_command_code_t;

/**
 * @addtogroup sl_bt_evt_l2cap_le_channel_open_request sl_bt_evt_l2cap_le_channel_open_request
 * @{
 * @brief Indicates that an LE credit-based connection request on a Bluetooth
 * connection is received
 *
 * The application must respond with the @ref
 * sl_bt_l2cap_send_le_channel_open_response command.
 */

/** @brief Identifier of the le_channel_open_request event */
#define sl_bt_evt_l2cap_le_channel_open_request_id                   0x014300a0

/***************************************************************************//**
 * @brief Data structure of the le_channel_open_request event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_le_channel_open_request_s
{
  uint8_t  connection; /**< The connection handle */
  uint16_t spsm;       /**< The protocol/services implemented by the peer
                            channel endpoint */
  uint16_t cid;        /**< The channel identifier on the local device */
  uint16_t max_sdu;    /**< The Maximum Service Data Unit size of the peer
                            channel endpoint */
  uint16_t max_pdu;    /**< The maximum PDU payload size of the peer channel
                            endpoint */
  uint16_t credit;     /**< The initial credit value of the peer channel
                            endpoint, i.e., number of PDUs that the local
                            channel endpoint can send */
  uint16_t remote_cid; /**< The channel identifier on the peer device */
});

typedef struct sl_bt_evt_l2cap_le_channel_open_request_s sl_bt_evt_l2cap_le_channel_open_request_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_le_channel_open_request

/**
 * @addtogroup sl_bt_evt_l2cap_le_channel_open_response sl_bt_evt_l2cap_le_channel_open_response
 * @{
 * @brief Indicates that an LE credit-based connection response is received
 *
 * Result code @ref sl_bt_l2cap_connection_result_successful in @p errorcode
 * implies that the logical channel is established and data can be sent or
 * received on the channel.
 *
 * If the connection request was rejected by the peer, indicated by @p
 * errorcode, the stack automatically closes the local channel and the user
 * application should clean up the resources associated to the channel
 * identifier.
 */

/** @brief Identifier of the le_channel_open_response event */
#define sl_bt_evt_l2cap_le_channel_open_response_id                  0x024300a0

/***************************************************************************//**
 * @brief Data structure of the le_channel_open_response event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_le_channel_open_response_s
{
  uint8_t  connection; /**< The Bluetooth connection handle on which the
                            response is received */
  uint16_t cid;        /**< The channel identifier on the local device */
  uint16_t max_sdu;    /**< The Maximum Service Data Unit size of the peer
                            channel endpoint */
  uint16_t max_pdu;    /**< The maximum PDU payload size of the peer channel
                            endpoint */
  uint16_t credit;     /**< The initial credit value of the peer channel
                            endpoint, i.e., number of PDUs the local channel
                            endpoint can send */
  uint16_t errorcode;  /**< Enum @ref sl_bt_l2cap_connection_result_t.

                            An L2CAP error code as the outcome of the connection
                            request.

                            Result @ref sl_bt_l2cap_connection_result_successful
                            indicates the connection request was accepted and
                            the logical channel is established. Other error code
                            value indicates the connection request was refused
                            by the peer device and other parameters of this
                            event must be ignored. */
  uint16_t remote_cid; /**< The channel identifier on the peer device */
});

typedef struct sl_bt_evt_l2cap_le_channel_open_response_s sl_bt_evt_l2cap_le_channel_open_response_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_le_channel_open_response

/**
 * @addtogroup sl_bt_evt_l2cap_channel_data sl_bt_evt_l2cap_channel_data
 * @{
 * @brief Indicates that data is received on a channel
 *
 * Each event should contain a K-frame of payload. The length of SDU should be
 * specified in the first 2 bytes of the first K-frame. The sum of the payload
 * lengths in received K-frames should be equal to the specified SDU length.
 * Otherwise, application should disconnect the channel using @ref
 * sl_bt_l2cap_close_channel command. The same should happen also if the SDU
 * length exceeds the max_sdu of local channel endpoint.
 */

/** @brief Identifier of the channel_data event */
#define sl_bt_evt_l2cap_channel_data_id                              0x034300a0

/***************************************************************************//**
 * @brief Data structure of the channel_data event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_channel_data_s
{
  uint8_t    connection; /**< The connection handle */
  uint16_t   cid;        /**< The channel identifier */
  uint8array data;       /**< Data received */
});

typedef struct sl_bt_evt_l2cap_channel_data_s sl_bt_evt_l2cap_channel_data_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_channel_data

/**
 * @addtogroup sl_bt_evt_l2cap_channel_credit sl_bt_evt_l2cap_channel_credit
 * @{
 * @brief Indicates that flow control credits are received on a channel
 * informing that the peer channel endpoint is capable of receiving more data
 */

/** @brief Identifier of the channel_credit event */
#define sl_bt_evt_l2cap_channel_credit_id                            0x044300a0

/***************************************************************************//**
 * @brief Data structure of the channel_credit event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_channel_credit_s
{
  uint8_t  connection; /**< The Bluetooth connection handle on which the credit
                            is received */
  uint16_t cid;        /**< The channel identifier */
  uint16_t credit;     /**< The credit value, i.e., the additional number of
                            PDUs the peer channel endpoint can receive */
});

typedef struct sl_bt_evt_l2cap_channel_credit_s sl_bt_evt_l2cap_channel_credit_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_channel_credit

/**
 * @addtogroup sl_bt_evt_l2cap_channel_closed sl_bt_evt_l2cap_channel_closed
 * @{
 * @brief Indicates that a credit-based logical channel is closed by the local
 * or peer device.
 */

/** @brief Identifier of the channel_closed event */
#define sl_bt_evt_l2cap_channel_closed_id                            0x054300a0

/***************************************************************************//**
 * @brief Data structure of the channel_closed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_channel_closed_s
{
  uint8_t  connection; /**< The connection handle */
  uint16_t cid;        /**< The channel identifier */
  uint16_t reason;     /**< An sl_status_t code describing disconnection reason */
});

typedef struct sl_bt_evt_l2cap_channel_closed_s sl_bt_evt_l2cap_channel_closed_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_channel_closed

/**
 * @addtogroup sl_bt_evt_l2cap_command_rejected sl_bt_evt_l2cap_command_rejected
 * @{
 * @brief Indicates that the peer device rejected a command
 *
 * This might happen if, for example, the device does not support the feature or
 * the command has an incorrect CID.
 */

/** @brief Identifier of the command_rejected event */
#define sl_bt_evt_l2cap_command_rejected_id                          0x064300a0

/***************************************************************************//**
 * @brief Data structure of the command_rejected event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_l2cap_command_rejected_s
{
  uint8_t  connection; /**< The Bluetooth connection handle on which the event
                            is received */
  uint8_t  code;       /**< Enum @ref sl_bt_l2cap_command_code_t. Code of the
                            rejected command */
  uint16_t reason;     /**< Enum @ref sl_bt_l2cap_command_reject_reason_t. The
                            rejection reason */
  uint16_t cid;        /**< The channel identifier */
});

typedef struct sl_bt_evt_l2cap_command_rejected_s sl_bt_evt_l2cap_command_rejected_t;

/** @} */ // end addtogroup sl_bt_evt_l2cap_command_rejected

/***************************************************************************//**
 *
 * Create and configure an L2CAP channel on a Bluetooth connection using the LE
 * credit based connection request packet. Event @ref
 * sl_bt_evt_l2cap_le_channel_open_response or @ref
 * sl_bt_evt_l2cap_command_rejected will be received after the peer device
 * responded to the request.
 *
 * @param[in] connection The connection handle
 * @param[in] spsm The protocol/services implemented by the local channel
 *   endpoint
 * @param[in] max_sdu @parblock
 *   The Maximum Service Data Unit size the local channel endpoint can accept
 *
 *     - <b>Range:</b> 23 to 65533
 *   @endparblock
 * @param[in] max_pdu @parblock
 *   The maximum PDU payload size the local channel endpoint can accept
 *
 *     - <b>Range:</b> 23 to 252
 *   @endparblock
 * @param[in] credit The initial credit value of the local channel endpoint,
 *   i.e., number of PDUs the peer channel endpoint can send
 * @param[out] cid The channel identifier
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_l2cap_command_rejected - Triggered when the peer device
 *     rejected the connection request. Typically a legacy peer device that does
 *     not support the feature could send this response. When this event is
 *     received under the context of opening a channel using this command, the
 *     stack automatically closes the local channel and the user application
 *     should clean up the resources associated to the channel identifier.
 *   - @ref sl_bt_evt_l2cap_le_channel_open_response - Triggered when an LE
 *     credit-based connection response has been received in response to this
 *     command. If the result code in @p errorcode of the event is not equal to
 *     @ref sl_bt_l2cap_connection_result_successful, it means that connection
 *     request was rejected by the peer device. In this case, the stack
 *     automatically closes the local channel and the user application should
 *     clean up the resources associated to the channel identifier.
 *   - @ref sl_bt_evt_l2cap_channel_closed - Triggered when an LE credit-based
 *     connection response has not been received within 30 seconds after this
 *     command has been issued.
 *
 ******************************************************************************/
sl_status_t sl_bt_l2cap_open_le_channel(uint8_t connection,
                                        uint16_t spsm,
                                        uint16_t max_sdu,
                                        uint16_t max_pdu,
                                        uint16_t credit,
                                        uint16_t *cid);

/***************************************************************************//**
 *
 * Send an LE credit-based connection response to an LE credit-based connection
 * request received in the @ref sl_bt_evt_l2cap_le_channel_open_request event.
 * Result code @ref sl_bt_l2cap_connection_result_successful in @p errorcode
 * implies that the logical channel is established and data can be sent or
 * received on the channel.
 *
 * To reject the connection request, use a result code other than
 * sl_bt_l2cap_connection_result_successful in @p errorcode. Other parameters in
 * this command are ignored in this case. The stack automatically closes the
 * local channel and the user application should clean up the resources
 * associated to the channel identifier.
 *
 * @param[in] connection The connection handle
 * @param[in] cid The channel identifier
 * @param[in] max_sdu @parblock
 *   The Maximum Service Data Unit size the local channel endpoint can accept
 *
 *     - <b>Range:</b> 23 to 65533
 *   @endparblock
 * @param[in] max_pdu @parblock
 *   The maximum PDU payload size the local channel endpoint can accept
 *
 *     - <b>Range:</b> 23 to 252
 *   @endparblock
 * @param[in] credit The initial credit value of the local channel endpoint,
 *   i.e., number of PDUs that the peer channel endpoint can send
 * @param[in] errorcode Enum @ref sl_bt_l2cap_connection_result_t. An L2CAP
 *   error code as the outcome of the connection request
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_l2cap_send_le_channel_open_response(uint8_t connection,
                                                      uint16_t cid,
                                                      uint16_t max_sdu,
                                                      uint16_t max_pdu,
                                                      uint16_t credit,
                                                      uint16_t errorcode);

/***************************************************************************//**
 *
 * Send data to the peer channel endpoint on a Bluetooth connection. If the SDU
 * length is larger than max_pdu size or 250 bytes, the application should
 * fragment the SDU into multiple K-frames and call this command once for each
 * K-frame.
 *
 * The first K-frame of the SDU should start with 2 bytes of SDU length followed
 * with payload. Therefore, the first K-frame contains PDU length minus 2 bytes
 * of the actual payload. All subsequent K-frames of the same SDU contain PDU
 * length of the payload. The sum of the payload lengths for K-frames should be
 * equal to the specified SDU length.
 *
 * @param[in] connection The connection handle
 * @param[in] cid The channel identifier
 * @param[in] data_len Length of data in @p data
 * @param[in] data Data to be sent. The data length must not be greater than the
 *   lesser of the peer channel endpoint max_pdu and 252 bytes, which is the
 *   maximum data length this command can send.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_l2cap_channel_send_data(uint8_t connection,
                                          uint16_t cid,
                                          size_t data_len,
                                          const uint8_t* data);

/***************************************************************************//**
 *
 * Send flow control credits to the peer channel endpoint indicating that the
 * local channel endpoint is capable of receiving more data.
 *
 * @param[in] connection The connection handle
 * @param[in] cid The channel identifier
 * @param[in] credit @parblock
 *   The credit value, i.e., the additional number of PDUs the peer channel
 *   endpoint can send
 *
 *     - <b>Range:</b> 1 to 65535
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_l2cap_channel_send_credit(uint8_t connection,
                                            uint16_t cid,
                                            uint16_t credit);

/***************************************************************************//**
 *
 * Send a disconnect request to close a credit-based logical channel.
 *
 * @param[in] connection The connection handle
 * @param[in] cid The channel identifier
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_l2cap_channel_closed - Triggered when an credit-based
 *     logical channel is closed in response to this command.
 *
 ******************************************************************************/
sl_status_t sl_bt_l2cap_close_channel(uint8_t connection, uint16_t cid);

/** @} */ // end addtogroup sl_bt_l2cap

/**
 * @addtogroup sl_bt_cte_transmitter CTE Transmitter
 * @{
 *
 * @brief CTE Transmitter
 *
 * Commands and events in this class manage Constant Tone Extension (CTE)
 * transmission.
 *
 * CTE feature is only supported by specific devices. Commands from this class
 * will return SL_STATUS_NOT_SUPPORTED on devices that do not support CTE.
 */

/* Command and Response IDs */
#define sl_bt_cmd_cte_transmitter_set_dtm_parameters_id              0x04440020
#define sl_bt_cmd_cte_transmitter_clear_dtm_parameters_id            0x05440020
#define sl_bt_cmd_cte_transmitter_enable_connection_cte_id           0x00440020
#define sl_bt_cmd_cte_transmitter_disable_connection_cte_id          0x01440020
#define sl_bt_cmd_cte_transmitter_enable_connectionless_cte_id       0x02440020
#define sl_bt_cmd_cte_transmitter_disable_connectionless_cte_id      0x03440020
#define sl_bt_cmd_cte_transmitter_enable_silabs_cte_id               0x06440020
#define sl_bt_cmd_cte_transmitter_disable_silabs_cte_id              0x07440020
#define sl_bt_rsp_cte_transmitter_set_dtm_parameters_id              0x04440020
#define sl_bt_rsp_cte_transmitter_clear_dtm_parameters_id            0x05440020
#define sl_bt_rsp_cte_transmitter_enable_connection_cte_id           0x00440020
#define sl_bt_rsp_cte_transmitter_disable_connection_cte_id          0x01440020
#define sl_bt_rsp_cte_transmitter_enable_connectionless_cte_id       0x02440020
#define sl_bt_rsp_cte_transmitter_disable_connectionless_cte_id      0x03440020
#define sl_bt_rsp_cte_transmitter_enable_silabs_cte_id               0x06440020
#define sl_bt_rsp_cte_transmitter_disable_silabs_cte_id              0x07440020

/***************************************************************************//**
 *
 * Set the CTE-related parameters of the LE transmitter test.
 *
 * @param[in] cte_length @parblock
 *   The length of the Constant Tone Extension in 8 us units
 *     - <b>0:</b> No CTE
 *     - <b>0x02 to 0x14:</b> CTE length
 *
 *   Default: 0 (no CTE)
 *   @endparblock
 * @param[in] cte_type @parblock
 *   CTE type
 *     - <b>0:</b> AoA CTE
 *     - <b>1:</b> AoD CTE with 1 us slots
 *     - <b>2:</b> AoD CTE with 2 us slots
 *
 *   Default: 0
 *   @endparblock
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over. This can be
 *   an empty array if CTE is unused.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_set_dtm_parameters(uint8_t cte_length,
                                                     uint8_t cte_type,
                                                     size_t switching_pattern_len,
                                                     const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Clear CTE-related parameters that were previously set for LE transmitter
 * test. Default values will be restored for these parameters.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_clear_dtm_parameters();

/***************************************************************************//**
 *
 * Enable different types of CTE responses on a connection. CTE response will be
 * sent once requested by the peer device using the CTE Request procedure.
 *
 * @param[in] connection Connection handle
 * @param[in] cte_types CTE types. Bitmask of the following:
 *     - <b>Bit 0:</b> AoA CTE response
 *     - <b>Bit 1:</b> AoD CTE response with 1 us slots
 *     - <b>Bit 2:</b> AoD CTE response with 2 us slots
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_enable_connection_cte(uint8_t connection,
                                                        uint8_t cte_types,
                                                        size_t switching_pattern_len,
                                                        const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Disable CTE responses on a connection.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_disable_connection_cte(uint8_t connection);

/***************************************************************************//**
 *
 * Start a connectionless CTE transmit. CTEs will be transmitted in periodic
 * advertisement packets. As a result, a periodic advertising has to be started
 * prior to this command.
 *
 * @param[in] handle Periodic advertising handle
 * @param[in] cte_length @parblock
 *   CTE length in 8 us units.
 *     - <b>Range:</b> 2 to 20
 *
 *     - Time Range: 16 us to 160 us
 *   @endparblock
 * @param[in] cte_type CTE type
 *     - <b>0:</b> AoA CTE
 *     - <b>1:</b> AoD CTE with 1 us slots
 *     - <b>2:</b> AoD CTE with 2 us slots
 * @param[in] cte_count The number of CTEs to be transmitted in each periodic
 *   advertising interval
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_enable_connectionless_cte(uint8_t handle,
                                                            uint8_t cte_length,
                                                            uint8_t cte_type,
                                                            uint8_t cte_count,
                                                            size_t switching_pattern_len,
                                                            const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Stop the connectionless CTE transmit.
 *
 * @param[in] handle Periodic advertising handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_disable_connectionless_cte(uint8_t handle);

/***************************************************************************//**
 *
 * Enable Silicon Labs CTE transmit. CTEs will be transmitted in extended
 * advertisement packets. As a result, extended advertising has to be started
 * prior this command.
 *
 * @param[in] handle Advertising handle
 * @param[in] cte_length @parblock
 *   CTE length in 8 us units.
 *     - <b>Range:</b> 2 to 20
 *
 *     - Time Range: 16 us to 160 us
 *   @endparblock
 * @param[in] cte_type CTE type
 *     - <b>0:</b> AoA CTE
 *     - <b>1:</b> AoD CTE with 1 us slots
 *     - <b>2:</b> AoD CTE with 2 us slots
 * @param[in] cte_count The number of CTEs to be transmitted in each extended
 *   advertising interval. Currently, only cte_count = 1 is supported.
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_enable_silabs_cte(uint8_t handle,
                                                    uint8_t cte_length,
                                                    uint8_t cte_type,
                                                    uint8_t cte_count,
                                                    size_t switching_pattern_len,
                                                    const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Disable Silicon Labs CTE transmit.
 *
 * @param[in] handle Advertising handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_transmitter_disable_silabs_cte(uint8_t handle);

/** @} */ // end addtogroup sl_bt_cte_transmitter

/**
 * @addtogroup sl_bt_cte_receiver CTE Receiver
 * @{
 *
 * @brief CTE Receiver
 *
 * Commands and events in this class manage Constant Tone Extension (CTE)
 * receiving.
 *
 * CTE feature is only supported by specific devices. Commands from this class
 * will return SL_STATUS_NOT_SUPPORTED on devices that do not support CTE.
 */

/* Command and Response IDs */
#define sl_bt_cmd_cte_receiver_set_dtm_parameters_id                 0x05450020
#define sl_bt_cmd_cte_receiver_clear_dtm_parameters_id               0x06450020
#define sl_bt_cmd_cte_receiver_set_sync_cte_type_id                  0x09450020
#define sl_bt_cmd_cte_receiver_set_default_sync_receive_parameters_id 0x0a450020
#define sl_bt_cmd_cte_receiver_set_sync_receive_parameters_id        0x0b450020
#define sl_bt_cmd_cte_receiver_configure_id                          0x00450020
#define sl_bt_cmd_cte_receiver_enable_connection_cte_id              0x01450020
#define sl_bt_cmd_cte_receiver_disable_connection_cte_id             0x02450020
#define sl_bt_cmd_cte_receiver_enable_connectionless_cte_id          0x03450020
#define sl_bt_cmd_cte_receiver_disable_connectionless_cte_id         0x04450020
#define sl_bt_cmd_cte_receiver_enable_silabs_cte_id                  0x07450020
#define sl_bt_cmd_cte_receiver_disable_silabs_cte_id                 0x08450020
#define sl_bt_rsp_cte_receiver_set_dtm_parameters_id                 0x05450020
#define sl_bt_rsp_cte_receiver_clear_dtm_parameters_id               0x06450020
#define sl_bt_rsp_cte_receiver_set_sync_cte_type_id                  0x09450020
#define sl_bt_rsp_cte_receiver_set_default_sync_receive_parameters_id 0x0a450020
#define sl_bt_rsp_cte_receiver_set_sync_receive_parameters_id        0x0b450020
#define sl_bt_rsp_cte_receiver_configure_id                          0x00450020
#define sl_bt_rsp_cte_receiver_enable_connection_cte_id              0x01450020
#define sl_bt_rsp_cte_receiver_disable_connection_cte_id             0x02450020
#define sl_bt_rsp_cte_receiver_enable_connectionless_cte_id          0x03450020
#define sl_bt_rsp_cte_receiver_disable_connectionless_cte_id         0x04450020
#define sl_bt_rsp_cte_receiver_enable_silabs_cte_id                  0x07450020
#define sl_bt_rsp_cte_receiver_disable_silabs_cte_id                 0x08450020

/**
 * @addtogroup sl_bt_cte_receiver_sync_cte_type CTE type flags for limiting periodic advertising synchronization
 * @{
 *
 * Defines sync CTE type flags that are used to limit what types of periodic
 * advertisers to sync to when scanning for periodic advertising synchronization
 * or receiving periodic advertising synchronization transfers.
 */

/** Do not sync to packets with an AoA Constant Tone Extension */
#define SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOA      0x1       

/** Do not sync to packets with an AoD Constant Tone Extension with 1 us slots
 * */
#define SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_1_US 0x2       

/** Do not sync to packets with an AoD Constant Tone Extension with 2 us slots
 * */
#define SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_2_US 0x4       

/** Do not sync to packets without a Constant Tone Extension */
#define SL_BT_CTE_RECEIVER_SYNC_TO_CTE_ONLY        0x10      

/** @} */ // end CTE type flags for limiting periodic advertising synchronization

/**
 * @addtogroup sl_bt_evt_cte_receiver_dtm_iq_report sl_bt_evt_cte_receiver_dtm_iq_report
 * @{
 * @brief IQ sample report from DTM CTE packets.
 */

/** @brief Identifier of the dtm_iq_report event */
#define sl_bt_evt_cte_receiver_dtm_iq_report_id                      0x024500a0

/***************************************************************************//**
 * @brief Data structure of the dtm_iq_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cte_receiver_dtm_iq_report_s
{
  uint16_t   status;          /**< Status of CTE IQ sampling */
  uint8_t    channel;         /**< The channel on which the CTE packet was
                                   received */
  int8_t     rssi;            /**< RSSI in the received CTE packet. Unit: dBm */
  uint8_t    rssi_antenna_id; /**< The ID of the antenna on which RSSI was
                                   measured */
  uint8_t    cte_type;        /**< The CTE type
                                     - <b>0:</b> AoA CTE response
                                     - <b>1:</b> AoD CTE response with 1us slots
                                     - <b>2:</b> AoD CTE response with 2us slots */
  uint8_t    slot_durations;  /**< Slot durations
                                     - <b>1:</b> Switching and sampling slots
                                       are 1 us each
                                     - <b>2:</b> Switching and sampling slots
                                       are 2 us each */
  uint16_t   event_counter;   /**< The event counter of the periodic advertising
                                   train or the connection */
  uint8array samples;         /**< IQ samples of the received CTE packet. I and
                                   Q samples follow each other alternately (I,
                                   Q, I, Q, ...) */
});

typedef struct sl_bt_evt_cte_receiver_dtm_iq_report_s sl_bt_evt_cte_receiver_dtm_iq_report_t;

/** @} */ // end addtogroup sl_bt_evt_cte_receiver_dtm_iq_report

/**
 * @addtogroup sl_bt_evt_cte_receiver_connection_iq_report sl_bt_evt_cte_receiver_connection_iq_report
 * @{
 * @brief IQ sample report from connection CTE packets.
 */

/** @brief Identifier of the connection_iq_report event */
#define sl_bt_evt_cte_receiver_connection_iq_report_id               0x004500a0

/***************************************************************************//**
 * @brief Data structure of the connection_iq_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cte_receiver_connection_iq_report_s
{
  uint16_t   status;          /**< Status of CTE IQ sampling */
  uint8_t    connection;      /**< Connection handle or periodic advertising
                                   synchronization handle */
  uint8_t    phy;             /**< The PHY on which the packet is received.
                                     - <b>1:</b> 1M PHY
                                     - <b>2:</b> 2M PHY */
  uint8_t    channel;         /**< The channel on which the CTE packet was
                                   received */
  int8_t     rssi;            /**< RSSI in the received CTE packet. Unit: dBm */
  uint8_t    rssi_antenna_id; /**< The ID of the antenna on which RSSI was
                                   measured */
  uint8_t    cte_type;        /**< The CTE type
                                     - <b>0:</b> AoA CTE response
                                     - <b>1:</b> AoD CTE response with 1us slots
                                     - <b>2:</b> AoD CTE response with 2us slots */
  uint8_t    slot_durations;  /**< Slot durations
                                     - <b>1:</b> Switching and sampling slots
                                       are 1 us each
                                     - <b>2:</b> Switching and sampling slots
                                       are 2 us each */
  uint16_t   event_counter;   /**< The event counter of the connection */
  uint8array samples;         /**< IQ samples of the received CTE packet. I and
                                   Q samples follow each other alternately (I,
                                   Q, I, Q, ...) */
});

typedef struct sl_bt_evt_cte_receiver_connection_iq_report_s sl_bt_evt_cte_receiver_connection_iq_report_t;

/** @} */ // end addtogroup sl_bt_evt_cte_receiver_connection_iq_report

/**
 * @addtogroup sl_bt_evt_cte_receiver_connectionless_iq_report sl_bt_evt_cte_receiver_connectionless_iq_report
 * @{
 * @brief IQ sample report from connectionless CTE packets.
 */

/** @brief Identifier of the connectionless_iq_report event */
#define sl_bt_evt_cte_receiver_connectionless_iq_report_id           0x014500a0

/***************************************************************************//**
 * @brief Data structure of the connectionless_iq_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cte_receiver_connectionless_iq_report_s
{
  uint16_t   status;          /**< Status of CTE IQ sampling */
  uint16_t   sync;            /**< Periodic advertising synchronization handle */
  uint8_t    channel;         /**< The channel on which the CTE packet was
                                   received */
  int8_t     rssi;            /**< RSSI in the received CTE packet. Unit: dBm */
  uint8_t    rssi_antenna_id; /**< The ID of the antenna on which RSSI was
                                   measured */
  uint8_t    cte_type;        /**< The CTE type
                                     - <b>0:</b> AoA CTE response
                                     - <b>1:</b> AoD CTE response with 1us slots
                                     - <b>2:</b> AoD CTE response with 2us slots */
  uint8_t    slot_durations;  /**< Slot durations
                                     - <b>1:</b> Switching and sampling slots
                                       are 1 us each
                                     - <b>2:</b> Switching and sampling slots
                                       are 2 us each */
  uint16_t   event_counter;   /**< The event counter of the periodic advertising
                                   train */
  uint8array samples;         /**< IQ samples of the received CTE packet. I and
                                   Q samples follow each other alternately (I,
                                   Q, I, Q, ...) */
});

typedef struct sl_bt_evt_cte_receiver_connectionless_iq_report_s sl_bt_evt_cte_receiver_connectionless_iq_report_t;

/** @} */ // end addtogroup sl_bt_evt_cte_receiver_connectionless_iq_report

/**
 * @addtogroup sl_bt_evt_cte_receiver_silabs_iq_report sl_bt_evt_cte_receiver_silabs_iq_report
 * @{
 * @brief IQ samples report from Silicon Labs CTE packets.
 */

/** @brief Identifier of the silabs_iq_report event */
#define sl_bt_evt_cte_receiver_silabs_iq_report_id                   0x034500a0

/***************************************************************************//**
 * @brief Data structure of the silabs_iq_report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_cte_receiver_silabs_iq_report_s
{
  uint16_t   status;          /**< Status of CTE IQ sampling */
  bd_addr    address;         /**< Bluetooth address of the remote device */
  uint8_t    address_type;    /**< Enum @ref sl_bt_gap_address_type_t.

                                   Advertiser address type.

                                   If the application does not include the
                                   bluetooth_feature_use_accurate_api_address_types
                                   component, @p address_type uses the following
                                   values:
                                     - <b>0:</b> Public address
                                     - <b>1:</b> Random address
                                     - <b>255:</b> No address provided
                                       (anonymous advertising)

                                   If the application includes the
                                   bluetooth_feature_use_accurate_api_address_types
                                   component, @p address_type uses enum @ref
                                   sl_bt_gap_address_type_t values:
                                     - <b>sl_bt_gap_public_address (0x0):</b>
                                       Public device address
                                     - <b>sl_bt_gap_static_address (0x1):</b>
                                       Static device address
                                     - <b>sl_bt_gap_random_resolvable_address
                                       (0x2):</b> Resolvable private random
                                       address
                                     - <b>sl_bt_gap_random_nonresolvable_address
                                       (0x3):</b> Non-resolvable private random
                                       address
                                     - <b>sl_bt_gap_anonymous_address
                                       (0xff):</b> No address provided
                                       (anonymous advertising)
                                     - <b>sl_bt_gap_public_address_resolved_from_rpa
                                       (0x4):</b> Public identity address
                                       resolved from a resolvable private
                                       address (RPA)
                                     - <b>sl_bt_gap_static_address_resolved_from_rpa
                                       (0x5):</b> Static identity address
                                       resolved from a resolvable private
                                       address (RPA) */
  uint8_t    phy;             /**< The PHY on which the packet is received.
                                     - <b>1:</b> 1M PHY
                                     - <b>2:</b> 2M PHY */
  uint8_t    channel;         /**< The channel on which the CTE packet was
                                   received */
  int8_t     rssi;            /**< RSSI in the received CTE packet. Unit: dBm */
  uint8_t    rssi_antenna_id; /**< The ID of the antenna on which RSSI was
                                   measured */
  uint8_t    cte_type;        /**< The CTE type
                                     - <b>0:</b> AoA CTE response
                                     - <b>1:</b> AoD CTE response with 1us slots
                                     - <b>2:</b> AoD CTE response with 2us slots */
  uint8_t    slot_durations;  /**< Slot durations
                                     - <b>1:</b> Switching and sampling slots
                                       are 1 us each
                                     - <b>2:</b> Switching and sampling slots
                                       are 2 us each */
  uint16_t   packet_counter;  /**< The event counter of the periodic advertising
                                   train or the connection */
  uint8array samples;         /**< IQ samples of the received CTE packet. I and
                                   Q samples follow each other alternately (I,
                                   Q, I, Q, ...) */
});

typedef struct sl_bt_evt_cte_receiver_silabs_iq_report_s sl_bt_evt_cte_receiver_silabs_iq_report_t;

/** @} */ // end addtogroup sl_bt_evt_cte_receiver_silabs_iq_report

/***************************************************************************//**
 *
 * Set CTE-related parameters of LE receiver test.
 *
 * @param[in] cte_length @parblock
 *   Expected CTE length in 8 us units
 *     - <b>0:</b> No CTE
 *     - <b>0x02 to 0x14:</b> Expected CTE length
 *
 *   Default: 0 (no CTE)
 *   @endparblock
 * @param[in] cte_type @parblock
 *   Expected CTE type
 *     - <b>0:</b> Expect AoA CTE
 *     - <b>1:</b> Expect AoD CTE with 1 us slots
 *     - <b>2:</b> Expect AoD CTE with 2 us slots
 *
 *   Default: 0
 *   @endparblock
 * @param[in] slot_durations @parblock
 *   Slot durations
 *     - <b>1:</b> Switching and sampling slots are 1 us each
 *     - <b>2:</b> Switching and sampling slots are 2 us each
 *
 *   Default: 1
 *   @endparblock
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over. This can be
 *   an empty array if CTE is unused.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cte_receiver_dtm_iq_report - Triggered when IQ samples
 *     have been received.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_set_dtm_parameters(uint8_t cte_length,
                                                  uint8_t cte_type,
                                                  uint8_t slot_durations,
                                                  size_t switching_pattern_len,
                                                  const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Clear CTE-related parameters that were previously set for LE receiver test.
 * Default values will be restored for these parameters.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_clear_dtm_parameters();

/***************************************************************************//**
 *
 * Set the sync CTE type to limit what types of periodic advertisers to sync to
 * when scanning for periodic advertising synchronization. The set parameter
 * takes effect immediately for all advertisers that have not already
 * established synchronization.
 *
 * @param[in] sync_cte_type @parblock
 *   Sync CTE type flags. This value can be a bitmask of multiple flags from
 *   @ref sl_bt_cte_receiver_sync_cte_type. Flags:
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOA (0x01, bit 0):</b> Do not sync
 *       to packets with an AoA Constant Tone Extension
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_1_US (0x02, bit 1):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 1 us slots
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_2_US (0x04, bit 2):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 2 us slots
 *     - <b>SL_BT_CTE_RECEIVER_SYNC_TO_CTE_ONLY (0x10, bit 4):</b> Do not sync
 *       to packets without a Constant Tone Extension
 *
 *   Default: 0
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_set_sync_cte_type(uint8_t sync_cte_type);

/***************************************************************************//**
 *
 * Set the default parameters and the CTE type limitation for receiving Periodic
 * Advertising Synchronization Transfers (PAST) over connections. The default
 * parameters will be in effect for all subsequent connections and do not affect
 * connections that have already opened. The parameters can be overridden for a
 * specific connection using command @ref
 * sl_bt_cte_receiver_set_sync_receive_parameters after the connection is
 * opened.
 *
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout.
 *
 * This command is relevant and available only when the application has included
 * the bluetooth_feature_past_receiver component into the build. If the PAST
 * receiver component is not included, this command returns the error
 * SL_STATUS_NOT_AVAILABLE.
 *
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_past_receiver_mode_t. The mode to specify how the Bluetooth
 *   stack reacts when synchronization information is received. Values:
 *     - <b>sl_bt_past_receiver_mode_ignore (0x0):</b> No attempt is made to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. No event will be triggered
 *       towards the application.
 *     - <b>sl_bt_past_receiver_mode_synchronize (0x1):</b> Attempt to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. When the information is
 *       received, an event will be triggered to indicate success or failure and
 *       to provide the application with the periodic advertising
 *       synchronization handle.
 *
 *   Default: @ref sl_bt_past_receiver_mode_ignore (No attempt is made to
 *   synchronize)
 *   @endparblock
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 10 to 16384
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *     -
 *         - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] sync_cte_type @parblock
 *   Sync CTE type flags to limit what types of periodic advertising trains to
 *   sync to when receiving sync transfers. This value can be a bitmask of
 *   multiple flags from @ref sl_bt_cte_receiver_sync_cte_type. Flags:
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOA (0x01, bit 0):</b> Do not sync
 *       to packets with an AoA Constant Tone Extension
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_1_US (0x02, bit 1):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 1 us slots
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_2_US (0x04, bit 2):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 2 us slots
 *     - <b>SL_BT_CTE_RECEIVER_SYNC_TO_CTE_ONLY (0x10, bit 4):</b> Do not sync
 *       to packets without a Constant Tone Extension
 *
 *   Default: 0 (do not limit synchronization by CTE type)
 *   @endparblock
 * @param[in] reporting_mode @parblock
 *   Enum @ref sl_bt_sync_reporting_mode_t. Specifies the initial mode for
 *   reporting data received in the periodic advertising train after it has
 *   achieved synchronization. Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 *   Default: @ref sl_bt_sync_report_all (Data received in periodic advertising
 *   trains is reported to the application)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_periodic_sync_transfer_received - Triggered after
 *     synchronization transfer is received for a periodic advertising train
 *     that does not have subevents or response slots.
 *   - @ref sl_bt_evt_pawr_sync_transfer_received - If the application includes
 *     the bluetooth_feature_pawr_sync component, triggered after
 *     synchronization transfer is received for a Periodic Advertising with
 *     Responses (PAwR) train.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_set_default_sync_receive_parameters(uint8_t mode,
                                                                   uint16_t skip,
                                                                   uint16_t timeout,
                                                                   uint8_t sync_cte_type,
                                                                   uint8_t reporting_mode);

/***************************************************************************//**
 *
 * Set the parameters and the CTE type limitation for receiving Periodic
 * Advertising Synchronization Transfers (PAST) over the specified connection.
 * The parameters do not affect periodic advertising trains that the device has
 * already synchronized to.
 *
 * Irrespective of the value of the @p skip parameter, the Controller stops
 * skipping packets if the skipping would cause a timeout.
 *
 * This command is relevant and available only when the application has included
 * the bluetooth_feature_past_receiver component into the build. If the PAST
 * receiver component is not included, this command returns the error
 * SL_STATUS_NOT_AVAILABLE.
 *
 * @param[in] connection Connection handle
 * @param[in] mode @parblock
 *   Enum @ref sl_bt_past_receiver_mode_t. The mode to specify how the Bluetooth
 *   stack reacts when synchronization information is received. Values:
 *     - <b>sl_bt_past_receiver_mode_ignore (0x0):</b> No attempt is made to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. No event will be triggered
 *       towards the application.
 *     - <b>sl_bt_past_receiver_mode_synchronize (0x1):</b> Attempt to
 *       synchronize to a periodic advertising train for which the
 *       synchronization information was received. When the information is
 *       received, an event will be triggered to indicate success or failure and
 *       to provide the application with the periodic advertising
 *       synchronization handle.
 *
 *   Default: @ref sl_bt_past_receiver_mode_ignore (Do not attempt to
 *   synchronize)
 *   @endparblock
 * @param[in] skip @parblock
 *   The maximum number of periodic advertising packets that can be skipped
 *   after a successful receive.
 *     - <b>Range:</b> 0 to 499
 *
 *     - <b>Default</b> : 0
 *   @endparblock
 * @param[in] timeout @parblock
 *   The maximum permitted time between successful receives. If this time is
 *   exceeded, synchronization is lost. Unit: 10 ms.
 *     - <b>Range:</b> 10 to 16384
 *
 *     - Unit: 10 ms
 *     - Time range: 100 ms to 163.84 s
 *
 *     - <b>Default</b> : 1000 ms
 *   @endparblock
 * @param[in] sync_cte_type @parblock
 *   Sync CTE type flags to limit what types of periodic advertising trains to
 *   sync to when receiving sync transfers. This value can be a bitmask of
 *   multiple flags from @ref sl_bt_cte_receiver_sync_cte_type. Flags:
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOA (0x01, bit 0):</b> Do not sync
 *       to packets with an AoA Constant Tone Extension
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_1_US (0x02, bit 1):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 1 us slots
 *     - <b>SL_BT_CTE_RECEIVER_DO_NOT_SYNC_TO_AOD_2_US (0x04, bit 2):</b> Do not
 *       sync to packets with an AoD Constant Tone Extension with 2 us slots
 *     - <b>SL_BT_CTE_RECEIVER_SYNC_TO_CTE_ONLY (0x10, bit 4):</b> Do not sync
 *       to packets without a Constant Tone Extension
 *
 *   Default: 0 (do not limit synchronization by CTE type)
 *   @endparblock
 * @param[in] reporting_mode @parblock
 *   Enum @ref sl_bt_sync_reporting_mode_t. Specifies the initial mode for
 *   reporting data received in the periodic advertising train after it has
 *   achieved synchronization. Values:
 *     - <b>sl_bt_sync_report_none (0x0):</b> Data received in periodic
 *       advertising trains is not reported to the application.
 *     - <b>sl_bt_sync_report_all (0x1):</b> Data received in periodic
 *       advertising trains is reported to the application.
 *
 *   Default: @ref sl_bt_sync_report_all (Data received in periodic advertising
 *   trains is reported to the application)
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_periodic_sync_transfer_received - Triggered after
 *     synchronization transfer is received for a periodic advertising train
 *     that does not have subevents or response slots.
 *   - @ref sl_bt_evt_pawr_sync_transfer_received - If the application includes
 *     the bluetooth_feature_pawr_sync component, triggered after
 *     synchronization transfer is received for a Periodic Advertising with
 *     Responses (PAwR) train.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_set_sync_receive_parameters(uint8_t connection,
                                                           uint8_t mode,
                                                           uint16_t skip,
                                                           uint16_t timeout,
                                                           uint8_t sync_cte_type,
                                                           uint8_t reporting_mode);

/***************************************************************************//**
 * @cond RESTRICTED
 *
 * Restricted/experimental API. Contact Silicon Labs sales for more information.
 *
 * Configure the CTE sampling mode.
 *
 * @param[in] flags @parblock
 *   Values:
 *     - <b>0:</b> Disable raw sample mode, only picked IQ samples are reported
 *       (1 IQ sample pair / slot)
 *     - <b>1:</b> Enable raw sample mode, every IQ sample is reported.
 *
 *   Default: 0
 *   @endparblock
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @endcond
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_configure(uint8_t flags);

/***************************************************************************//**
 *
 * Start IQ samplings on a connection. A CTE requests will be initiated
 * periodically on the given connection and IQ sampling will be made on the
 * received CTE responses.
 *
 * @param[in] connection Connection handle
 * @param[in] interval Measurement interval. CTE requests may be sent less
 *   often. For example, if a connection event is missed for any reason, the CTE
 *   request will be sent in the next connection event.
 *     - <b>0:</b> No interval. The request is initiated only once.
 *     - <b>Other values N:</b> Initiate the request every N-th connection
 *       events
 * @param[in] cte_length @parblock
 *   Minimum CTE length requested in 8 us units.
 *     - <b>Range:</b> 2 to 20
 *
 *     - Time Range: 16 us to 160 us
 *   @endparblock
 * @param[in] cte_type Requested CTE type
 *     - <b>0:</b> AoA CTE
 *     - <b>1:</b> AoD CTE with 1 us slots
 *     - <b>2:</b> AoD CTE with 2 us slots
 * @param[in] slot_durations Slot durations
 *     - <b>1:</b> Switching and sampling slots are 1 us each
 *     - <b>2:</b> Switching and sampling slots are 2 us each
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cte_receiver_connection_iq_report - Triggered when IQ
 *     samples have been received.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_enable_connection_cte(uint8_t connection,
                                                     uint16_t interval,
                                                     uint8_t cte_length,
                                                     uint8_t cte_type,
                                                     uint8_t slot_durations,
                                                     size_t switching_pattern_len,
                                                     const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Stop the IQ sampling on a connection. CTEs will not be requested on the given
 * connection.
 *
 * @param[in] connection Connection handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_disable_connection_cte(uint8_t connection);

/***************************************************************************//**
 *
 * Start IQ sampling on a periodic advertising synchronization. IQ samples are
 * taken on each CTE found in the periodic advertisements.
 *
 * @param[in] sync Periodic advertising synchronization handle
 * @param[in] slot_durations Slot durations
 *     - <b>1:</b> Switching and sampling slots are 1 us each
 *     - <b>2:</b> Switching and sampling slots are 2 us each
 * @param[in] cte_count   - <b>0:</b> Sample and report all available CTEs
 *     - <b>Other values:</b> Maximum number of sampled CTEs in each periodic
 *       advertising interval
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cte_receiver_connectionless_iq_report - Triggered when IQ
 *     samples have been received.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_enable_connectionless_cte(uint16_t sync,
                                                         uint8_t slot_durations,
                                                         uint8_t cte_count,
                                                         size_t switching_pattern_len,
                                                         const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Stop IQ sampling on a periodic advertising synchronization.
 *
 * @param[in] sync Periodic advertising synchronization handle
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_disable_connectionless_cte(uint16_t sync);

/***************************************************************************//**
 *
 * Enable IQ sampling of Silicon Labs CTE found in extended advertisements.
 *
 * @param[in] slot_durations Slot durations
 *     - <b>1:</b> Switching and sampling slots are 1 us each
 *     - <b>2:</b> Switching and sampling slots are 2 us each
 * @param[in] cte_count   - <b>0:</b> Sample and report all available CTEs
 *     - <b>Other values:</b> Maximum number of sampled CTEs in each extended
 *       advertising interval
 * @param[in] switching_pattern_len Length of data in @p switching_pattern
 * @param[in] switching_pattern Antenna switching pattern. Antennas will be
 *   switched in this order with the antenna switch pins during CTE. If the CTE
 *   is longer than the switching pattern, the pattern starts over.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_cte_receiver_silabs_iq_report - Triggered when IQ samples
 *     of Silicon Labs CTE have been received.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_enable_silabs_cte(uint8_t slot_durations,
                                                 uint8_t cte_count,
                                                 size_t switching_pattern_len,
                                                 const uint8_t* switching_pattern);

/***************************************************************************//**
 *
 * Disable IQ sampling of Silicon Labs CTE.
 *
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_cte_receiver_disable_silabs_cte();

/** @} */ // end addtogroup sl_bt_cte_receiver

/**
 * @addtogroup sl_bt_connection_analyzer Connection Analyzer
 * @{
 *
 * @brief Connection Analyzer
 *
 * Analyze Bluetooth packets that are transmitted on the connections of external
 * devices. Include component bluetooth_feature_connection_analyzer to use the
 * commands and events in this class.
 */

/* Command and Response IDs */
#define sl_bt_cmd_connection_analyzer_start_id                       0x00480020
#define sl_bt_cmd_connection_analyzer_stop_id                        0x01480020
#define sl_bt_rsp_connection_analyzer_start_id                       0x00480020
#define sl_bt_rsp_connection_analyzer_stop_id                        0x01480020

/**
 * @addtogroup sl_bt_connection_analyzer_flags Connection Analyzer Configuration flags
 * @{
 *
 * Defines the configuration flags in connection analyzer.
 */

/** Indicates a given time value, e.g., @p start_time_us in @ref
 * sl_bt_connection_analyzer_start, is a value relative to the current time. */
#define SL_BT_CONNECTION_ANALYZER_RELATIVE_TIME 0x1       

/** @} */ // end Connection Analyzer Configuration flags

/**
 * @addtogroup sl_bt_evt_connection_analyzer_report sl_bt_evt_connection_analyzer_report
 * @{
 * @brief Triggered when packets transmitted on a connection are captured.
 */

/** @brief Identifier of the report event */
#define sl_bt_evt_connection_analyzer_report_id                      0x004800a0

/***************************************************************************//**
 * @brief Data structure of the report event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_analyzer_report_s
{
  uint8_t analyzer;        /**< The handle of the connection analyzer */
  int8_t  central_rssi;    /**< RSSI measurement of the packet transmitted by
                                the Central device. Units: dBm.
                                  - <b>Range:</b> -127 to +20 */
  int8_t  peripheral_rssi; /**< RSSI measurement of the packet transmitted by
                                the Peripheral device. Units: dBm.
                                  - <b>Range:</b> -127 to +20 */
});

typedef struct sl_bt_evt_connection_analyzer_report_s sl_bt_evt_connection_analyzer_report_t;

/** @} */ // end addtogroup sl_bt_evt_connection_analyzer_report

/**
 * @addtogroup sl_bt_evt_connection_analyzer_completed sl_bt_evt_connection_analyzer_completed
 * @{
 * @brief Triggered when the operation of analyzing a connection is completed.
 */

/** @brief Identifier of the completed event */
#define sl_bt_evt_connection_analyzer_completed_id                   0x014800a0

/***************************************************************************//**
 * @brief Data structure of the completed event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_connection_analyzer_completed_s
{
  uint8_t  analyzer; /**< The handle of the connection analyzer */
  uint16_t reason;   /**< An sl_status_t code describing the reason of stopping
                          the operation */
});

typedef struct sl_bt_evt_connection_analyzer_completed_s sl_bt_evt_connection_analyzer_completed_t;

/** @} */ // end addtogroup sl_bt_evt_connection_analyzer_completed

/***************************************************************************//**
 *
 * Start to analyze another device's connection and report the RSSI
 * measurements. The parameters in this command provide necessary information to
 * identify the connection and schedule operations to follow its transmissions.
 *
 * If the other device uses Silabs' Bluetooth stack, the information of the
 * connection could be retrieved with command @ref
 * sl_bt_connection_get_scheduling_details. The method of passing the
 * information to this device is application layer logic.
 *
 * @param[in] access_address Access address of the connection
 * @param[in] crc_init The CRC initialization value
 * @param[in] interval The connection interval. Time = Value x 1.25 ms
 * @param[in] supervision_timeout The connection supervision time. Time = Value
 *   x 10 ms
 * @param[in] central_clock_accuracy The central device's clock accuracy index
 *   value
 * @param[in] central_phy Enum @ref sl_bt_gap_phy_t. The PHY that the Central
 *   device is transmitting on. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[in] peripheral_phy Enum @ref sl_bt_gap_phy_t. The PHY that the
 *   Peripheral device is transmitting on. Values:
 *     - <b>sl_bt_gap_phy_1m (0x1):</b> 1M PHY
 *     - <b>sl_bt_gap_phy_2m (0x2):</b> 2M PHY
 *     - <b>sl_bt_gap_phy_coded (0x4):</b> Coded PHY, 125k (S=8) or 500k (S=2)
 * @param[in] channel_selection_algorithm Enum @ref
 *   sl_bt_gap_channel_selection_algorithm_t. The channel selection algorithm.
 *   Values:
 *     - <b>sl_bt_gap_channel_selection_algorithm_1 (0x0):</b> Channel selection
 *       algorithm #1
 *     - <b>sl_bt_gap_channel_selection_algorithm_2 (0x1):</b> Channel selection
 *       algorithm #2
 * @param[in] hop The hop increment when the channel selection algorithm #1 is
 *   used on the connection. Ignored if @p channel_selection_algorithm indicates
 *   that the connection uses channel selection algorithm #2.
 * @param[in] channel_map @parblock
 *   5 byte bit field in little endian format. Only the first 37 bits are used.
 *   Bit 0 of the first byte is channel 0, bit 0 of the second byte is channel
 *   8, etc. Ignore bits 37-39 that are reserved for future use.
 *
 *   A channel is unused when its bit is 0. A channel is used when its bit is 1.
 *   @endparblock
 * @param[in] channel The data channel number that transmissions will use in the
 *   next connection event
 * @param[in] event_counter The event counter of the next connection event
 * @param[in] start_time_us The start time of the given connection event in
 *   microseconds. The semantics depend on whether the configuration flag
 *   SL_BT_CONNECTION_ANALYZER_RELATIVE_TIME is set in parameter @p flags:
 *     - When the flag is set, the value is a time relative to the current time.
 *       A negative value means that the start time was in the past.
 *     - When the flag is not set, the value is an absolute time converted from
 *       the PROTIMER tick.
 * @param[in] flags Configuration flags. This value is a bitmask of @ref
 *   sl_bt_connection_analyzer_flags.
 * @param[out] analyzer A handle that is assigned to the connection analyzer.
 *   This handle is valid only if the result code of this command is
 *   SL_STATUS_OK.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @b Events
 *   - @ref sl_bt_evt_connection_analyzer_report - Triggered when a packet
 *     transmitted from the given device is captured.
 *   - @ref sl_bt_evt_connection_analyzer_completed - Triggered when analyzing a
 *     connection is completed in the Link Layer for some reason.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_analyzer_start(uint32_t access_address,
                                            uint32_t crc_init,
                                            uint16_t interval,
                                            uint16_t supervision_timeout,
                                            uint8_t central_clock_accuracy,
                                            uint8_t central_phy,
                                            uint8_t peripheral_phy,
                                            uint8_t channel_selection_algorithm,
                                            uint8_t hop,
                                            const sl_bt_connection_channel_map_t *channel_map,
                                            uint8_t channel,
                                            uint16_t event_counter,
                                            int32_t start_time_us,
                                            uint32_t flags,
                                            uint8_t *analyzer);

/***************************************************************************//**
 *
 * Stop analyzing another device's Bluetooth connection. The operation is
 * stopped when this command returns, and the stack does not send additional
 * event.
 *
 * @param[in] analyzer The handle of the connection analyzer to stop
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_connection_analyzer_stop(uint8_t analyzer);

/** @} */ // end addtogroup sl_bt_connection_analyzer

/**
 * @addtogroup sl_bt_user User Messaging
 * @{
 *
 * @brief User Messaging
 *
 * This class provides commands and events, which can be used by a NCP host and
 * target to implement a communication mechanism with a custom proprietary
 * protocol. An application must decide whether and how the command and event
 * are used. The Bluetooth host stack does not produce or consume any messages
 * belonging to this class.
 */

/* Command and Response IDs */
#define sl_bt_cmd_user_message_to_target_id                          0x00ff0020
#define sl_bt_cmd_user_manage_event_filter_id                        0x01ff0020
#define sl_bt_cmd_user_reset_to_dfu_id                               0x02ff0020
#define sl_bt_cmd_user_cs_service_message_to_target_id               0x03ff0020
#define sl_bt_rsp_user_message_to_target_id                          0x00ff0020
#define sl_bt_rsp_user_manage_event_filter_id                        0x01ff0020
#define sl_bt_rsp_user_reset_to_dfu_id                               0x02ff0020
#define sl_bt_rsp_user_cs_service_message_to_target_id               0x03ff0020

/**
 * @addtogroup sl_bt_evt_user_message_to_host sl_bt_evt_user_message_to_host
 * @{
 * @brief Used by the target application on a device to initiate communication
 * and send a message to the NCP host
 *
 * Do not send event messages in the context of the user command handling.
 */

/** @brief Identifier of the message_to_host event */
#define sl_bt_evt_user_message_to_host_id                            0x00ff00a0

/***************************************************************************//**
 * @brief Data structure of the message_to_host event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_user_message_to_host_s
{
  uint8array message; /**< The message */
});

typedef struct sl_bt_evt_user_message_to_host_s sl_bt_evt_user_message_to_host_t;

/** @} */ // end addtogroup sl_bt_evt_user_message_to_host

/**
 * @addtogroup sl_bt_evt_user_cs_service_message_to_host sl_bt_evt_user_cs_service_message_to_host
 * @{
 * @brief Used by the Channel Sounding service component on target device to
 * send a message, e.g., a distance measurement, to the NCP host
 */

/** @brief Identifier of the cs_service_message_to_host event */
#define sl_bt_evt_user_cs_service_message_to_host_id                 0x01ff00a0

/***************************************************************************//**
 * @brief Data structure of the cs_service_message_to_host event
 ******************************************************************************/
PACKSTRUCT( struct sl_bt_evt_user_cs_service_message_to_host_s
{
  uint8array message; /**< The message */
});

typedef struct sl_bt_evt_user_cs_service_message_to_host_s sl_bt_evt_user_cs_service_message_to_host_t;

/** @} */ // end addtogroup sl_bt_evt_user_cs_service_message_to_host

/***************************************************************************//**
 *
 * Used by an NCP host to send a message to the target application on device.
 * The application on the target must send the response with @ref
 * sl_bt_send_rsp_user_message_to_target.
 *
 * @param[in] data_len Length of data in @p data
 * @param[in] data The message
 * @param[in] max_response_size Size of output buffer passed in @p response
 * @param[out] response_len On return, set to the length of output data written
 *   to @p response
 * @param[out] response The response message
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_user_message_to_target(size_t data_len,
                                         const uint8_t* data,
                                         size_t max_response_size,
                                         size_t *response_len,
                                         uint8_t *response);

/***************************************************************************//**
 *
 * Manage NCP event filter. When the event filter is in use, API events passing
 * the filter will be sent to the host and those that do not pass are discarded
 * by the target. For functionality details, see the NCP component in the
 * Bluetooth SDK.
 *
 * By default, the NCP does not use the event filter.
 *
 * @param[in] data_len Length of data in @p data
 * @param[in] data The message for managing event filter
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_user_manage_event_filter(size_t data_len,
                                           const uint8_t* data);

/***************************************************************************//**
 *
 * Reset the target device to DFU mode from the NCP host. This command is used
 * by specific SDK DFU component on the target device for the functionality
 * related to DFU. Do not use it in other circumstances. This command does not
 * have a response.
 *
 *
 ******************************************************************************/
void sl_bt_user_reset_to_dfu();

/***************************************************************************//**
 *
 * Used by an NCP host to send a message to the Channel Sounding service
 * component on target device. The Channel Sounding service on target device
 * must send the response with @ref
 * sl_bt_send_rsp_user_cs_service_message_to_target.
 *
 * @param[in] data_len Length of data in @p data
 * @param[in] data The message
 * @param[in] max_response_size Size of output buffer passed in @p response
 * @param[out] response_len On return, set to the length of output data written
 *   to @p response
 * @param[out] response The response message
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 ******************************************************************************/
sl_status_t sl_bt_user_cs_service_message_to_target(size_t data_len,
                                                    const uint8_t* data,
                                                    size_t max_response_size,
                                                    size_t *response_len,
                                                    uint8_t *response);

/** @} */ // end addtogroup sl_bt_user


/***************************************************************************//**
 * @addtogroup sl_bt_common_types BT Common Types
 * @{
 *  @brief BT common types
 */

/**
 * @brief Data structure of BT API messages
 */
PACKSTRUCT( struct sl_bt_msg {
  /** API protocol header consisting of event identifier and data length */
  uint32_t   header;

  /** Union of API event types */
  union {
    uint8_t handle;
    sl_bt_evt_dfu_boot_t                                         evt_dfu_boot; /**< Data field for dfu boot event*/
    sl_bt_evt_dfu_boot_failure_t                                 evt_dfu_boot_failure; /**< Data field for dfu boot_failure event*/
    sl_bt_evt_system_boot_t                                      evt_system_boot; /**< Data field for system boot event*/
    sl_bt_evt_system_error_t                                     evt_system_error; /**< Data field for system error event*/
    sl_bt_evt_system_resource_exhausted_t                        evt_system_resource_exhausted; /**< Data field for system resource_exhausted event*/
    sl_bt_evt_system_external_signal_t                           evt_system_external_signal; /**< Data field for system external_signal event*/
    sl_bt_evt_system_soft_timer_t                                evt_system_soft_timer; /**< Data field for system soft_timer event*/
    sl_bt_evt_resource_status_t                                  evt_resource_status; /**< Data field for resource status event*/
    sl_bt_evt_advertiser_timeout_t                               evt_advertiser_timeout; /**< Data field for advertiser timeout event*/
    sl_bt_evt_advertiser_scan_request_t                          evt_advertiser_scan_request; /**< Data field for advertiser scan_request event*/
    sl_bt_evt_periodic_advertiser_status_t                       evt_periodic_advertiser_status; /**< Data field for periodic_advertiser status event*/
    sl_bt_evt_scanner_legacy_advertisement_report_t              evt_scanner_legacy_advertisement_report; /**< Data field for scanner legacy_advertisement_report event*/
    sl_bt_evt_scanner_extended_advertisement_report_t            evt_scanner_extended_advertisement_report; /**< Data field for scanner extended_advertisement_report event*/
    sl_bt_evt_sync_closed_t                                      evt_sync_closed; /**< Data field for sync closed event*/
    sl_bt_evt_periodic_sync_opened_t                             evt_periodic_sync_opened; /**< Data field for periodic_sync opened event*/
    sl_bt_evt_periodic_sync_transfer_received_t                  evt_periodic_sync_transfer_received; /**< Data field for periodic_sync transfer_received event*/
    sl_bt_evt_periodic_sync_report_t                             evt_periodic_sync_report; /**< Data field for periodic_sync report event*/
    sl_bt_evt_pawr_sync_opened_t                                 evt_pawr_sync_opened; /**< Data field for pawr_sync opened event*/
    sl_bt_evt_pawr_sync_transfer_received_t                      evt_pawr_sync_transfer_received; /**< Data field for pawr_sync transfer_received event*/
    sl_bt_evt_pawr_sync_subevent_report_t                        evt_pawr_sync_subevent_report; /**< Data field for pawr_sync subevent_report event*/
    sl_bt_evt_pawr_advertiser_subevent_data_request_t            evt_pawr_advertiser_subevent_data_request; /**< Data field for pawr_advertiser subevent_data_request event*/
    sl_bt_evt_pawr_advertiser_subevent_tx_failed_t               evt_pawr_advertiser_subevent_tx_failed; /**< Data field for pawr_advertiser subevent_tx_failed event*/
    sl_bt_evt_pawr_advertiser_response_report_t                  evt_pawr_advertiser_response_report; /**< Data field for pawr_advertiser response_report event*/
    sl_bt_evt_connection_opened_t                                evt_connection_opened; /**< Data field for connection opened event*/
    sl_bt_evt_connection_parameters_t                            evt_connection_parameters; /**< Data field for connection parameters event*/
    sl_bt_evt_connection_set_parameters_failed_t                 evt_connection_set_parameters_failed; /**< Data field for connection set_parameters_failed event*/
    sl_bt_evt_connection_phy_status_t                            evt_connection_phy_status; /**< Data field for connection phy_status event*/
    sl_bt_evt_connection_get_remote_tx_power_completed_t         evt_connection_get_remote_tx_power_completed; /**< Data field for connection get_remote_tx_power_completed event*/
    sl_bt_evt_connection_tx_power_t                              evt_connection_tx_power; /**< Data field for connection tx_power event*/
    sl_bt_evt_connection_remote_tx_power_t                       evt_connection_remote_tx_power; /**< Data field for connection remote_tx_power event*/
    sl_bt_evt_connection_remote_used_features_t                  evt_connection_remote_used_features; /**< Data field for connection remote_used_features event*/
    sl_bt_evt_connection_data_length_t                           evt_connection_data_length; /**< Data field for connection data_length event*/
    sl_bt_evt_connection_statistics_t                            evt_connection_statistics; /**< Data field for connection statistics event*/
    sl_bt_evt_connection_request_subrate_failed_t                evt_connection_request_subrate_failed; /**< Data field for connection request_subrate_failed event*/
    sl_bt_evt_connection_subrate_changed_t                       evt_connection_subrate_changed; /**< Data field for connection subrate_changed event*/
    sl_bt_evt_connection_closed_t                                evt_connection_closed; /**< Data field for connection closed event*/
    sl_bt_evt_gatt_mtu_exchanged_t                               evt_gatt_mtu_exchanged; /**< Data field for gatt mtu_exchanged event*/
    sl_bt_evt_gatt_service_t                                     evt_gatt_service; /**< Data field for gatt service event*/
    sl_bt_evt_gatt_characteristic_t                              evt_gatt_characteristic; /**< Data field for gatt characteristic event*/
    sl_bt_evt_gatt_descriptor_t                                  evt_gatt_descriptor; /**< Data field for gatt descriptor event*/
    sl_bt_evt_gatt_characteristic_value_t                        evt_gatt_characteristic_value; /**< Data field for gatt characteristic_value event*/
    sl_bt_evt_gatt_descriptor_value_t                            evt_gatt_descriptor_value; /**< Data field for gatt descriptor_value event*/
    sl_bt_evt_gatt_procedure_completed_t                         evt_gatt_procedure_completed; /**< Data field for gatt procedure_completed event*/
    sl_bt_evt_gatt_server_attribute_value_t                      evt_gatt_server_attribute_value; /**< Data field for gatt_server attribute_value event*/
    sl_bt_evt_gatt_server_user_read_request_t                    evt_gatt_server_user_read_request; /**< Data field for gatt_server user_read_request event*/
    sl_bt_evt_gatt_server_user_write_request_t                   evt_gatt_server_user_write_request; /**< Data field for gatt_server user_write_request event*/
    sl_bt_evt_gatt_server_characteristic_status_t                evt_gatt_server_characteristic_status; /**< Data field for gatt_server characteristic_status event*/
    sl_bt_evt_gatt_server_execute_write_completed_t              evt_gatt_server_execute_write_completed; /**< Data field for gatt_server execute_write_completed event*/
    sl_bt_evt_gatt_server_indication_timeout_t                   evt_gatt_server_indication_timeout; /**< Data field for gatt_server indication_timeout event*/
    sl_bt_evt_gatt_server_notification_tx_completed_t            evt_gatt_server_notification_tx_completed; /**< Data field for gatt_server notification_tx_completed event*/
    sl_bt_evt_test_dtm_completed_t                               evt_test_dtm_completed; /**< Data field for test dtm_completed event*/
    sl_bt_evt_sm_passkey_display_t                               evt_sm_passkey_display; /**< Data field for sm passkey_display event*/
    sl_bt_evt_sm_passkey_request_t                               evt_sm_passkey_request; /**< Data field for sm passkey_request event*/
    sl_bt_evt_sm_confirm_passkey_t                               evt_sm_confirm_passkey; /**< Data field for sm confirm_passkey event*/
    sl_bt_evt_sm_bonded_t                                        evt_sm_bonded; /**< Data field for sm bonded event*/
    sl_bt_evt_sm_bonding_failed_t                                evt_sm_bonding_failed; /**< Data field for sm bonding_failed event*/
    sl_bt_evt_sm_confirm_bonding_t                               evt_sm_confirm_bonding; /**< Data field for sm confirm_bonding event*/
    sl_bt_evt_external_bondingdb_data_request_t                  evt_external_bondingdb_data_request; /**< Data field for external_bondingdb data_request event*/
    sl_bt_evt_external_bondingdb_data_t                          evt_external_bondingdb_data; /**< Data field for external_bondingdb data event*/
    sl_bt_evt_external_bondingdb_data_ready_t                    evt_external_bondingdb_data_ready; /**< Data field for external_bondingdb data_ready event*/
    sl_bt_evt_cs_security_enable_complete_t                      evt_cs_security_enable_complete; /**< Data field for cs security_enable_complete event*/
    sl_bt_evt_cs_config_complete_t                               evt_cs_config_complete; /**< Data field for cs config_complete event*/
    sl_bt_evt_cs_procedure_enable_complete_t                     evt_cs_procedure_enable_complete; /**< Data field for cs procedure_enable_complete event*/
    sl_bt_evt_cs_result_t                                        evt_cs_result; /**< Data field for cs result event*/
    sl_bt_evt_cs_result_continue_t                               evt_cs_result_continue; /**< Data field for cs result_continue event*/
    sl_bt_evt_cs_read_remote_supported_capabilities_complete_t   evt_cs_read_remote_supported_capabilities_complete; /**< Data field for cs read_remote_supported_capabilities_complete event*/
    sl_bt_evt_cs_test_end_completed_t                            evt_cs_test_end_completed; /**< Data field for cs_test end_completed event*/
    sl_bt_evt_l2cap_le_channel_open_request_t                    evt_l2cap_le_channel_open_request; /**< Data field for l2cap le_channel_open_request event*/
    sl_bt_evt_l2cap_le_channel_open_response_t                   evt_l2cap_le_channel_open_response; /**< Data field for l2cap le_channel_open_response event*/
    sl_bt_evt_l2cap_channel_data_t                               evt_l2cap_channel_data; /**< Data field for l2cap channel_data event*/
    sl_bt_evt_l2cap_channel_credit_t                             evt_l2cap_channel_credit; /**< Data field for l2cap channel_credit event*/
    sl_bt_evt_l2cap_channel_closed_t                             evt_l2cap_channel_closed; /**< Data field for l2cap channel_closed event*/
    sl_bt_evt_l2cap_command_rejected_t                           evt_l2cap_command_rejected; /**< Data field for l2cap command_rejected event*/
    sl_bt_evt_cte_receiver_dtm_iq_report_t                       evt_cte_receiver_dtm_iq_report; /**< Data field for cte_receiver dtm_iq_report event*/
    sl_bt_evt_cte_receiver_connection_iq_report_t                evt_cte_receiver_connection_iq_report; /**< Data field for cte_receiver connection_iq_report event*/
    sl_bt_evt_cte_receiver_connectionless_iq_report_t            evt_cte_receiver_connectionless_iq_report; /**< Data field for cte_receiver connectionless_iq_report event*/
    sl_bt_evt_cte_receiver_silabs_iq_report_t                    evt_cte_receiver_silabs_iq_report; /**< Data field for cte_receiver silabs_iq_report event*/
    sl_bt_evt_connection_analyzer_report_t                       evt_connection_analyzer_report; /**< Data field for connection_analyzer report event*/
    sl_bt_evt_connection_analyzer_completed_t                    evt_connection_analyzer_completed; /**< Data field for connection_analyzer completed event*/
    sl_bt_evt_user_message_to_host_t                             evt_user_message_to_host; /**< Data field for user message_to_host event*/
    sl_bt_evt_user_cs_service_message_to_host_t                  evt_user_cs_service_message_to_host; /**< Data field for user cs_service_message_to_host event*/
    uint8_t payload[SL_BGAPI_MAX_PAYLOAD_SIZE];
  } data;
});

/**
 * @brief Type definition for the data structure of BT API messages
 */
typedef struct sl_bt_msg sl_bt_msg_t;

/** @} */ // end addtogroup sl_bt_common_types
/******************************************************************************/

/***************************************************************************//**
 * @addtogroup sl_bt_utility_functions Utility Functions
 *  @brief Utility functions for applications on SoC
 *  @{
 */

/**
 * Get the next event that requires processing by user application. Application
 * is not blocked if no event is waiting.
 *
 * @param event the pointer for storing the new event
 * @return SL_STATUS_OK if a new event is returned, or SL_STATUS_NOT_FOUND if no
 *   event is waiting; other value indicates an error occurred
 */
sl_status_t sl_bt_pop_event(sl_bt_msg_t* event);

/**
 * Check whether events are in queue pending for processing.
 * Call @ref sl_bt_pop_event to process pending events.
 *
 * @return true if event is pending; false otherwise
 */
bool sl_bt_event_pending(void);

/**
 * Check whether events are in queue pending for processing and return the next
 * event length in bytes if events are pending.
 * Call @ref sl_bt_pop_event to process pending events.
 *
 * @return the next event length if event is pending; 0 otherwise
 */
uint32_t sl_bt_event_pending_len(void);


/**
 * Run the Bluetooth stack to process scheduled tasks. Events for user
 * application may be generated as a result of this operation.
 */
void sl_bt_run();

/**
 * Handle an API command in binary format.
 *
 * This is provided to NCP target applications for processing commands received
 * from NCP transport. This function is a synonym for a generic BGAPI function
 * @ref sl_bgapi_handle_command. If the NCP application uses an RTOS, use the
 * sequence documented in @ref sl_bgapi_handle_command to handle the BGAPI
 * command in a thread-safe manner.
 *
 * @param hdr the command header
 * @param data the command payload in a byte array
 */
void sl_bt_handle_command(uint32_t hdr, void* data);

/**
 * Get the response to the command currently been handled.
 *
 * This is provided to NCP target applications for processing commands received
 * from NCP transport. This function is a synonym for a generic BGAPI function
 * @ref sl_bgapi_get_command_response.
 */
static inline sl_bt_msg_t* sl_bt_get_command_response()
{
  return (sl_bt_msg_t*) sl_bgapi_get_command_response();
}

/**
 * Priority message handler function if user application requires the use of
 * PendSV interrupt.
 *
 * If scheduler_callback function pointer in configuration struct is something
 * else than NULL, then stack will not install its own PendSV IRQ handler but
 * instead uses callback/handler functions.
 *
 * When application receives call to the scheduler_callback function it must
 * schedule the call to gecko_priority_handle function to later time to run on
 * high priority thread. This callback may happen inside radio IRQ so processing
 * must not block and has to happen as fast as possible.
 *
 * Recommended implementation: High priority thread is loop that waits on binary
 * semaphore and calls gecko_priority_handler. The scheduler_callback is simple
 * function that only signals the semaphore.
 *
 */
void sl_bt_priority_handle(void);

/**
 * @brief Signal the Bluetooth stack that an external event has happened.
 *
 * Signals can be used to report status changes from interrupt context or from
 * other threads to application. Signals are bits that are automatically cleared
 * after application has been notified.
 *
 * If the Platform Core Interrupt API has been configured to use the
 * CORE_ATOMIC_METHOD_BASEPRI as the implementation method of atomic sections,
 * this function must not be called from an interrupt handler with a priority
 * higher than CORE_ATOMIC_BASE_PRIORITY_LEVEL.
 *
 * @param signals is a bitmask defining active signals that are reported back to
 *   the application by system_external_signal-event.
 * @return SL_STATUS_OK if the operation is successful,
 *   SL_STATUS_NO_MORE_RESOURCE indicating the request could not be processed
 *   due to resource limitation at the moment, or SL_STATUS_INVALID_STATE when
 *   the on-demand start feature is used and the stack is currently stopped.
 */
sl_status_t sl_bt_external_signal(uint32_t signals);

/**
 * Signals stack to send system_awake event when application received wakeup
 * signal.
 */
void sl_bt_send_system_awake();

/**
 * Signals stack to send system_error event when in case of an error.
 */
void sl_bt_send_system_error(uint16_t reason, uint8_t data_len, const uint8_t *data);

/**
 * Tells if a SL_BT_API message is sensitive.
 *
 * @param[in] message_header The header of the SL_BT_API message
 * @return 1 if the message is sensitive; otherwise 0
 */
uint8_t sl_bt_is_sensitive_message(uint32_t message_header);

/**
 * Sends the NCP host a message whose SL_BT_MSG_ID is
 * gecko_rsp_user_message_to_target_id.
 *
 * This a utility helping a NCP host and target application to exchange user
 * data. Do not use it in SoC mode.
 */
void sl_bt_send_rsp_user_message_to_target(uint16_t result, uint8_t data_len, uint8_t *data);

/**
 * Sends the NCP host a message whose SL_BT_MSG_ID is
 * gecko_evt_user_message_to_host_id.
 *
 * This a utility helping a NCP host and target application to exchange user
 * data. Do not use it in SoC mode.
 */
void sl_bt_send_evt_user_message_to_host(uint8_t data_len, uint8_t *data);

/**
 * Sends the NCP host a message whose SL_BT_MSG_ID is
 * gecko_rsp_user_manage_event_filter_id.
 *
 * This a utility helping a NCP host and target application to manage event
 * filter. Do not use it in SoC mode.
 */
void sl_bt_send_rsp_user_manage_event_filter(uint16_t result);

/**
 * Sends the NCP host a message whose SL_BT_MSG_ID is
 * gecko_rsp_user_cs_service_message_to_target_id.
 *
 * This a utility helping a NCP host and target application to exchange user
 * data. Do not use it in SoC mode.
 */
void sl_bt_send_rsp_user_cs_service_message_to_target(uint16_t result, uint8_t data_len, uint8_t *data);

/**
 * Sends the NCP host a message whose SL_BT_MSG_ID is
 * gecko_evt_user_cs_service_message_to_host_id.
 *
 * This a utility helping a NCP host and target application to exchange user
 * data. Do not use it in SoC mode.
 */
void sl_bt_send_evt_user_cs_service_message_to_host(uint8_t data_len, uint8_t *data);


/** @} */ // end addtogroup sl_bt_utility_functions
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif