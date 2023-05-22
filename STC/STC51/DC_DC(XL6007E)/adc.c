#include "adc.h"
#include "lcd.h"

uint8_t ch = 0;

int AD_Value = 0;

/*----------------------------
�����ʱ
----------------------------*/
void Delay_ADC(uint16_t n)
{
    uint16_t x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

/*----------------------------
��ʼ��ADC
----------------------------*/
void ADC_Config()
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
    Delay_ADC(2);                       //ADC�ϵ粢��ʱ
}

void adcInit(void)
{
	ADC_Config();                      //��ʼ��ADC
	IE = 0xa0;                      //ʹ��ADC�ж�
																	//��ʼADת��
	
}

/*----------------------------
ADC�жϷ������
----------------------------*/
void adc_isr() interrupt 5
{
    ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
		
		AD_Value = ADC_RES<<2;
		AD_Value = AD_Value|(ADC_RESL&0x03);
	
		if(ch==0)
			VOUT = AD_Value*(5/1024.0)*4;
		else
			VIN = AD_Value*(5/1024.0)*4;
	
		if(++ch>1)
			ch = 0;
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;

    
}
