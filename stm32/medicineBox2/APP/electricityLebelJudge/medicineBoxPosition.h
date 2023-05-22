#ifndef __medicineBoxPosition_H
#define __medicineBoxPosition_H

#include "stm32f10x.h"

#define medicineBoxPositionIOClock											RCC_APB2Periph_GPIOA
#define medicineBoxPositionIOPort												GPIOA
#define medicineBoxPositionIO														GPIO_Pin_11

void medicineBoxPositionIO_INIT(void);
uint8_t medicineBoxPositionStaus(void);

#endif
