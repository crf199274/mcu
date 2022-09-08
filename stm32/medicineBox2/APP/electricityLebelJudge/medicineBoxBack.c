#include "medicineBoxBack.h"

/*************************************************
函数名: medicineBoxBackIO_INIT
功能: 取药判断IO初始化
入口参数: 无
返回值: 无
*************************************************/
void medicineBoxBackIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_Init_my;
	
	RCC_APB2PeriphClockCmd(medicineBoxBackIOClock,ENABLE);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init_my.GPIO_Pin=medicineBoxBackIO;
	GPIO_Init(medicineBoxBackIOPort,&GPIO_Init_my);
}

/*************************************************
函数名: fingerprintStaus
功能: 读取取药判断IOStaus
入口参数: 无
返回值: Staus 0-放回 1-未放回
*************************************************/
uint8_t medicineBoxBackStaus(void)
{
	return GPIO_ReadInputDataBit(medicineBoxBackIOPort,medicineBoxBackIO);
}
