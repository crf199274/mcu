#include "DC_DC.h"
#include "delay.h"
#include "oled.h"

uint16_t     pwm;                //����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����

float VOL = VOL_MIN;

int pwm_error = 0;

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
		return 0;
	else if(occ<=0)
		return CYCLE;
	else
		return CYCLE*occ;
		
}

void LoadPWM(uint16_t i)
{
	if(i>CYCLE)
		i = CYCLE;
	
	P_SW2=0X80;
	
  PWMCFG = 0x00;					//�ر�PWMģ��
	
	PWM0T1 = i;
	PWM0T2 = 0x0000;
	
	PWMCFG = 0x01;					//����PWMģ��
	
}


void DC_DC_PWM_Init(void)
{
	PWMSET = 0x01;					//ʹ��PWMģ��(������ʹ��ģ���������ò���Ч)
	
	P_SW2 = 0x80;
	PWMCKS = 0x00;					//PWMʱ��Ϊϵͳʱ��
	PWMC = CYCLE;						//����PWM����Ϊ1000H ��PWMʱ��
	PWM0T1 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM0 ͨ������͵�ƽ
	PWM0T2 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM0 ͨ������ߵ�ƽ
	PWM0CR = 0x80;					//ʹ��PWM0 ���
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//����PWMģ��

}
