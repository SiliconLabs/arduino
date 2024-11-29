void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
  }
}
