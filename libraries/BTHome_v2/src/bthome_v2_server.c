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

#include <stdlib.h>
#include <string.h>

#include "sl_bt_api.h"
#include "mbedtls/ccm.h"

#include "bthome_v2_server_nvm3.h"
#include "bthome_v2_server.h"

#include "app_log.h"


#ifndef MAX_ENCRYPT_DEVICE
#define MAX_ENCRYPT_DEVICE    5
#endif

#ifndef MAX_DEVICE
#define MAX_DEVICE            10
#endif



// -----------------------------------------------------------------------------
//                          Static Variables Declarations
// -----------------------------------------------------------------------------
static bthome_v2_server_callback_ptr_t bthome_v2_callback;

// store found bthomev2 devices
static bthome_v2_server_device_t device[MAX_DEVICE];
static uint8_t device_count = 0;

// The CCM context for decrypt
static mbedtls_ccm_context encrypt_ctx;


#define MAX_INTERESTED_DEVICE   5

static uint8_t interested_device[MAX_INTERESTED_DEVICE][6];
static uint8_t interested_count = 0;




// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static bool find_bthome_v2_device(
  sl_bt_evt_scanner_legacy_advertisement_report_t *response,
  uint8_t *payload,
  uint8_t *payload_length);

static sl_status_t decrypt_device_data(uint8_t index,
                                       uint8_t *data,
                                       uint8_t *data_length);

static int get_device_index(uint8_t *mac);
static uint8_t get_byte_number(uint8_t id);
static uint16_t get_factor(uint8_t id);



// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Start scan for BThome devices..
 ******************************************************************************/
sl_status_t bthome_v2_server_start_scan_network(void)
{
  sl_status_t sc;

  // set parameters for scan
  sc = sl_bt_scanner_set_parameters(sl_bt_scanner_scan_mode_passive,
                                    16,
                                    16);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  // start scanning devices
  sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                           sl_bt_scanner_discover_generic);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Register encrypt key for device by MAC address.
 ******************************************************************************/
sl_status_t bthome_v2_server_key_register(uint8_t *mac, uint8_t *key)
{
  sl_status_t sc;

  if ((mac == NULL) || (key == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bthome_v2_server_nvm3_save_device_key(mac, key);

  return sc;
}

/***************************************************************************//**
 *  Remove encrypt key.
 ******************************************************************************/
sl_status_t bthome_v2_server_key_remove(uint8_t *mac)
{
  sl_status_t sc;

  if (mac == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bthome_v2_server_nvm3_remove_device_key(mac);

  return sc;
}

/***************************************************************************//**
 *  Get encrypt key.
 ******************************************************************************/
sl_status_t bthome_v2_server_key_get(uint8_t *mac, uint8_t *key)
{
  sl_status_t sc;

  if ((mac == NULL) || (key == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bthome_v2_server_nvm3_find_device_key(mac, key);

  return sc;
}

/***************************************************************************//**
 *  Check whether the device encrypted and the key is registered.
 ******************************************************************************/
sl_status_t bthome_v2_server_check_device(uint8_t *mac,
                                          bool *encrypted,
                                          bool *key_available)
{
  sl_status_t sc;
  int index;
  uint8_t *payload;
  uint8_t key[16];

  if ((mac == NULL) || (encrypted == NULL) || (key_available == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  index = get_device_index(mac);
//  app_log("r\nget_device_index %d ", index);

  if (index == -1) {
    return SL_STATUS_NOT_FOUND;
  }

  payload = device[index].payload;
  *encrypted = false;

  // first byte in payload is information flag
  if (payload[0] & (1 << 0)) {
    *encrypted = true;
  }

  *key_available = false;
  sc = bthome_v2_server_nvm3_find_device_key(mac, key);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *key_available = true;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Read sensor data (even for encrypted).
 ******************************************************************************/
sl_status_t bthome_v2_server_sensor_data_read(uint8_t *mac,
                                              bthome_v2_server_sensor_data_t *object_data,
                                              uint8_t object_max,
                                              uint8_t *object_count,
                                              uint32_t *update_time)
{
  sl_status_t sc = SL_STATUS_OK;
  int index;
  uint8_t encryption;
  uint8_t payload[PAYLOAD_MAX_LEN];
  uint8_t payload_length = 0;
  uint8_t i = 0;
  uint8_t count = 0;

  if ((mac == NULL) || (object_data == NULL) || (object_count == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  index = get_device_index(mac);
  if (index == -1) {
    return SL_STATUS_NOT_FOUND;
  }
  // check encryption bit
  encryption = device[index].payload[0] & (1 << 0);
  if (encryption) {
    sc = decrypt_device_data(index, payload, &payload_length);
    if (sc != SL_STATUS_OK) {
      return sc;
    }
  } else {
    payload_length = device[index].payload_lenth - 1;
    memcpy(payload, &(device[index].payload[1]), payload_length);
  }

  while ((i < payload_length) && (count < object_max)) {
    object_data[count].object_id = payload[i];
    object_data[count].data_length =
      get_byte_number(object_data[count].object_id);
    object_data[count].factor =
      get_factor(object_data[count].object_id);
    object_data[count].data = 0;
    for (uint8_t j = 0; j < object_data[count].data_length; j++) {
      object_data[count].data |= (uint32_t)(payload[i + 1 + j] << (j * 8));
    }
    i = i + object_data[count].data_length + 1;
    count++;
  }

  *object_count = count;
  if (update_time != NULL) {
    *update_time = device[index].last_update_time;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Register callback for found new devices.
 ******************************************************************************/
void bthome_v2_server_register_callback(
  bthome_v2_server_callback_ptr_t callback_ptr)
{
  bthome_v2_callback = callback_ptr;
}

/***************************************************************************//**
 *  Default callback function.
 ******************************************************************************/
SL_WEAK void bthome_v2_server_found_device_callback(uint8_t *mac,
                                                    uint8_t *payload,
                                                    uint8_t payload_length)
{
  (void)mac;
  (void)payload;
  (void)payload_length;
}

/***************************************************************************//**
 *  Bluetooth stack event handler.
 ******************************************************************************/
void bthome_v2_server_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header))
  {
    // -------------------------------
    case sl_bt_evt_system_boot_id:
      mbedtls_ccm_init(&encrypt_ctx);
      bthome_v2_server_register_callback(
        (bthome_v2_server_callback_ptr_t)bthome_v2_server_found_device_callback);
      break;

    // -------------------------------
    case sl_bt_evt_scanner_legacy_advertisement_report_id:
    {
      sl_bt_evt_scanner_legacy_advertisement_report_t *resp =
        &evt->data.evt_scanner_legacy_advertisement_report;
      uint8_t mac[6];
      uint8_t payload[PAYLOAD_MAX_LEN];
      uint8_t payload_length;
      bool new_device_found = true;

      if (find_bthome_v2_device(resp, payload, &payload_length)) {
        // reverse MAC address
        for (uint8_t i = 0; i < 6; i++) {
          mac[i] = resp->address.addr[5 - i];
        }

        // check if found new bthomev2 device
        for (uint8_t i = 0; i < device_count; i++) {
          if (memcmp(mac, device[i].mac, 6) == 0) {
            // update payload data of old device
            memcpy(device[i].payload,
                   payload,
                   payload_length);
            device[i].payload_lenth = payload_length;
            device[i].last_update_time = sl_sleeptimer_get_tick_count();
            new_device_found = false;
            break;
          }
        }

        // store new device
        if (new_device_found && (device_count < MAX_DEVICE)) {
          memcpy(device[device_count].mac, mac, 6);
          memcpy(device[device_count].payload,
                 payload,
                 payload_length);
          device[device_count].payload_lenth = payload_length;
          device[device_count].last_update_time =
            sl_sleeptimer_get_tick_count();
          device_count++;

          app_log("\r\ndevice_count %d",  device_count );

          // callback function call here
          bthome_v2_callback(mac, payload, payload_length);
        }
      }
      break;
    }
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Find BThome device.
 ******************************************************************************/
static bool find_bthome_v2_device(
  sl_bt_evt_scanner_legacy_advertisement_report_t *response,
  uint8_t *payload,
  uint8_t *payload_length)
{
  uint8_t ad_length;
  uint8_t ad_type;
  bool device_found = false;
  uint8_t i = 0;

  while (i < response->data.len - 1) {
    ad_length = response->data.data[i];
    ad_type = response->data.data[i + 1];

    // Type 0x16 = The BTHome (service)
    // UUID = D2FC
    if (ad_type == 0x16) {
      if ((response->data.data[i + 2] == 0xD2)
          && (response->data.data[i + 3] == 0xFC)) {
        device_found = true;
        *payload_length = ad_length - 3;
        memcpy(payload, &(response->data.data[i + 4]), *payload_length);
      }
    }
    // advance to the next AD struct
    i = i + ad_length + 1;
  }
  return device_found;
}

/***************************************************************************//**
 * Get device index.
 ******************************************************************************/
static int get_device_index(uint8_t *mac)
{

  for (uint8_t i = 0; i < interested_count; i++) {

    if (memcmp(mac, device[i].mac, 6) == 0) {

      return i;
    }
  }
  return -1;
}

/***************************************************************************//**
 * Decrypt device data.
 ******************************************************************************/
static sl_status_t decrypt_device_data(uint8_t index,
                                       uint8_t *data,
                                       uint8_t *data_length)
{
  sl_status_t sc;
  uint8_t mic[MIC_LEN];
  uint8_t nonce[NONCE_LEN];
  uint8_t len = device[index].payload_lenth - 8 - 1;
  uint8_t ciphertext[len];
  uint8_t sensor_data[len];
  uint8_t bind_key[BIND_KEY_LEN];

  mbedtls_ccm_free(&encrypt_ctx);

  sc = bthome_v2_server_nvm3_find_device_key(device[index].mac, bind_key);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INVALID_KEY;
  }

  sc = mbedtls_ccm_setkey(&encrypt_ctx,
                          MBEDTLS_CIPHER_ID_AES,
                          bind_key,
                          BIND_KEY_LEN * 8);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  // build initialization vector (nonce)
  // MAC
  nonce[0] = device[index].mac[0];
  nonce[1] = device[index].mac[1];
  nonce[2] = device[index].mac[2];
  nonce[3] = device[index].mac[3];
  nonce[4] = device[index].mac[4];
  nonce[5] = device[index].mac[5];

  // UUID
  nonce[6] = UUID1;
  nonce[7] = UUID2;
  // BTHome information byte
  nonce[8] = device[index].payload[0];
  // Counter
  nonce[9] = device[index].payload[device[index].payload_lenth - 8];
  nonce[10] = device[index].payload[device[index].payload_lenth - 7];
  nonce[11] = device[index].payload[device[index].payload_lenth - 6];
  nonce[12] = device[index].payload[device[index].payload_lenth - 5];
  // MIC
  mic[0] = device[index].payload[device[index].payload_lenth - 4];
  mic[1] = device[index].payload[device[index].payload_lenth - 3];
  mic[2] = device[index].payload[device[index].payload_lenth - 2];
  mic[3] = device[index].payload[device[index].payload_lenth - 1];
  // cipher text
  for (uint8_t i = 0; i < len; i++) {
    ciphertext[i] = device[index].payload[i + 1];
  }

  sc = mbedtls_ccm_auth_decrypt(&encrypt_ctx, len,
                                nonce, NONCE_LEN,
                                NULL, 0,
                                ciphertext, sensor_data,
                                mic, MIC_LEN);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INVALID_KEY;
  }
  *data_length = len;
  memcpy(data, sensor_data, len);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Returns the data size use for property.
 ******************************************************************************/
static uint8_t get_byte_number(uint8_t id)
{
  switch (id)
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
static uint16_t get_factor(uint8_t id)
{
  switch (id)
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

//CLi functionality



uint8_t get_device_count(void)
{
  return interested_count;
}

void get_device_mac(uint8_t index, uint8_t *mac)
{
  memcpy(mac, interested_device[index], 6);
}


void interested_add(uint8_t *mac_adr)
{
  app_log("<<interested list add>>\r\n");

  for (uint8_t i = 0; i < interested_count; i++) {
    if (memcmp(interested_device[i], mac_adr, 6) == 0) {
      app_log("Device already exists\r\n");
      return;
    }
  }
  // Add to interested list
  if (interested_count < (MAX_INTERESTED_DEVICE - 1)) {
    for (uint8_t i = 0; i < 6; i++) {
      interested_device[interested_count][i] = mac_adr[i];
    }
    interested_count++;
  }
}


void interested_remove(uint8_t *mac_adr)
{
  uint8_t index = MAX_INTERESTED_DEVICE;

  app_log("<<interested list remove>>\r\n");

  // Find index of device in interested list
  for (uint8_t i = 0; i < interested_count; i++) {
    if (memcmp(interested_device[i], mac_adr, 6) == 0) {
      index = i;
      break;
    }
  }

  if (index == MAX_INTERESTED_DEVICE) {
    app_log("Device not exists\r\n");
    return;
  }

  // Remove and rearrange device
  for (uint8_t i = index; i < interested_count; i++) {
    memcpy(interested_device[i], interested_device[i + 1], 6);
  }
  for (uint8_t j = 0; j < 6; j++) {
    interested_device[interested_count][j] = 0;
  }

  interested_count--;
}

void device_list(void){
  sl_status_t sc;
  bthome_v2_server_data_t tmp;

  app_log("<<device list>>\r\n");

  for (uint8_t i = 0; i < MAX_ENCRYPT_DEVICE; i++) {
    sc = bthome_v2_server_nvm3_read(i, tmp.data);
    if (sc == SL_STATUS_OK) {
      app_log("->MAC: ");
      for (uint8_t j = 0; j < 6; j++) {
        app_log("%.2x ", tmp.bthome_nvm3.mac[j]);
      }
      app_log("\r\n");

      app_log("  bind key: ");
      for (uint8_t j = 0; j < 16; j++) {
        app_log("%.2x ", tmp.bthome_nvm3.key[j]);
      }
      app_log("\r\n");
    }
  }
}

#ifdef __cplusplus
}
#endif
