#ifndef __OLED_H
#define __OLED_H

#include "typedef.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "DC_DC.h"
#include "IIC.h"

void oled_init(void);
void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch);
void oled_screen_fill(unsigned char cmd);

#endif
