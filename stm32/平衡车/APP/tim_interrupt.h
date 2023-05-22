#ifndef  _tim_interrupt_H
#define _tim_interrupt_H

#include "OpBit.h"

void tim_interrupt_init(u16 Period,u16 Prescaler);
void TIM4_IRQHandler(void);
int num_changing(void);


#endif
