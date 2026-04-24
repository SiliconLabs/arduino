void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  delay(200);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
}

void loop()
{
  Serial.println(analogRead(0));
  delay(500);
}
