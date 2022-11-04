#include "ws2812.h"

void ws2812_delay100us()       //@24.000MHz
{
    unsigned char i, j;

    i = 3;
    j = 82;
    do
    {
        while (--j);
    } while (--i);
}

void LightCube_Reset(void)
{
	DO = 0;
	ws2812_delay100us();
	ws2812_delay100us();
	ws2812_delay100us();
}

void WS2812_bit(bit data_bit)
{
	if(data_bit)
	{
		DO=1;
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		DO=0;	
	}
	else
	{
		DO=1;
		_nop_();_nop_();_nop_();_nop_();
		DO=0;
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
	P1M0 |= 0x00;                                //ÉèÖÃP1.0ÎªADC¿Ú
	P1M1 |= 0x01;
	LightCube_Reset();
}