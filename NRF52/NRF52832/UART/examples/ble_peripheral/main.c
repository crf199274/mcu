#include "nrf_gpio.h"
#include "led.h"
#include "uart.h"
#include "nrf_delay.h"


int main()
{
	LED_Init();
	
	uartInit();
	
	while(1)
	{
		LED_OPEN_CLOSE(LED_3,LED_ON);
		nrf_delay_ms(500);
		LED_OPEN_CLOSE(LED_3,LED_OFF);
		nrf_delay_ms(500);
		printf(" 2020.08.1 За·з!\r\n");
	}
}
