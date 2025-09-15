/*
 * EEPROM Iteration
 *
 * A set of example snippets highlighting the simplest methods for traversing the EEPROM.
 * 
 * Running this sketch is not necessary, this is simply highlighting certain programming methods.
 */

#include <EEPROM.h>

void setup() {
  /***
    Iterate the EEPROM using a for loop.
  ***/
  for (int index = 0 ; index < EEPROM.length() ; index++) {
    //Add one to each cell in the EEPROM
    EEPROM[ index ] += 1;
  }

  /***
    Iterate the EEPROM using a while loop.
  ***/
  int index = 0;

  while (index < EEPROM.length()) {
    //Add one to each cell in the EEPROM
    EEPROM[ index ] += 1;
    index++;
  }

  /***
    Iterate the EEPROM using a do-while loop.
  ***/
  int idx = 0;  //Used 'idx' to avoid name conflict with 'index' above.

  do {
    //Add one to each cell in the EEPROM
    EEPROM[ idx ] += 1;
    idx++;
  } while (idx < EEPROM.length());
}

void loop() {
  /* Empty loop */
}
