#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"

sbit DO = P1^3;

typedef struct{
	
	unsigned char R;
	
	unsigned char G;
	
	unsigned char B;
	
	
}WsColor;

void ws2812b_INIT(void);
void ws2812bWriteAColor(WsColor wsColor);
void LightCube_Reset(void);

#endif
