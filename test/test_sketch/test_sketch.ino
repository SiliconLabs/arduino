void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("TEST!");

  Wire.begin();
  Wire.end();

  SPI.begin();
  SPI.transfer(0x42);
  SPI.end();

  uint8_t val = digitalRead(PA0);

  val = analogRead(PA0);
  Serial.println(val, HEX);

  analogWrite(PA0, 128);

  tone(PA0, 440, 0);
  noTone(PA0);
}

void loop()
{
  Serial.println(getMcuUniqueIdStr());
  Serial.println(getCpuTemp());
  Serial.println(millis());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
