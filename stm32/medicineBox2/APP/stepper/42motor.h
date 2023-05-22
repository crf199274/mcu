#ifndef __42Motor_H
#define __42Motor_H

#include "stm32f10x.h"

//定义IO和时钟
#define motorClock								RCC_APB2Periph_GPIOC
#define motorGPIO_Port						GPIOC
#define motorPin_DIR							GPIO_Pin_8
//#define motorPin_SLEEP						GPIO_Pin_1
//#define motorPin_RESET						GPIO_Pin_2
#define motorPin_MS1							GPIO_Pin_10
#define motorPin_MS2							GPIO_Pin_11
#define motorPin_MS3							GPIO_Pin_12
#define motorPin_ENABLE						GPIO_Pin_9


void _42motorInit(_Bool MS1,_Bool MS2,_Bool MS3);
uint8_t step90T(uint8_t step);
uint8_t step90F(uint8_t step);

#endif
