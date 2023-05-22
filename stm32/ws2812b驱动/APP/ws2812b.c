#include "stm32f10x.h"
#include "stdalay.h"
#include "ws2812b.h"

void ws2812bIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_Init_my);
	
	//PE^0����ߵ�ƽ
	GPIOE->BSRR = 1;
	
}

//@ws2812bReset
/*
function :
ÿһ֡���ݴ�����ɺ���һ�θ�λ�źſ�������ʹws2812b������ɫ��
argument : null
*/
void ws2812bReset(void)
{
	//PE^0����͵�ƽ
	GPIOE->BRR = 1;
	dalay_us(400);
}

//@ws2812bReset
/*
function :
дһ���ֽڵ���ɫ���ݡ�
argument : color - ��ɫ����
*/
void ws2812bWriteByte(uint8_t color)
{
	uint8_t i = 0;
	
	for(i=0;i<8;i++)
	{
		if(color&0x80)
		{
			//PE^0����ߵ�ƽ
			GPIOE->BSRR = 1;
			//13.8ns*85=1175ns
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();
			//PE^0����͵�ƽ
			GPIOE->BRR = 1;
			//13.8ns*23=320ns
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		}else{
			//PE^0����ߵ�ƽ
			GPIOE->BSRR = 1;
			//13.8ns*23=320ns
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			//PE^0����͵�ƽ
			GPIOE->BRR = 1;
			//13.8ns*85=1175ns
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();
		}
		
		color <<= 1;
		
	}
	
}

//@ws2812bReset
/*
function :
дһ����ɫ���ݡ�
argument : wsColor - ��ɫ���ݽṹ��
*/
void ws2812bWriteAColorData(WsColor wsColor)
{
	ws2812bWriteByte(wsColor.G);
	ws2812bWriteByte(wsColor.R);
	ws2812bWriteByte(wsColor.B);
}

//@ws2812bReset
/*
function :
��ʼ��ws2812b
argument : null
*/
void ws2812bInit(void)
{
	uint8_t i = 0;
	WsColor wsColor = {0x00,0x00,0x00};
	
	ws2812bIO_Init();
	
	for(i=0;i<LED_Size;i++)
	{
		ws2812bWriteAColorData(wsColor);
	}
	ws2812bReset();
	
}
