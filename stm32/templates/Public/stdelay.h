#ifndef  __STDELAY_H
#define  __STDELAY_H

#include "OpBit.h"

void stdelay_init(u8 SYSCLK);
void delay_us(u32 nus);
void stdelay_ms(u16 nms);

#endif
