/*
 * Magic Wand (with Silabs Hardware Accelerated Kernel)
 *
 * This example demonstrates gesture recognition using an AI/ML compatible board
 * combined with the Modulino Movement module. The program reads accelerometer
 * and gyroscope data, processes it through a TensorFlow Lite model, and detects
 * predefined gestures: wing, ring, and slope. If no significant movement is detected,
 * the system remains idle until motion occurs.
 *
 * The program initializes the TensorFlow Lite interpreter, sets up the LSM6DSOX
 * sensor for data collection, and runs inference to recognize gestures based
 * on real-time IMU data.
 *
 * The circuit:
 *  - A Silicon Labs MCU based board supporting AI/ML (see the readme)
 *  - LSM6DSOX IMU sensor connected via Wire (I2C)
 *
 * Libraries:
 *  - stm32duino LSM6DSOX library (https://github.com/stm32duino/LSM6DSOX)
 *
 * This example code is in the public domain.
 * Copyright (c) 2025 Arduino
 */

#include "SilabsTFLiteMicro.h"
#include "LSM6DSOXSensor.h"

#define SEQUENCE_LENGTH         100
#define SIGNAL_CHANNELS         6

#define GESTURE_COUNT            4
#define WING_GESTURE             0
#define RING_GESTURE             1
#define SLOPE_GESTURE            2
#define NO_GESTURE               3

#define DETECTION_THRESHOLD      0.9f

// The QWIIC connector on the xG24 Explorer Kit is assigned to Wire1
// On other boards we use the default I2C instance (Wire)
#ifdef ARDUINO_BOARD_SILABS_XG24EXPLORERKIT
TwoWire& IMU_Wire = Wire1;
#else
TwoWire& IMU_Wire = Wire;
#endif

static TfLiteTensor* model_input;
static tflite::MicroInterpreter* interpreter;
static int input_length;
static TfLiteTensor *output;

typedef struct model_output {
  float gesture[GESTURE_COUNT];
} model_output_t;

typedef float acc_data_t;

#define FREQUENCY_IMU           52.0f
LSM6DSOXSensor lsm6dsoxSensor = LSM6DSOXSensor(&IMU_Wire, LSM6DSOX_I2C_ADD_L);

bool accelerometer_setup();
void accelerometer_read(acc_data_t* dst, int n);

void setup()
{
  Serial.begin(115200);
  Serial.println("Magic Wand - Silabs TensorFlowLite");
  Serial.println("init...");

  // Init TFLite model
  sl_tflite_micro_init();

  // Obtain pointer to the model's input tensor
  model_input = sl_tflite_micro_get_input_tensor();
  interpreter = sl_tflite_micro_get_interpreter();
  output = sl_tflite_micro_get_output_tensor();

  // Print model input parameters
  Serial.print("model_input->dims->size = ");
  Serial.println(model_input->dims->size);
  Serial.print("model_input->dims->data[0] = ");
  Serial.println(model_input->dims->data[0]);
  Serial.print("model_input->dims->data[1] = ");
  Serial.println(model_input->dims->data[1]);
  Serial.print("model_input->dims->data[2] = ");
  Serial.println(model_input->dims->data[2]);
  Serial.print("model_input->type = ");
  Serial.println(model_input->type);

  // Check model input parameters
  if ((model_input->dims->size != 2) || (model_input->dims->data[0] != 1)
      || (model_input->dims->data[1] != SEQUENCE_LENGTH * SIGNAL_CHANNELS)
      || (model_input->type != kTfLiteFloat32)) {
    Serial.println("error: bad input tensor parameters in model");
    while (1) ;
  }

  // Print model input length
  input_length = model_input->bytes / sizeof(float);
  Serial.print("input_length = ");
  Serial.println(input_length);

  // Initialize accelerometer
  bool setup_status = accelerometer_setup();
  if (!setup_status) {
    Serial.println("error: accelerometer setup failed\n");
    while (1) ;
  }

  Serial.println("ready");
}

void loop()
{
  // Insert data from accelerometer to the model
  acc_data_t *dst = (acc_data_t *) model_input->data.f;
  size_t n = input_length;
  // Wait until a significant movement is detected
  bool movementDetected = false;
  Serial.println("Waiting for significant movement...");

  while (!movementDetected) {
    uint8_t acceleroStatus;
    lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);

    // Proceed only if new data is available
    if (acceleroStatus == 1) {
      int32_t acceleration[3];
      lsm6dsoxSensor.Get_X_Axes(acceleration);

      // Calculate the absolute sum of the acceleration components
      int32_t absSum = abs(acceleration[0]) + abs(acceleration[1]) + abs(acceleration[2]);

      // If the movement exceeds the threshold, update the state
      if (absSum > 1500) {
        movementDetected = true;
        Serial.println("Movement detected: start collecting data!");
      }
    }
  }

  // Get accelerometer values
  accelerometer_read(dst, n);

  // Run inference
  unsigned long start_time = micros();  // Start time
  TfLiteStatus invoke_status = interpreter->Invoke();
  unsigned long end_time = micros();    // End time

  unsigned long elapsed_time = end_time - start_time;  // Time elapsed in ms
  Serial.print("Time taken by Invoke(): ");
  Serial.print(elapsed_time);
  Serial.println(" us");

  if (invoke_status == kTfLiteOk) {
    // Analyze the results to obtain a prediction
    const model_output_t *output = (const model_output_t *)interpreter->output(0)->data.f;

    // Print inference results (Gesture, probability)
    int max_i = 0;
    float max_val = 0;
    for (int i = 0; i < GESTURE_COUNT; i++) {
      switch (i) {
        case WING_GESTURE: Serial.print("W"); break;
        case RING_GESTURE: Serial.print("O"); break;
        case SLOPE_GESTURE: Serial.print("L"); break;
        case NO_GESTURE: Serial.print("No gesture"); break;
      }
      Serial.print(": ");
      Serial.print(interpreter->output(0)->data.f[i]);
      Serial.println();
      if (output->gesture[i] > max_val) {
        max_val = output->gesture[i];
        max_i = i;
      }
    }

    // Print the graphical representation of the recognized gesture
    if (max_val >= DETECTION_THRESHOLD) {
      switch (max_i) {
        case WING_GESTURE:
          Serial.println("detection = wing (W)");
          Serial.println("*       *");
          Serial.println("*       *");
          Serial.println("*   *   *");
          Serial.println(" * * * * ");
          Serial.println("  *   *  ");
          break;
        case RING_GESTURE:
          Serial.println("detection = ring (O)");
          Serial.println("  *****  ");
          Serial.println(" *     * ");
          Serial.println("*       *");
          Serial.println(" *     * ");
          Serial.println("  *****  ");
          break;
        case SLOPE_GESTURE:
          Serial.println("detection = slope (L)");
          Serial.println("*       ");
          Serial.println("*       ");
          Serial.println("*       ");
          Serial.println("*       ");
          Serial.println("******* ");
          break;
        case NO_GESTURE:
          Serial.println("No gesture");
          break;
      }

      // After gesture recognition wait 1 sec
      delay(1000);
    }
  } else {
    printf("error: inference failed");
  }
}

bool accelerometer_setup()
{
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

  return true;
}

void accelerometer_read(acc_data_t* dst, int n)
{
  int i = 0;

  while (i < n) {
    uint8_t acceleroStatus;
    uint8_t gyroStatus;
    lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);
    lsm6dsoxSensor.Get_G_DRDY_Status(&gyroStatus);

    if (acceleroStatus == 1) {
      int32_t acceleration[3];
      int32_t rotation[3];

      lsm6dsoxSensor.Get_X_Axes(acceleration);
      lsm6dsoxSensor.Get_G_Axes(rotation);

      // Normalize acceleration and rotation values to the range [0, 1]
      // - Acceleration values range: [-2000, 2000] mG -> [0, 1]
      // - Rotation values range: mdps/1000 -> [-2000, 2000] dps -> [0, 1]
      dst[i] = (float)(acceleration[0] + 2000) / 4000;
      dst[i + 1] = (float)(acceleration[1] + 2000) / 4000;
      dst[i + 2] = (float)(acceleration[2] + 2000) / 4000;
      dst[i + 3] = (float)((float)rotation[0] / 1000 + 2000) / 4000;
      dst[i + 4] = (float)((float)rotation[1] / 1000 + 2000) / 4000;
      dst[i + 5] = (float)((float)rotation[2] / 1000 + 2000) / 4000;

      i += SIGNAL_CHANNELS;
    }
  }
}
