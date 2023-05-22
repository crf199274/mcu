#include "stc8.h"

sbit LED_R=P3^2;
sbit LED_G=P3^3;
sbit LED_B=P5^4;
sbit control=P5^5;

static int time = 0;
static int change = 0;
int flag = 10;
char mode = 0;

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void change_exe()
{
	switch(flag)
	{
		case 10:
			flag = 20;
			LED_R=1;
			LED_G=0;
			LED_B=0;
			break;
		case 20:
			flag = 30;
			LED_R=0;
			LED_G=1;
			LED_B=0;
			break;
		case 30:
			flag = 60;
			LED_R=0;
			LED_G=0;
			LED_B=1;
			break;
		case 60:
			flag = 70;
			LED_R=1;
			LED_G=1;
			LED_B=0;
			break;
		case 70:
			flag = 80;
			LED_R=1;
			LED_G=0;
			LED_B=1;
			break;
		case 80:
			flag = 10;
			LED_R=0;
			LED_G=1;
			LED_B=1;
			break;
	}
	
}

void control_exe(int contr)
{
	if(mode)
	{
		LED_R=0;
		LED_G=0;
		LED_B=0;
	}
	
	
	if(contr>=8&&contr<=12)//time:10-红
	{
		LED_R=1;
		LED_G=0;
		LED_B=0;
	}
	
	if(contr>=18&&contr<=22)//time:20-绿
	{
		LED_R=0;
		LED_G=1;
		LED_B=0;
	}
	
	if(contr>=28&&contr<=32)//time:30-蓝
	{
		LED_R=0;
		LED_G=0;
		LED_B=1;
	}
	
	if(contr>=38&&contr<=42)//time:40-关
	{
		LED_R=0;
		LED_G=0;
		LED_B=0;
	}
	
	if(contr>=48&&contr<=52)//time:50-开-白
	{
		LED_R=1;
		LED_G=1;
		LED_B=1;
	}
	
	if(contr>=58&&contr<=62)//time:60-黄
	{
		LED_R=1;
		LED_G=1;
		LED_B=0;
	}
	
	if(contr>=68&&contr<=72)//time:70-紫
	{
		LED_R=1;
		LED_G=0;
		LED_B=1;
	}
	
	if(contr>=78&&contr<=82)//time:80-青
	{
		LED_R=0;
		LED_G=1;
		LED_B=1;
	}
	
	if(contr>=88&&contr<=92)//time:90-流水灯
	{
		mode = 1;
		change=0;
	}
	
	if(contr<88||contr>92)
	{
		mode = 0;
	}
	
	time = 0;
	
}

void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初始值
	TH0 = 0xFB;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		
	ET0 = 1;                                    //使能定时器中断
	EA = 1;
}

void main()
{
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	LED_R=0;
	LED_G=0;
	LED_B=0;
	
	Timer0Init();
	
	while(1)
	{
		if(mode)
		{
			if(change==0)
				change_exe();
			
			Delay1ms();
			
			change++;
			
			if(change>=500)
				change = 0;
			
		}
		
		if(control==1)
		{
			TR0 = 1;		//定时器0开始计时
			while(control);
			TR0 = 0;
			control_exe(time);
		}
		
	}
}

void TM0_Isr() interrupt 1
{
  time++;
}
