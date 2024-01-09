/*
   ezWS2812 colors example

   Fades through all colors on all WS2812 LEDs connected to the SPI MOSI pin.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezWS2812.h>

#define NUMBER_OF_LEDS 8

ezWS2812 my_leds(NUMBER_OF_LEDS);
const uint32_t delay_time_ms = 5;

void setup()
{
  my_leds.begin();
}

void loop()
{
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 255;

  while (blue > 0) {
    my_leds.set_all(red, green, blue);
    green++;
    blue--;
    delay(delay_time_ms);
  }

  while (green > 0) {
    my_leds.set_all(red, green, blue);
    green--;
    red++;
    delay(delay_time_ms);
  }

  while (red > 0) {
    my_leds.set_all(red, green, blue);
    red--;
    blue++;
    delay(delay_time_ms);
  }
}
