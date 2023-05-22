#include "time.h"
#include "OLED.h"
#include "typedef.h"

uint16_t time_cnt = 0;

void timeInit(void)//2����@24.000MHz
{
	AUXR |= 0x80;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x80;				//���ö�ʱ��ʼֵ
	TH0 = 0x44;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
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
