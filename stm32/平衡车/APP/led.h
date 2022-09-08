#ifndef  _led_H
#define _led_H

#include "OpBit.h"

#define LED1_ON       GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define LED1_OFF    GPIO_SetBits(GPIOE,GPIO_Pin_2)

void led_init(void);

#endif
