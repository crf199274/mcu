#include "stdalay.h"

static u8    dalay_nus=0;
static u16  dalay_nms=0;

void stdalay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	dalay_nus=SYSCLK/8;
	dalay_nms=(u16)(dalay_nus)*1000;
}


void dalay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*dalay_nus;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0x00;
}


void stdalay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=nms*dalay_nms;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL=0x00;
}
