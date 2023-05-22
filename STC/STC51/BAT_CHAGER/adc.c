#include "adc.h"
#include "oled.h"
#include "stc15.h"

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
			IOUT3 = AD_Value*IOUT3_K;
		else if(ch==1)
			IOUT1 = AD_Value*IOUT1_K;
		else if(ch==2)
			IOUT2 = AD_Value*IOUT2_K;
		else if(ch==3)
			VOUT2 = AD_Value*VOUT2_K;
		else if(ch==4)
			VOUT3 = AD_Value*VOUT3_K;
		else
			VOUT1 = AD_Value*VOUT1_K;
	
		if(++ch>5)
			ch = 0;
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;

    
}
