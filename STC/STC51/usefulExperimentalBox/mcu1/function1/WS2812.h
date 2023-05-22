#ifndef __WS2812_H
#define __WS2812_H

#include "stc8a8k64d4.h"
#include "intrins.h"

sbit WS2812B = P4^4;

typedef struct{
	
	unsigned char R;
	
	unsigned char G;
	
	unsigned char B;
	
	
}WsColor;

void ws2812b_INIT(void);
void ws2812bWriteAColor(WsColor wsColor);
void LightCube_Reset(void);

#endif
