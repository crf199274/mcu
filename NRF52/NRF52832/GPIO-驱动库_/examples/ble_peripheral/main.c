#include "nrf52.h"
#include "nrf_gpio.h"
#include "led.h"
#include "nrf_delay.h"

int main()
{
	LED_Init();
	
	while(1)
	{
		NRF_GPIOTE->TASKS_OUT[0]=1;//由config寄存器里的极性配置觉得输出的信号
		NRF_GPIOTE->TASKS_OUT[1]=1;
		nrf_delay_ms(500);
	}
}
