/*
 * EEPROM Update
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 * 
 * If a value has not changed in the EEPROM, it is not overwritten
 * which would reduce the life span of the EEPROM unnecessarily.
 */

#include <EEPROM.h>

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int address = 0;

void setup() {
  /* Empty setup */
}

void loop() {
  /***
    need to divide by 4 because analog inputs range from
    0 to 1023 and each byte of the EEPROM can only hold a
    value from 0 to 255.
  ***/
  int val = analogRead(0) / 4;

  /***
    Update the particular EEPROM cell.
    these values will remain there when the board is
    turned off.
  ***/
  EEPROM.update(address, val);

  /***
    The function EEPROM.update(address, val) is equivalent to the following:

    if( EEPROM.read(address) != val ){
      EEPROM.write(address, val);
    }
  ***/

  /***
    Advance to the next address, when at the end restart at the beginning.
  ***/
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }

  delay(100);
}
