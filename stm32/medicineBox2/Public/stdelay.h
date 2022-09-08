#ifndef    __STDALAY_H
#define   __STDALAY_H

#include "OpBit.h"

void stdelay_init(u8 SYSCLK);
void stdelay_us(uint32_t nus);
void stdelay_ms(uint16_t nms);
void stdelay_s(uint16_t ns);
void stdelay_100ms(uint16_t n100ms);

#endif
