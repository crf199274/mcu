#ifndef __OLED_H
#define __OLED_H

#include "typedef.h"

extern float VOUT1;
extern float VOUT2;
extern float VOUT3;
extern float IOUT1;
extern float IOUT2;
extern float IOUT3;

extern uint8_t T1;
extern uint8_t T2;
extern uint8_t T3;

extern uint16_t MIN1;
extern uint16_t MIN2;
extern uint16_t MIN3;

void oled_init(void);
void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch);
void updata_OLED(void);
void oled_screen_fill(unsigned char cmd);

#endif
