#ifdef __cplusplus
extern "C"{
#endif

/***************************************************************************//**
 * @file bthome_v2.h
 * @brief BTHome v2.
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
#ifndef BTHOME_V2_H_
#define BTHOME_V2_H_

/***************************************************************************//**
 * @addtogroup bthome_v2
 * @{
 *
 * @brief
 *  The implementation of BLE advertisement packets in the BTHome format.
 ******************************************************************************/

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This library is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include "sl_status.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------
#define BLE_ADVERT_MAX_LEN              31
// 23 = 31(BLE_ADVERT_MAX_LEN)-3(FLAG)-1(SERVICE_DATA)-2(UUID)-1(ENCRYPT)
//      -1(service data length bit)
// ENABLE_ENCRYPT will use extra 8 bytes,
// so each Measurement should smaller than 15
#define MEASUREMENT_MAX_LEN             23
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
 *    Typedef for data block.
 ******************************************************************************/
typedef struct data_block
{
  uint8_t object_id;
  uint8_t data[4];
  uint8_t data_len;
} data_block_t;

/***************************************************************************//**
 * @brief
 *    Initializes function for advertising packet.
 *
 * @param[in] device_name
 *    Name of device.
 * @param[in] encryption
 *    Encryption enable.
 *     - <b>true (0x0):</b> Use encrypt
 *     - <b>false (0x1):</b> Not use encrypt
 * @param[in] key
 *    Pointer to key used for encrypt provide in string types.
 * @param[in] trigger_based_device
 *    The trigger based device flag.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_init(uint8_t *device_name,
                           bool encryption,
                           uint8_t const * const key,
                           bool trigger_based_device);

/***************************************************************************//**
 * @brief
 *    Set the name of the device. Name will be add in advertising packet.
 *
 * @param[in] device_name
 *    Pointer to device name.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_set_device_name(uint8_t *device_name);

/***************************************************************************//**
 * @brief
 *    Add device event to the packet.
 *    Devices can also send events. Events are a combination of a device type
 *    (like a button or dimmer) and an event (like "press" or "long press").
 *
 * @param[in] sensor_id
 *    Type of sensor used.
 * @param[in] state
 *    State of device.
 * @param[in] steps
 *    Number of steps.
 ******************************************************************************/
void bthome_v2_add_measurement_state(uint8_t sensor_id,
                                     uint8_t state,
                                     uint8_t steps);

/***************************************************************************//**
 * @brief
 *    Add sensor measured integer data to the packet.
 *
 * @param[in] sensor_id
 *    Type of sensor used.
 * @param[in] value
 *    Measured value in integer.
 ******************************************************************************/
void bthome_v2_add_measurement(uint8_t sensor_id,
                               uint64_t value);

/***************************************************************************//**
 * @brief
 *    Add sensor measured float data to the packet.
 *
 * @param[in] sensor_id
 *    Type of sensor used.
 * @param[in] value
 *    Measured value in float.
 ******************************************************************************/
void bthome_v2_add_measurement_float(uint8_t sensor_id,
                                     float value);

/***************************************************************************//**
 * @brief
 *    Reset the inner packet and state of driver.
 ******************************************************************************/
void bthome_v2_reset_measurement(void);

/***************************************************************************//**
 * @brief
 *    Build packet used for advertise.
 ******************************************************************************/

void bthome_v2_build_packet(void);

/***************************************************************************//**
 * @brief
 *    Send user-defined advertising data packet.
 *
 *    If advertising mode is currently enabled, the new advertising data will be
 *    used immediately
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_send_packet(void);

/***************************************************************************//**
 * @brief
 *    Start advertising.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_start(void);

/***************************************************************************//**
 * @brief
 *    Stop advertising.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t bthome_v2_stop(void);

/***************************************************************************//**
 * @brief
 *    Returns with the inner advertise state of the driver.
 *
 * @return
 *    Inner state of the advertising set.
 ******************************************************************************/
bool bthome_v2_is_advertising();

/***************************************************************************//**
 * @brief
 *    Bluetooth stack event handler.
 *    This function shall be called from the
 *    void sl_bt_on_event(sl_bt_msg_t *evt) function to handle BLE events.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 ******************************************************************************/
void bthome_v2_bt_on_event(sl_bt_msg_t *evt);

#endif /* BTHOME_V2_H_ */

#ifdef __cplusplus
}
#endif
