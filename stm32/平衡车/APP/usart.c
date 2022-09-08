#include "usart.h"
#include "stdio.h"

u8 r=0;

void my_usart_init(u16 BaudRate)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_10;
//	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	
	USART_InitTypeDef  USART_Init_my;
	USART_Init_my.USART_BaudRate=BaudRate;
	USART_Init_my.USART_WordLength=USART_WordLength_8b;
	USART_Init_my.USART_StopBits=USART_StopBits_1;
	USART_Init_my.USART_Parity=USART_Parity_No;
	USART_Init_my.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init_my.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_Init_my);
	
	USART_Cmd(USART1,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_TC,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=3;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==1)
	{
		r=USART_ReceiveData(USART1);
		USART_SendData(USART1,r);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
	return ch;
}
