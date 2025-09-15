/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2023 Silicon Laboratories Inc. www.silabs.com
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

// This file is an adapter between the different I2C instances and the Arduino Core driver
#ifndef ARDUINO_I2C_CONFIG_H
#define ARDUINO_I2C_CONFIG_H

#include "sl_i2cspm.h"
#include "sl_i2cspm_qwiic_config.h"

extern I2CSPM_Init_TypeDef init_qwiic;
extern I2CSPM_Init_TypeDef* arduino_i2c_config;

#define SL_I2C_PERIPHERAL        SL_I2CSPM_QWIIC_PERIPHERAL
#define SL_I2C_PERIPHERAL_NUM    SL_I2CSPM_QWIIC_PERIPHERAL_NO
#define SL_I2C_SCL_PORT          SL_I2CSPM_QWIIC_SCL_PORT
#define SL_I2C_SCL_PIN           SL_I2CSPM_QWIIC_SCL_PIN
#define SL_I2C_SDA_PORT          SL_I2CSPM_QWIIC_SDA_PORT
#define SL_I2C_SDA_PIN           SL_I2CSPM_QWIIC_SDA_PIN

#endif // ARDUINO_I2C_CONFIG_H
