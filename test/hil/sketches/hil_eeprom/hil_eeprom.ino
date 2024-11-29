#include <EEPROM.h>

String line_buffer = "";
bool line_ready = false;

void serialEvent()
{
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      line_ready = true;
    } else {
      line_buffer += c;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  Serial.println();
  Serial.println("EEPROM HIL test");
}

void loop()
{
  if (line_ready) {
    Serial.println("> " + line_buffer);

    static uint32_t eeprom_address = 0u;
    char command = line_buffer.charAt(0);
    int parameter = line_buffer.substring(1).toInt();

    switch (command) {
      case 'a':
        eeprom_address = parameter;
        Serial.printf("EEPROM address set to %u\n", eeprom_address);
        break;
      case 'r':
        // Read from address
        Serial.printf("EEPROM read: %u\n", EEPROM.read(eeprom_address));
        break;
      case 'w':
        // Write to address
        EEPROM.write(eeprom_address, parameter);
        Serial.printf("EEPROM write: %u\n", parameter);
        break;
      case 'b':
        // Reboot
        Serial.println("Rebooting...");
        delay(100);
        systemReset();
        break;
      default:
        // Invalid command
        Serial.println("Invalid command");
        break;
    }

    line_ready = false;
    line_buffer = "";
  }
}
