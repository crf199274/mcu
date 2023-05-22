#ifndef __DC_DC_H
#define __DC_DC_H

#include "stc8a8k64d4.h"
#include "typedef.h"

#define			VREF						1.25														//单位V
#define			R1							300.0														//单位K
#define			R2							13.0														//单位K
#define			R3							40.0														//R3+R4 单位K
#define			VMAX						5.0															//单位V
#define			VMIN						0.0															//单位V

#define			VOL_LIMIT_MAX		(VREF*(1+R1/R2)+(VREF-VMIN)*(R1/R3))
#define			VOL_LIMIT_MIN		(VREF*(1+R1/R2)+(VREF-VMAX)*(R1/R3))	

#define			VOL_MAX					(int)VOL_LIMIT_MAX							//用户限制电压最大
#define			VOL_MIN					5																//用户限制电压最小

#define CYCLE           2400

extern uint16_t pwm;
extern int pwm_error;

extern float VOL;

void LoadPWM(uint16_t i);
void DC_DC_PWM_Init(void);
uint16_t pwm_map(float occ);
uint16_t VOL_OCC(float vol);
	

#endif
