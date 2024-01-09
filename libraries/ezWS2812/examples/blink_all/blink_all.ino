/*
   ezWS2812 blink all example

   Blinks all WS2812 LEDs connected to the SPI MOSI pin.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezWS2812.h>

#define NUMBER_OF_LEDS 8

ezWS2812 my_leds(NUMBER_OF_LEDS);

void setup()
{
  my_leds.begin();
  my_leds.set_all(0, 0, 0);
}

void loop()
{
  // Red
  my_leds.set_all(255, 0, 0);
  delay(500);
  // Green
  my_leds.set_all(0, 255, 0);
  delay(500);
  // Blue
  my_leds.set_all(0, 0, 255);
  delay(500);
  // Off
  my_leds.set_all(0, 0, 0);
  delay(500);
}
