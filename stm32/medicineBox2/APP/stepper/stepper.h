#ifndef __STEPPER_H
#define __STEPPER_H

#include "stm32f10x.h"                  // Device header

#define STEPPER_TIM_GPIO_CLK  	RCC_APB2Periph_GPIOA
#define STEPPER_TIM_GPIO_PORT 	GPIOA
#define STEPPER_TIM_GPIO_PIN		GPIO_Pin_6

#define STEPPER_TIM_CLK  				RCC_APB1Periph_TIM3
#define STEPPER_TIMx						TIM3

#define ENABLE_Pin 							GPIO_Pin_9
#define ENABLE_GPIO_Port 				GPIOC
#define DIR_Pin 								GPIO_Pin_8
#define DIR_GPIO_Port 					GPIOC
#define MS2_Pin 								GPIO_Pin_12
#define MS2_GPIO_Port 					GPIOC
#define MS3_Pin 								GPIO_Pin_11
#define MS3_GPIO_Port 					GPIOC
#define MS1_Pin 								GPIO_Pin_10
#define MS1_GPIO_Port 					GPIOC

#define MS2_HIGHT								GPIO_SetBits(MS2_GPIO_Port,MS2_Pin)
#define MS1_HIGHT								GPIO_SetBits(MS1_GPIO_Port,MS1_Pin)
#define MS3_HIGHT								GPIO_SetBits(MS3_GPIO_Port,MS3_Pin)
#define MS2_LOW									GPIO_ResetBits(MS2_GPIO_Port,MS2_Pin)

#define DIR_CW									GPIO_SetBits(DIR_GPIO_Port,DIR_Pin)
#define DIR_CCW									GPIO_ResetBits(DIR_GPIO_Port,DIR_Pin)

#define A4988_ENABLE						GPIO_ResetBits(ENABLE_GPIO_Port,ENABLE_Pin)
#define A4988_DISABLE						GPIO_SetBits(ENABLE_GPIO_Port,ENABLE_Pin)

void STEPPER_TIMx_GPIO_Config(void); 
void STEPPER_TIMx_Mode_Config(void);
void STEPPER_PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);

void STEPPER_GPIO_Config(void);
void STEPPER_GPIO_Init(void);

void STEPPER_Rotation( uint16_t angle);
void STEPPER_Stop(void);
void STEPPER_INIT(void);
#endif

