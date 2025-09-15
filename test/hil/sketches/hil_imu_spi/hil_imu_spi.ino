#include <SPI.h>

#define SPI_READ_MASK 0x80

#define ICM20689_REG_PWR_MGMT_1 0x6B
#define ICM20689_REG_WHOAMI 0x75
#define ICM20689_MASK_CLKSEL_AUTO_PLL 0x01
#define ICM20689_MASK_RESET 0x80

#ifdef ARDUINO_BOARD_SILABS_XG27DEVKIT
#define IMU_CS PB2
#define IMU_EN PB4
#elif defined(ARDUINO_BOARD_SILABS_XG24DEVKIT)
#define IMU_CS PA7
#define IMU_EN PC9
#endif

uint8_t imu_read_register(uint8_t reg)
{
  uint8_t data = 0u;
  digitalWrite(IMU_CS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  SPI.transfer(reg | SPI_READ_MASK);
  data = SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(IMU_CS, HIGH);
  return data;
}

void imu_write_register(uint8_t reg, uint8_t data)
{
  digitalWrite(IMU_CS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  SPI.transfer(reg);
  SPI.transfer(data);
  SPI.endTransaction();
  digitalWrite(IMU_CS, HIGH);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);

  SPI.begin();

  pinMode(IMU_CS, OUTPUT);
  pinMode(IMU_EN, OUTPUT);
  digitalWrite(IMU_EN, HIGH);
  digitalWrite(IMU_CS, HIGH);

  delay(200);

  // Clock select auto PLL
  imu_write_register(ICM20689_REG_PWR_MGMT_1, ICM20689_MASK_CLKSEL_AUTO_PLL);

  // Reset
  imu_write_register(ICM20689_REG_PWR_MGMT_1, ICM20689_MASK_RESET);
  delay(1);

  // Clock select auto PLL
  imu_write_register(ICM20689_REG_PWR_MGMT_1, ICM20689_MASK_CLKSEL_AUTO_PLL);
}

void loop()
{
  // Read WHOAMI
  uint8_t whoami = imu_read_register(ICM20689_REG_WHOAMI);
  Serial.print("IMU WHOAMI: 0x");
  Serial.println(whoami, HEX);
  delay(500);
}
