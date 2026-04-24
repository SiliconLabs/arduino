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
#include <stdlib.h>
#include <sl_string.h>
#include "sl_bt_api.h"
#include "bthome_v2.h"
#include "mbedtls/ccm.h"

// -----------------------------------------------------------------------------
//                          Static Variables Declarations
// -----------------------------------------------------------------------------
static uint8_t sensor_data_index;
static uint8_t sensor_data[MEASUREMENT_MAX_LEN] = { 0 };
static uint8_t *dev_name;
static bool b_encrypt_enable;
static bool b_trigger_device;
static bool b_sort_enable;

static uint8_t last_object_id;

// The CCM context for encrypt
static mbedtls_ccm_context encrypt_ctx;
// Store key used for encrypt
static unsigned char bind_key[BIND_KEY_LEN];

static uint32_t encrypt_count;

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
static bool is_advertising = false;

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static uint8_t get_byte_number(uint8_t sens);

static uint16_t get_factor(uint8_t sens);

static void sort_sensor_data(void);

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Initializes function for driver.
 ******************************************************************************/
sl_status_t bthome_v2_init(uint8_t *device_name,
                           bool encryption,
                           uint8_t const * const key,
                           bool trigger_based_device)
{
  sl_status_t sc = SL_STATUS_OK;
  char octet[2];

  if (device_name != NULL) {
    dev_name = device_name;
  }

  if (key != NULL) {
    // Check if key is invalid
    if (!sl_str_is_empty((const char *)key)) {
      // Parse key to 128 bit length format
      for (uint8_t i = 0; i < BIND_KEY_LEN; i++) {
        memcpy(octet, (uint8_t *)&key[2 * i], 2);
        bind_key[i] = (uint8_t)strtol(octet, NULL, 16);
      }

      if (encryption) {
        b_encrypt_enable = true;

        encrypt_count = rand() % 0x427;

        mbedtls_ccm_init(&encrypt_ctx);
        sc = mbedtls_ccm_setkey(&encrypt_ctx,
                                MBEDTLS_CIPHER_ID_AES,
                                bind_key,
                                BIND_KEY_LEN * 8);
        if (sc != SL_STATUS_OK) {
          return sc;
        }
      } else {
        b_encrypt_enable = false;
      }
    }
  }

  b_trigger_device = trigger_based_device;
  bthome_v2_reset_measurement();

  return sc;
}

/***************************************************************************//**
 *  Set the name of the device.
 ******************************************************************************/
sl_status_t bthome_v2_set_device_name(uint8_t *device_name)
{
  if (device_name != NULL) {
    if (!sl_str_is_empty((const char *)device_name)) {
      dev_name = device_name;
      return SL_STATUS_OK;
    }
  }

  return SL_STATUS_FAIL;
}

/***************************************************************************//**
 *  Build packet used for advertise.
 ******************************************************************************/
void bthome_v2_build_packet(void)
{
  bd_addr address;
  uint8_t address_type;
  // dev_name length
  uint8_t dn_length;
  // build encrypt
  uint8_t ciphertext[MEASUREMENT_MAX_LEN];
  uint8_t encryption_mic[MIC_LEN];
  // build initialization vector (nonce)
  uint8_t nonce[NONCE_LEN];
  uint8_t *p_count = (uint8_t *)(&encrypt_count);
  // build data
  uint8_t payload_data[BLE_ADVERT_MAX_LEN] = { 0 };
  uint8_t payload_count = 0;
  uint8_t service_data[MEASUREMENT_MAX_LEN] = { 0 };
  uint8_t service_count = 0;

  // the Object ids have to be applied in numerical order (from low to high)
  if (b_sort_enable) {
    sort_sensor_data();
  }

  // head
  payload_data[payload_count++] = FLAG1;
  payload_data[payload_count++] = FLAG2;
  payload_data[payload_count++] = FLAG3;

  // local name
  if (!sl_str_is_empty((const char *)dev_name)) {
    // +1(flag Complete local name)
    dn_length = sl_strlen((char *)dev_name) + 1;

    if (b_encrypt_enable) {
      // deal with the device name to make sure the adv length <= 31
      // 18=3(FLAG)+1(device name length bit)+1(COMPLETE_NAME)+1(SERVICE_DATA)
      //    +2(UUID)+1(ENCRYPT)+4(nonce)+4(mic)+1(serviceData length bit)
      if (dn_length > BLE_ADVERT_MAX_LEN - sensor_data_index - 18) {
        dn_length = BLE_ADVERT_MAX_LEN - sensor_data_index - 18;
      }
    } else {
      // 10=3(FLAG)+1(device name length bit)+1(COMPLETE_NAME)
      //    +1(SERVICE_DATA)+2(UUID)+1(ENCRYPT)+1(serviceData length bit)
      if (dn_length > BLE_ADVERT_MAX_LEN - sensor_data_index - 10) {
        dn_length = BLE_ADVERT_MAX_LEN - sensor_data_index - 10;
      }
    }

    // Add the length of the Name
    // Complete_Name: Complete local name -- Short_Name: Shortened Name
    payload_data[payload_count++] = dn_length;
    payload_data[payload_count++] = COMPLETE_NAME;
    for (uint8_t i = 0; i < (dn_length - 1); i++) {
      payload_data[payload_count++] = dev_name[i];
    }
  }

  // DO NOT CHANGE -- Service Data - 16-bit UUID
  service_data[service_count++] = SERVICE_DATA;
  // DO NOT CHANGE -- UUID
  service_data[service_count++] = UUID1;
  // DO NOT CHANGE -- UUID
  service_data[service_count++] = UUID2;

  // The encryption
  if (b_encrypt_enable) {
    if (b_trigger_device) {
      service_data[service_count++] = ENCRYPT_TRIGGER_BASE;
    } else {
      service_data[service_count++] = ENCRYPT;
    }

    // Extract unique ID from BT Address.
    sl_bt_system_get_identity_address(&address, &address_type);

    // MAC
    nonce[0] = address.addr[5];
    nonce[1] = address.addr[4];
    nonce[2] = address.addr[3];
    nonce[3] = address.addr[2];
    nonce[4] = address.addr[1];
    nonce[5] = address.addr[0];
    // UUID
    nonce[6] = UUID1;
    nonce[7] = UUID2;
    // BTHome information flag
    nonce[8] = service_data[service_count - 1];
    // Counter
    nonce[9] = p_count[0];
    nonce[10] = p_count[1];
    nonce[11] = p_count[2];
    nonce[12] = p_count[3];

    // add some padding to the payload if needed
    while (sensor_data_index < 5) {
      sensor_data[sensor_data_index] = 0xFF;
      sensor_data_index++;
    }

    // encrypt sensorData
    mbedtls_ccm_encrypt_and_tag(&encrypt_ctx, sensor_data_index,
                                nonce, NONCE_LEN,
                                0, 0,
                                sensor_data, ciphertext,
                                encryption_mic, MIC_LEN);
    // Add ciphertext
    for (uint8_t i = 0; i < sensor_data_index; i++) {
      service_data[service_count++] = ciphertext[i];
    }
    // Add Counter
    service_data[service_count++] = nonce[9];
    service_data[service_count++] = nonce[10];
    service_data[service_count++] = nonce[11];
    service_data[service_count++] = nonce[12];
    encrypt_count++;

    // Add MIC
    service_data[service_count++] = encryption_mic[0];
    service_data[service_count++] = encryption_mic[1];
    service_data[service_count++] = encryption_mic[2];
    service_data[service_count++] = encryption_mic[3];
  } else {
    if (b_trigger_device) {
      service_data[service_count++] = NO_ENCRYPT_TRIGGER_BASE;
    } else {
      service_data[service_count++] = NO_ENCRYPT;
    }

    for (uint8_t i = 0; i < sensor_data_index; i++) {
      // Add the sensor data to the Service Data
      service_data[service_count++] = sensor_data[i];
    }
  }

  // Add the length of the Service Data
  payload_data[payload_count++] = service_count;
  // Finalize the packet
  for (uint8_t i = 0; i < service_count; i++) {
    payload_data[payload_count++] = service_data[i];
  }
  // Add to advertise packet
  sl_bt_legacy_advertiser_set_data(advertising_set_handle,
                                   sl_bt_advertiser_advertising_data_packet,
                                   sizeof(payload_data),
                                   payload_data);
}

/***************************************************************************//**
 *  Send user-defined advertising data packet.
 ******************************************************************************/
sl_status_t bthome_v2_send_packet(void)
{
  sl_status_t sc = SL_STATUS_OK;

  if (sensor_data_index > 0) {
    bthome_v2_build_packet();

    if (!is_advertising) {
      sc = bthome_v2_start();
    }
  }

  return sc;
}

/***************************************************************************//**
 *  Reset the inner packet and state of driver.
 ******************************************************************************/
void bthome_v2_reset_measurement(void)
{
  sensor_data_index = 0;
  last_object_id = 0;
  b_sort_enable = false;
}

/***************************************************************************//**
 *  Add device event to the packet.
 ******************************************************************************/
void bthome_v2_add_measurement_state(uint8_t sensor_id,
                                     uint8_t state,
                                     uint8_t steps)
{
  if ((sensor_data_index + 2 + ((steps > 0) ? 1 : 0))
      <= (MEASUREMENT_MAX_LEN - (b_encrypt_enable ? 8 : 0))) {
    sensor_data[sensor_data_index] = sensor_id & 0xff;
    sensor_data_index++;
    sensor_data[sensor_data_index] = state & 0xff;
    sensor_data_index++;
    if (steps > 0) {
      sensor_data[sensor_data_index] = steps & 0xff;
      sensor_data_index++;
    }
    if (!b_sort_enable) {
      if (sensor_id < last_object_id) {
        b_sort_enable = true;
      }
    }
    last_object_id = sensor_id;
  } else {
    bthome_v2_send_packet();
    bthome_v2_add_measurement_state(sensor_id, state, steps);
  }
}

/***************************************************************************//**
 *  Add sensor measured integer data to the packet.
 ******************************************************************************/
void bthome_v2_add_measurement(uint8_t sensor_id, uint64_t value)
{
  uint8_t size = get_byte_number(sensor_id);
  uint16_t factor = get_factor(sensor_id);
  if ((sensor_data_index + size + 1)
      <= (MEASUREMENT_MAX_LEN - (b_encrypt_enable ? 8 : 0))) {
    sensor_data[sensor_data_index] = sensor_id & 0xff;
    sensor_data_index++;
    for (uint8_t i = 0; i < size; i++)
    {
      sensor_data[sensor_data_index] =
        (uint8_t)(((value * factor) >> (8 * i)) & 0xff);
      sensor_data_index++;
    }
    if (!b_sort_enable) {
      if (sensor_id < last_object_id) {
        b_sort_enable = true;
      }
    }
    last_object_id = sensor_id;
  } else {
    bthome_v2_send_packet();
    bthome_v2_add_measurement(sensor_id, value);
  }
}

/***************************************************************************//**
 *  Add sensor measured float data to the packet.
 ******************************************************************************/
void bthome_v2_add_measurement_float(uint8_t sensor_id, float value)
{
  uint8_t size = get_byte_number(sensor_id);
  uint16_t factor = get_factor(sensor_id);
  if ((sensor_data_index + size + 1)
      <= (MEASUREMENT_MAX_LEN - (b_encrypt_enable ? 8 : 0))) {
    uint64_t value2 = (uint64_t)(value * factor);
    sensor_data[sensor_data_index] = sensor_id & 0xff;
    sensor_data_index++;
    for (uint8_t i = 0; i < size; i++)
    {
      sensor_data[sensor_data_index] = (uint8_t)((value2 >> (8 * i)) & 0xff);
      sensor_data_index++;
    }
    if (!b_sort_enable) {
      if (sensor_id < last_object_id) {
        b_sort_enable = true;
      }
    }
    last_object_id = sensor_id;
  } else {
    bthome_v2_send_packet();
    bthome_v2_add_measurement_float(sensor_id, value);
  }
}

/***************************************************************************//**
 *  Start advertising.
 ******************************************************************************/
sl_status_t bthome_v2_start(void)
{
  sl_status_t sc = SL_STATUS_OK;

  if (!is_advertising) {
    // Start advertising
    sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                       sl_bt_legacy_advertiser_non_connectable);
    is_advertising = true;
  }
  return sc;
}

/***************************************************************************//**
 *  Stop advertising.
 ******************************************************************************/
sl_status_t bthome_v2_stop(void)
{
  sl_status_t sc;

  // Start advertising
  sc = sl_bt_advertiser_stop(advertising_set_handle);
  is_advertising = false;

  return sc;
}

/***************************************************************************//**
 *  Returns with the inner advertise state of the driver.
 ******************************************************************************/
bool bthome_v2_is_advertising()
{
  return is_advertising;
}

/***************************************************************************//**
 *  Bluetooth stack event handler.
 ******************************************************************************/
void bthome_v2_bt_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header))
  {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Check if advertising set is invalid
      if (advertising_set_handle == 0xff) {
        sl_bt_advertiser_create_set(&advertising_set_handle);

        // Set advertising interval to 100ms.
        sl_bt_advertiser_set_timing(
          advertising_set_handle,
          160, // min. adv. interval (milliseconds * 1.6)
          160, // max. adv. interval (milliseconds * 1.6)
          0,   // adv. duration
          0);  // max. num. adv. events
      }
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      if (bthome_v2_is_advertising()) {
        bthome_v2_stop();
      }
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      if (!bthome_v2_is_advertising()) {
        sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                              sl_bt_advertiser_general_discoverable);
        bthome_v2_start();
      }
      break;
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Returns the data size use for property.
 ******************************************************************************/
static uint8_t get_byte_number(uint8_t sens)
{
  switch (sens)
  {
    case ID_PACKET:
    case ID_BATTERY:
    case ID_COUNT:
    case ID_HUMIDITY:
    case ID_MOISTURE:
    case ID_UV:
    case STATE_BATTERY_LOW:
    case STATE_BATTERY_CHARHING:
    case STATE_CO:
    case STATE_COLD:
    case STATE_CONNECTIVITY:
    case STATE_DOOR:
    case STATE_GARAGE_DOOR:
    case STATE_GAS_DETECTED:
    case STATE_GENERIC_BOOLEAN:
    case STATE_HEAT:
    case STATE_LIGHT:
    case STATE_LOCK:
    case STATE_MOISTURE:
    case STATE_MOTION:
    case STATE_MOVING:
    case STATE_OCCUPANCY:
    case STATE_OPENING:
    case STATE_PLUG:
    case STATE_POWER_ON:
    case STATE_PRESENCE:
    case STATE_PROBLEM:
    case STATE_RUNNING:
    case STATE_SAFETY:
    case STATE_SMOKE:
    case STATE_SOUND:
    case STATE_TAMPER:
    case STATE_VIBRATION:
    case STATE_WINDOW:
    case EVENT_BUTTON:
      return 1;
    case ID_DURATION:
    case ID_ENERGY:
    case ID_GAS:
    case ID_ILLUMINANCE:
    case ID_POWER:
    case ID_PRESSURE:
      return 3;
    case ID_COUNT4:
    case ID_ENERGY4:
    case ID_GAS4:
    case ID_VOLUME:
    case ID_WATER:
      return 4;
    default:
      return 2;
  }
}

/***************************************************************************//**
 * Returns the factor use for property.
 ******************************************************************************/
static uint16_t get_factor(uint8_t sens)
{
  switch (sens)
  {
    case ID_DISTANCEM:
    case ID_ROTATION:
    case ID_TEMPERATURE:
    case ID_VOLTAGE1:
    case ID_VOLUME1:
    case ID_UV:
      return 10;
    case ID_DEWPOINT:
    case ID_HUMIDITY_PRECISE:
    case ID_ILLUMINANCE:
    case ID_MASS:
    case ID_MASSLB:
    case ID_MOISTURE_PRECISE:
    case ID_POWER:
    case ID_PRESSURE:
    case ID_SPD:
    case ID_TEMPERATURE_PRECISE:
      return 100;
    case ID_CURRENT:
    case ID_DURATION:
    case ID_ENERGY:
    case ID_ENERGY4:
    case ID_GAS:
    case ID_GAS4:
    case ID_VOLTAGE:
    case ID_VOLUME:
    case ID_VOLUMEFR:
    case ID_WATER:
      return 1000;
    default:
      return 1;
  }
}

/***************************************************************************//**
 * Sort the sensor data.
 ******************************************************************************/
static void sort_sensor_data(void)
{
  uint8_t i, j, k, data_block_num;
  data_block_t data_block[MEASUREMENT_MAX_LEN / 2 + 1];
  data_block_t temp_data_block;

  for (i = 0, j = 0, data_block_num = 0; j < sensor_data_index; i++) {
    // copy the object id
    data_block[i].object_id = sensor_data[j];
    data_block_num++;
    // copy the data length
    if (sensor_data[j] == EVENT_DIMMER) {
      if (sensor_data[j + 1] == EVENT_DIMMER_NONE) {
        data_block[i].data_len = 1;
      } else {
        data_block[i].data_len = 2;
      }
    } else {
      data_block[i].data_len = get_byte_number(sensor_data[j]);
    }
    // copy the data
    for (k = 0; k < data_block[i].data_len; k++) {
      data_block[i].data[k] = sensor_data[j + 1 + k];
    }
    // move to the next object id location
    j = j + data_block[i].data_len + 1;
  }

  if (data_block_num > 1) {
    // bubble sort
    for (i = 0; i < data_block_num - 1; i++) {
      for (j = 0; j < data_block_num - 1 - i; j++) {
        if (data_block[j].object_id > data_block[j + 1].object_id) {
          memcpy(&temp_data_block, &data_block[j], sizeof(data_block_t));
          memcpy(&data_block[j], &data_block[j + 1], sizeof(data_block_t));
          memcpy(&data_block[j + 1], &temp_data_block, sizeof(data_block_t));
        }
      }
    }
    // copy the new order to sensor_data array
    for (i = 0, j = 0; i < data_block_num && j < sensor_data_index; i++) {
      sensor_data[j] = data_block[i].object_id;
      for (k = 0; k < data_block[i].data_len; k++) {
        sensor_data[j + 1 + k] = data_block[i].data[k];
      }
      j = j + data_block[i].data_len + 1;
    }
  }
}

#ifdef __cplusplus
}
#endif //extern