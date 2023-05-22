#ifndef __DC_DC_H
#define __DC_DC_H

#include "stc15.h"
#include "typedef.h"

#define			VOL_LIMIT_MAX		15.25
#define			VOL_LIMIT_MIN		4.05	

#define			VOL_MAX					15							//用户限制电压最大
#define			VOL_MIN					5								//用户限制电压最小

#define     MAIN_Fosc       24000000UL      //定义主时钟

#define     PWM_DUTY        10000            //定义PWM的周期，数值为时钟周期数，假如使用24.576MHZ的主频，则PWM频率为6000HZ。

#define     PWM_HIGH_MIN    32              //限制PWM输出的最小占空比。用户请勿修改。
#define     PWM_HIGH_MAX    (PWM_DUTY-PWM_HIGH_MIN) //限制PWM输出的最大占空比。用户请勿修改

extern uint16_t pwm;

extern float VOL;

void LoadPWM(uint16_t i);
void DC_DC_PWM_Init(void);
uint16_t pwm_map(float occ);
uint16_t VOL_OCC(float vol);
	

#endif
