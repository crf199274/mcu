#include "uart.h"

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void uartInit(void)
{
	uint32_t err_code;
	
	//�������ò����ṹ��
	const app_uart_comm_params_t comm_params =
  {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          UART_HWFC,
          false,
#if defined (UART_PRESENT)
          NRF_UART_BAUDRATE_115200
#else
          NRF_UARTE_BAUDRATE_115200
#endif
	};
	
	//���ڳ�ʼ����FIFO)
  APP_UART_FIFO_INIT(&comm_params,
											 UART_RX_BUF_SIZE,
											 UART_TX_BUF_SIZE,
											 uart_error_handle,//�жϻص�
											 APP_IRQ_PRIORITY_LOWEST,
											 err_code);
	
	APP_ERROR_CHECK(err_code);
	
}
