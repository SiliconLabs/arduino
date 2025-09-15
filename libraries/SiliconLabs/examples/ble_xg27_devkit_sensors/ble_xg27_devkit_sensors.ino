/*
   xG27 Dev Kit sensors BLE example

   This example showcases the xG27 Dev Kit's built-in sensors using BLE.
   It collects and processes sensor data from the board and provides graphical
   feedback via the Simplicity Connect iOS/Android app.

   On startup the sketch will start a BLE advertisement with the configured name, then
   it will accept any incoming connection. The device offers GATT services and characteristics
   for reporting the data collected from it's onboard sensors including:
   - One Silicon Labs Si7021 relative humidity & temperature sensor
   - One Silicon Labs Si7210 hall effect sensor
   - One Vishay VEML6035 ambient light sensor
   - One TDK InvenSense ICM-20648 6-axis inertial measurement sensor
   - Two Knowles SPK0641HT4H-1 MEMS microphones with PDM output

   The example requires the following libraries:
   - [Adafruit Si7021](https://github.com/adafruit/Adafruit_Si7021)
   - [SparkFun Ambient Light Sensor Arduino Library](https://github.com/sparkfun/SparkFun_Ambient_Light_Sensor_Arduino_Library)
   - [ICM20689](https://github.com/finani/ICM20689)
   - Silabs Si7210 Hall Effect Sensor
   - SilabsMicrophonePDM

   With the Simplicity Connect app you can test this example with two different methods:
   - Method 1: Open the Simplicity Connect app on your smartphone. Go to Demo tab -> Environment -> Connect to the device
     with the name "Dev Kit #xxxxx" -> The demo application automatically requests data from the xG27 Dev Kit and
     displays it on the main dashboard.
   - Method 2: Open the Simplicity Connect app on your smartphone. Go to the Scan tab -> Find your device in the Bluetooth Browser, advertising
     as "Dev Kit #xxxxx", and tap Connect. Find the Service at the GATT database. There are four services: Device Information
     Service, Hall Effect Service, IMU Service, and Environmental Sensing Service. Try to read, re-read the characteristics
     and enable notification on this service to get the value from sensors.

   Find out more on the API usage at: https://docs.silabs.com/bluetooth/latest/bluetooth-stack-api/

   This example only works with the 'BLE (Silabs)' protocol stack variant.

   Get the Simplicity Connect app:
    - https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo
    - https://apps.apple.com/us/app/efr-connect-ble-mobile-app/id1030932759

   Compatible boards:
   - xG27 DevKit

   Author: Thnguyen3 (Silicon Labs)
 */

#include <Adafruit_Si7021.h>
#include <SparkFun_VEML6030_Ambient_Light_Sensor.h>
#include <ICM20689.h>
#include <Si7210_hall.h>
#include <SilabsMicrophonePDM.h>
#include <math.h>
#include <string.h>

#define SENSOR_ENABLE                                     (PC6)
//*************************************************************************//
// Temperature Sensor
static void rht_temperature_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data);
static void rht_humidity_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data);

static uint16_t rht_humidity = 0;
static int16_t rht_temperature = 0;

Adafruit_Si7021 temp_humidity_sensor;

//*************************************************************************//
// Hall Sensor
#define HALL_SENSOR_ADD                                   (0x30)
#define HALL_SCALE                                        (20000)
#define HALL_MEASUREMENT_INTERVAL_MS                      (250)
#define SL_GATT_SERVICE_HALL_FIELD_STRENGTH_INVALID       (0x7FFFFFFF)

static void hall_update(void);
static void hall_field_strength_notify(void);
static void sl_gatt_service_hall_sensor_step(void);
static float sl_gatt_service_hall_get(void);
static void hall_char_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data);
static void hall_char_config_changed_cb(sl_bt_evt_gatt_server_characteristic_status_t *data);
static void sl_gatt_service_hall_on_event(sl_bt_msg_t * evt);

static uint8_t hall_connection = 0;
static bool hall_field_strength_notification = false;
static int32_t hall_field_strength_value = 0;

Si7210 hall_sensor(Wire, HALL_SENSOR_ADD);

//*************************************************************************//
// Ambient Light Sensor
#define AMBIENT_LIGHT_ADDR                                (0x29)

static void lux_ambient_light_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data);

SparkFun_Ambient_Light light_sensor(AMBIENT_LIGHT_ADDR);

//*************************************************************************//
// IMU Sensor
#define SL_GATT_SERVICE_IMU_OVEC_INVALID                  (0x7FFF)
#define SL_GATT_SERVICE_IMU_AVEC_INVALID                  (0x7FFF)
#define IMU_MEASUREMENT_INTERVAL_MS                       (300)
#define IMU_SPI_CS_PIN                                    (D3)
#define IMU_ENABLE_PIN                                    (PB4)

static void imu_acceleration_notify(void);
static void imu_orientation_notify(void);
static void sl_gatt_service_imu_sensor_step(void);
static void imu_char_config_changed_cb(sl_bt_evt_gatt_server_characteristic_status_t *data);
static sl_status_t sl_gatt_service_imu_get(int16_t ovec[3], int16_t avec[3]);

static uint8_t imu_connection = 0;
static bool imu_acceleration_notification = false;
static bool imu_orientation_notification = false;
static int16_t imu_avec[3] = { 0, 0, 0 };
static int16_t imu_ovec[3] = { 0, 0, 0 };

ICM20689 IMU(SPI, IMU_SPI_CS_PIN);

//*************************************************************************//
//  Mic PDM
#define MIC_IIR_WEIGHT                                    (0.1f)
#define PDM_DATA_PIN                                      (PB1)
#define PDM_CLOCK_PIN                                     (PB0)
#define PDM_PWR_PIN                                       (PC7)

static void sound_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data);
static void sl_gatt_service_sound_on_event(sl_bt_msg_t *evt);

static int16_t sound_level = 0;
static float soundLevel_db = 0;
int16_t pdm_sample_buffer[1000];

MicrophonePDM pdm_mic(PDM_DATA_PIN, PDM_CLOCK_PIN, PDM_PWR_PIN);

//*************************************************************************//
// BLE
static void ble_initialize_gatt_db(void);
static void ble_start_advertising(void);

//*************************************************************************//
// Generic access service
#define ADVERTISE_DEVICE_NAME_LEN_MAX 20

static uint16_t generic_access_service_handle;
static uint16_t name_characteristic_handle;
static const uint8_t advertised_name[] = "Dev Kit #00000";

typedef struct {
  uint8_t flags_length;          // Length of the Flags field
  uint8_t flags_type;            // Type of the Flags field
  uint8_t flags;                 // Flags field
  uint8_t mandatory_data_length; // Length of the mandatory data field
  uint8_t mandatory_data_type;   // Type of the mandatory data field
  uint8_t company_id[2];         // Company ID
  uint8_t firmware_id[2];        // Firmware ID
  uint8_t local_name_length;     // Length of the local name field
  uint8_t local_name_type;       // Type of the local name field
  uint8_t local_name[ADVERTISE_DEVICE_NAME_LEN_MAX]; // Local name field
} advertise_scan_response_t;

static advertise_scan_response_t adv_scan_response;

//*************************************************************************//
// Device information service
static uint16_t device_information_service_handle;
static uint16_t manufacturer_characteristic_handle;
static const uint8_t manufacturer_name[] = "Silicon Laboratories";
static uint16_t model_characteristic_handle;
static const uint8_t model_name[] = "BRD2602A";
static uint16_t serial_characteristic_handle;
static const uint8_t serial_name[] = "4963";
static uint16_t hardware_characteristic_handle;
static const uint8_t hardware_name[] = "A02";
static uint16_t firmware_characteristic_handle;
static const uint8_t firmware_name[] = "4.3.1";
static uint16_t systemid_characteristic_handle;
static const uint8_t systemid_name[] = "0x00";

//*************************************************************************//
static uint16_t gattdb_session_id;

// Hall effect service
static uint16_t hall_effect_service_handle;
static uint16_t hall_effect_field_strength_characteristic_handle;
static uint32_t hall_tick_count = 0;

// IMU service
static uint16_t imu_service_handle;
static uint16_t imu_acceleration_characteristic_handle;
static uint16_t imu_orientation_characteristic_handle;
static uint32_t imu_tick_count = 0;

// Environment service
static uint16_t environment_service_handle;
static uint16_t environment_ambient_light_characteristic_handle;
static uint16_t environment_temperature_characteristic_handle;
static uint16_t environment_humidity_characteristic_handle;
static uint16_t environment_sound_level_characteristic_handle;

void setup()
{
  Serial.begin(115200);
  // Enable power for the Si7210, Si7021 and VEML6035
  pinMode(SENSOR_ENABLE, OUTPUT);
  digitalWrite(SENSOR_ENABLE, HIGH);

  // Enable power for the IMU
  pinMode(IMU_ENABLE_PIN, OUTPUT);
  digitalWrite(IMU_ENABLE_PIN, HIGH);

  // Deselect the SPI flash
  pinMode(PC3, OUTPUT);
  digitalWrite(PC3, HIGH);

  // Wait for the sensor power supplies to stabilize
  delay(500);

  Serial.println("Silicon Labs xG27 Dev Kit BLE sensor example");

  // Init the Si7021 temperature and humidity sensor
  if (!temp_humidity_sensor.begin()) {
    Serial.println("Temperature and humidity sensor initialization failed");
  } else {
    Serial.println("Temperature and humidity sensor initialization successful");
  }

  // Init the VEML6030 light sensor
  if (light_sensor.begin()) {
    Serial.println("Light sensor initialization successful");
  } else {
    Serial.println("Light sensor initialization failed");
  }
  light_sensor.setGain(.125);
  light_sensor.setIntegTime(100);

  // Init the Si7210 hall sensor
  if (hall_sensor.begin()) {
    Serial.println("Hall sensor initialization successful");
  } else {
    Serial.println("Hall sensor initialization failed");
  }

  // Init the PDM microphone
  if (pdm_mic.begin(1, 44100, pdm_sample_buffer, 1000)) {
    pdm_mic.getSamples();
    Serial.println("Microphone initialization successful");
  } else {
    Serial.println("Microphone initialization failed");
  }

  // Init the ICM20689 IMU
  if (IMU.begin() >= 0) {
    Serial.println("IMU initialization successful");
  } else {
    Serial.println("IMU initialization failed");
  }
}

void loop()
{
  // Continuously sample the PDM microphone then apply an IIR filter to the measured value
  if (pdm_mic.sampleBufferReady()) {
    float sl;
    if (pdm_mic.calculateSoundLevel(sl, 0)) {
      soundLevel_db = (sl * MIC_IIR_WEIGHT) + (soundLevel_db * (1 - MIC_IIR_WEIGHT));
    }
    pdm_mic.getSamples();
  }

  // Process the value from the IMU every IMU_MEASUREMENT_INTERVAL_MS
  if ((millis() - imu_tick_count) >= IMU_MEASUREMENT_INTERVAL_MS) {
    imu_tick_count = millis();
    sl_gatt_service_imu_sensor_step();
  }

  // Process the value from hall sensor every HALL_MEASUREMENT_INTERVAL_MS
  if ((millis() - hall_tick_count) >= HALL_MEASUREMENT_INTERVAL_MS) {
    hall_tick_count = millis();
    sl_gatt_service_hall_sensor_step();
  }
}

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_gatt_service_hall_on_event(evt);
  sl_gatt_service_imu_on_event(evt);
  sl_gatt_service_sound_on_event(evt);

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
    {
      Serial.begin(115200);
      Serial.println("BLE stack booted");

      // Initialize the application specific GATT table
      ble_initialize_gatt_db();
      ble_start_advertising();
      Serial.println("BLE advertisement started");
    }
    break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("BLE connection opened");
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      Serial.println("BLE connection closed");

      // Restart the advertisement
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
      break;

    // -------------------------------
    // This event indicates that the value of an attribute in the local GATT
    // database was changed by a remote GATT client
    case sl_bt_evt_gatt_server_attribute_value_id:
      break;

    // -------------------------------
    // This event is received when a GATT characteristic status changes
    case sl_bt_evt_gatt_server_characteristic_status_id:
      break;

    // -------------------------------
    // Indicates that a remote GATT client is attempting to read a value of an attribute from the local GATT database
    case sl_bt_evt_gatt_server_user_read_request_id:
    {
      sl_bt_evt_gatt_server_user_read_request_t *data = &evt->data.evt_gatt_server_user_read_request;

      // Environment Sensing Characteristic Read Request
      if (environment_temperature_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic) {
        rht_temperature_read_cb(data);
      } else if (environment_humidity_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic) {
        rht_humidity_read_cb(data);
      } else if (environment_ambient_light_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic) {
        lux_ambient_light_read_cb(data);
      }
    }
    break;

    // -------------------------------
    // Default event handler
    default:
      break;
  }
}

/**************************************************************************//**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
static void ble_start_advertising(void)
{
  static uint8_t advertising_set_handle = 0xff;
  static bool init = true;
  sl_status_t sc;

  if (init) {
    bd_addr address;
    uint8_t address_type;
    uint32_t unique_id;
    const char* device_name_prefix = (const char*)"Dev Kit #";  // Full device name = "Dev Kit #00000"
    uint8_t device_name_prefix_length = strlen((const char*)device_name_prefix);
    char full_device_name[ADVERTISE_DEVICE_NAME_LEN_MAX];

    // Create advertise scan response data

    // Get unique id
    sl_bt_system_get_identity_address(&address, &address_type);
    unique_id = 0xFFFFFF & *((uint32_t*) address.addr);

    // Update full_device_name = "device_name_prefix + unique id"
    // Copy full_device_name to adv_scan_response.local_name
    strcpy(full_device_name, (const char*)device_name_prefix);
    (void)snprintf((char*)(full_device_name + device_name_prefix_length), 6, "%05d", (int)(unique_id & 0xFFFF));
    strcpy((char*)adv_scan_response.local_name, (const char*)full_device_name);

    adv_scan_response.local_name_length = strlen((const char*)adv_scan_response.local_name) + 1;
    adv_scan_response.local_name_type = 0x09;       // Local name type
    adv_scan_response.flags_length = 2;             // Advertise flags length
    adv_scan_response.flags_type = 0x01;            // Advertise flags type
    adv_scan_response.flags = 0x02 | 0x04;          // Advertise flags LE_GENERAL_DISCOVERABLE and BR_EDR_NOT_SUPPORTED
    adv_scan_response.mandatory_data_length = 5;    // Advertise mandatory data length
    adv_scan_response.mandatory_data_type = 0xFF;   // Advertise mandatory data type manufacturer
    adv_scan_response.company_id[0] = 0x47;         // Company ID = 0x0047
    adv_scan_response.company_id[1] = 0x00;
    adv_scan_response.firmware_id[0] = 0x02;        // Firmware ID = 0x0002
    adv_scan_response.firmware_id[1] = 0x00;

    // Create an advertising set
    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert_status(sc);

    // Set advertising interval to 100ms
    sc = sl_bt_advertiser_set_timing(
      advertising_set_handle,
      160,   // minimum advertisement interval (milliseconds * 1.6)
      160,   // maximum advertisement interval (milliseconds * 1.6)
      0,     // advertisement duration
      0);    // maximum number of advertisement events
    app_assert_status(sc);
    init = false;
  }

  // Set user-defined advertising data packet or scan response packet on an advertising set.
  sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, sizeof(adv_scan_response), (uint8_t*)(&adv_scan_response));

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
  app_assert_status(sc);
}

/**************************************************************************//**
 * Read the temperature from Si7021 sensor and send the value over BLE
 * @param[in] data sl_bt_evt_gatt_server_user_read_request_t
 * coming from the Bluetooth stack
 *****************************************************************************/
static void rht_temperature_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data)
{
  // update measurement data
  float temp = temp_humidity_sensor.readTemperature();

  // Convert to integer value
  rht_temperature = temp * 100;

  Serial.print("temperature_characteristic read response; value=");
  Serial.println(temp);

  sl_status_t sc = sl_bt_gatt_server_send_user_read_response(
    data->connection,
    data->characteristic,
    0,
    sizeof(rht_temperature),
    (uint8_t*)&rht_temperature,
    NULL);

  app_assert_status(sc);
}

/**************************************************************************//**
 * Read the humidity from Si7021 sensor and send the value over BLE
 * @param[in] data sl_bt_evt_gatt_server_user_read_request_t
 * coming from the Bluetooth stack
 *****************************************************************************/
static void rht_humidity_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data)
{
  // update measurement data
  float humidity = temp_humidity_sensor.readHumidity();
  // Convert to integer value
  rht_humidity = humidity * 100;

  Serial.print("humidity_characteristic read response; value=");
  Serial.println(humidity);

  sl_status_t sc = sl_bt_gatt_server_send_user_read_response(
    data->connection,
    data->characteristic,
    0,
    sizeof(rht_humidity),
    (uint8_t*)&rht_humidity,
    NULL);

  app_assert_status(sc);
}

/**************************************************************************//**
 * Read the lux value from ambient light sensor and send the value over BLE
 * @param[in] data sl_bt_evt_gatt_server_user_read_request_t
 * coming from the Bluetooth stack
 *****************************************************************************/
static void lux_ambient_light_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data)
{
  // update measurement data
  uint32_t luxVal = light_sensor.readLight() * 100;

  Serial.print("lux_ambient_light_characteristic response; value=");
  Serial.println(luxVal);

  sl_status_t sc = sl_bt_gatt_server_send_user_read_response(
    data->connection,
    data->characteristic,
    0,
    sizeof(luxVal),
    (uint8_t*)&luxVal,
    NULL);

  app_assert_status(sc);
}

/**************************************************************************//**
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
static void ble_initialize_gatt_db(void)
{
  sl_status_t sc;
  // Create a new GATT database
  sc = sl_bt_gattdb_new_session(&gattdb_session_id);
  app_assert_status(sc);

  //*************************************************************************//
  // Add the Generic Access service to the GATT DB
  const uint8_t generic_access_service_uuid[] = { 0x00, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(generic_access_service_uuid),
                                generic_access_service_uuid,
                                &generic_access_service_handle);
  app_assert_status(sc);

  // Add the Device Name characteristic to the Generic Access service
  // The value of the Device Name characteristic will be advertised
  const sl_bt_uuid_16_t device_name_characteristic_uuid = { .data = { 0x00, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              generic_access_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              device_name_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(advertised_name) - 1,
                                              sizeof(advertised_name) - 1,
                                              advertised_name,
                                              &name_characteristic_handle);
  app_assert_status(sc);

  // Start the Generic Access service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, generic_access_service_handle);
  app_assert_status(sc);

  //*************************************************************************//
  // Add the Device Information service to the GATT DB
  const uint8_t device_information_service_uuid[] = { 0x0A, 0x18 };
  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(device_information_service_uuid),
                                device_information_service_uuid,
                                &device_information_service_handle);
  app_assert_status(sc);

  // Add the Manufacturer Name String characteristic to the Device Information service
  // The value of the Manufacturer Name String characteristic will be advertised
  const sl_bt_uuid_16_t manufacturer_name_characteristic_uuid = { .data = { 0x29, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              manufacturer_name_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(manufacturer_name) - 1,
                                              sizeof(manufacturer_name) - 1,
                                              manufacturer_name,
                                              &manufacturer_characteristic_handle);
  app_assert_status(sc);

  // Add the Model Number String characteristic to the Device Information service
  // The value of the Model Number String characteristic will be advertised
  const sl_bt_uuid_16_t model_name_characteristic_uuid = { .data = { 0x24, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              model_name_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(model_name) - 1,
                                              sizeof(model_name) - 1,
                                              model_name,
                                              &model_characteristic_handle);
  app_assert_status(sc);

  // Add the Serial Number String characteristic to the Device Information service
  // The value of the Serial Number String characteristic will be advertised
  const sl_bt_uuid_16_t serial_number_characteristic_uuid = { .data = { 0x25, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              serial_number_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(serial_name) - 1,
                                              sizeof(serial_name) - 1,
                                              serial_name,
                                              &serial_characteristic_handle);
  app_assert_status(sc);

  // Add the Hardware Revision String characteristic to the Device Information service
  // The value of the Hardware Revision String characteristic will be advertised
  const sl_bt_uuid_16_t hardware_revision_characteristic_uuid = { .data = { 0x27, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              hardware_revision_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(hardware_name) - 1,
                                              sizeof(hardware_name) - 1,
                                              hardware_name,
                                              &hardware_characteristic_handle);
  app_assert_status(sc);

  // Add the Firmware Revision String characteristic to the Device Information service
  // The value of the Firmware Revision String characteristic will be advertised
  const sl_bt_uuid_16_t firmware_revision_characteristic_uuid = { .data = { 0x26, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              firmware_revision_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(firmware_name) - 1,
                                              sizeof(firmware_name) - 1,
                                              firmware_name,
                                              &firmware_characteristic_handle);
  app_assert_status(sc);

  // Add the System ID String characteristic to the Device Information service
  // The value of the System ID String characteristic will be advertised
  const sl_bt_uuid_16_t system_id_characteristic_uuid = { .data = { 0x23, 0x2A } };
  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              device_information_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              system_id_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              sizeof(systemid_name) - 1,
                                              sizeof(systemid_name) - 1,
                                              systemid_name,
                                              &systemid_characteristic_handle);
  app_assert_status(sc);

  // Start the Device Information service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, device_information_service_handle);
  app_assert_status(sc);

  //*************************************************************************//
  // Add the Hall Effect service to the GATT DB
  const uuid_128 hall_effect_service_uuid = { .data = {
                                                0x7f, 0x95, 0x4f, 0xaa,
                                                0xd1, 0xb3, 0x36, 0x99,
                                                0xc5, 0x4e, 0x00, 0x2f,
                                                0xc5, 0xdb, 0x98, 0xf5
                                              } };

  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(hall_effect_service_uuid),
                                hall_effect_service_uuid.data,
                                &hall_effect_service_handle);
  app_assert_status(sc);

  // Add the field strength characteristic to the hall effect service
  // The value of the field strength characteristic will be advertised
  const uuid_128 hall_effect_field_strength_characteristic_uuid = { .data = {
                                                                      0x7f, 0x95, 0x4f, 0xaa,
                                                                      0xd1, 0xb3, 0x36, 0x99,
                                                                      0xc5, 0x4e, 0x02, 0x2f,
                                                                      0xc5, 0xdb, 0x98, 0xf5
                                                                    } };

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               hall_effect_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                               0x00,
                                               0x00,
                                               hall_effect_field_strength_characteristic_uuid,
                                               sl_bt_gattdb_user_managed_value,
                                               0,
                                               0,
                                               NULL,
                                               &hall_effect_field_strength_characteristic_handle);
  app_assert_status(sc);

  // Start the Hall Effect service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, hall_effect_service_handle);
  app_assert_status(sc);

  //*************************************************************************//
  // Add the IMU service to the GATT DB
  const uuid_128 imu_service_uuid = { .data {
                                        0x9f, 0xdc, 0x9c, 0x81,
                                        0xff, 0xfe, 0x5d, 0x88,
                                        0xe5, 0x11, 0xe5, 0x4b,
                                        0xf4, 0x49, 0xe6, 0xa4
                                      } };

  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(imu_service_uuid),
                                imu_service_uuid.data,
                                &imu_service_handle);
  app_assert_status(sc);

  // Add the Acceleration characteristic to the imu service
  // The value of the Acceleration characteristic will be advertised
  const uuid_128 imu_acceleration_characteristic_uuid = { .data = {
                                                            0x9f, 0xdc, 0x9c, 0x81,
                                                            0xff, 0xfe, 0x5d, 0x88,
                                                            0xe5, 0x11, 0xe5, 0x4b,
                                                            0xe2, 0xf6, 0xc1, 0xc4
                                                          } };
  uint8_t imu_avec[3] = { 0, 0, 0 };

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               imu_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                               0x00,
                                               0x00,
                                               imu_acceleration_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               3,
                                               3,
                                               imu_avec,
                                               &imu_acceleration_characteristic_handle);
  app_assert_status(sc);

  // Add the Orientation characteristic to the imu service
  // The value of the Orientation characteristic will be advertised
  const uuid_128 imu_orientation_characteristic_uuid = { .data = {
                                                           0x9a, 0xf4, 0x94, 0xe9,
                                                           0xb5, 0xf3, 0x9f, 0xba,
                                                           0xdd, 0x45, 0xe3, 0xbe,
                                                           0x94, 0xb6, 0xc4, 0xb7
                                                         } };
  uint8_t imu_ovec[3] = { 0, 0, 0 };

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               imu_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_NOTIFY,
                                               0x00,
                                               0x00,
                                               imu_orientation_characteristic_uuid,
                                               sl_bt_gattdb_fixed_length_value,
                                               3,
                                               3,
                                               imu_ovec,
                                               &imu_orientation_characteristic_handle);
  app_assert_status(sc);

  // Start the imu service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, imu_service_handle);
  app_assert_status(sc);

  //*************************************************************************//
  // Add the Environment Sensing service to the GATT DB
  const uint8_t environment_service_uuid[] = { 0x1a, 0x18 };

  sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                sl_bt_gattdb_primary_service,
                                SL_BT_GATTDB_ADVERTISED_SERVICE,
                                sizeof(environment_service_uuid),
                                environment_service_uuid,
                                &environment_service_handle);
  app_assert_status(sc);

  // Add the Ambient Light characteristic to the Environment Sensing service
  // The value of the Ambient Light characteristic will be advertised
  const uuid_128 ambient_light_characteristic_uuid = { .data = {
                                                         0x2e, 0xa3, 0xf4, 0x54,
                                                         0x87, 0x9f, 0xde, 0x8d,
                                                         0xeb, 0x45, 0xd9, 0xbf,
                                                         0x13, 0x69, 0x54, 0xc8
                                                       } };

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               environment_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ,
                                               0x00,
                                               0x00,
                                               ambient_light_characteristic_uuid,
                                               sl_bt_gattdb_user_managed_value,
                                               0,
                                               0,
                                               NULL,
                                               &environment_ambient_light_characteristic_handle);
  app_assert_status(sc);

  // Add the Temperature characteristic to the  Environment Sensing service
  // The value of the Temperature characteristic will be advertised
  const sl_bt_uuid_16_t temperature_characteristic_uuid = { .data = {
                                                              0x6e, 0x2a
                                                            } };

  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              environment_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              temperature_characteristic_uuid,
                                              sl_bt_gattdb_user_managed_value,
                                              0,
                                              0,
                                              NULL,
                                              &environment_temperature_characteristic_handle);
  app_assert_status(sc);

  // Add the Humidity characteristic to the  Environment Sensing service
  // The value of the Humidity characteristic will be advertised
  const sl_bt_uuid_16_t humidity_characteristic_uuid = { .data = {
                                                           0x6f, 0x2a
                                                         } };

  sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                              environment_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_READ,
                                              0x00,
                                              0x00,
                                              humidity_characteristic_uuid,
                                              sl_bt_gattdb_user_managed_value,
                                              0,
                                              0,
                                              NULL,
                                              &environment_humidity_characteristic_handle);
  app_assert_status(sc);

  // Add the Sound Level characteristic to the  Environment Sensing service
  // The value of the Sound Level characteristic will be advertised
  const uuid_128 soundlevel_characteristic_uuid = { .data = {
                                                      0x2e, 0xa3, 0xf4, 0x54,
                                                      0x87, 0x9f, 0xde, 0x8d,
                                                      0xeb, 0x45, 0x02, 0xbf,
                                                      0x13, 0x69, 0x54, 0xc8
                                                    } };

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                               environment_service_handle,
                                               SL_BT_GATTDB_CHARACTERISTIC_READ,
                                               0x00,
                                               0x00,
                                               soundlevel_characteristic_uuid,
                                               sl_bt_gattdb_user_managed_value,
                                               0,
                                               0,
                                               NULL,
                                               &environment_sound_level_characteristic_handle);
  app_assert_status(sc);

  // Start the Environment Sensing service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, environment_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}

/**************************************************************************//**
 * Update the field_strength value from hall sensor
 * convert the value from mT to uT, round to closest integer
 *****************************************************************************/
static void hall_update(void)
{
  // get measurement values
  float field_strength = sl_gatt_service_hall_get();

  // convert mT to uT, round to closest integer
  hall_field_strength_value = lroundf(field_strength * 1000);
}

/**************************************************************************//**
 * Notify the field strength value from hall sensor over BLE
 *****************************************************************************/
static void hall_field_strength_notify(void)
{
  sl_status_t sc = sl_bt_gatt_server_send_notification(
    hall_connection,
    hall_effect_field_strength_characteristic_handle,
    sizeof(hall_field_strength_value),
    (uint8_t*)&hall_field_strength_value);

  app_assert_status(sc);

  Serial.print("hall_field_strength_notify response; value=");
  Serial.println(hall_field_strength_value);
}

/**************************************************************************//**
 * sl_gatt_service_hall_sensor_step: This function is called every
 * HALL_MEASUREMENT_INTERVAL_MS to update field strength value and
 * send a notification over BLE.
 *****************************************************************************/
static void sl_gatt_service_hall_sensor_step(void)
{
  hall_update();

  if (hall_field_strength_notification) {
    hall_field_strength_notify();
  }
}

/**************************************************************************//**
 * This function is called when a hall field effect characteristic
 * read request is received. It updates the field strength value from the
 * hall sensor and sends this value over BLE.
 *****************************************************************************/
static void hall_char_read_cb(sl_bt_evt_gatt_server_user_read_request_t * data)
{
  uint8_t* value = NULL;
  size_t value_len = 0;

  hall_update();

  if (hall_effect_field_strength_characteristic_handle == data->characteristic) {
    value = (uint8_t*)&hall_field_strength_value;
    value_len = sizeof(hall_field_strength_value);
    Serial.print("hall_char_read_cb response; value=");
    Serial.println(hall_field_strength_value);

    sl_status_t sc = sl_bt_gatt_server_send_user_read_response(
      data->connection,
      data->characteristic,
      0,
      value_len,
      value,
      NULL);

    app_assert_status(sc);
  }
}

/**************************************************************************//**
 * This function is called when the hall field effect characteristic
 * configuration is changed. It updates the 'hall_field_strength_notification'
 * flag accordingly.
 *****************************************************************************/
static void hall_char_config_changed_cb(sl_bt_evt_gatt_server_characteristic_status_t * data)
{
  bool enable = sl_bt_gatt_disable != data->client_config_flags;
  hall_connection = data->connection;

  if (hall_effect_field_strength_characteristic_handle == data->characteristic) {
    hall_field_strength_notification = enable;

    if (enable) {
      // update measurement data
      hall_update();
      // send the first notification
      hall_field_strength_notify();
    }
  }
}

/**************************************************************************//**
 * Process BLE event related to hall effect service
 *****************************************************************************/
static void sl_gatt_service_hall_on_event(sl_bt_msg_t * evt)
{
  // Handle stack events
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_connection_closed_id:
      hall_field_strength_notification = false;
      break;

    case sl_bt_evt_gatt_server_user_read_request_id:
      if ((hall_effect_field_strength_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic)) {
        hall_char_read_cb(&evt->data.evt_gatt_server_user_read_request);
      }
      break;

    case sl_bt_evt_gatt_server_characteristic_status_id:
      if ((sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags)
          && (hall_effect_field_strength_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic)) {
        // client characteristic configuration changed by remote GATT client
        hall_char_config_changed_cb(&evt->data.evt_gatt_server_characteristic_status);
      }
      break;
  }
}

/**************************************************************************//**
 * Get field strength from hall effect sensor
 *****************************************************************************/
static float sl_gatt_service_hall_get(void)
{
  return hall_sensor.measure(HALL_SCALE);;
}

/**************************************************************************//**
 * imu_acceleration_notify
 *****************************************************************************/
static void imu_acceleration_notify(void)
{
  sl_status_t sc = sl_bt_gatt_server_send_notification(
    imu_connection,
    imu_acceleration_characteristic_handle,
    sizeof(imu_avec),
    (uint8_t*)imu_avec);

  if (sc != SL_STATUS_OK) {
    Serial.print("Failed to send characteristic notification; sc=0x");
    Serial.println((int)sc, HEX);
  } else {
    Serial.print("IMU acceleration notify: ");
    Serial.print(imu_avec[0]);
    Serial.print(" *** ");
    Serial.print(imu_avec[1]);
    Serial.print(" *** ");
    Serial.println(imu_avec[2]);
    Serial.println("****************");
  }
}

/**************************************************************************//**
 * imu_orientation_notify
 *****************************************************************************/
static void imu_orientation_notify(void)
{
  sl_status_t sc = sl_bt_gatt_server_send_notification(
    imu_connection,
    imu_orientation_characteristic_handle,
    sizeof(imu_ovec),
    (uint8_t*)imu_ovec);
  if (sc != SL_STATUS_OK) {
    Serial.print("Failed to send characteristic notification; sc=0x");
    Serial.println((int)sc, HEX);
  } else {
    Serial.print("IMU orientation notify: ");
    Serial.print(imu_ovec[0]);
    Serial.print(" *** ");
    Serial.print(imu_ovec[1]);
    Serial.print(" *** ");
    Serial.println(imu_ovec[2]);
    Serial.println("****************");
  }
}

/**************************************************************************//**
 * Update imu_acceleration_notification and imu_orientation_notification value
 *****************************************************************************/
static void imu_char_config_changed_cb(sl_bt_evt_gatt_server_characteristic_status_t *data)
{
  bool enable = sl_bt_gatt_disable != data->client_config_flags;
  imu_connection = data->connection;

  if (imu_acceleration_characteristic_handle == data->characteristic) {
    imu_acceleration_notification = enable;
  } else if (imu_orientation_characteristic_handle == data->characteristic) {
    imu_orientation_notification = enable;
  }
}

/**************************************************************************//**
 * Process BLE event related to the IMU sensor service
 *****************************************************************************/
void sl_gatt_service_imu_on_event(sl_bt_msg_t *evt)
{
  // Handle stack events
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_connection_closed_id:
      imu_acceleration_notification = false;
      imu_orientation_notification = false;
      break;

    case sl_bt_evt_gatt_server_characteristic_status_id:
      if ((sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags)
          && ((imu_acceleration_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic)
              || (imu_orientation_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic))) {
        // client characteristic configuration changed by remote GATT client
        imu_char_config_changed_cb(&evt->data.evt_gatt_server_characteristic_status);
      }
      break;
  }
}

/**************************************************************************//**
 * sl_gatt_service_imu_sensor_step: This function is called every
 * IMU_MEASUREMENT_INTERVAL_MS to get IMU sensor values
 * and send a notification over BLE.
 *****************************************************************************/
static void sl_gatt_service_imu_sensor_step(void)
{
  sl_status_t sc = sl_gatt_service_imu_get(imu_ovec, imu_avec);

  if (SL_STATUS_OK == sc || SL_STATUS_NOT_INITIALIZED == sc) {
    if (SL_STATUS_NOT_INITIALIZED == sc) {
      for (int i = 0; i < 3; i++) {
        imu_ovec[i] = SL_GATT_SERVICE_IMU_OVEC_INVALID;
        imu_avec[i] = SL_GATT_SERVICE_IMU_AVEC_INVALID;
      }
    }

    if (imu_acceleration_notification) {
      imu_acceleration_notify();
    }
    if (imu_orientation_notification) {
      imu_orientation_notify();
    }
  }
}

/**************************************************************************//**
 * Get IMU sensor value (acceleration and orientation)
 *****************************************************************************/
static sl_status_t sl_gatt_service_imu_get(int16_t ovec[3], int16_t avec[3])
{
  IMU.readSensor();
  ovec[0] = IMU.getAccelX_mss();
  ovec[1] = IMU.getAccelY_mss();
  ovec[2] = IMU.getAccelZ_mss();

  avec[0] = IMU.getGyroX_rads();
  avec[1] = IMU.getGyroY_rads();
  avec[2] = IMU.getGyroZ_rads();

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * Process BLE event related to the Sound Level service
 *****************************************************************************/
static void sl_gatt_service_sound_on_event(sl_bt_msg_t *evt)
{
  // Handle stack events
  if (sl_bt_evt_gatt_server_user_read_request_id == SL_BT_MSG_ID(evt->header)) {
    if (environment_sound_level_characteristic_handle == evt->data.evt_gatt_server_user_read_request.characteristic) {
      sound_read_cb(&evt->data.evt_gatt_server_user_read_request);
    }
  }
}

/**************************************************************************//**
 * Convert sound level from dB to 0.01 dB then send this value over BLE
 *****************************************************************************/
static void sound_read_cb(sl_bt_evt_gatt_server_user_read_request_t *data)
{
  /* Sound level from sensor represented in dB, characteristic represented in 0.01 dB */
  sound_level = (int16_t)(soundLevel_db * 100.0f);

  sl_status_t sc = sl_bt_gatt_server_send_user_read_response(
    data->connection,
    data->characteristic,
    0,
    sizeof(sound_level),
    (uint8_t*)&sound_level,
    NULL);
  app_assert_status(sc);

  Serial.print("sound_read_cb response; value=");
  Serial.println(soundLevel_db);
}

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif
