#ifndef __GUI_H
#define __GUI_H

#include "stm32f10x.h"

#define GUI_ERROR_INFO 0
#define GUI_OK_INFO 1

//字体大小(12,16,24,32,48,64)
#define INFO					12
#define INFO2         12

typedef enum{
	
	GUI_OK =0,
	GUI_NOT_MOUNT,
	GUI_NOT_OPEN,
	GUI_NOT_SEEK,
	GUI_NOT_READ,
	
}GUI_ERROR;


GUI_ERROR GUI_Backgroud(uint8_t *s);
GUI_ERROR GUI_WIFI_LOGO(uint8_t *s);
GUI_ERROR ONENET_LOGO(uint8_t *s);
void GUI_realtime(uint8_t *date,uint8_t *time);
uint8_t GUI_ShowUC_12x12(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUC_16x16(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUC_24x24(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUC_32x32(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUC_48x48(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUC_64x64(uint32_t unicode,uint16_t x,uint16_t y);
uint8_t GUI_ShowUCString(uint16_t *s,uint16_t x,uint16_t y,uint8_t Ssize);
void UTF8TansformUnicode(uint8_t *s1,uint16_t *s2,uint8_t n);
void GUI_INFOPRINT(uint8_t *s);
void GUI_INFOPRINT2(uint8_t *s);

#endif
