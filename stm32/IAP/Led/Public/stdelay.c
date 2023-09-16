#include "stdelay.h"

static u8    delay_nus=0;
static u16  delay_nms=0;

void stdelay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	delay_nus=SYSCLK/8;
	delay_nms=(u16)(delay_nus)*1000;
}


void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*delay_nus;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0x00;
}


void stdelay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=nms*delay_nms;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0x00;
}
