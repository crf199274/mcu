#ifndef __DC_DC_H
#define __DC_DC_H

#include "stc8a8k64d4.h"
#include "typedef.h"
#include "delay.h"

struct DC_DC{
	
	/* 保存pwm的值 */
	uint16_t pwm;
	/* 保存pwm的误差值 */
	int pwm_error;
	/* 记录当前的电压值 */
	float vol;
	/* 输入电压 */
	float Vin;
	/* 输出电压 */
	float Vout;
	/* 输入电流 */
	float Iin;
	/* 输出电流 */
	float Iout;
	
};

extern struct DC_DC DC_DC_1;

#define			VREF						1.25														//单位V
#define			R1							300.0														//单位K
#define			R2							13.0														//单位K
#define			R3							40.0														//R3+R4 单位K
#define			VMAX						5.0															//单位V
#define			VMIN						0.0															//单位V

#define			VOL_LIMIT_MAX		(VREF*(1+R1/R2)+(VREF-VMIN)*(R1/R3))	//理论最大值
#define			VOL_LIMIT_MIN		(VREF*(1+R1/R2)+(VREF-VMAX)*(R1/R3))	//理论最小值

#define			VOL_MAX					(int)VOL_LIMIT_MAX							//用户限制电压最大
#define			VOL_MIN					5																//用户限制电压最小

#define CYCLE           2400

void LoadPWM(uint16_t pwm_value);
void DC_DC_PWM_Init(void);
uint16_t pwm_map(float occ);
uint16_t voltageToPWM(float vol);
	

#endif
