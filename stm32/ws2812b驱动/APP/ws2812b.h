#ifndef __WS2812B_H
#define __WS2812B_H

typedef unsigned char uint8_t;

//µÆ´ø³¤¶È
#define LED_Size			30

typedef struct{
	
	uint8_t R;
	
	uint8_t G;
	
	uint8_t B;
	
	
}WsColor;

void ws2812bInit(void);
void ws2812bReset(void);
void ws2812bWriteAColorData(WsColor wsColor);

#endif
