#ifndef __medicineBoxBack_H
#define __medicineBoxBack_H

#include "stm32f10x.h"

#define medicineBoxBackIOClock											RCC_APB2Periph_GPIOA
#define medicineBoxBackIOPort												GPIOA
#define medicineBoxBackIO														GPIO_Pin_12

void medicineBoxBackIO_INIT(void);
uint8_t medicineBoxBackStaus(void);

#endif
