/*
   Matter window covering example

   The example shows how to create a Matter powered window covering device with the Arduino Matter API.

   The example creates a position aware lift capable Matter window covering device,
   simulates the window covering motor actuation delay and displays the current lift value using the built-in LED.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */
#include <Matter.h>
#include <MatterWindowCovering.h>

MatterWindowCovering matter_blinds;

void update_onboard_led(uint8_t brightness);

void setup()
{
  Serial.begin(115200);
  Matter.begin();
  matter_blinds.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  update_onboard_led(0);

  Serial.println("Matter window covering");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_blinds.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop()
{
  static uint16_t current_lift_raw = 0u;
  uint16_t requested_lift_raw = matter_blinds.get_requested_lift_position_raw();
  // Return if the requested lift position is the same as the current - no movement requested
  if (current_lift_raw == requested_lift_raw) {
    return;
  }

  // Get the current and requested lift position in percents
  int32_t current_percent = matter_blinds.get_actual_lift_position_percent();
  int32_t requested_percent = matter_blinds.get_requested_lift_position_percent();

  // If the current and requested percentage is the same - then the requested raw difference is so small that it's lost to rounding
  if (current_percent == requested_percent) {
    // Set the actual position to the requested position
    matter_blinds.set_actual_lift_position_raw(requested_lift_raw);
    return;
  }

  Serial.printf("New position requested; current='%d%% (%u)' requested='%d%% (%u)' - ", current_percent, current_lift_raw, requested_percent, requested_lift_raw);

  int32_t step = 0;
  // Determine the direction we need to go to achieve the requested position
  if (current_percent > requested_percent) {
    // Signal the Matter device that we're moving towards the 'closed' position
    matter_blinds.set_current_operation(MatterWindowCovering::WINDOW_COVERING_CLOSING);
    Serial.println("closing...");
    step = -1;
  } else {
    // Signal the Matter device that we're moving towards the 'open' position
    matter_blinds.set_current_operation(MatterWindowCovering::WINDOW_COVERING_OPENING);
    Serial.println("opening...");
    step = 1;
  }

  // Iterate while we're not in the requested position
  do {
    // Step one percent in the requested direction
    current_percent += step;
    // Calculate the LED value and display it
    uint8_t current_led_brightness = map(current_percent, 0, 100, 0, 255);
    update_onboard_led(current_led_brightness);
    // Print the current percent
    Serial.printf("%d%%\n", current_percent);
    // Delay a small amount to simulate a real motor actuating
    delay(50);
    // Update the Matter network with the current position
    matter_blinds.set_actual_lift_position_percent(current_percent);
  } while (current_percent != requested_percent);

  // Set the actual position to the requested position
  // Percent calculation rounds down to the nearest integer - this smooths out all rounding losses if a fraction of a percent is requested
  // and sets the actual position exactly to the requested
  matter_blinds.set_actual_lift_position_raw(requested_lift_raw);
  // Signal the Matter device that we're not moving anymore
  matter_blinds.set_current_operation(MatterWindowCovering::WINDOW_COVERING_STOPPED);
  // Print the current position
  Serial.printf("Moved to %u%%\n", matter_blinds.get_actual_lift_position_percent());
  current_lift_raw = requested_lift_raw;
}

void update_onboard_led(uint8_t brightness)
{
  // If our built-in LED is active LOW, we need to invert the brightness value
  if (LED_BUILTIN_ACTIVE == LOW) {
    analogWrite(LED_BUILTIN, 255 - brightness);
  } else {
    analogWrite(LED_BUILTIN, brightness);
  }
}
