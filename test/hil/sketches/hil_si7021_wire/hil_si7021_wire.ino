#include <Adafruit_Si7021.h>

Adafruit_Si7021 my_si7021;

#ifdef ARDUINO_BOARD_SILABS_XG27DEVKIT
#define SENSOR_EN PC6
#elif defined(ARDUINO_BOARD_SILABS_XG24DEVKIT)
#define SENSOR_EN PC9
#endif

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  pinMode(SENSOR_EN, OUTPUT);
  digitalWrite(SENSOR_EN, HIGH);
  delay(500);
  Wire.begin();
  my_si7021.begin();
}

void loop()
{
  float humidity = my_si7021.readHumidity();
  float temp = my_si7021.readTemperature();
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(humidity);
  delay(500);
}
