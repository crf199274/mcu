#include "adc.h"
#include "oled.h"
#include "delay.h"
#include "intrins.h"

uint8_t ch = 0;

uint16_t AD_Value = 0;

/*----------------------------
初始化ADC
----------------------------*/
void ADC_Config()
{
	P1M0 = 0x00;                                //设置P1.0为ADC口
	P1M1 = 0x0f;

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

void adcInit(void)
{
	ADC_Config();                      //初始化ADC
	Delay1ms();
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
		VOUT = AD_Value*VOUT_K;
	else if(ch==1)
		VIN = AD_Value*VIN_K;
	else if(ch==2)
		BAT_I = AD_Value*BAT_I_K;
	else
		VOUT_I = AD_Value*VOUT_I_K;

	if(++ch>3)
		ch = 0;
	
	ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | ch;    //启动 AD 转换

    
}