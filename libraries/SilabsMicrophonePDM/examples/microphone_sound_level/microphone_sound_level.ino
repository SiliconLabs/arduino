/*
   Microphone sound level example

   This example uses the pulse-density modulation microphone on the Silicon Labs xG27 Dev Kit board
   to measure sound pressure level (SPL) and show the values on the serial console.

   Compatible boards:
   - xG27 Dev Kit

   Author: Vinh Le (Silicon Labs)
 */

#include <SilabsMicrophonePDM.h>

// Weight coefficient of First Order Digital IIR Filter
#define MIC_IIR_WEIGHT    0.1f

#define PDM_DATA_PIN      PB1
#define PDM_CLOCK_PIN     PB0
#define PDM_PWR_PIN       PC7

#define PDM_SAMPLE_RATE            44100
#define PDM_SAMPLE_BUFFER_SIZE     1000
#define PDM_N_CHANNELS             1

int16_t sample_buffer[PDM_SAMPLE_BUFFER_SIZE];
MicrophonePDM micPDM(PDM_DATA_PIN, PDM_CLOCK_PIN, PDM_PWR_PIN);

void setup()
{
  Serial.begin(115200);
  Serial.println("Silicon Labs PDM microphone sound level example");

  // Initialize microphone with sample rate and number of channels
  micPDM.begin(PDM_N_CHANNELS,
               PDM_SAMPLE_RATE,
               sample_buffer,
               PDM_SAMPLE_BUFFER_SIZE);

  // Start reading samples from the microphone
  micPDM.getSamples();
  Serial.println("Sampling started");
}

void loop()
{
  if (!micPDM.sampleBufferReady()) {
    return;
  }

  float sl;
  if (micPDM.calculateSoundLevel(sl, 0)) {
    static float soundLevel = 0.0f;
    // Apply IIR filter on the measured value
    soundLevel = (sl * MIC_IIR_WEIGHT) + (soundLevel * (1 - MIC_IIR_WEIGHT));
    Serial.print("Sound level: ");
    Serial.print(soundLevel);
    Serial.print(" dB");
    Serial.println();
  }

  // Read new samples from the microphone
  micPDM.getSamples();
}
