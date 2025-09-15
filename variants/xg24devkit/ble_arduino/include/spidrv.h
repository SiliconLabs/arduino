/***************************************************************************//**
 * @file
 * @brief SPIDRV API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef __SILICON_LABS_SPIDRV_H__
#define __SILICON_LABS_SPIDRV_H__

#ifndef SLI_LIBRARY_BUILD
#include "em_device.h"
#include "sl_device_peripheral.h"
#include "dmadrv.h"

#include "sl_gpio.h"
#if defined(_SILICON_LABS_32B_SERIES_3)
#include "sl_hal_eusart.h"
#endif
#endif

#include "ecode.h"
#include "spidrv_config.h"
#include "sl_sleeptimer.h"
#include "sl_slist.h"
#include "sl_enum.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup spidrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup spidrv_error_codes Error Codes
 * @{
 ******************************************************************************/

#define ECODE_EMDRV_SPIDRV_OK                (ECODE_OK)                               ///< A successful return value.
#define ECODE_EMDRV_SPIDRV_ILLEGAL_HANDLE    (ECODE_EMDRV_SPIDRV_BASE | 0x00000001)   ///< An illegal SPI handle.
#define ECODE_EMDRV_SPIDRV_PARAM_ERROR       (ECODE_EMDRV_SPIDRV_BASE | 0x00000002)   ///< An illegal input parameter.
#define ECODE_EMDRV_SPIDRV_BUSY              (ECODE_EMDRV_SPIDRV_BASE | 0x00000003)   ///< The SPI port is busy.
#define ECODE_EMDRV_SPIDRV_TIMER_ALLOC_ERROR (ECODE_EMDRV_SPIDRV_BASE | 0x00000004)   ///< Unable to allocate timeout timer.
#define ECODE_EMDRV_SPIDRV_TIMEOUT           (ECODE_EMDRV_SPIDRV_BASE | 0x00000005)   ///< An SPI transfer timeout.
#define ECODE_EMDRV_SPIDRV_IDLE              (ECODE_EMDRV_SPIDRV_BASE | 0x00000006)   ///< No SPI transfer in progress.
#define ECODE_EMDRV_SPIDRV_ABORTED           (ECODE_EMDRV_SPIDRV_BASE | 0x00000007)   ///< An SPI transfer has been aborted.
#define ECODE_EMDRV_SPIDRV_MODE_ERROR        (ECODE_EMDRV_SPIDRV_BASE | 0x00000008)   ///< SPI master used slave API or vica versa.
#define ECODE_EMDRV_SPIDRV_DMA_ALLOC_ERROR   (ECODE_EMDRV_SPIDRV_BASE | 0x00000009)   ///< Unable to allocate DMA channels.
/** @} (end addtogroup error codes) */

/// SPI driver instance type.
SL_ENUM(SPIDRV_Type_t) {
  spidrvMaster = 0,               ///< Act as an SPI master.
  spidrvSlave  = 1                ///< Act as an SPI slave.
};

/// SPI bus bit order.
SL_ENUM(SPIDRV_BitOrder_t) {
  spidrvBitOrderLsbFirst = 0,     ///< LSB bit is transmitted first.
  spidrvBitOrderMsbFirst = 1      ///< MSB bit is transmitted first.
};

/// SPI clock mode (clock polarity and phase).
SL_ENUM(SPIDRV_ClockMode_t) {
  spidrvClockMode0 = 0,           ///< SPI mode 0: CLKPOL=0, CLKPHA=0.
  spidrvClockMode1 = 1,           ///< SPI mode 1: CLKPOL=0, CLKPHA=1.
  spidrvClockMode2 = 2,           ///< SPI mode 2: CLKPOL=1, CLKPHA=0.
  spidrvClockMode3 = 3            ///< SPI mode 3: CLKPOL=1, CLKPHA=1.
};

/// SPI master chip select (CS) control scheme.
SL_ENUM(SPIDRV_CsControl_t) {
  spidrvCsControlAuto = 0,        ///< CS controlled by the SPI driver.
  spidrvCsControlApplication = 1  ///< CS controlled by the application.
};

/// SPI slave transfer start scheme.
SL_ENUM(SPIDRV_SlaveStart_t) {
  spidrvSlaveStartImmediate = 0,  ///< Transfer starts immediately.
  spidrvSlaveStartDelayed = 1     ///< Transfer starts when the bus is idle (CS deasserted).
};

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
/// Type of a USART peripheral
SL_ENUM(SPIDRV_PeripheralType_t) {
  spidrvPeripheralTypeUsart = 0,         ///< USART peripheral
  spidrvPeripheralTypeEusart = 1         ///< EUSART peripheral
};

SL_ENUM(SPIDRV_State_t) {
  spidrvStateIdle = 0,
  spidrvStateTransferring = 1
};
/// @endcond
struct SPIDRV_HandleData;

/***************************************************************************//**
 * @brief
 *  SPIDRV transfer completion callback function.
 *
 * @details
 *  Called when a transfer is complete. An
 *  application should check the transferStatus and itemsTransferred values.
 *
 * @param[in] handle
 *   The SPIDRV device handle used to start the transfer.
 *
 * @param[in] transferStatus
 *   A number of bytes actually transferred.
 *
 * @param[in] itemsTransferred
 *   A number of bytes transferred.
 *
 * @return
 *   @ref ECODE_EMDRV_SPIDRV_OK on success, @ref ECODE_EMDRV_SPIDRV_TIMEOUT
 *   on timeout. Timeouts are only relevant for slave mode transfers.
 ******************************************************************************/
typedef void (*SPIDRV_Callback_t)(struct SPIDRV_HandleData *handle,
                                  Ecode_t transferStatus,
                                  int itemsTransferred);

/**
 * @struct SPIDRV_Init
 * @struct SPIDRV_Init_t
 * @brief An SPI driver instance initialization structure.
 *   Contains a number of SPIDRV configuration options.
 *   This structure is passed to @ref SPIDRV_Init() when initializing a SPIDRV instance.
 *   Some common initialization data sets are predefined in @ref spidrv_init_structs.
 */
typedef struct SPIDRV_Init {
  void               *port;             ///< The USART used for SPI.
  sl_gpio_port_t      portTx;           ///< Tx port.
  sl_gpio_port_t      portRx;           ///< Rx port.
  sl_gpio_port_t      portClk;          ///< Clock port.
  sl_gpio_port_t      portCs;           ///< Chip select port.
  uint8_t             pinTx;            ///< Tx pin.
  uint8_t             pinRx;            ///< Rx pin.
  uint8_t             pinClk;           ///< Clock pin.
  uint8_t             pinCs;            ///< Chip select pin.
  uint32_t            bitRate;          ///< An SPI bitrate.
  uint32_t            frameLength;      ///< An SPI framelength, valid numbers are 4..16
  uint32_t            dummyTxValue;     ///< The value to transmit when using SPI receive API functions.
  SPIDRV_Type_t       type;             ///< An SPI type, master or slave.
  SPIDRV_BitOrder_t   bitOrder;         ///< A bit order on the SPI bus, MSB or LSB first.
  SPIDRV_ClockMode_t  clockMode;        ///< SPI mode, CLKPOL/CLKPHASE setting.
  SPIDRV_CsControl_t  csControl;        ///< A select master mode chip select (CS) control scheme.
  SPIDRV_SlaveStart_t slaveStartMode;   ///< A slave mode transfer start scheme.
} SPIDRV_Init_t;

#ifndef SLI_LIBRARY_BUILD
/// An SPI driver instance handle data structure.
/// The handle is allocated by the application using the SPIDRV.
/// Several concurrent driver instances can exist in an application. The application is
/// neither supposed to write or read the contents of the handle.
typedef struct SPIDRV_HandleData {
  /// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
  union {
#if defined(USART_PRESENT)
    USART_TypeDef           *usartPort;
#endif
#if defined(EUSART_PRESENT)
    EUSART_TypeDef          *eusartPort;
#endif
    void                    *__reserved_space;
  } peripheral;
  SPIDRV_Init_t             initData;
  unsigned int              txDMACh;
  unsigned int              rxDMACh;
  DMADRV_PeripheralSignal_t txDMASignal;
  DMADRV_PeripheralSignal_t rxDMASignal;
  SPIDRV_Callback_t         userCallback;
  uint32_t                  dummyRx;
  int                       transferCount;
  int                       remaining;
  sl_gpio_port_t            portCs;
  uint8_t                   pinCs;
  Ecode_t                   transferStatus;
  volatile SPIDRV_State_t   state;
  sl_peripheral_t           usartPeripheral;
  volatile bool             blockingCompleted;
  int                       em1RequestCount;
  SPIDRV_PeripheralType_t   peripheralType;
  sl_sleeptimer_timer_handle_t timer;
  sl_slist_node_t           node;
  /// @endcond
} SPIDRV_HandleData_t;

/// An SPI driver instance handle.
typedef SPIDRV_HandleData_t * SPIDRV_Handle_t;
#else
typedef void * SPIDRV_Handle_t;
#endif

/***************************************************************************//**
 * @addtogroup spidrv_init_structs Init Configuration Data
 * Example default configuration data for \ref SPIDRV_Init
 * @{
 ******************************************************************************/
#if defined (_GPIO_USART_ROUTEEN_MASK) /* Series 2 devices */
/// Configuration data for SPI master using USART0.
#define SPIDRV_MASTER_DEFAULT                                          \
  {                                                                    \
    USART0,                     /* USART port                       */ \
    SL_GPIO_PORT_C,             /* USART Tx port location number    */ \
    SL_GPIO_PORT_C,             /* USART Rx port location number    */ \
    SL_GPIO_PORT_C,             /* USART Clk port location number   */ \
    SL_GPIO_PORT_C,             /* USART Cs port location number    */ \
    2,                          /* USART Tx port location number    */ \
    3,                          /* USART Rx port location number    */ \
    4,                          /* USART Clk pin location number    */ \
    5,                          /* USART Cs pin location number     */ \
    1000000,                    /* Bitrate                          */ \
    8,                          /* Frame length                     */ \
    0,                          /* Dummy tx value for rx only funcs */ \
    spidrvMaster,               /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,     /* Bit order on bus                 */ \
    spidrvClockMode0,           /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,        /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate   /* Slave start transfers immediately*/ \
  }

/// Configuration data for SPI slave.
#define SPIDRV_SLAVE_DEFAULT                                           \
  {                                                                    \
    USART0,                     /* USART port                       */ \
    SL_GPIO_PORT_C,             /* USART Tx port location number    */ \
    SL_GPIO_PORT_C,             /* USART Rx port location number    */ \
    SL_GPIO_PORT_C,             /* USART Clk port location number   */ \
    SL_GPIO_PORT_C,             /* USART Cs port location number    */ \
    2,                          /* USART Tx port location number    */ \
    3,                          /* USART Rx port location number    */ \
    4,                          /* USART Clk pin location number    */ \
    5,                          /* USART Cs pin location number     */ \
    0,                          /* Bitrate                          */ \
    8,                          /* Frame length                     */ \
    0,                          /* Dummy tx value for rx only funcs */ \
    spidrvSlave,                /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,     /* Bit order on bus                 */ \
    spidrvClockMode0,           /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,        /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate   /* Slave start transfers immediately*/ \
  }
#endif /* _USART_ROUTELOC0_MASK */

#if defined(EUSART0)
#if defined(_SILICON_LABS_32B_SERIES_2)
#define SPIDRV_MASTER_EUSART0                                        \
  {                                                                  \
    EUSART0,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART0                                         \
  {                                                                  \
    EUSART0,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#else /* Series 3 and above */
#define SPIDRV_MASTER_EUSART0                                        \
  {                                                                  \
    EUSART0,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART0                                         \
  {                                                                  \
    EUSART0,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#endif
#endif

#if defined(EUSART1)
#if defined(_SILICON_LABS_32B_SERIES_2)
#define SPIDRV_MASTER_EUSART1                                        \
  {                                                                  \
    EUSART1,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART1                                         \
  {                                                                  \
    EUSART1,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#else /* Series 3 and above */
#define SPIDRV_MASTER_EUSART1                                        \
  {                                                                  \
    EUSART1,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART1                                         \
  {                                                                  \
    EUSART1,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#endif
#endif

#if defined(EUSART2)
#if defined(_SILICON_LABS_32B_SERIES_2)
#define SPIDRV_MASTER_EUSART2                                        \
  {                                                                  \
    EUSART2,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART2                                         \
  {                                                                  \
    EUSART2,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#else /* Series 3 and above */
#define SPIDRV_MASTER_EUSART2                                        \
  {                                                                  \
    EUSART2,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART2                                         \
  {                                                                  \
    EUSART2,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#endif
#endif

#if defined(EUSART3)
#if defined(_SILICON_LABS_32B_SERIES_2)
#define SPIDRV_MASTER_EUSART3                                        \
  {                                                                  \
    EUSART3,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART3                                         \
  {                                                                  \
    EUSART3,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#else /* Series 3 and above */
#define SPIDRV_MASTER_EUSART3                                        \
  {                                                                  \
    EUSART3,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART3                                         \
  {                                                                  \
    EUSART3,                  /* EUSART port                      */ \
    SL_GPIO_PORT_A,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_A,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_A,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    6,                        /* EUSART Rx port location number   */ \
    7,                        /* EUSART Clk pin location number   */ \
    5,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#endif
#endif

#if defined(EUSART4)
#if defined(_SILICON_LABS_32B_SERIES_2)
#define SPIDRV_MASTER_EUSART4                                        \
  {                                                                  \
    EUSART4,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART4                                         \
  {                                                                  \
    EUSART4,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#else /* Series 3 and above */
#define SPIDRV_MASTER_EUSART4                                        \
  {                                                                  \
    EUSART4,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvMaster,             /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }

#define SPIDRV_SLAVE_EUSART4                                         \
  {                                                                  \
    EUSART4,                  /* EUSART port                      */ \
    SL_GPIO_PORT_C,           /* EUSART Tx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Rx port location number   */ \
    SL_GPIO_PORT_C,           /* EUSART Clk port location number  */ \
    SL_GPIO_PORT_C,           /* EUSART Cs port location number   */ \
    0,                        /* EUSART Tx port location number   */ \
    1,                        /* EUSART Rx port location number   */ \
    2,                        /* EUSART Clk pin location number   */ \
    3,                        /* EUSART Cs pin location number    */ \
    1000000,                  /* Bitrate                          */ \
    8,                        /* Frame length                     */ \
    0,                        /* Dummy tx value for rx only funcs */ \
    spidrvSlave,              /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,   /* Bit order on bus                 */ \
    spidrvClockMode0,         /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,      /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate /* Slave start transfers immediately*/ \
  }
#endif
#endif
/** @} (end addtogroup Configuration data) */

Ecode_t   SPIDRV_AbortTransfer(SPIDRV_Handle_t handle);

Ecode_t   SPIDRV_DeInit(SPIDRV_Handle_t handle);

Ecode_t   SPIDRV_GetBitrate(SPIDRV_Handle_t handle,
                            uint32_t *bitRate);

Ecode_t   SPIDRV_GetFramelength(SPIDRV_Handle_t handle,
                                uint32_t *frameLength);

Ecode_t   SPIDRV_GetTransferStatus(SPIDRV_Handle_t handle,
                                   int *itemsTransferred,
                                   int *itemsRemaining);

Ecode_t   SPIDRV_Init(SPIDRV_Handle_t handle,
                      SPIDRV_Init_t *initData);

Ecode_t   SPIDRV_MReceive(SPIDRV_Handle_t handle,
                          void *buffer,
                          int count,
                          SPIDRV_Callback_t callback);

Ecode_t   SPIDRV_MReceiveB(SPIDRV_Handle_t handle,
                           void *buffer,
                           int count);

Ecode_t   SPIDRV_MTransfer(SPIDRV_Handle_t handle,
                           const void *txBuffer,
                           void *rxBuffer,
                           int count,
                           SPIDRV_Callback_t callback);

Ecode_t   SPIDRV_MTransferB(SPIDRV_Handle_t handle,
                            const void *txBuffer,
                            void *rxBuffer,
                            int count);

Ecode_t   SPIDRV_MTransferSingleItemB(SPIDRV_Handle_t handle,
                                      uint32_t txValue,
                                      void *rxValue);

Ecode_t   SPIDRV_MTransmit(SPIDRV_Handle_t handle,
                           const void *buffer,
                           int count,
                           SPIDRV_Callback_t callback);

Ecode_t   SPIDRV_MTransmitB(SPIDRV_Handle_t handle,
                            const void *buffer,
                            int count);

Ecode_t   SPIDRV_SetBitrate(SPIDRV_Handle_t handle,
                            uint32_t bitRate);

Ecode_t   SPIDRV_SetFramelength(SPIDRV_Handle_t handle,
                                uint32_t frameLength);

Ecode_t   SPIDRV_SReceive(SPIDRV_Handle_t handle,
                          void *buffer,
                          int count,
                          SPIDRV_Callback_t callback,
                          int timeoutMs);

Ecode_t   SPIDRV_SReceiveB(SPIDRV_Handle_t handle,
                           void *buffer,
                           int count,
                           int timeoutMs);

Ecode_t   SPIDRV_STransfer(SPIDRV_Handle_t handle,
                           const void *txBuffer,
                           void *rxBuffer,
                           int count,
                           SPIDRV_Callback_t callback,
                           int timeoutMs);

Ecode_t   SPIDRV_STransferB(SPIDRV_Handle_t handle,
                            const void *txBuffer,
                            void *rxBuffer,
                            int count,
                            int timeoutMs);

Ecode_t   SPIDRV_STransmit(SPIDRV_Handle_t handle,
                           const void *buffer,
                           int count,
                           SPIDRV_Callback_t callback,
                           int timeoutMs);

Ecode_t   SPIDRV_STransmitB(SPIDRV_Handle_t handle,
                            const void *buffer,
                            int count,
                            int timeoutMs);

/** @} (end addtogroup spidrv) */

#ifdef __cplusplus
}
#endif

#endif /* __SILICON_LABS_SPIDRV_H__ */
