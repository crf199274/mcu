#include "medicineBoxPosition.h"

/*************************************************
函数名: medicineBoxPositionIO_INIT
功能: 药仓位置判断IO初始化
入口参数: 无
返回值: 无
*************************************************/
void medicineBoxPositionIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_Init_my;
	
	RCC_APB2PeriphClockCmd(medicineBoxPositionIOClock,ENABLE);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init_my.GPIO_Pin=medicineBoxPositionIO;
	GPIO_Init(medicineBoxPositionIOPort,&GPIO_Init_my);
	
}

/*************************************************
函数名: medicineBoxPositionStaus
功能: 读取药仓位置Staus
入口参数: 无
返回值: Staus 0-归位 1-未归位
*************************************************/
uint8_t medicineBoxPositionStaus(void)
{
	return GPIO_ReadInputDataBit(medicineBoxPositionIOPort,medicineBoxPositionIO);
}
