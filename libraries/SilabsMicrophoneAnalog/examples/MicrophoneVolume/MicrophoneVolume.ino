/*
   Analog microphone volume example

   The example showcases the usage of analog MEMS microphones and dims the
   on-board LED based on the microphone's input volume.
   This example is compatible with all Silicon Labs Arduino boards, however
   it requires an analog microphone on-board or connected to the specified pin.

   Author: Áron Gyapjas (Silicon Labs)
 */

#include <SilabsMicrophoneAnalog.h>

// This configuration is for the MSM381ACT001 microphone on the Seeed Studio XIAO MG24
// Change it according to your hardware
#define MIC_DATA_PIN  PC9
#define MIC_PWR_PIN   PC8
#define NUM_SAMPLES   128
#define MIC_VALUE_MIN 735
#define MIC_VALUE_MAX 900

// Buffers for storing samples
uint32_t mic_buffer[NUM_SAMPLES];
uint32_t mic_buffer_local[NUM_SAMPLES];

volatile bool data_ready_flag = false;
MicrophoneAnalog micAnalog(MIC_DATA_PIN, MIC_PWR_PIN);
void mic_samples_ready_cb();
void calculate_and_display_voice_level();

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  micAnalog.begin(mic_buffer, NUM_SAMPLES);
  Serial.println("Microphone initialized...");

  micAnalog.startSampling(mic_samples_ready_cb);
  Serial.println("Sampling started...");
}

void loop()
{
  if (data_ready_flag) {
    data_ready_flag = false;
    calculate_and_display_voice_level();
  }
}

// Called when the requested amount of samples are available from the microphone
void mic_samples_ready_cb()
{
  // Copy data to the local buffer in order to prevent it from overwriting
  memcpy(mic_buffer_local, mic_buffer, NUM_SAMPLES * sizeof(uint32_t));
  data_ready_flag = true;
}

void calculate_and_display_voice_level() {
  // Rolling average for smoothing the voice level
  static uint32_t rolling_average = 0u;

  // Stop sampling in order to prevent overwriting the current data
  micAnalog.stopSampling();

  // Get the average of the sampled values
  uint32_t voice_level = (uint32_t)micAnalog.getAverage(mic_buffer_local, NUM_SAMPLES);
  // Adjust the voice level relative to minimum/maximum of the microphone's output
  voice_level = constrain(voice_level, MIC_VALUE_MIN, MIC_VALUE_MAX);
  // Calculate the rolling average
  rolling_average = (voice_level + rolling_average) / 2;

  // Map the current average level to brightness
  int brightness = map(rolling_average, MIC_VALUE_MIN, MIC_VALUE_MAX, 0, 255);
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_BUILTIN, 255 - brightness);
  } else {
    analogWrite(LED_BUILTIN, brightness);
  }
  // Print the average voice level (you can use the Serial Plotter to view this value on a graph)
  Serial.println(rolling_average);

  // Restart sampling
  micAnalog.startSampling(mic_samples_ready_cb);
}
