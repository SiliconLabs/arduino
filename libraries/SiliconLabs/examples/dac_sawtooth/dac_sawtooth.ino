/*
   DAC sawtooth generator example

   The example shows the basic usage of the DAC (Digital to Analog converter).

   The sketch will generate a sawtooth signal on the board's DAC0 output pin.
   The DAC outputs on the MG24 based boards are PB00 and PB01 for channel 0 and 1.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Ezurio Lyra 24P 20dBm Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
void setup()
{
  Serial.begin(115200);
  // Set the DAC resolution to 12 bits
  analogWriteResolution(12);
  // Select the 1.25V reference voltage (feel free to change it)
  analogReferenceDAC(DAC_VREF_1V25);
}

void loop()
{
  static int value = 0;
  analogWrite(DAC0, value);
  Serial.println(value);

  value++;
  if (value == 4095) {
    value = 0;
  }
}
