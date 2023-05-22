#include "pwm.h"

void pwm_init(u16 per,u16 pers)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//打开定时器时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用时钟
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//复用映射
	//io口配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	//定时器参数配置
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_my;
	TIM_TimeBase_my.TIM_Period=per;
	TIM_TimeBase_my.TIM_Prescaler=pers;
	TIM_TimeBase_my.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBase_my.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBase_my);
	//pwm配置
	TIM_OCInitTypeDef  TIM_OCInit_my;
	TIM_OCInit_my.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInit_my.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInit_my.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OC1Init(TIM3,&TIM_OCInit_my);
	//使能
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}
