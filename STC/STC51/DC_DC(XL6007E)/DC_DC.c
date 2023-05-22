#include "DC_DC.h"
#include "delay.h"
#include "lcd.h"

uint16_t     pwm;                //����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����

sbit    P_PWM = P3^5;       //����PWM������š�

uint16_t     PWM_high,PWM_low;   //�м�������û������޸ġ�

float VOL = VOL_MIN;

uint16_t VOL_OCC(float vol)
{
	if(vol<VOL_MIN||vol>VOL_MAX)
		return pwm;
	
	return pwm_map(1.0-(vol-VOL_LIMIT_MIN)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN));
}


//ooc>=0 | occ<=1
uint16_t pwm_map(float occ)
{
	if(occ>=1)
		return PWM_HIGH_MIN;
	else if(occ<=0)
		return PWM_HIGH_MAX;
	else
		return PWM_HIGH_MAX*occ;
		
}

void LoadPWM(uint16_t i)
{
    uint16_t j;

    if(i > PWM_HIGH_MAX)        i = PWM_HIGH_MAX;   //���д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
    if(i < PWM_HIGH_MIN)        i = PWM_HIGH_MIN;   //���д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
    j = 65536UL - PWM_DUTY + i; //����PWM�͵�ƽʱ��
    i = 65536UL - i;            //����PWM�ߵ�ƽʱ��
    EA = 0;
    PWM_high = i;   //װ��PWM�ߵ�ƽʱ��
    PWM_low  = j;   //װ��PWM�͵�ƽʱ��
    EA = 1;
}


void DC_DC_PWM_Init(void)
{
	P_PWM = 0;
	P3M1 &= ~(1 << 5);  //P3.5 ����Ϊ�������
	P3M0 |=  (1 << 5);

	
	TR0 = 0;        //ֹͣ����
	ET0 = 1;        //�����ж�
	PT0 = 1;        //�����ȼ��ж�
	TMOD &= ~0x03;  //����ģʽ,0: 16λ�Զ���װ
	AUXR |=  0x80;  //1T
	TMOD &= ~0x04;  //��ʱ
	INT_CLKO |=  0x01;  //���ʱ��

	
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;    //��ʼ����
	
	pwm = VOL_OCC(VOL);
	LoadPWM(pwm);

}

void timer0_int (void) interrupt 1
{
    if(P_PWM)
    {
        TH0 = (uint8_t)(PWM_low >> 8);   //���������ߵ�ƽ����װ�ص͵�ƽʱ�䡣
        TL0 = (uint8_t)PWM_low;
    }
    else
    {
        TH0 = (uint8_t)(PWM_high >> 8);  //���������͵�ƽ����װ�ظߵ�ƽʱ�䡣
        TL0 = (uint8_t)PWM_high;
    }
}
