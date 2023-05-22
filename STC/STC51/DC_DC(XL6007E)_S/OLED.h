#ifndef __OLED_H
#define __OLED_H

#include "typedef.h"

extern float VIN; 
extern float VOUT;
extern float BAT_I;
extern float VOUT_I;
extern float OCC;
extern uint16_t Value;

void oled_init(void);
void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch);
void updata_OLED(void);
void tip_OLED(char str[]);
void oled_screen_fill(unsigned char cmd);

#endif
