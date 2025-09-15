/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * Please see eeprom_iteration for a more in depth
 * look at how to traverse the EEPROM.
 */

#include <EEPROM.h>

void setup() {
  // initialize the LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // iterate through each byte of the EEPROM storage.
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  // turn the LED on when we're done
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  /* Empty loop. */
}
