#include "ws2812.h"

void Delay300us()		//@24.000MHz
{
	unsigned char i, j;

	i = 10;
	j = 87;
	do
	{
		while (--j);
	} while (--i);
}

void LightCube_Reset(void)
{
	Delay300us();
}

void WS2812_bit(bit data_bit)
{
	if(data_bit)
	{
		WS2812B=1;
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		WS2812B=0;	
	}
	else
	{
		WS2812B=1;
		_nop_();_nop_();_nop_();_nop_();
		WS2812B=0;
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();
	}
}

void WS2812_rand_color(unsigned char R,unsigned char G,unsigned char B)
{
	WS2812_bit(G&0X80);WS2812_bit(G&0X40);WS2812_bit(G&0X20);WS2812_bit(G&0X10);
	WS2812_bit(G&0X08);WS2812_bit(G&0X04);WS2812_bit(G&0X02);WS2812_bit(G&0X01);
	
	WS2812_bit(R&0X80);WS2812_bit(R&0X40);WS2812_bit(R&0X20);WS2812_bit(R&0X10);
	WS2812_bit(R&0X08);WS2812_bit(R&0X04);WS2812_bit(R&0X02);WS2812_bit(R&0X01);
	
	WS2812_bit(B&0X80);WS2812_bit(B&0X40);WS2812_bit(B&0X20);WS2812_bit(B&0X10);
	WS2812_bit(B&0X08);WS2812_bit(B&0X04);WS2812_bit(B&0X02);WS2812_bit(B&0X01);
}

void ws2812bWriteAColor(WsColor wsColor)
{
	WS2812_rand_color(wsColor.R,wsColor.G,wsColor.B);
}

void ws2812b_INIT(void)
{
	P4M0 |= 0x10;                                
	P4M1 |= 0x00;
	
	WS2812_rand_color(0,0,0);
	LightCube_Reset();
	
}