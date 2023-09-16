#include "adc.h"

struct adc_msg adc_msg_1={0};

/*----------------------------
��ʼ��ADC
----------------------------*/
void ADC_Config()
{
	P1M0 = 0x00;                                //����P1.0ΪADC��
	P1M1 = 0x0f;

	P_SW2 |= 0x80;
	ADCTIM = 0x3f;
	ADCEXCFG = 0x06;
	P_SW2 &= 0x7f;
	EADC = 1;                                   //ʹ��ADC�ж�
	EA = 1;
	ADCCFG = 0x0f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16
	ADC_CONTR = 0x80;                           //ʹ��ADCģ��
	ADC_CONTR |= 0x40;                          //����ADת��

}

void adcInit(void)
{
	ADC_Config();                      //��ʼ��ADC
	Delay1ms();
}

/*----------------------------
ADC�жϷ������
----------------------------*/
void adc_isr() interrupt 5
{
	
  ADC_CONTR &= ~0x20;     //���ADC������־
		
	adc_msg_1.AD_Value = ADC_RES<<8;
	adc_msg_1.AD_Value = adc_msg_1.AD_Value|ADC_RESL;
	adc_msg_1.AD_Value = adc_msg_1.AD_Value>>4;
	
	if(adc_msg_1.ch==0)
		DC_DC_1.Vout = adc_msg_1.AD_Value*VOUT_K;
	else if(adc_msg_1.ch==1)
		DC_DC_1.Vin = adc_msg_1.AD_Value*VIN_K;
	else if(adc_msg_1.ch==2)
		DC_DC_1.Iin = adc_msg_1.AD_Value*IIN_K;
	else
		DC_DC_1.Iout = adc_msg_1.AD_Value*IOUT_K;

	if(++adc_msg_1.ch>3)
		adc_msg_1.ch = 0;
	
	ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | adc_msg_1.ch;    //���� AD ת��

    
}