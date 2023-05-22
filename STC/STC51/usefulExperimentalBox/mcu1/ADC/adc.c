#include "adc.h"

typedef unsigned char	 	uint8_t;
typedef unsigned int	 	uint16_t;
typedef unsigned long		uint32_t;

unsigned char ch = 0;

unsigned int AD_Value = 0;

unsigned int SW_Freq = 0;
unsigned int lastSW_Freq = 0;
unsigned char falg1 = 0;

unsigned int M_Freq = 0;
unsigned int lastM_Freq = 0;
unsigned char falg2 = 0;

WsColor RGB1={0,0,0};
WsColor lastRGB1={0,0,0};
unsigned char falg3 = 0;
unsigned char falg4 = 0;
unsigned char falg5 = 0;

/*----------------------------
初始化ADC
----------------------------*/
void adcInit()
{
	P1M0 = 0x00;                                //设置P1.0为ADC口
	P1M1 = 0x1f;
	
	P_SW2 |= 0x80;
	ADCTIM = 0x3f;
	ADCEXCFG = 0x06;
	P_SW2 &= 0x7f;
	EADC = 1;                                   //使能ADC中断
	EA = 1;
	ADCCFG = 0x0f;                              //设置ADC时钟为系统时钟/2/16/16
	ADC_CONTR = 0x80;                           //使能ADC模块
	ADC_CONTR |= 0x40;                          //启动AD转换
	
}

/*----------------------------
ADC中断服务程序
----------------------------*/
void adc_isr() interrupt 5
{
	
  ADC_CONTR &= ~0x20;     //清除ADC结束标志
		
	AD_Value = ADC_RES<<8;
	AD_Value = AD_Value|ADC_RESL;
	AD_Value = AD_Value>>4;
	
	if(ch==0)
	{
		lastSW_Freq = SW_Freq;
		SW_Freq = ((AD_Value+1)/8)*50+50;
		if(SW_Freq!=lastSW_Freq)
			falg1 = 1;
	}
	else if(ch==1)
	{
		lastM_Freq = M_Freq;
		M_Freq = (int)((AD_Value+1)/40.5545);
		if(M_Freq!=lastM_Freq)
			falg2 = 1;
	}
	else if(ch==2)
	{
		lastRGB1.R = RGB1.R;
		RGB1.R = (AD_Value+1)/16;
		if(lastRGB1.R != RGB1.R)
			falg3 = 1;
	}
	else if(ch==3)
	{
		lastRGB1.G = RGB1.G;
		RGB1.G = (AD_Value+1)/16;
		if(lastRGB1.G != RGB1.G)
			falg4 = 1;
	}
	else
	{
		lastRGB1.B = RGB1.B;
		RGB1.B = (AD_Value+1)/16;
		if(lastRGB1.B != RGB1.B)
			falg5 = 1;
	}

	if(++ch>4)
		ch = 0;
	
	ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | ch;    //启动 AD 转换

    
}
