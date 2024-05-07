/*
   ezWS2812 individual LEDs example

   Demonstrates the addressing of individual WS2812 LEDs.
   Creates a running light through the WS2812 LEDs connected to the SPI MOSI pin.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezWS2812.h>

#define NUMBER_OF_LEDS 8u

ezWS2812 my_leds(NUMBER_OF_LEDS);

void setup()
{
  my_leds.begin();
}

void loop()
{
  static uint32_t current_led = 0;

  // Go through all the LEDs
  noInterrupts();
  for (uint32_t i = 0; i < NUMBER_OF_LEDS; i++) {
    // If 'i' is at the current LED - we turn that LED to green
    if (i == current_led) {
      // Parameters are: number_of_leds_to_set, R, G, B, brightness, end_transfer
      my_leds.set_pixel(1, 0, 255, 0, 100, false);
    } else {
      // Turn all other LEDs off
      my_leds.set_pixel(1, 0, 0, 0, 0, false);
    }
  }
  // End the transfer
  my_leds.end_transfer();
  interrupts();

  // Move the current LED up by one
  current_led++;
  if (current_led >= NUMBER_OF_LEDS) {
    current_led = 0;
  }
  // Wait a bit
  delay(100);
}
