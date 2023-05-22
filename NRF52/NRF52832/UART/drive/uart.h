#ifndef __uart_H
#define __uart_H

#include "app_uart.h"
#include "nrf_peripherals.h"
#include "stdio.h"
#include "nrf.h"

#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

#define RX_PIN_NUMBER	8
#define TX_PIN_NUMBER 6
#define RTS_PIN_NUMBER 7
#define CTS_PIN_NUMBER 5


void uartInit(void);

#endif
