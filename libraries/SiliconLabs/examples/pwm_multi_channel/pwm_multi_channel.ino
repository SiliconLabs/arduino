/*
   Multi-Channel PWM

   This sketch demonstrates the use of up to 5 simultaneous PWM channels on Silicon Labs boards.
   Each channel can have an independent duty cycle (0-100%), all running at the same frequency (1kHz default).

   The sketch outputs PWM signals with different duty cycles on pins D3-D7:
   - D3: 20% duty cycle
   - D4: 40% duty cycle
   - D5: 60% duty cycle
   - D6: 80% duty cycle
   - D7: 100% duty cycle (always HIGH)

   You can connect LEDs (with appropriate current-limiting resistors) to these pins to see
   different brightness levels, or measure the PWM signals with an oscilloscope.

   Hardware Implementation:
   - Channels 0-2 use TIMER0 (CC0, CC1, CC2)
   - Channels 3-4 use TIMER1 (CC0, CC1)
   - All channels share the same frequency (GSDK constraint)
   - Each channel has independent duty cycle control

   Compatible with all Silicon Labs Arduino boards.

   Author: Greg Matthew Crossley (github.com/gregmatthewcrossley)
 */

// Define PWM output pins
#define PWM_PIN_1  D3
#define PWM_PIN_2  D4
#define PWM_PIN_3  D5
#define PWM_PIN_4  D6
#define PWM_PIN_5  D7

// Define duty cycles (0-255, where 255 = 100%)
#define DUTY_20_PERCENT   51   // 20% of 255
#define DUTY_40_PERCENT   102  // 40% of 255
#define DUTY_60_PERCENT   153  // 60% of 255
#define DUTY_80_PERCENT   204  // 80% of 255
#define DUTY_100_PERCENT  255  // 100% of 255

void setup()
{
  Serial.begin(115200);
  delay(2000);  // Wait for Serial connection

  Serial.println("Multi-Channel PWM Demo");
  Serial.println();
  Serial.println("This demo shows 5 simultaneous PWM channels");
  Serial.println("with different duty cycles on pins D3-D7.");
  Serial.println();

  // Configure pins as outputs
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  pinMode(PWM_PIN_3, OUTPUT);
  pinMode(PWM_PIN_4, OUTPUT);
  pinMode(PWM_PIN_5, OUTPUT);

  // Start PWM on all 5 channels with different duty cycles
  Serial.println("Starting PWM channels...");

  analogWrite(PWM_PIN_1, DUTY_20_PERCENT);
  Serial.println("  D3: 20% duty cycle");

  analogWrite(PWM_PIN_2, DUTY_40_PERCENT);
  Serial.println("  D4: 40% duty cycle");

  analogWrite(PWM_PIN_3, DUTY_60_PERCENT);
  Serial.println("  D5: 60% duty cycle");

  analogWrite(PWM_PIN_4, DUTY_80_PERCENT);
  Serial.println("  D6: 80% duty cycle");

  analogWrite(PWM_PIN_5, DUTY_100_PERCENT);
  Serial.println("  D7: 100% duty cycle (always HIGH)");

  Serial.println();
  Serial.println("All PWM channels active at 1kHz frequency!");
  Serial.println("Connect LEDs or an oscilloscope to see the outputs.");
}

void loop()
{
  // PWM continues running automatically
  // You can add code here to dynamically change duty cycles if desired

  delay(1000);
}
