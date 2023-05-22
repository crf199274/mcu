#include "nrf52.h"
#include "nrf_gpio.h"
#include "led.h"
#include "nrf_delay.h"

int main()
{
	LED_Init();
	
	while(1)
	{
		nrf_drv_gpiote_out_task_trigger(LED_3);
		nrf_drv_gpiote_out_task_trigger(LED_2);
		nrf_delay_ms(500);
	}
}
