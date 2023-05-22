#ifndef __userKey_H
#define __userKey_H

#include "stm32f10x.h"

#define userKeyIOClock 												RCC_APB2Periph_GPIOA
#define userKeyIOPort													GPIOA
#define userKeyIO															GPIO_Pin_8


void userKeyINIT(void);
uint8_t userKeyStaus(void);

#endif
