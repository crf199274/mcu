#ifndef    __STDALAY_H
#define   __STDALAY_H

#include "OpBit.h"

void stdelay_init(uint8_t SYSCLK);
void stdelay_us(uint32_t nus);
void stdelay_ms(uint16_t nms);

#endif
