#include "led.h"
#include "uart.h"
#include "nrf_delay.h"
#include "stdio.h"
#include "oled.h"


int main()
{
	
	LED_Init();
	
	uartInit();
	
	twi_master_init();
	
//	nrf_delay_ms(2000);
	
	oled_init();
	
	oled_show_line(" 2020.08.1 !",1,8);
	
	oled_show_line(" 2020.08.1 !",2,16);
	
	while(1)
	{
		LED_OPEN_CLOSE(LED_3,LED_ON);
		nrf_delay_ms(500);
		LED_OPEN_CLOSE(LED_3,LED_OFF);
		nrf_delay_ms(500);
		printf(" 2020.08.1 За·з!\r\n");
	}
}
