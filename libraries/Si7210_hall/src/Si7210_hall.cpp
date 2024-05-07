/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Si7210_hall.h"

Si7210::Si7210() :
  _i2c(Wire),
  _address(SI7210_DEFAULT_I2C_ADDRESS),
  _i2cRate(SI7210_DEFAULT_I2C_SPEED)
{
  ;
}

Si7210::Si7210(TwoWire& bus = Wire, uint8_t address = SI7210_DEFAULT_I2C_ADDRESS) :
  _i2c(bus),
  _address(address),
  _i2cRate(SI7210_DEFAULT_I2C_SPEED)
{
  ;
}

/***************************************************************************//**
 * @brief
 *    Does device-specific initialization for the Si7210 chip.
 *
 * @return
 *    Returns true on success, false otherwise
 ******************************************************************************/
bool Si7210::begin()
{
  uint8_t chipId;
  uint8_t revId;

  // starting the I2C bus
  _i2c.begin();
  // setting the I2C clock
  _i2c.setClock(_i2cRate);

  delay(1);
  wakeUp();

  identify(chipId, revId);
  if (chipId != SI7210_CHIP_ID) {
    return false;
  }

  configure();
  return true;
}

/****************Si7210 Public Functions***************************************/

/***************************************************************************//**
 * @brief
 *    Configure the Si7210 chip.
 ******************************************************************************/
void Si7210::configure()
{
  uint8_t value;
  uint8_t swop, swhyst;

  wakeUp();
  // Stop the measurement loop
  setRegisterBits(SI7210_REG_ADDR_POWER_CTRL,
                  SI7210_REG_POWER_CTRL_STOP_MASK);

  if ((_threshold == 0.0)
      && (_hysteresis == 0.0)
      && (_polarity == 0)
      && (_outputInvert == 0)) {
    // Use default values in the device for all parameters
    clearRegisterBits(SI7210_REG_ADDR_POWER_CTRL,
                      SI7210_REG_POWER_CTRL_USESTORE_MASK);
  } else {
    // Program sw_low4field and sw_op bit fields
    swop = calculateSwOp(_threshold);
    value = 0;

    if (_outputInvert) {
      value = (1 << SI7210_REG_CTRL1_SW_LOW4FIELD_SHIFT)
              & SI7210_REG_CTRL1_SW_LOW4FIELD_MASK;
    }
    value |= (swop << SI7210_REG_CTRL1_SW_OP_SHIFT) & SI7210_REG_CTRL1_SW_OP_MASK;

    writeRegister(SI7210_REG_ADDR_CTRL1, value);

    // Program sw_fieldpolsel and sw_hyst bit fields
    swhyst = calculateSwHyst(_hysteresis, false);

    value = (_polarity << SI7210_REG_CTRL2_SW_FIELDPOLSEL_SHIFT)
            & SI7210_REG_CTRL2_SW_FIELDPOLSEL_MASK;

    value |= (swhyst << SI7210_REG_CTRL2_SW_HYST_SHIFT)
             & SI7210_REG_CTRL2_SW_HYST_MASK;

    writeRegister(SI7210_REG_ADDR_CTRL2, value);

    // Enable storing of these parameters in sleep mode
    setRegisterBits(SI7210_REG_ADDR_POWER_CTRL,
                    SI7210_REG_POWER_CTRL_USESTORE_MASK);
  }
  // Enable sleep timer and clear stop bit to start operation
  setRegisterBits(SI7210_REG_ADDR_CTRL3,
                  SI7210_REG_CTRL3_SLTIMEENA_MASK);
  // Resume operation
  clearRegisterBits(SI7210_REG_ADDR_POWER_CTRL,
                    SI7210_REG_POWER_CTRL_STOP_MASK);
}

/***************************************************************************//**
 * @brief
 *    Perform a measurement.
 *
 * @param[in] scale
 *    Desired scale in uT
 *
 * @return
 *    Returns the measured field strength value in mT
 ******************************************************************************/
float Si7210::measure(uint32_t scale)
{
  int32_t mT;
  bool range200mT;

  if (scale > 20500) {
    range200mT = true;
  } else {
    range200mT = false;
  }

  mT = readMagfieldDataAndSltimeena(range200mT);
  float result = ((float) mT) / 1000;

  return result;
}

/***************************************************************************//**
 * @brief
 *    Put Si7210 into Sleep (No-measurement) Mode.
 *    Wake-up command needs to be issued to become responsive.
 ******************************************************************************/
void Si7210::sleep()
{
  uint8_t val;

  val = readRegister(SI7210_REG_ADDR_CTRL3);

  // Clear the sleep timer enable bit
  val = (val & ~SI7210_REG_CTRL3_SLTIMEENA_MASK);
  writeRegister(SI7210_REG_ADDR_CTRL3, val);

  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);

  // Clear the oneburst and stop bits, set the sleep bit
  val = ((val & ~(SI7210_REG_POWER_CTRL_ONEBURST_MASK
                  | SI7210_REG_POWER_CTRL_STOP_MASK))
         | SI7210_REG_POWER_CTRL_SLEEP_MASK);
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);
}

/***************************************************************************//**
 * @brief
 *    Put the Si7210 into Sleep w/ Measurement Mode: OUTPUT is updated 200 ms.
 ******************************************************************************/
void Si7210::sleepSltimeena()
{
  uint8_t val;

  val = readRegister(SI7210_REG_ADDR_CTRL3);
  // Set the sleep timer enable bit
  val = ((val & SI7210_REG_CTRL3_SW_TAMPER_MASK) | SI7210_REG_CTRL3_SLTIMEENA_MASK);
  writeRegister(SI7210_REG_ADDR_CTRL3, val);

  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);
  // Clear the oneburst, stop and sleep bits
  val = (val & ~(SI7210_REG_POWER_CTRL_ONEBURST_MASK
                 | SI7210_REG_POWER_CTRL_STOP_MASK
                 | SI7210_REG_POWER_CTRL_SLEEP_MASK));
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);
}

/***************************************************************************//**
 * @brief
 *    Wake up the Hall sensor chip.
 *
 * @return
 *    Returns true on success, false otherwise
 ******************************************************************************/
bool Si7210::wakeUp()
{
  _i2c.beginTransmission(_address);
  _i2c.write(0x00);
  uint8_t ret = _i2c.endTransmission();

  delay(1);
  if (ret) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 * @brief
 *    Read out Si7210 Conversion Data - 15bits.
 *
 * @return
 *    Returns the raw magnetic field conversion data (15 bits)
 ******************************************************************************/
int16_t Si7210::readData()
{
  uint8_t val;
  int16_t data;

  val = readRegister(SI7210_REG_ADDR_DSPSIGM);

  uint8_t flag = val >> SI7210_REG_DSPSIGM_FRESH_SHIFT;
  data = ((uint16_t)(val  & SI7210_REG_DSPSIGM_DSPSIGM_MASK)) << 8;

  val = readRegister(SI7210_REG_ADDR_DSPSIGL);
  data |= val;
  data = data - 16384;

  if (flag == 0) {
    data = 0;
  }

  return data;
}

/***************************************************************************//**
 * @brief
 *    Read out Si7210 Revision and ID.
 *
 * @param[in] id
 *    Si7210 part ID
 *
 * @param[in] rev
 *    Si7210 part Revision
 ******************************************************************************/
void Si7210::identify(uint8_t &id, uint8_t &rev)
{
  wakeUp();

  uint8_t val = readRegister(SI7210_REG_ADDR_HREVID);
  rev = val & SI7210_REG_HREVID_REVID_MASK;
  id = val >> SI7210_REG_HREVID_CHIPID_SHIFT;

  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);
  // Clear the oneburst and sleep bits, set the stop bit
  val = ((val & ~(SI7210_REG_POWER_CTRL_ONEBURST_MASK
                  | SI7210_REG_POWER_CTRL_SLEEP_MASK))
         | SI7210_REG_POWER_CTRL_STOP_MASK);
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);
}

/***************************************************************************//**
 * @brief
 *    Change Mag-Field scale to 200mT.
 *    If desired, must be performed after power-up or wake-up from sleep.
 ******************************************************************************/
void Si7210::setRange200mT()
{
  uint8_t val;
  uint8_t ret;
  uint8_t otpAddr = SI7210_OTP_ADDR_COEFFS_200MT;
  const uint8_t writeAddr[] = {
    SI7210_REG_ADDR_A0,
    SI7210_REG_ADDR_A1,
    SI7210_REG_ADDR_A2,
    SI7210_REG_ADDR_A3,
    SI7210_REG_ADDR_A4,
    SI7210_REG_ADDR_A5
  };

  for (uint8_t i = 0; i < sizeof(writeAddr); i++ ) {
    ret = readOtpRegister(otpAddr++, val);

    if (!ret) {
      writeRegister(writeAddr[i], val);
    }
  }
}

/***************************************************************************//**
 * @brief
 *   Perform burst-conversion(4 samples), read mT-data, and
 *   put part into sltimeena-sleep mode where OUT is updated every 200 ms.
 *
 * @param[in] range200mT
 *   range200mT=false : full-scale equals 20mT
 *   range200mT=true  : full-scale equals 200mT
 *
 * @return
 *    Mag-field conversion reading, signed 32-bit integer
 *    mTdata must be divided by 1000 to get decimal value in mT units
 ******************************************************************************/
int32_t Si7210::readMagfieldDataAndSltimeena(bool range200mT)
{
  uint8_t val;
  int16_t data;
  int32_t mTdata;

  wakeUp();

  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);

  // Clear oneburst and sleep bits, set Usestore and stop to stop measurements
  val = ((val & ~(SI7210_REG_POWER_CTRL_ONEBURST_MASK
                  | SI7210_REG_POWER_CTRL_SLEEP_MASK))
         | (SI7210_REG_POWER_CTRL_USESTORE_MASK
            | SI7210_REG_POWER_CTRL_STOP_MASK));
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);

  // Burst sample size = 4 (2^2), number of samples to average = 4 (2^2)
  writeRegister(SI7210_REG_ADDR_CTRL4,
                ((2 << SI7210_REG_CTRL4_DF_BURSTSIZE_SHIFT)
                 | (2 << SI7210_REG_CTRL4_DF_BW_SHIFT)));

  if (range200mT) {
    setRange200mT();
  }
  // Clear stop and sleep bits, set Usestore and oneburst to start a burst of measurements
  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);

  val = ((val & ~(SI7210_REG_POWER_CTRL_STOP_MASK
                  | SI7210_REG_POWER_CTRL_SLEEP_MASK))
         | (SI7210_REG_POWER_CTRL_USESTORE_MASK
            | SI7210_REG_POWER_CTRL_ONEBURST_MASK));
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);

  // Wait until the measurement is done
  do {
    val = readRegister(SI7210_REG_ADDR_POWER_CTRL);
  } while (val >> SI7210_REG_POWER_CTRL_MEAS_SHIFT);

  data = readData();

  // To convert mTdata to decimal value, divide by 1000
  if (range200mT) {
    mTdata = (data * 125 / 10);
  } else {
    mTdata = (data * 125 / 100);
  }

  // Go to sleep with sleep timer enabled
  sleepSltimeena();

  return mTdata;
}

/***************************************************************************//**
 * @brief
 *   Wake-up from Sleep, perform burst-conversion(4 samples), read mT-data,
 *   and put part into sleep mode (no-measurement). Requires Wake-Up.
 *
 * @param[in] range200mT
 *   range200mT=false : full-scale equals 20mT
 *   range200mT=true  : full-scale equals 200mT
 *
 * @return
 *    Mag-field conversion reading, signed 32-bit integer
 *    mTdata must be divided by 1000 to get decimal value in mT units
 ******************************************************************************/
int32_t Si7210::readMagfieldDataAndSleep(bool range200mT)
{
  uint8_t val;
  int16_t data;
  int32_t mTdata;

  wakeUp();

  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);

  // Clear oneburst and sleep bits, set Usestore and stop to stop measurements
  val = ((val & ~(SI7210_REG_POWER_CTRL_ONEBURST_MASK
                  | SI7210_REG_POWER_CTRL_SLEEP_MASK))
         | (SI7210_REG_POWER_CTRL_USESTORE_MASK
            | SI7210_REG_POWER_CTRL_STOP_MASK));
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);

  // Burst sample size = 4 (2^2), number of samples to average = 4 (2^2)
  writeRegister(SI7210_REG_ADDR_CTRL4,
                ((2 << SI7210_REG_CTRL4_DF_BURSTSIZE_SHIFT)
                 | (2 << SI7210_REG_CTRL4_DF_BW_SHIFT)));

  if (range200mT) {
    setRange200mT();
  }
  // Clear stop and sleep bits, set Usestore and oneburst to start a burst of measurements
  val = readRegister(SI7210_REG_ADDR_POWER_CTRL);

  val = ((val & ~(SI7210_REG_POWER_CTRL_STOP_MASK
                  | SI7210_REG_POWER_CTRL_SLEEP_MASK))
         | (SI7210_REG_POWER_CTRL_USESTORE_MASK
            | SI7210_REG_POWER_CTRL_ONEBURST_MASK));
  writeRegister(SI7210_REG_ADDR_POWER_CTRL, val);

  // Wait until the measurement is done
  do {
    val = readRegister(SI7210_REG_ADDR_POWER_CTRL);
  } while (val >> SI7210_REG_POWER_CTRL_MEAS_SHIFT);

  data = readData();

  // To convert mTdata to decimal value, divide by 1000
  if (range200mT) {
    mTdata = (data * 125 / 10);
  } else {
    mTdata = (data * 125 / 100);
  }

  // Go to sleep
  sleep();

  return mTdata;
}

/***************************************************************************//**
 * @brief
 *    Read register from the OTP area of the Si7021 device.
 *
 * @param[in] otpAddr
 *    The register address to read from in the sensor
 *
 * @param[in] otpData
 *    The data read from the device
 *
 * @return
 *    Returns true on success, false otherwise
 ******************************************************************************/
bool Si7210::readOtpRegister(uint8_t otpAddr, uint8_t &otpData)
{
  uint8_t val = readRegister(SI7210_REG_ADDR_OTP_CTRL);
  if (val & SI7210_REG_OTP_CTRL_BUSY_MASK) {
    return false;
  }

  writeRegister(SI7210_REG_ADDR_OTP_ADDR, otpAddr);
  writeRegister(SI7210_REG_ADDR_OTP_CTRL, SI7210_REG_OTP_CTRL_READ_EN_MASK);
  otpData = readRegister(SI7210_REG_ADDR_OTP_DATA);
  return true;
}

/***************************************************************************//**
 * @brief
 *    Write a register in the Hall sensor device.
 *
 * @param[in] reg
 *    The register address to write
 *
 * @param[in] value
 *    The value to write to the register
 ******************************************************************************/
void Si7210::writeRegister(uint8_t reg, uint8_t value)
{
  _i2c.beginTransmission(_address);
  _i2c.write(reg);
  _i2c.write(value);
  _i2c.endTransmission();
}

/***************************************************************************//**
 * @brief
 *    Read register from the Hall sensor device.
 *
 * @param[in] reg
 *    The register address to read from in the sensor
 *
 * @return
 *    Returns the data read from the device
 ******************************************************************************/
uint8_t Si7210::readRegister(uint8_t reg)
{
  _i2c.beginTransmission(_address);
  _i2c.write(reg);
  _i2c.endTransmission();
  _i2c.requestFrom(_address, 1);
  uint8_t regVal = _i2c.read();
  return regVal;
}

/***************************************************************************//**
 * @brief
 *    Set the given bit(s) in a register in the Hall sensor device.
 *
 * @param[in] addr
 *    The address of the register
 *
 * @param[in] mask
 *    The mask specifies which bits should be set. If a given bit of the mask is
 *    1, that register bit will be set to 1. All the other register bits will be
 *    untouched.
 ******************************************************************************/
void Si7210::setRegisterBits(uint8_t reg, uint8_t mask)
{
  uint8_t value = readRegister(reg);
  value |= mask;
  writeRegister(reg, value);
}

/***************************************************************************//**
 * @brief
 *    Clear the given bit(s) in a register in the Hall sensor device.
 *
 * @param[in] addr
 *    The address of the register
 *
 * @param[in] mask
 *    The mask specifies which bits should be clear. If a given bit of the mask
 *    is 1 that register bit will be cleared to 0. All the other register bits
 *    will be untouched.
 ******************************************************************************/
void Si7210::clearRegisterBits(uint8_t reg, uint8_t mask)
{
  uint8_t value = readRegister(reg);
  value &= ~mask;
  writeRegister(reg, value);
}

/***************************************************************************//**
 * @brief
 *    Return the tamper level configured in the chip.
 *
 * @return
 *    The tamper level in mT
 ******************************************************************************/
float Si7210::getTamperThreshold()
{
  return 19.87f;
}

/***************************************************************************//**
 * @brief
 *    Calculate the sw_op value from the threshold by finding the inverse of
 *    the formula:
 *    threshold = (16 + sw_op[3:0]) * 2^sw_op[6:4]
 *
 * @param[in] threshold
 *    Threshold value
 *
 * @return
 *    The value of the sw_op bitfield
 ******************************************************************************/
uint8_t Si7210::calculateSwOp(float threshold)
{
  int th;
  uint8_t a;
  uint8_t swop;

  th = (int) (threshold / 0.005);

  if (th == 0) {
    // threshold = 0, when swop = 127
    return 127;
  } else if (th < 16) {
    threshold = 16;
  } else if (threshold > 3840) {
    threshold = 3840;
  }

  a = th / 16;
  swop = 0;
  while (a != 0x01) {
    a = a >> 1;   // Find the most significant 1 of th
    swop += 0x10;   // increment sw_op[6:4]
  }

  // Add remainder as sw_op[3:0]
  swop |= th / (1 << (swop >> 4)) - 16;

  return swop;
}

/***************************************************************************//**
 * @brief
 *    Calculate the sw_hyst value from the hysteresis by finding the inverse of
 *    the formula:
 *    hysteresis = (8 + sw_hyst[2:0]) * 2^sw_hyst[5:3]
 *
 * @param[in] hysteresis
 *    Hysteresis value
 *
 * @param[in] scale200mT
 *    scale200mT=false : full-scale equals 20mT
 *    scale200mT=true  : full-scale equals 200mT
 *
 * @return
 *    The value of the sw_hyst bitfield
 ******************************************************************************/
uint8_t Si7210::calculateSwHyst(float hysteresis, bool scale200mT)
{
  int hyst;
  uint8_t a;
  uint8_t swhyst;

  hyst = (int) (hysteresis / 0.005 + 0.5);

  if (scale200mT) {
    hyst /= 10;
  }

  if (hyst == 0) {
    // When sw_op = 63 the hysteresis is set to zero
    return 63;
  } else if (hyst < 8) {
    hyst = 8;
  } else if (hyst > 1792) {
    hyst = 1792;
  }

  a = hyst / 8;
  swhyst = 0;

  while (a != 0x01) {
    a = a >> 1;
    swhyst += 0x08;
  }

  swhyst |= hyst / (1 << (swhyst >> 3)) - 8;

  return swhyst;
}

/***************************************************************************//**
 * @brief
 *    Calculate the sw_tamper value from the tamper threshold by finding the
 *    inverse of the formula:
 *    tamper = (16 + sw_tamper[3:0]) * 2^(sw_tamper[5:4] + 5)
 *
 * @param[in] tamper
 *    Tamper value
 *
 * @param[in] scale200mT
 *    scale200mT=false : full-scale equals 20mT
 *    scale200mT=true  : full-scale equals 200mT
 *
 * @return
 *    The value of the sw_tamper bitfield
 ******************************************************************************/
uint8_t Si7210::calculateSwTamper(float tamper, bool scale200mT)
{
  int tamp;
  int exp;
  uint8_t a;
  uint8_t swtamp;

  tamp = (int) (tamper / 0.005 + 0.5);

  if (scale200mT) {
    tamp /= 10;
  }

  if (tamper == 0) {
    // When sw_tamper = 63 the tamper is set to zero
    return 63;
  } else if (tamp < 512) {
    tamp = 512;
  } else if (tamp > 3968) {
    tamp = 3968;
  }

  a = tamp / 16;

  exp = 0;
  while (a != 0x01) {
    a = a >> 1;
    exp++;
  }

  swtamp = (exp - 5) << 4;

  swtamp |= tamp / (1 << exp) - 16;

  return swtamp;
}

/***************************************************************************//**
 * @brief
 *    Calculate the slTime value from the sleep time by finding the inverse of
 *    the formula:
 *    tsleep = (32 + slTime[4:0]) * 2^(8 + slTime[7:5]) / 12 MHz
 *
 * @param[in] samplePeriod
 *    The sleep time
 *
 * @param[in] slFast
 *    The value of the slFast bit
 *
 * @return
 *    The value of the slTime bitfield
 ******************************************************************************/
uint8_t Si7210::calculateSltime(uint32_t samplePeriod, uint8_t &slFast)
{
  int a;
  uint8_t slTime;

  if (samplePeriod == 0) {
    slFast = 1;
    slTime = 0;
    return slTime;
  }

  // Impose limits
  if (samplePeriod < 11) {
    samplePeriod = 11;
  } else if (samplePeriod > 172000) {
    samplePeriod = 172000;
  }

  // Decide on wether we need slFast or not
  if (samplePeriod < 750) {
    slFast = 1;
    a = samplePeriod * 12 / 32 / 4;
  } else {
    slFast = 0;
    a = samplePeriod * 12 / 32 / 256;
  }

  slTime = 0;
  while (a != 0x01) {
    a = a >> 1;
    slTime += 0x20;
  }

  if (slFast) {
    slTime |= samplePeriod * 12 / (4 << (slTime >> 5)) - 32;
  } else {
    slTime |= samplePeriod * 12 / (256 << (slTime >> 5)) - 32;
  }

  return slTime;
}
