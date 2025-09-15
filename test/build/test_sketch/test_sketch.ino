#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <ArduinoLowPower.h>
#include <WatchdogTimer.h>

void btn_isr_handler()
{
  ;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("TEST!");

  Wire.begin();
  Wire.setClock(400000);
  Wire.beginTransmission(0x42);
  Wire.write(0x91);
  uint8_t i2c_data[] = { 0x40, 0x41, 0x42 };
  Wire.write(i2c_data, sizeof(i2c_data));
  Wire.endTransmission();

  Wire.beginTransmission(0x42);
  uint8_t i2c_rx = Wire.requestFrom(0x42, 1, true);
  Serial.println(i2c_rx);
  Wire.endTransmission();
  Wire.end();

  SPI.begin();
  SPISettings settings(8000000, MSBFIRST, SPI_MODE1);
  uint32_t spi_freq = settings.getClockFreq();
  SPIMode spi_datamode = settings.getDataMode();
  BitOrder spi_bitorder = settings.getBitOrder();

  SPI.beginTransaction(settings);
  uint8_t spi_ret8 = SPI.transfer(0x42);
  uint16_t spi_ret16 = SPI.transfer16(0x4242);
  Serial.println(spi_ret8);
  Serial.println(spi_ret16);

  uint8_t spi_data[] = { 0x42, 0x42, 0x42 };
  SPI.transfer(spi_data, sizeof(spi_data));
  SPI.endTransaction();

  Serial.println(spi_freq);
  Serial.println(spi_datamode);
  Serial.println(spi_bitorder);

  SPI.end();

  attachInterrupt(D2, &btn_isr_handler, RISING);
  detachInterrupt(D2);

  uint8_t val = digitalRead(PA0);

  val = analogRead(PA0);
  Serial.println(val, HEX);
  analogReadResolution(5);
  val = analogRead(PA1);
  Serial.println(val, OCT);

  analogWrite(PA0, 128);

  tone(PA0, 440, 0);
  noTone(PA0);

  delayMicroseconds(420);
  yield();

  Serial.println(millis());
  Serial.println(micros());

  shiftOut(PA0, PA1, MSBFIRST, 0x69);
  uint8_t data = shiftIn(D0, D1, LSBFIRST);
  Serial.println(data, OCT);

  unsigned long pulse_data = pulseIn(PA0, HIGH, 1000);
  pulse_data = pulseInLong(D0, LOW, 2000);
  Serial.println(pulse_data);

  EEPROM.write(0, 0x42);
  uint8_t eeprom_data = EEPROM.read(0);
  Serial.println(eeprom_data, HEX);

  WatchdogTimer.begin();
  WatchdogTimer.setTimeout(WDOG_PERIOD_257_MS);
  Serial.println(WatchdogTimer.watchdogResetHappened());
  WatchdogTimer.feed();
  WatchdogTimer.setTimeout(WDOG_PERIOD_4_S);
  WatchdogTimer.attachInterrupt(nullptr);
  WatchdogTimer.detachInterrupt();
  WatchdogTimer.setWatchdogOffWhileSleeping(true);
  WatchdogTimer.end();

  size_t total_heap = getTotalHeapSize();
  Serial.println(total_heap);
  size_t used_heap = getUsedHeapSize();
  Serial.println(used_heap);
  size_t free_heap = getFreeHeapSize();
  Serial.println(free_heap);
  size_t max_heap_usage = getHeapHighWatermark();
  Serial.println(max_heap_usage);
  resetHeapHighWatermark();

  Serial.println(getCurrentBoardType());
  Serial.println(getCurrentRadioStackType());
}

void loop()
{
  Serial.println(getDeviceUniqueIdStr().c_str());
  Serial.println(getCPUTemp());
  Serial.println(millis());
  Serial.println(micros());
  Serial.println(getCPUClock());
  Serial.println(getCPUCycleCount());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  Serial.println(LowPower.wokeUpFromDeepSleep());
  Serial.println(LowPower.deepSleepMemorySize());
  LowPower.deepSleepMemoryWrite(12, 420);
  uint32_t deep_sleep_data = LowPower.deepSleepMemoryRead(12);
  Serial.println(deep_sleep_data);
  LowPower.attachInterruptWakeup(PC0, nullptr, LOW);
  LowPower.idle();
  LowPower.idle(1000);
  LowPower.sleep();
  LowPower.sleep(1000);
  LowPower.deepSleep();
  LowPower.deepSleep(1000);
}
