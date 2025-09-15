/***************************************************************************//**
 * @file
 * @brief IO Stream EUSART Component.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_IOSTREAM_EUSART_H
#define SL_IOSTREAM_EUSART_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_iostream.h"
#include "sl_iostream_uart.h"
#include "sl_status.h"
#include "sl_slist.h"
#include "sl_device_peripheral.h"
#include "sl_device_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup iostream
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup iostream_eusart I/O Stream EUSART
 * @brief I/O Stream EUSART
 * @details
 * ## Overview
 *
 *   The Enhanced Universal Synchronous / Asynchronous Receiver / Transmitter controller(s)
 *   (EUSART) can be used as a UART and can, therefore, be connected to an external transceiver
 *   to communicate with another host using the serial link. Multiple instances can be
 *   instantiated as long as they are bound to a unique EUSART peripheral. The flow control
 *   is supported as well.
 *
 * ## Initialization
 *
 *   The stream sets itself as the default stream at the end of the initialization
 *   function.You must reconfigure the default interface if you have multiple streams
 *   in your project else the last stream initialized will be set as the system default
 *   stream.
 *
 * ## Power manager integration
 *
 *   On transmission side, I/O Stream-EUSART adds a requirement on EM1 or EM2, depending on
 *   the frequency mode, until the transmit is really completed. Once the transmit is
 *   completed the requirement is removed and the system can go to a lower energy mode if allowed.
 *
 *   On the receive side, a requirement on EM1 or EM2, depending on the frequency mode, is
 *   added if the application can receive data asynchronously. Meaning that the system can
 *   receive data when the MCU is in sleep mode (EM1 or EM2 to allow the reception). Allowing
 *   the reception in sleep mode will obviously increase the power consumption.
 *   Otherwise, If the system doesn't expect to receive anything, no requirement is added and
 *   the system can go to the lowest Energy mode allowed. You can control this behavior using a
 *   configuration per instance, see the following configuration:
 *
 *       SL_IOSTREAM_EUSART_<instance_name>_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION
 *
 * @{
 ******************************************************************************/
/// @brief Struct representing UART parity configuration
SL_ENUM(sl_iostream_eusart_uart_parity_t) {
  SL_IOSTREAM_EUSART_UART_NO_PARITY,            ///< No parity
  SL_IOSTREAM_EUSART_UART_EVEN_PARITY,          ///< Even parity
  SL_IOSTREAM_EUSART_UART_ODD_PARITY            ///< Odd parity
};

/// @brief Struct representing UART stop bits configuration
SL_ENUM(sl_iostream_eusart_uart_stop_bits_t) {
  SL_IOSTREAM_EUSART_UART_STOP_BITS_0P5,        ///< 0.5 stop bits
  SL_IOSTREAM_EUSART_UART_STOP_BITS_1P5,        ///< 1.5 stop bits
  SL_IOSTREAM_EUSART_UART_STOP_BITS_1,          ///< 1 stop bits
  SL_IOSTREAM_EUSART_UART_STOP_BITS_2           ///< 2 stop bits
};

/// @brief Struct representing UART flow control configuration
SL_ENUM(sl_iostream_eusart_uart_flow_control_t) {
  SL_IOSTREAM_EUSART_UART_FLOW_CTRL_NONE,       ///< No flow control
  SL_IOSTREAM_EUSART_UART_FLOW_CTRL_CTS,        ///< CTS HWFC
  SL_IOSTREAM_EUSART_UART_FLOW_CTRL_RTS,        ///< RTS HWFC
  SL_IOSTREAM_EUSART_UART_FLOW_CTRL_CTS_RTS,    ///< CTS & RTS HWFC
  SL_IOSTREAM_EUSART_UART_FLOW_CTRL_SOFT        ///< Software flow control
};

/// @brief Struct representing an I/O Stream EUSART configuration.
typedef struct {
  sl_peripheral_t eusart;                               ///< EUSART peripheral
  uint8_t eusart_nbr;                                   ///< EUSART peripheral number
  uint32_t baudrate;                                    ///< UART baudrate
  sl_iostream_eusart_uart_parity_t parity;              ///< UART parity
  sl_iostream_eusart_uart_stop_bits_t stop_bits;        ///< UART stop bits
  sl_iostream_eusart_uart_flow_control_t flow_control;  ///< Flow control
  bool enable_high_frequency;                           ///< Used for legacy clock management
  sl_bus_clock_t bus_clock;                             ///< Peripheral Clock
#if defined(EUSART_COUNT)
  uint8_t port_index;                                   ///< Port index for GPIO routing
#endif
  uint8_t tx_port;                                      ///< Transmit port
  uint8_t tx_pin;                                       ///< Transmit pin
  uint8_t rx_port;                                      ///< Receive port
  uint8_t rx_pin;                                       ///< Receive pin
  uint8_t cts_port;                                     ///< Flow control, CTS port
  uint8_t cts_pin;                                      ///< Flow control, CTS pin
  uint8_t rts_port;                                     ///< Flow control, RTS port
  uint8_t rts_pin;                                      ///< Flow control, RTS pin
} sl_iostream_eusart_config_t;

/// @brief Struct representing an I/O Stream EUSART context.
typedef struct {
  sl_iostream_uart_context_t context;           ///< context
  sl_peripheral_t eusart;                       ///< eusart
  sl_bus_clock_t bus_clock;                     ///< Peripheral Clock
  uint8_t tx_port;                              ///< Transmit port
  uint8_t tx_pin;                               ///< Transmit pin
  uint8_t rx_port;                              ///< Receive port
  uint8_t rx_pin;                               ///< Receive pin
  uint8_t cts_port;                             ///< Flow control, CTS port
  uint8_t cts_pin;                              ///< Flow control, CTS pin
  uint8_t rts_port;                             ///< Flow control, RTS port
  uint8_t rts_pin;                              ///< Flow control, RTS pin
  uint8_t flags;                                ///< Configuration flags
#if (defined(SL_CATALOG_POWER_MANAGER_PRESENT))
  sl_slist_node_t node;
#endif
} sl_iostream_eusart_context_t;

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * EUSART Stream init.
 *
 * @param[in] iostream_uart  I/O Stream UART handle.
 *
 * @param[in] uart_config  I/O Stream UART config.
 *
 * @param[in] eusart_config  EUSART configuration.
 *
 * @param[in] eusart_context  EUSART Instance context.
 *
 * @return  Status result
 ******************************************************************************/
sl_status_t sl_iostream_eusart_init(sl_iostream_uart_t *iostream_uart,
                                    sl_iostream_uart_config_t *uart_config,
                                    sl_iostream_eusart_config_t *eusart_config,
                                    sl_iostream_eusart_context_t *eusart_context);

/*******************************************************************************
 * EUSART interrupt handler.
 * @param[in] iostream_uart   I/O Stream UART handle.
 ******************************************************************************/
void sl_iostream_eusart_irq_handler(sl_iostream_uart_t *iostream_uart);

/** @} (end addtogroup iostream_eusart) */
/** @} (end addtogroup iostream) */

#ifdef __cplusplus
}
#endif

#endif /* SL_IOSTREAM_EUSART_H */
