#include "tim_interrupt.h"
#include "adc.h"
#include "stdio.h"

void tim_interrupt_init(u16 Period,u16 Prescaler)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//��ʱ��
	
//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//���ö�ʱ������	
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_my;
	TIM_TimeBase_my.TIM_Period=Period;
	TIM_TimeBase_my.TIM_Prescaler=Prescaler;
	TIM_TimeBase_my.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBase_my.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBase_my);
	
//���ö�ʱ�����ж�����
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

//����nvic
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=0;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
	
//�򿪶�ʱ��
  TIM_Cmd(TIM4,ENABLE);
}

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)
	{
		int num1=0;
	  float num2=0;
		num1=Get_ADC_Num(ADC_Channel_1,10);
		printf("adc_num:%d\r\n",num1);
		num2=(float)((3.3/4096)*num1);
		printf("voltage:%f V\r\n",num2);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}
