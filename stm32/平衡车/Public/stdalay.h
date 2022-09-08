#ifndef    __STDALAY_H
#define   __STDALAY_H

#include "stm32f10x.h"

void stdalay_init(u8 SYSCLK);
void dalay_us(u32 nus);
void stdalay_ms(u16 nms);

#endif
