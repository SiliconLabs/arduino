/*
   xG27 Dev Kit sensors example

   This example collects and processes sensor data from the xG27 Dev Kit and prints them on the serial console.

   The xG27 Dev Kit contains a set of sensors that can be accessed from the EFR32BG27. These are:
   - One Silicon Labs Si7021 relative humidity & temperature sensor
      Adafruit_Si7021 (https://github.com/adafruit/Adafruit_Si7021)
   - One Silicon Labs Si7210 hall effect sensor
      Si7210Hall (Silabs library)
   - One Vishay VEML6035 ambient light sensor
      SparkFun_Ambient_Light_Sensor_Arduino_Library (https://github.com/sparkfun/SparkFun_Ambient_Light_Sensor_Arduino_Library/tree/master)
   - One TDK InvenSense ICM-20648 6-axis inertial measurement sensor
      ICM20689 (https://github.com/finani/ICM20689)
   - Two Knowles SPK0641HT4H-1 MEMS microphones with PDM output
      SilabsMicrophonePDM (included with the core)

   Compatible boards:
   - xG27 Dev Kit

   Author: Nghia Bui (Silicon Labs)
 */

#include <Adafruit_Si7021.h>
#include <SparkFun_VEML6030_Ambient_Light_Sensor.h>
#include <ICM20689.h>
#include <Si7210_hall.h>
#include <SilabsMicrophonePDM.h>

#define IMU_SPI_CS_PIN            D3

#define LIGHT_SENSOR_I2C_ADDR     0x29

#define HALL_SENSOR_I2C_ADDR      0x30
#define HALL_SCALE                20000

#define PDM_MIC_IIR_WEIGHT        0.1f
#define PDM_SAMPLE_RATE           44100
#define PDM_SAMPLE_BUFFER_SIZE    1000
#define PDM_N_CHANNELS            1

int16_t pdm_sample_buffer[PDM_SAMPLE_BUFFER_SIZE];

Si7210 hall_sensor(Wire, HALL_SENSOR_I2C_ADDR);
Adafruit_Si7021 temp_humidity_sensor;
SparkFun_Ambient_Light light_sensor(LIGHT_SENSOR_I2C_ADDR);
ICM20689 IMU(SPI, IMU_SPI_CS_PIN);
MicrophonePDM pdm_mic(PB1, PB0, PC7);

void measureSensors()
{
  float mag = hall_sensor.measure(HALL_SCALE);
  float humidity = temp_humidity_sensor.readHumidity();
  float temp = temp_humidity_sensor.readTemperature();
  long luxVal = light_sensor.readLight();
  static float soundLevel;
  IMU.readSensor();
  if (pdm_mic.sampleBufferReady()) {
    float sl;
    if (pdm_mic.calculateSoundLevel(sl, 0)) {
      // Apply IIR filter on the measured value
      soundLevel = (sl * PDM_MIC_IIR_WEIGHT) + (soundLevel * (1 - PDM_MIC_IIR_WEIGHT));
    }
    pdm_mic.getSamples();
  }

  Serial.print("Sound level: ");
  Serial.print(soundLevel);
  Serial.print(" dB | ");

  Serial.print("Magnetic flux: ");
  Serial.print(mag, 3);
  Serial.print(" mT | ");

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% | ");

  Serial.print("Light: ");
  Serial.print(luxVal);
  Serial.println(" lux | ");

  Serial.print("accel X: ");
  Serial.print(IMU.getAccelX_mss(), 6);
  Serial.print(" | accel Y: ");
  Serial.print(IMU.getAccelY_mss(), 6);
  Serial.print(" | accel Z: ");
  Serial.println(IMU.getAccelZ_mss(), 6);
}

void setup()
{
  Serial.begin(115200);
  // Enable power for the Si7210, Si7021 and VEML6035
  pinMode(PC6, OUTPUT);
  digitalWrite(PC6, HIGH);
  // Enable power for the IMU
  pinMode(PB4, OUTPUT);
  digitalWrite(PB4, HIGH);
  // Enable power for the microphones
  pinMode(PC7, OUTPUT);
  digitalWrite(PC7, HIGH);
  // Deselect the SPI flash
  pinMode(PC3, OUTPUT);
  digitalWrite(PC3, HIGH);
  // Wait for the sensor power supplies to stabilize
  delay(200);

  Serial.println("xG27 Dev Kit sensors example");
  // Init the PDM microphone
  int status = pdm_mic.begin(PDM_N_CHANNELS,
                             PDM_SAMPLE_RATE,
                             pdm_sample_buffer,
                             PDM_SAMPLE_BUFFER_SIZE);
  if (status < 0) {
    Serial.println("Microphone initialization failed");
    Serial.print("Status: ");
    Serial.println(status);
  } else {
    Serial.println("Microphone initialization successful");
  }
  pdm_mic.getSamples();

  // Init the Si7021 temperature and humidity sensor
  if (!temp_humidity_sensor.begin()) {
    Serial.println("Temperature and humidity sensor initialization failed");
  } else {
    Serial.println("Temperature and humidity sensor initialization successful");
  }

  // Init the Si7210 hall sensor
  if (!hall_sensor.begin()) {
    Serial.println("Hall sensor initialization failed");
  } else {
    Serial.println("Hall sensor initialization successful");
  }

  // Init the VEML6030 light sensor
  if (light_sensor.begin()) {
    Serial.println("Light sensor initialization successful");
  } else {
    Serial.println("Light sensor initialization failed");
  }
  light_sensor.setGain(.125);
  light_sensor.setIntegTime(100);

  // Init the ICM20689 IMU
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization failed");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }
}

void loop()
{
  const uint32_t MEASUREMENT_INTERVAL_MS = 500u;
  static uint32_t previousMillis = 0u;
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {
    previousMillis = currentMillis;
    measureSensors();
  }
}
