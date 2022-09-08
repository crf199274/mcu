#ifndef __userLed_H
#define __userLed_H

#include "stm32f10x.h"

//实物LED的R、B脚接反了

#define LED_B_IOClock 																RCC_APB2Periph_GPIOC
#define LED_B_IOPort																	GPIOC
#define LED_B_IO																			GPIO_Pin_6

#define LED_R_IOClock 																RCC_APB2Periph_GPIOC
#define LED_R_IOPort																	GPIOC
#define LED_R_IO																			GPIO_Pin_7

void LED_INIT(void);
void LED_R_Staus(_Bool staus);
void LED_B_Staus(_Bool staus);

#endif
