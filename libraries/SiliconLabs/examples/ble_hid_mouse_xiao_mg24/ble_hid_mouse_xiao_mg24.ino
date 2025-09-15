/*
   BLE HID mouse

   The example implements a simple Bluetooth mouse that uses the
   IMU (Inertial Measurement Unit) acceleration data to move the cursor.

   Find out more on the API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   Compatible boards:
   - Seeed Studio XIAO MG24 Sense (LSM6DS3)

   Libraries required:
   - Seeed Arduino LSM6DS3 (https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3 - available from the Library Manager)

   Author: Zoltan Fegyveres & Tamas Jozsi (Silicon Labs)
 */

#include <LSM6DS3.h>
#include "Wire.h"

#define DEVICE_NAME "XIAO MG24 Mouse"

#define IMU_ACC_X_THRESHOLD 10
#define IMU_ACC_Y_THRESHOLD 10

// Mouse button events
#define LMB_PRESSED 1

// HID report data
struct mouse_data {
  int8_t delta_x;
  int8_t delta_y;
  uint8_t buttons;
};
static mouse_data report;

// HID report data buffer
static uint8_t report_array[] = { 0x00, 0x00, 0x00 };

static uint8_t connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
static uint32_t bonding_handle = SL_BT_INVALID_BONDING_HANDLE;
static uint16_t hid_report;

// Device information service
const uint8_t manufacturer[] = "Silicon Labs";
const uint8_t model_no[] = "1";
const uint8_t serial_no[] = "1";
const uint8_t hw_rev[] = "1";
const uint8_t fw_rev[] = "1";
const uint8_t sw_rev[] = "1";

static bd_addr ble_address = { 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u };
static bool button_press = false;
static bool button_press_prev = false;
static int32_t acc_x = 0, acc_y = 0;
static sl_status_t sc = SL_STATUS_OK;

static void ble_initialize_gatt_db();
static void ble_start_advertising();
static void mouse_button_callback();

// Optionally connect a button for left click
#define MOUSE_BUTTON D0

LSM6DS3 myIMU(I2C_MODE, 0x6A);

void setup()
{
  // Initialize report data
  memset(&report, 0, sizeof(report));

  // Enable the IMU power
  pinMode(PD5, OUTPUT);
  digitalWrite(PD5, HIGH);
  delay(300);

  pinMode(MOUSE_BUTTON, INPUT_PULLUP);
  attachInterrupt(MOUSE_BUTTON, mouse_button_callback, CHANGE);

  Serial.begin(115200);

  Serial.println("XIAO MG24 BLE mouse");

  myIMU.begin();
  Serial.println("---");
  Serial.println("IMU initialized");
}

void loop()
{
  // Update 'left mouse button' bit
  if (button_press) {
    report.buttons |= LMB_PRESSED;
    if (!button_press_prev) {
      button_press_prev = true;
      Serial.println("Button pressed");
    }
  } else {
    button_press_prev = false;
    report.buttons &= ~LMB_PRESSED;
  }

  // Change x and y for correct orientation of the boards
  acc_y = (int32_t)(myIMU.readFloatAccelX() * 10.0f);
  acc_x = (int32_t)(myIMU.readFloatAccelY() * 10.0f * -1.0f);

  // In case the acceleration value would surpass the threshold value
  // in positive or negative direction assign the threshold value
  if (acc_x > IMU_ACC_X_THRESHOLD) {
    report.delta_x = IMU_ACC_X_THRESHOLD;
  } else if (acc_x < (-1 * IMU_ACC_X_THRESHOLD)) {
    report.delta_x = (-1 * IMU_ACC_X_THRESHOLD);
  } else {
    report.delta_x = acc_x;
  }

  if (acc_y > IMU_ACC_Y_THRESHOLD) {
    report.delta_y = IMU_ACC_Y_THRESHOLD;
  } else if (acc_y < (-1 * IMU_ACC_Y_THRESHOLD)) {
    report.delta_y = (-1 * IMU_ACC_Y_THRESHOLD);
  } else {
    report.delta_y = acc_y;
  }

  memcpy(report_array, &report, sizeof(report));
  if (connection_handle != SL_BT_INVALID_CONNECTION_HANDLE && bonding_handle != SL_BT_INVALID_BONDING_HANDLE) {
    // Indicate report data change with GATT notification
    sc = sl_bt_gatt_server_notify_all(hid_report, sizeof(report_array), report_array);
    if (sc != SL_STATUS_OK) {
      Serial.print("sl_bt_gatt_server_notify_all() returned with error code 0x");
      Serial.println(sc, HEX);
    } else {
      Serial.print("cursor [delta-X: ");
      Serial.print(report.delta_x, DEC);
      Serial.print(" delta-Y: ");
      Serial.print(report.delta_y, DEC);
      Serial.print(" ] LMB: ");
      Serial.println(report.buttons, HEX);
    }
  }
}

/******************************************************************************
 * Mouse button callback
 *****************************************************************************/
void mouse_button_callback()
{
  if (digitalRead(MOUSE_BUTTON) == LOW) {
    button_press = true;
  } else {
    button_press = false;
  }
}

/******************************************************************************
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t* evt)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t ble_address_type;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready
    case sl_bt_evt_system_boot_id:
    {
      // Get BLE address and address type
      sc = sl_bt_system_get_identity_address(&ble_address, &ble_address_type);
      app_assert_status(sc);

      // Print welcome message
      Serial.begin(115200);
      Serial.println();
      Serial.println("BLE stack booted");

      // Initialize the application specific GATT DB
      ble_initialize_gatt_db();

      // HID input devices requires mandatory secure level and bonding
      sc = sl_bt_sm_configure(0, sl_bt_sm_io_capability_noinputnooutput);
      app_assert_status(sc);

      // Allow bonding
      sc = sl_bt_sm_set_bondable_mode(1);
      app_assert_status(sc);

      ble_start_advertising();
    }
    break;

    // -------------------------------
    // This event indicates that a BLE connection has been opened
    case sl_bt_evt_connection_opened_id:
    {
      // Store the connection handle which will be needed for sending indications
      connection_handle = evt->data.evt_connection_opened.connection;
      bonding_handle = evt->data.evt_connection_opened.bonding;
      Serial.print("Connection opened - handle 0x");
      Serial.println(connection_handle, HEX);

      if (bonding_handle == SL_BT_INVALID_BONDING_HANDLE) {
        Serial.println("Connection not bonded yet");
      } else {
        Serial.println("Connection bonded");
      }

      Serial.println("Increase security");
      sc = sl_bt_sm_increase_security(evt->data.evt_connection_opened.connection);
      app_assert_status(sc);
    }
    break;

    // -------------------------------
    // This event indicates that bonding was successful
    case sl_bt_evt_sm_bonded_id:
    {
      Serial.print("Bonded - handle: 0x");
      Serial.print(evt->data.evt_sm_bonded.connection, HEX);
      bonding_handle = evt->data.evt_sm_bonded.bonding;
      connection_handle = evt->data.evt_sm_bonded.connection;

      Serial.print(" - security mode: 0x");
      Serial.println(evt->data.evt_sm_bonded.security_mode, HEX);
    }
    break;

    // -------------------------------
    // This event indicates that a BLE connection has closed
    case sl_bt_evt_connection_closed_id:
    {
      Serial.print("Connection closed - handle: 0x");
      Serial.print(connection_handle, HEX);
      Serial.print(" reason: 0x");
      Serial.println(evt->data.evt_connection_closed.reason, HEX);

      connection_handle = SL_BT_INVALID_CONNECTION_HANDLE;
      bonding_handle = SL_BT_INVALID_BONDING_HANDLE;

      sc = sl_bt_sm_delete_bondings();
      Serial.println("Deleted bondings");
      app_assert_status(sc);

      ble_start_advertising();
    }
    break;

    // -------------------------------
    // This event indicates that the connection parameters have changed
    case sl_bt_evt_connection_parameters_id:
    {
      Serial.print("Set connection parameters, security_mode: ");
      Serial.println(evt->data.evt_connection_parameters.security_mode, HEX);
    }
    break;

    // -------------------------------
    // This event indicates that bonding has failed
    case sl_bt_evt_sm_bonding_failed_id:
    {
      Serial.print("Bonding failed, reason: 0x");
      Serial.println(evt->data.evt_sm_bonding_failed.reason, HEX);
      Serial.println("Delete bondings.");

      sc = sl_bt_sm_delete_bondings();
      app_assert_status(sc);

      Serial.println("Bondings deleted");
      Serial.print("Close connection - handle: 0x");
      Serial.println(evt->data.evt_sm_bonding_failed.connection, HEX);
    }
    break;

    // -------------------------------
    // Default event handler
    default:
      break;
  }
}

/******************************************************************************
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
static void ble_start_advertising()
{
  static uint8_t advertising_set_handle = 0xff;
  static bool init = true;
  sl_status_t sc;

  if (init) {
    // Create an advertising set
    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert_status(sc);

    // Set advertising interval to 100ms
    sc = sl_bt_advertiser_set_timing(
      advertising_set_handle,
      160,  // Minimum advertisement interval (milliseconds * 1.6)
      160,  // Maximum advertisement interval (milliseconds * 1.6)
      0,    // Advertisement duration
      0);   // Maximum number of advertisement events
    app_assert_status(sc);

    init = false;
  }

  // Generate data for advertising
  sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
  app_assert_status(sc);

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
  app_assert_status(sc);

  Serial.print("Started advertising as '");
  Serial.print(DEVICE_NAME);
  Serial.print("' address: ");
  // Print address in format 'FF:FF:FF:FF:FF:FF'
  for (uint8_t i = (sizeof(bd_addr) - 1); i > 0; i--) {
    Serial.print(ble_address.addr[i], HEX);
    Serial.print(":");
  }
  Serial.println(ble_address.addr[0], HEX);
}

/******************************************************************************
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
static void ble_initialize_gatt_db()
{
  sl_status_t sc;
  uint16_t gattdb_session_id;
  uint16_t service;
  uint16_t characteristic;
  uint16_t descriptor;

  // Create a new GATT database
  sc = sl_bt_gattdb_new_session(&gattdb_session_id);
  app_assert_status(sc);

  // Generic access service
  uint8_t generic_access_service_uuid[] = { 0x00, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                2,
                                generic_access_service_uuid,
                                &service);
  app_assert_status(sc);

  // Device name characteristic
  sl_bt_uuid_16_t device_name_uuid = { .data = { 0x00, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              (SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE),
                                              0,
                                              0,
                                              device_name_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              strlen(DEVICE_NAME),
                                              strlen(DEVICE_NAME),
                                              (uint8_t *)DEVICE_NAME,
                                              &characteristic);
  app_assert_status(sc);

  // Appearance characteristic
  sl_bt_uuid_16_t appearence_uuid = { .data = { 0x01, 0x2A } };
  const uint8_t appearance_value[] = { 0xC2, 0x03 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              appearence_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              2,
                                              sizeof(appearance_value),
                                              appearance_value,
                                              &characteristic);
  app_assert_status(sc);

  // Generic access service start
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // Battery service
  const uint8_t battery_service_uuid[] = { 0x0F, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                0,
                                sizeof(battery_service_uuid),
                                battery_service_uuid,
                                &service);
  app_assert_status(sc);

  // Battery level characteristic
  const sl_bt_uuid_16_t battery_level_uuid = { .data = { 0x19, 0x2A } };
  const uint8_t battery_level_init_value = 100;
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              battery_level_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(battery_level_init_value),
                                              sizeof(battery_level_init_value),
                                              &battery_level_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // Characteristic presentation format descriptor
  const sl_bt_uuid_16_t chara_presentation_format_descriptor_uuid = { .data = { 0x04, 0x29 } };
  const uint8_t chara_presentation_format_value[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          0,
                                          chara_presentation_format_descriptor_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          sizeof(chara_presentation_format_value),
                                          sizeof(chara_presentation_format_value),
                                          chara_presentation_format_value,
                                          &descriptor);
  app_assert_status(sc);

  // Client characteristic configuration descriptor
  const sl_bt_uuid_16_t client_configuration_descriptor_uuid = { .data = { 0x02, 0x29 } };
  const uint8_t client_configuration_value[] = { 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          (SL_BT_GATTDB_DESCRIPTOR_READ | SL_BT_GATTDB_DESCRIPTOR_WRITE),
                                          0,
                                          client_configuration_descriptor_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          sizeof(client_configuration_value),
                                          sizeof(client_configuration_value),
                                          client_configuration_value,
                                          &descriptor);
  app_assert_status(sc);

  // Battery service start
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // Device information service
  const uint8_t device_info_service_uuid[] = { 0x0A, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                0,
                                sizeof(device_info_service_uuid),
                                device_info_service_uuid,
                                &service);
  app_assert_status(sc);

  // Manufacturer name string characteristic
  const sl_bt_uuid_16_t manufacturer_uuid = { .data = { 0x29, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              manufacturer_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(manufacturer) - 1),
                                              (sizeof(manufacturer) - 1),
                                              manufacturer,
                                              &characteristic);
  app_assert_status(sc);

  // Model number string characteristic
  const sl_bt_uuid_16_t model_no_uuid = { .data = { 0x24, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              model_no_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(model_no) - 1),
                                              (sizeof(model_no) - 1),
                                              model_no,
                                              &characteristic);
  app_assert_status(sc);

  // Serial number string characteristic
  const sl_bt_uuid_16_t serial_no_uuid = { .data = { 0x25, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              serial_no_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(serial_no) - 1),
                                              (sizeof(serial_no) - 1),
                                              serial_no,
                                              &characteristic);
  app_assert_status(sc);

  // Hardware revision string characteristic
  const sl_bt_uuid_16_t hw_rev_uuid = { .data = { 0x27, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              hw_rev_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(hw_rev) - 1),
                                              (sizeof(hw_rev) - 1),
                                              hw_rev,
                                              &characteristic);
  app_assert_status(sc);

  // Firmware revision string characteristic
  const sl_bt_uuid_16_t fw_rev_uuid = { .data = { 0x26, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              fw_rev_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(fw_rev) - 1),
                                              (sizeof(fw_rev) - 1),
                                              fw_rev,
                                              &characteristic);
  app_assert_status(sc);

  // Software revision string characteristic
  const sl_bt_uuid_16_t sw_rev_uuid = { .data = { 0x28, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              sw_rev_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              (sizeof(sw_rev) - 1),
                                              (sizeof(sw_rev) - 1),
                                              sw_rev,
                                              &characteristic);
  app_assert_status(sc);

  // System ID characteristic
  const sl_bt_uuid_16_t sys_id_uuid = { .data = { 0x23, 0x2A } };
  const uint8_t sys_id_initial_value[] = { 0x12, 0x34, 0x56, 0xFF, 0xFE, 0x9A, 0xBC, 0xDE };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              sys_id_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(sys_id_initial_value),
                                              sizeof(sys_id_initial_value),
                                              sys_id_initial_value,
                                              &characteristic);
  app_assert_status(sc);

  // PnP ID characteristic
  const sl_bt_uuid_16_t pnp_id_uuid = { .data = { 0x50, 0x2A } };
  const uint8_t pnp_id_initial_value[] = { 0x02, 0x10, 0xC4, 0x00, 0x01, 0x00, 0x01 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              pnp_id_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(pnp_id_initial_value),
                                              sizeof(pnp_id_initial_value),
                                              pnp_id_initial_value,
                                              &characteristic);
  app_assert_status(sc);

  // Device information service start
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // HID service
  uint8_t hid_service_uuid[] = { 0x12, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                2,
                                hid_service_uuid,
                                &service);
  app_assert_status(sc);

  // Protocol mode characteristic
  sl_bt_uuid_16_t hid_protocol_mode_uuid = { .data = { 0x4E, 0x2A } };
  const uint8_t hid_protocol_mode_init_value[] = { 1 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              (SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE),
                                              0,
                                              0,
                                              hid_protocol_mode_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hid_protocol_mode_init_value),
                                              sizeof(hid_protocol_mode_init_value),
                                              hid_protocol_mode_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // HID report characteristic
  const sl_bt_uuid_16_t hid_report_uuid = { .data = { 0x4D, 0x2A } };
  const uint8_t hid_report_init_value[] = { 0x00, 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              (SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE | SL_BT_GATTDB_CHARACTERISTIC_NOTIFY),
                                              0,
                                              0,
                                              hid_report_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hid_report_init_value),
                                              sizeof(hid_report_init_value),
                                              hid_report_init_value,
                                              &characteristic);
  app_assert_status(sc);
  hid_report = characteristic;

  // HID report reference descriptor
  const sl_bt_uuid_16_t hid_report_reference_desc_uuid = { .data = { 0x08, 0x29 } };
  const uint8_t hid_report_reference_desc_init_val[] = { 0x00, 0x01 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          SL_BT_GATTDB_ENCRYPTED_READ,
                                          hid_report_reference_desc_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          sizeof(hid_report_reference_desc_init_val),
                                          sizeof(hid_report_reference_desc_init_val),
                                          hid_report_reference_desc_init_val,
                                          &descriptor);
  app_assert_status(sc);

  // HID report map characteristic
  const sl_bt_uuid_16_t hid_report_map_uuid = { .data = { 0x4B, 0x2A } };
  const uint8_t hid_report_map_init_value[] = { 0x05, 0x01, // Usage page (Generic Desktop)
                                                0x09, 0x02, // Usage (Mouse)
                                                0xA1, 0x01, // Collection (Application)
                                                0x09, 0x01, //   UsageId (Pointer)
                                                0xA1, 0x00, //   Collection (Physical)
                                                0x09, 0x30, //     UsageId (x)
                                                0x09, 0x31, //     UsageId (y)
                                                0x15, 0x80, //     LogicalMinimum(-128)
                                                0x25, 0x7F, //     LogicalMaximum(127)
                                                0x95, 0x02, //     ReportCount(2)
                                                0x75, 0x08, //     ReportSize(8)
                                                0x81, 0x06, //     Input(Data, Variable, Relative, NoWrap, Linear, PreferredState, NoNullPosition, BitField)
                                                0x05, 0x09, //     UsagePage(Button)
                                                0x19, 0x01, //     UsageIdMin(Button 1)
                                                0x29, 0x03, //     UsageIdMax(Button 3)
                                                0x15, 0x00, //     LogicalMinimum(0)
                                                0x25, 0x01, //     LogicalMaximum(1)
                                                0x95, 0x03, //     ReportCount(3)
                                                0x75, 0x01, //     ReportSize(1)
                                                0x81, 0x02, //     Input(Data, Variable, Absolute, NoWrap, Linear, PreferredState, NoNullPosition, BitField)
                                                0x95, 0x01, //     ReportCount(1)
                                                0x75, 0x05, //     ReportSize(5)
                                                0x81, 0x03, //     Input(Constant, Variable, Absolute, NoWrap, Linear, PreferredState, NoNullPosition, BitField)
                                                0xC0,       //   EndCollection()
                                                0xC0 };     // EndCollection()
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              SL_BT_GATTDB_ENCRYPTED_READ,
                                              0,
                                              hid_report_map_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hid_report_map_init_value),
                                              sizeof(hid_report_map_init_value),
                                              hid_report_map_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // HID external report reference descriptor
  const sl_bt_uuid_16_t hid_external_report_reference_descriptor_uuid = { .data = { 0x07, 0x29 } };
  const uint8_t hid_external_report_reference_value[] = { 0x00, 0x00 };
  sc = sl_bt_gattdb_add_uuid16_descriptor(gattdb_session_id,
                                          characteristic,
                                          SL_BT_GATTDB_DESCRIPTOR_READ,
                                          0,
                                          hid_external_report_reference_descriptor_uuid,
                                          sl_bt_gattdb_fixed_length_value,
                                          sizeof(hid_external_report_reference_value),
                                          sizeof(hid_external_report_reference_value),
                                          hid_external_report_reference_value,
                                          &descriptor);
  app_assert_status(sc);

  // HID information characteristic
  const sl_bt_uuid_16_t hid_info_uuid = { .data = { 0x4A, 0x2A } };
  const uint8_t hid_info_init_value[] = { 0x01, 0x11, 0x00, 0x02 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0,
                                              0,
                                              hid_info_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hid_info_init_value),
                                              sizeof(hid_info_init_value),
                                              hid_info_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // HID control point characteristic
  const sl_bt_uuid_16_t hid_control_point_uuid = { .data = { 0x4C, 0x2A } };
  const uint8_t hid_control_point_init_value[] = { 0x00 };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              service,
                                              SL_BT_GATTDB_CHARACTERISTIC_WRITE_NO_RESPONSE,
                                              0,
                                              0,
                                              hid_control_point_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hid_control_point_init_value),
                                              sizeof(hid_control_point_init_value),
                                              hid_control_point_init_value,
                                              &characteristic);
  app_assert_status(sc);

  // HID service start
  sc = sl_bt_gattdb_start_service(gattdb_session_id, service);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
