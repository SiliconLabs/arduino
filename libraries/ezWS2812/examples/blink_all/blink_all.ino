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
  noInterrupts();
  my_leds.set_all(255, 0, 0);
  interrupts();
  delay(500);
  // Green
  noInterrupts();
  my_leds.set_all(0, 255, 0);
  interrupts();
  delay(500);
  // Blue
  noInterrupts();
  my_leds.set_all(0, 0, 255);
  interrupts();
  delay(500);
  // Off
  noInterrupts();
  my_leds.set_all(0, 0, 0);
  interrupts();
  delay(500);
}
