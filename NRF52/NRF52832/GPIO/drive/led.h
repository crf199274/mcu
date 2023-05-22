#ifndef __LED_H
#define __LED_H

#include "nrf52.h"

#define LOW						 0
#define HIGH					 1

#define LED_ON				 false
#define LED_OFF				 true

#define KEY_0					 13

#define LED_0          17
#define LED_1          18
#define LED_2          19
#define LED_3          20

void LED_Init(void);
void LED_OPEN_CLOSE(int ledx,_Bool state);

#endif
