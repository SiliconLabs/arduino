/*
 * Magic Wand - IMU Capture Data Logging
 *
 * This example demonstrates how to use the Modulino Movement Module with the LSM6DSOX
 * sensor for motion detection and data logging. The program waits for a
 * significant movement, collects 100 samples of sensor data, and outputs it
 * in CSV format over Serial.
 *
 * The circuit:
 *  - A Silicon Labs MCU based board supporting AI/ML (see the readme)
 *  - LSM6DSOX IMU sensor connected to Wire (I2C)
 *
 * Features:
 *  - Detects significant motion based on an acceleration threshold.
 *  - Logs acceleration and gyroscope data after motion is detected.
 *  - Outputs data in a format suitable for analysis (CSV).
 *
 * Libraries:
 *  - stm32duino LSM6DSOX library (https://github.com/stm32duino/LSM6DSOX)
 *
 * This example code is in the public domain.
 * Copyright (c) 2025 Arduino
 */

#include "LSM6DSOXSensor.h"

// The QWIIC connector on the xG24 Explorer Kit is assigned to Wire1
// On other boards we use the default I2C instance (Wire)
#ifdef ARDUINO_BOARD_SILABS_XG24EXPLORERKIT
TwoWire& IMU_Wire = Wire1;
#else
TwoWire& IMU_Wire = Wire;
#endif

#define FREQUENCY_IMU           52.0f // Available values are +- 12.0, 26.0, 52.0, 104.0, 208.0, 416.0, 833.0, 1667.0, 3333.0, 6667.0 Hz
LSM6DSOXSensor lsm6dsoxSensor = LSM6DSOXSensor(&IMU_Wire, LSM6DSOX_I2C_ADD_L);

const uint32_t accelerationThreshold = 1500; // threshold of significant in mG's
const int numSamples = 100;

int samplesRead = numSamples;

void setup()
{
  Serial.begin(115200);
  while (!Serial) ;

  IMU_Wire.begin();

  // Default clock is 100kHz. LSM6DSOX also supports 400kHz, let's use it
  IMU_Wire.setClock(400000);

  // Init the sensor
  lsm6dsoxSensor.begin();

  // Enable accelerometer and gyroscope
  if (lsm6dsoxSensor.Enable_X() != LSM6DSOX_OK || lsm6dsoxSensor.Enable_G() != LSM6DSOX_OK) {
    Serial.println("Error enabling accelero and gyro");
    while (1) ;
  }

  // Available values are +- 2, 4, 8, 16 G
  lsm6dsoxSensor.Set_X_FS(2);

  // Set Accelerometer sample rate
  lsm6dsoxSensor.Set_X_ODR(FREQUENCY_IMU);

  // Available values are +- 125, 250, 500, 1000, 2000 dps
  lsm6dsoxSensor.Set_G_FS(2000);

  // Set Gyroscope sample rate
  lsm6dsoxSensor.Set_G_ODR(FREQUENCY_IMU);

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop()
{
  int32_t acceleration[3];
  int32_t rotation[3];

  // wait for significant motion
  while (samplesRead == numSamples) {
    uint8_t acceleroStatus;
    lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);
    if (acceleroStatus == 1) {
      // read the acceleration data
      lsm6dsoxSensor.Get_X_Axes(acceleration);

      // sum up the absolutes
      uint32_t aSum = fabs(acceleration[0]) + fabs(acceleration[1]) + fabs(acceleration[2]);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    uint8_t acceleroStatus;
    uint8_t gyroStatus;
    lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);
    lsm6dsoxSensor.Get_G_DRDY_Status(&gyroStatus);
    if (acceleroStatus == 1 && gyroStatus == 1) {
      // read the acceleration
      lsm6dsoxSensor.Get_X_Axes(acceleration);
      lsm6dsoxSensor.Get_G_Axes(rotation);

      samplesRead++;

      // print the data in CSV format
      Serial.print(acceleration[0]); // mg (+-2000)
      Serial.print(',');
      Serial.print(acceleration[1]);
      Serial.print(',');
      Serial.print(acceleration[2]);
      Serial.print(',');
      Serial.print(rotation[0] / 1000); // dps (+- 2000)
      Serial.print(',');
      Serial.print(rotation[1] / 1000);
      Serial.print(',');
      Serial.print(rotation[2] / 1000);
      Serial.println();
    }
  }
}
