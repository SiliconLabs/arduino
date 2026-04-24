/***************************************************************************//**
 * @file bthome_v2_server.h
 * @brief BTHome v2 - Server Library.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
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
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#ifndef BTHOME_V2_SERVER_H_
#define BTHOME_V2_SERVER_H_

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This library is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include "sl_status.h"
#include "sl_bluetooth.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------
// 24 = 31(BLE_ADVERT_MAX_LEN)-3(FLAG)-1(length)-1(SERVICE_DATA)-2(UUID)
#define PAYLOAD_MAX_LEN                 24
#define BIND_KEY_LEN                    16
#define NONCE_LEN                       13
#define MIC_LEN                         4

#define FLAG                            0x020106
#define FLAG1                           0x02
#define FLAG2                           0x01
#define FLAG3                           0x06
#define UUID                            0xD2FC
#define UUID1                           0xD2
#define UUID2                           0xFC
#define SERVICE_DATA                    0x16

#define NO_ENCRYPT                      0x40
#define NO_ENCRYPT_TRIGGER_BASE         0x44
#define ENCRYPT                         0x41
#define ENCRYPT_TRIGGER_BASE            0x45

#define SHORT_NAME                      0x08
#define COMPLETE_NAME                   0x09

#define ID_PACKET                       0x00
#define ID_BATTERY                      0x01
#define ID_CO2                          0x12
#define ID_COUNT                        0x09
#define ID_COUNT2                       0x3D
#define ID_COUNT4                       0x3E
#define ID_CURRENT                      0x43
#define ID_DEWPOINT                     0x08
#define ID_DISTANCE                     0x40
#define ID_DISTANCEM                    0x41
#define ID_DURATION                     0x42
#define ID_ENERGY                       0x0A
#define ID_ENERGY4                      0x4D
#define ID_GAS                          0x4B
#define ID_GAS4                         0x4C
#define ID_HUMIDITY                     0x2E
#define ID_HUMIDITY_PRECISE             0x03
#define ID_ILLUMINANCE                  0x05
#define ID_MASS                         0x06
#define ID_MASSLB                       0x07
#define ID_MOISTURE                     0x2F
#define ID_MOISTURE_PRECISE             0x14
#define ID_PM25                         0x0D
#define ID_PM10                         0x0E
#define ID_POWER                        0x0B
#define ID_PRESSURE                     0x04
#define ID_ROTATION                     0x3F
#define ID_SPD                          0x44
#define ID_TEMPERATURE_PRECISE          0x02
#define ID_TEMPERATURE                  0x45
#define ID_TVOC                         0x13
#define ID_VOLTAGE                      0x0C
#define ID_VOLTAGE1                     0x4A
#define ID_VOLUME                       0x4E
#define ID_VOLUME1                      0x47
#define ID_VOLUME2                      0x48
#define ID_VOLUMEFR                     0x49
#define ID_UV                           0x46
#define ID_WATER                        0x4F

#define STATE_BATTERY_LOW               0x15
#define STATE_BATTERY_CHARHING          0x16
#define STATE_CO                        0x17
#define STATE_COLD                      0x18
#define STATE_CONNECTIVITY              0x19
#define STATE_DOOR                      0x1A
#define STATE_GARAGE_DOOR               0x1B
#define STATE_GAS_DETECTED              0x1C
#define STATE_GENERIC_BOOLEAN           0x0F
#define STATE_HEAT                      0x1D
#define STATE_LIGHT                     0x1E
#define STATE_LOCK                      0x1F
#define STATE_MOISTURE                  0x20
#define STATE_MOTION                    0x21
#define STATE_MOVING                    0x22
#define STATE_OCCUPANCY                 0x23
#define STATE_OPENING                   0x11
#define STATE_PLUG                      0x24
#define STATE_POWER_ON                  0x10
#define STATE_PRESENCE                  0x25
#define STATE_PROBLEM                   0x26
#define STATE_RUNNING                   0x27
#define STATE_SAFETY                    0x28
#define STATE_SMOKE                     0x29
#define STATE_SOUND                     0x2A
#define STATE_TAMPER                    0x2B
#define STATE_VIBRATION                 0x2C
#define STATE_WINDOW                    0x2D

#define STATE_ON                        0x01
#define STATE_OFF                       0x00

#define EVENT_BUTTON                    0x3A
#define EVENT_BUTTON_NONE               0x00
#define EVENT_BUTTON_PRESS              0x01
#define EVENT_BUTTON_DOUBLE_PRESS       0x02
#define EVENT_BUTTON_TRIPLE_PRESS       0x03
#define EVENT_BUTTON_LONG_PRESS         0x04
#define EVENT_BUTTON_LONG_DOUBLE_PRESS  0x05
#define EVENT_BUTTTON_LONG_TRIPLE_PRESS 0x06

#define EVENT_DIMMER                    0x3C
#define EVENT_DIMMER_NONE               0x00
#define EVENT_DIMMER_LEFT               0x01
#define EVENT_DIMMER_RIGHT              0x02

/***************************************************************************//**
 * @brief
 *    Typedef for callback.
 ******************************************************************************/
typedef void (*bthome_v2_server_callback_ptr_t)(uint8_t *mac,
                                                uint8_t *payload,
                                                uint8_t payload_length);

/***************************************************************************//**
 * @brief
 *    Typedef for object sensor data.
 ******************************************************************************/
typedef struct sensor_data
{
  uint8_t object_id;
  uint8_t data_length;
  uint16_t factor;
  uint32_t data;
} bthome_v2_server_sensor_data_t;

/***************************************************************************//**
 * @brief
 *    Typedef for database of device.
 ******************************************************************************/
typedef struct bthome_device
{
  uint8_t mac[6];
  uint8_t payload[PAYLOAD_MAX_LEN];
  uint8_t payload_lenth;
  uint32_t last_update_time;
} bthome_v2_server_device_t;

/***************************************************************************//**
 * @brief
 *    Start scan for BThome devices.
 *    This function shall be call in bluetooth boot event.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_start_scan_network(void);

/***************************************************************************//**
 * @brief
 *    Register encrypt key for device by MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[in] key
 *    Device encrypt key, 16 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_key_register(uint8_t *mac, uint8_t *key);

/***************************************************************************//**
 * @brief
 *    Remove encrypt key by MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_key_remove(uint8_t *mac);

/***************************************************************************//**
 * @brief
 *    Get encrypt key for device by MAC address.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[out] key
 *    Pointer to device encrypt key, 16 byte length
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_key_get(uint8_t *mac, uint8_t *key);

/***************************************************************************//**
 * @brief
 *    Check whether the device encrypted and the key is registered.
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[out] encrypted
 *    True if the data is encrypted
 * @param[out] key_available
 *    True if the key already available
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_check_device(uint8_t *mac,
                                          bool *encrypted,
                                          bool *key_available);

/***************************************************************************//**
 * @brief
 *    Read sensor data (even for encrypted).
 *
 * @param[in] mac
 *    Device MAC address, 6 byte length
 * @param[out] object_data
 *    Pointer to a list of object data
 * @param[in] object_max
 *    Number of max object data, get by user
 * @param[out] object_count
 *    Number of object data can parse
 * @param[out] update_time
 *    Get time when data is updated
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_server_sensor_data_read(uint8_t *mac,
                                              bthome_v2_server_sensor_data_t *object_data,
                                              uint8_t object_max,
                                              uint8_t *object_count,
                                              uint32_t *update_time);

/***************************************************************************//**
 * @brief
 *    Register callback for found new devices.
 *
 * @param[in] callback_ptr
 *    Callback function pointer
 *
 * @return
 *    Error status
 ******************************************************************************/
void bthome_v2_server_register_callback(
  bthome_v2_server_callback_ptr_t callback_ptr);

/***************************************************************************//**
 * @brief
 *    Default callback function when found new devices.
 *    User can override this function for other use.
 ******************************************************************************/
SL_WEAK void bthome_v2_server_found_device_callback(uint8_t *mac,
                                                    uint8_t *payload,
                                                    uint8_t payload_length);

/***************************************************************************//**
 * @brief
 *    Bluetooth stack event handler.
 *    This function default called from the
 *    void sl_bt_process_event(sl_bt_msg_t *evt) function to handle BLE events.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 ******************************************************************************/
void bthome_v2_server_on_event(sl_bt_msg_t *evt);


uint8_t get_device_count(void);
void get_device_mac(uint8_t index, uint8_t *mac);


void interested_add(uint8_t *mac_adr);
void interested_remove(uint8_t *mac_adr);
void device_list(void);

#endif /* BTHOME_V2_SERVER_H_ */

#ifdef __cplusplus
}
#endif
