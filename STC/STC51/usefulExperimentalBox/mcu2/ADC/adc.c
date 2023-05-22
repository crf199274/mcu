#include "adc.h"

unsigned char ch = 0;

unsigned int AD_Value = 0;

float VOUT = 0;
float IOUT = 0;
float BAT = 0;

void adcInit()
{
	P1M0 = 0x00;                                //����P1.0ΪADC��
	P1M1 = 0x02;
	P3M0 = 0x00;
	P3M1 = 0x04;
	P_SW2 |= 0x80;
	ADCTIM = 0x3f;                              //����ADC�ڲ�ʱ��
	ADCEXCFG |=0x06; 
	P_SW2 &= 0x7f;
	ADCCFG = 0x0f;                              //����ADCʱ��Ϊϵͳʱ��/2/16
	ADC_CONTR = 0x80;                           //ʹ��ADCģ��
	EADC = 1;                                   //ʹ��ADC�ж�
	ADC_CONTR |= 0x40;                          //����ADת��
	
}

void ADC_Isr() interrupt 5
{
	
  ADC_CONTR &= ~0x20;                         //���жϱ�־
    
	AD_Value = ADC_RES<<8;
	AD_Value = AD_Value|ADC_RESL;
	AD_Value = AD_Value>>6;
	
	switch(ch)
	{
		case 0:VOUT = AD_Value*VOUT_K-0.21;ch = 1;break;
		case 1:IOUT = (AD_Value*VOUT_I_K)*1000;ch = 10;break;
		case 10:BAT = AD_Value*VIN_K+0.21;ch = 0;break;
	}
	
  ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | ch;    //���� AD ת��
	
}
