/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it to the serial.
 */

#include <EEPROM.h>

int address = 0;
byte value;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();

  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }

  delay(500);
}
