#ifndef __DC_DC_H
#define __DC_DC_H

#include "stc8a8k64d4.h"
#include "typedef.h"
#include "delay.h"

struct DC_DC{
	
	/* ����pwm��ֵ */
	uint16_t pwm;
	/* ����pwm�����ֵ */
	int pwm_error;
	/* ��¼��ǰ�ĵ�ѹֵ */
	float vol;
	/* �����ѹ */
	float Vin;
	/* �����ѹ */
	float Vout;
	/* ������� */
	float Iin;
	/* ������� */
	float Iout;
	
};

extern struct DC_DC DC_DC_1;

#define			VREF						1.25														//��λV
#define			R1							300.0														//��λK
#define			R2							13.0														//��λK
#define			R3							40.0														//R3+R4 ��λK
#define			VMAX						5.0															//��λV
#define			VMIN						0.0															//��λV

#define			VOL_LIMIT_MAX		(VREF*(1+R1/R2)+(VREF-VMIN)*(R1/R3))	//�������ֵ
#define			VOL_LIMIT_MIN		(VREF*(1+R1/R2)+(VREF-VMAX)*(R1/R3))	//������Сֵ

#define			VOL_MAX					(int)VOL_LIMIT_MAX							//�û����Ƶ�ѹ���
#define			VOL_MIN					5																//�û����Ƶ�ѹ��С

#define CYCLE           2400

void LoadPWM(uint16_t pwm_value);
void DC_DC_PWM_Init(void);
uint16_t pwm_map(float occ);
uint16_t voltageToPWM(float vol);
	

#endif
