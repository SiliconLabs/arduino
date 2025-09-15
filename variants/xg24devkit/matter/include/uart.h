#ifndef _UART_H
#define _UART_H

#include <stdbool.h>

/**
 * The size of the receive buffer
 *
 */
#define RECEIVE_BUFFER_SIZE 128

/**
 * This function initializes the UART interface.
 *
 */
void efr32UartInit(void);

/**
 * This function performs UART processing.
 *
 */
void efr32UartProcess(void);

/**
 * This function informs the caller whether UART
 * operations are ready to process
 *
 * @return true if RX or TX data is ready to process.
 */
bool efr32UartIsDataReady(void);

#endif // _UART_H
