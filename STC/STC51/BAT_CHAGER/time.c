#include "time.h"
#include "OLED.h"
#include "typedef.h"

uint16_t time_cnt = 0;

void timeInit(void)//2毫秒@24.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x80;				//设置定时初始值
	TH0 = 0x44;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

void TM0_Isr() interrupt 1
{
	if(++time_cnt>=500)
	{
		time_cnt=0;
		
    if(T1)
		{
			if(++MIN1>60000)
				MIN1=0;
		}
		
		if(T2)
		{
			if(++MIN2>60000)
				MIN2=0;
		}
		
		if(T3)
		{
			if(++MIN3>60000)
				MIN3=0;
		}
	}
}
