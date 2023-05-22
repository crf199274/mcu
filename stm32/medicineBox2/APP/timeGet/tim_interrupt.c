#include "tim_interrupt.h"

/*************************************************
函数名: TIM4_init
功能: 定时
入口参数: uint16_t Period 计数周期 		uint16_t Prescaler  时钟分频   公式 溢出时间:Tout = Period*Prescaler/Tclk ;
返回值: 无
*************************************************/
void TIM4_init(uint16_t Period,uint16_t Prescaler)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//打开时钟
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//配置定时器内容	
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_my;
	TIM_TimeBase_my.TIM_Period=Period;
	TIM_TimeBase_my.TIM_Prescaler=Prescaler-1;
	TIM_TimeBase_my.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBase_my.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBase_my);
	
//配置定时器的中断类型
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

//配置nvic
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=1;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
	
//打开定时器
//  TIM_Cmd(TIM4,ENABLE);
}

/*************************************************
函数名: TIM2_init
功能: 定时
入口参数: uint16_t Period 计数周期 		uint16_t Prescaler  时钟分频   公式 溢出时间:Tout = Period*Prescaler/Tclk ;
返回值: 无
*************************************************/
void TIM2_init(uint16_t Period,uint16_t Prescaler)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//打开时钟
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//配置定时器内容	
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_my;
	TIM_TimeBase_my.TIM_Period=Period;
	TIM_TimeBase_my.TIM_Prescaler=Prescaler;
	TIM_TimeBase_my.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBase_my.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBase_my);
	
//配置定时器的中断类型
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

//配置nvic
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=2;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
	
//打开定时器
  TIM_Cmd(TIM2,ENABLE);
}

/*************************************************
函数名: TIM3_init
功能: 定时
入口参数: uint16_t Period 计数周期 		uint16_t Prescaler  时钟分频   公式 溢出时间:Tout = Period*Prescaler/Tclk ;
返回值: 无
*************************************************/
void PWM_init(uint16_t Period,uint16_t Prescaler)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//打开时钟
	
	//io配置
	RCC_APB2PeriphClockCmd(motorPWM_PortClock,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=motorPWM_Pin;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(motorPWM_GPIO_Port,&GPIO_Init_my);
	
//配置定时器内容	
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_my;
	TIM_TimeBase_my.TIM_Period=Period;
	TIM_TimeBase_my.TIM_Prescaler=Prescaler;
	TIM_TimeBase_my.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBase_my.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBase_my);
	
	//pwm配置
	TIM_OCInitTypeDef  TIM_OCInit_my;
	TIM_OCInit_my.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInit_my.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInit_my.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInit_my.TIM_Pulse = Period/2;														//CCR设置	
	TIM_OC1Init(TIM3,&TIM_OCInit_my);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	//打开定时器
  TIM_Cmd(TIM3,ENABLE);
	//输出PWM
	TIM_ARRPreloadConfig(TIM3,ENABLE);
}
