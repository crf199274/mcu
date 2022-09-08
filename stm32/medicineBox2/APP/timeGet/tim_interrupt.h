#ifndef  _tim_interrupt_H
#define _tim_interrupt_H

#include "stm32f10x.h"

//PWM ‰≥ˆÕ®µ¿IO≈‰÷√
#define	motorPWM_PortClock									RCC_APB2Periph_GPIOA
#define motorPWM_GPIO_Port									GPIOA
#define motorPWM_Pin												GPIO_Pin_6

void TIM4_init(uint16_t Period,uint16_t Prescaler);
void TIM2_init(uint16_t Period,uint16_t Prescaler);
void PWM_init(uint16_t Period,uint16_t Prescaler);


#endif
