#include "usart.h"
#include "stdio.h"

int 	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0; //蓝牙遥控相关的变量

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	 //取出int型变量的低字节
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 //	取存储在此变量下一内存字节的内容，高字节
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

void usart_init(uint32_t BaudRate)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_9;//TX
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_10;//RX
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=3;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
}

void usartSendString(uint8_t * data,uint8_t len)
{
	for(int i=0;i<len;i++)
	{
		USART_SendData(USART1,*data++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);
	}
}

void usartSendSpeedDataToANO(uint16_t Speed_L,uint16_t Speed_R)
{
	uint8_t sum=0;
	uint8_t buf[9]={0xAA,0xAA,0xF1,0x04};
	
	buf[4]=BYTE1(Speed_L);
	buf[5]=BYTE0(Speed_L);
	
	buf[6]=BYTE1(Speed_R);
	buf[7]=BYTE0(Speed_R);
	
	for(int i=0;i<8;i++)
	{
		sum += buf[i];
	}
	
	buf[8]=sum;
	
	usartSendString(buf,9);
	
}

void usartSendMPU6050DataToANO(float pitch,float gyroy)
{
	uint8_t sum=0;
	uint8_t buf[13]={0xAA,0xAA,0xF1,0x08};
	
	buf[4]=BYTE3(pitch);
	buf[5]=BYTE2(pitch);
	buf[6]=BYTE1(pitch);
	buf[7]=BYTE0(pitch);
	
	buf[8]=BYTE3(gyroy);
	buf[9]=BYTE2(gyroy);
	buf[10]=BYTE1(gyroy);
	buf[11]=BYTE0(gyroy);
	
	for(int i=0;i<12;i++)
	{
		sum += buf[i];
	}
	
	buf[12]=sum;
	
	usartSendString(buf,13);
	
}

void USART1_IRQHandler()
{
	static uint8_t uart_receive = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==1)
	{
		uart_receive=USART_ReceiveData(USART1);
//		USART_SendData(USART1,uart_receive);
		if(uart_receive==0x00)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
		else if(uart_receive==0x01)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
		else if(uart_receive==0x05)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
		else if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
													Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
		else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	    //右
													Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
		else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;		//////////////刹车
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
