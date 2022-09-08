#include "usart.h"
#include "stdio.h"

void my_usart_init(uint32_t BaudRate)
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
//	USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=3;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
}

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}
