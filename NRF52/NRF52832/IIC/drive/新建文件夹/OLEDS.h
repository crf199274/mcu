#ifndef __OLED_H
#define __OLED_H

#include "IIC.h"


void oled_init(void);
//void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch);
void oled_screen_fill(unsigned char cmd);

#endif
