/***************************************************************************//**
 * @file
 * @brief I2C simple poll-based master mode driver
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_I2CSPM_H
#define SL_I2CSPM_H

#include "em_device.h"

#if defined(_SILICON_LABS_32B_SERIES_2)
#include "em_i2c.h"
#endif

#include "sl_gpio.h"

/***************************************************************************//**
 * @addtogroup i2cspm I2C Simple Polled Master
 * @brief I2C Simple Polled Master driver
 *
 * @details
 *   This driver supports master mode, single bus-master only. It blocks
 *   while waiting for the transfer is complete, polling for completion in EM0.
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/
#if defined(_SILICON_LABS_32B_SERIES_3)
/**
 * @brief
 *   Standard mode max frequency assuming using 4:4 ratio for Nlow:Nhigh.
 * @details
 *   From I2C specification: Min Tlow = 4.7us, min Thigh = 4.0us,
 *   max Trise=1.0us, max Tfall=0.3us. Since ratio is 4:4, have to use
 *   worst case value of Tlow or Thigh as base.
 *
 *   1/(Tlow + Thigh + 1us + 0.3us) = 1/(4.7 + 4.7 + 1.3)us = 93458Hz.
 * @note
 *   Due to chip characteristics, max value is somewhat reduced.
 */
#define I2C_FREQ_STANDARD_MAX   100000

/**
 * @brief
 *   Fast mode max frequency assuming using 6:3 ratio for Nlow:Nhigh.
 * @details
 *   From I2C specification: Min Tlow = 1.3us, min Thigh = 0.6us,
 *   max Trise=0.3us, max Tfall=0.3us. Since ratio is 6:3, have to use
 *   worst case value of Tlow or 2xThigh as base.
 *
 *   1/(Tlow + Thigh + 0.3us + 0.3us) = 1/(1.3 + 0.65 + 0.6)us = 392157Hz.
 */
#define I2C_FREQ_FAST_MAX        392157

/**
 * @brief
 *   Fast mode+ max frequency assuming using 11:6 ratio for Nlow:Nhigh.
 * @details
 *   From I2C specification: Min Tlow = 0.5us, min Thigh = 0.26us,
 *   max Trise=0.12us, max Tfall=0.12us. Since ratio is 11:6, have to use
 *   worst case value of Tlow or (11/6)xThigh as base.
 *
 *   1/(Tlow + Thigh + 0.12us + 0.12us) = 1/(0.5 + 0.273 + 0.24)us = 987167Hz.
 */
#define I2C_FREQ_FASTPLUS_MAX    987167

/**
 * @brief
 *   Indicate plain write sequence: S+ADDR(W)+DATA0+P.
 * @details
 *   @li S - Start
 *   @li ADDR(W) - address with W/R bit cleared
 *   @li DATA0 - Data taken from buffer with index 0
 *   @li P - Stop
 */
#define I2C_FLAG_WRITE          0x0001

/**
 * @brief
 *   Indicate plain read sequence: S+ADDR(R)+DATA0+P.
 * @details
 *   @li S - Start
 *   @li ADDR(R) - Address with W/R bit set
 *   @li DATA0 - Data read into buffer with index 0
 *   @li P - Stop
 */
#define I2C_FLAG_READ           0x0002

/**
 * @brief
 *   Indicate combined write/read sequence: S+ADDR(W)+DATA0+Sr+ADDR(R)+DATA1+P.
 * @details
 *   @li S - Start
 *   @li Sr - Repeated start
 *   @li ADDR(W) - Address with W/R bit cleared
 *   @li ADDR(R) - Address with W/R bit set
 *   @li DATAn - Data written from/read into buffer with index n
 *   @li P - Stop
 */
#define I2C_FLAG_WRITE_READ     0x0004

/**
 * @brief
 *   Indicate write sequence using two buffers: S+ADDR(W)+DATA0+DATA1+P.
 * @details
 *   @li S - Start
 *   @li ADDR(W) - Address with W/R bit cleared
 *   @li DATAn - Data written from buffer with index n
 *   @li P - Stop
 */
#define I2C_FLAG_WRITE_WRITE    0x0008

#endif //_SILICON_LABS_32B_SERIES_3

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
#if defined(_SILICON_LABS_32B_SERIES_3)
/// I2C transfer events.
typedef enum {
  i2cTransferInProgress,                            ///< I2C Transfer in progress.
  i2cTransferDone,                                  ///< I2C data transfer complete.
  i2cTransferNack,                                  ///< I2C NACK.
  i2cTransferBusErr,                                ///< I2C Bus Error.
  i2cTransferArbLost,                               ///< I2C Arbitration Lost.
  i2cTransferUsageFault,                            ///< I2C Usage Fault.
} I2C_TransferReturn_TypeDef;

/// Clock low to high ratio settings.
typedef enum {
  i2cClockHLRStandard  = _I2C_CTRL_CLHR_STANDARD,      /**< Ratio is 4:4 */
  i2cClockHLRAsymetric = _I2C_CTRL_CLHR_ASYMMETRIC,    /**< Ratio is 6:3 */
  i2cClockHLRFast      = _I2C_CTRL_CLHR_FAST           /**< Ratio is 11:3 */
} I2C_ClockHLR_TypeDef;
#endif //_SILICON_LABS_32B_SERIES_3

/*******************************************************************************
 ********************************   STRUCTS   **********************************
 ******************************************************************************/

/// I2C driver instance initialization structure. This data structure contains a number of I2C
/// configuration options required for driver instance initialization.This struct is passed to
/// @ref I2CSPM_Init() when initializing a I2CSPM instance.
typedef struct {
  I2C_TypeDef           *port;          ///< Peripheral port.
  sl_gpio_port_t        sclPort;        ///< SCL pin port number.
  uint8_t               sclPin;         ///< SCL pin number.
  sl_gpio_port_t        sdaPort;        ///< SDA pin port number.
  uint8_t               sdaPin;         ///< SDA pin number.
  uint32_t              i2cRefFreq;     ///< I2C reference clock.
  uint32_t              i2cMaxFreq;     ///< I2C max bus frequency to use.
  I2C_ClockHLR_TypeDef  i2cClhr;        ///< Clock low/high ratio control.
} I2CSPM_Init_TypeDef;

#if defined(_SILICON_LABS_32B_SERIES_3)
/// Master mode transfer message structure used to define a complete I2C transfer sequence (from start to stop).
/// The structure allows for defining the following types of sequences(refer to defines for sequence details):
/// #I2C_FLAG_READ - Data read into buf[0].data,
/// #I2C_FLAG_WRITE - Data written from buf[0].data,
/// #I2C_FLAG_WRITE_READ - Data written from buf[0].data and read into buf[1].data
/// #I2C_FLAG_WRITE_WRITE - Data written from buf[0].data and buf[1].data.
typedef struct {
  /// Address to use after (repeated) start.
  /// Layout details, A = Address bit, X = don't care bit (set to 0):
  /// 7 bit address - Use format AAAA AAAX.
  /// 10 bit address - Use format XXXX XAAX AAAA AAAA.
  uint16_t addr;

  /// Flags defining sequence type and details, see I2C_FLAG_ defines.
  uint16_t flags;

  /// Buffers used to hold data to send from or receive into, depending on sequence type.
  struct {
    /// Buffer used for data to transmit/receive, must be @p len long.
    uint8_t  *data;

    /// Number of bytes in @p data to send or receive.
    /// Notice that when receiving data to this buffer, at least 1 byte must be received.
    /// Setting @p len to 0 in the receive case is considered a usage fault.
    /// Transmitting 0 bytes is legal, in which case only the address
    /// is transmitted after the start condition.
    uint16_t len;
  } buf[2];
} I2C_TransferSeq_TypeDef;
#endif //_SILICON_LABS_32B_SERIES_3

/// I2CSPM Peripheral.
typedef I2C_TypeDef sl_i2cspm_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Initialize I2C peripheral.
 *
 * @details
 *   This driver supports master mode only, single bus-master. In addition
 *   to configuring the I2C peripheral module, it also configures DK/STK
 *   specific setup in order to use the I2C bus.
 *
 * @param[in] init
 *   Pointer to I2C initialization structure.
 ******************************************************************************/
void I2CSPM_Init(I2CSPM_Init_TypeDef *init);

/***************************************************************************//**
 * @brief
 *   Perform I2C transfer.
 *
 * @param[in] i2c
 *   Pointer to the peripheral port
 *
 * @param[in] seq
 *   Pointer to sequence structure defining the I2C transfer to take place. The
 *   referenced structure must exist until the transfer has fully completed.
 *
 * @return
 *   Returns status of ongoing transfer.
 ******************************************************************************/
I2C_TransferReturn_TypeDef I2CSPM_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq);

#ifdef __cplusplus
}
#endif

/** @} (end group i2cspm) */

#endif /* SL_I2CSPM_H */
