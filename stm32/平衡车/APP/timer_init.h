#ifndef  __timer_init_H
#define __timer_init_H

#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   // number of pulses per revolution

void Timer1_Init(int per,int pers);
void NVIC_Config(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(char num);
void MOTO_PWM_Init(u32 arr, int psc);
void MOTO_PWM_Out(u16 moto_l, u16 moto_r);

#endif
