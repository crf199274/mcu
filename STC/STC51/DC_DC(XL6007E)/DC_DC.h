#ifndef __DC_DC_H
#define __DC_DC_H

#include "stc15.h"
#include "typedef.h"

#define			VOL_LIMIT_MAX		15.25
#define			VOL_LIMIT_MIN		4.05	

#define			VOL_MAX					15							//�û����Ƶ�ѹ���
#define			VOL_MIN					5								//�û����Ƶ�ѹ��С

#define     MAIN_Fosc       24000000UL      //������ʱ��

#define     PWM_DUTY        10000            //����PWM�����ڣ���ֵΪʱ��������������ʹ��24.576MHZ����Ƶ����PWMƵ��Ϊ6000HZ��

#define     PWM_HIGH_MIN    32              //����PWM�������Сռ�ձȡ��û������޸ġ�
#define     PWM_HIGH_MAX    (PWM_DUTY-PWM_HIGH_MIN) //����PWM��������ռ�ձȡ��û������޸�

extern uint16_t pwm;

extern float VOL;

void LoadPWM(uint16_t i);
void DC_DC_PWM_Init(void);
uint16_t pwm_map(float occ);
uint16_t VOL_OCC(float vol);
	

#endif
