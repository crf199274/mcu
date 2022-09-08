#include "userKey.h"

/*************************************************
函数名: userKeyINIT
功能: 用户按键IO初始化-----可拓展(通过电平持续时间，按键可以复用功能)
入口参数: 无
返回值: 无
*************************************************/
void userKeyINIT(void)
{
	GPIO_InitTypeDef  GPIO_Init_my;
	
	RCC_APB2PeriphClockCmd(userKeyIOClock,ENABLE);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init_my.GPIO_Pin=userKeyIO;
	GPIO_Init(userKeyIOPort,&GPIO_Init_my);

}

/*************************************************
函数名: userKeyStaus
功能: 用户按键Staus
入口参数: 无
返回值: Staus 0-按下 1-未按下
*************************************************/
uint8_t userKeyStaus(void)
{
	return GPIO_ReadInputDataBit(userKeyIOPort,userKeyIO);
}
	

