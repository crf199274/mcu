#include "userLed.h"

/*************************************************
函数名: LED_INIT
功能: LEDIO初始化
入口参数: 无
返回值: 无
*************************************************/
void LED_INIT(void)
{
	GPIO_InitTypeDef  GPIO_Init_my;
	
	RCC_APB2PeriphClockCmd(LED_R_IOClock|LED_B_IOClock,ENABLE);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=LED_R_IO;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_R_IOPort,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=LED_B_IO;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_B_IOPort,&GPIO_Init_my);
	
	LED_R_Staus(1);
	LED_B_Staus(0);
	
}

/*************************************************
函数名: LED_R_Staus
功能: _Bool staus 1-开灯 0-关灯
入口参数: 无
返回值: 无
*************************************************/
void LED_R_Staus(_Bool staus)
{
	if(staus)
	{
		GPIO_ResetBits(LED_R_IOPort,LED_R_IO);
	}else{
		GPIO_SetBits(LED_R_IOPort,LED_R_IO);
	}
}

/*************************************************
函数名: LED_B_Staus
功能: _Bool staus 1-开灯 0-关灯
入口参数: 无
返回值: 无
*************************************************/
void LED_B_Staus(_Bool staus)
{
	if(staus)
	{
		GPIO_ResetBits(LED_B_IOPort,LED_B_IO);
	}else{
		GPIO_SetBits(LED_B_IOPort,LED_B_IO);
	}
}
