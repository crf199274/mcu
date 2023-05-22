#ifndef  _DELAY_H
#define  _DELAY_H

#include "stm32f10x.h"

void stdelay_init(uint8_t SYSCLK);
void stdelay_us(uint32_t nus);
void stdelay_ms(uint16_t nms);

#endif
