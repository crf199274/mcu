#include "DC_DC.h"

struct DC_DC DC_DC_1 = {0,0,VOL_MIN,0,0,0,0};

/* ���ѹֵ��Ӧ��PWMֵ */
/* arg 		vol			:voltage_value
/* return		pwm_value
*/
uint16_t voltageToPWM(float vol)
{
	if(vol<VOL_MIN||vol>VOL_MAX)
		return DC_DC_1.pwm;
	
	return pwm_map(1.0-(vol-VOL_LIMIT_MIN)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN));
}


/* ����ռ�ձȼ���pwm��ֵ */
/* arg occ :ռ�ձ� */
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

/* ����PWM */
/* brief 		����pwm_value����ʱ���ص�Ӳ���� */
/* arg		pwm_value 		:pwm��ֵ */
void LoadPWM(uint16_t pwm_value)
{
	if(pwm_value>CYCLE)
		pwm_value = CYCLE;
	
	P_SW2=0X80;
	
  PWMCFG = 0x00;					//�ر�PWMģ��
	
	PWM0T1 = pwm_value;
	PWM0T2 = 0x0000;
	
	PWMCFG = 0x01;					//����PWMģ��
	
}


/* PWM��ʼ��*/
/* brief		Ϊ����XL6007E1�ṩһ·PWM */
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
