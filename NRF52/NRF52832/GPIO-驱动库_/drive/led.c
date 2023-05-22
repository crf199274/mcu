#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "led.h"

void LED_Init(void)
{
	
	NVIC_EnableIRQ(GPIOTE_IRQn);//中断嵌套设置
	
	NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)//设置极性，翻转
                           | (LED_1 << GPIOTE_CONFIG_PSEL_Pos)  //绑定管脚
                           | (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);//设置模式
	
	NRF_GPIOTE->CONFIG[1] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)//输出低电平
                           | (LED_2<< GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);////配置任务输出状态、绑定通道、任务模式（详细说明请参看青风教程）
	
}
