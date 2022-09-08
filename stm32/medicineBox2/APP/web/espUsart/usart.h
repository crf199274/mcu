#ifndef  _usart_H
#define  _usart_H

#include "stm32f10x.h"

void my_usart_init(uint32_t BaudRate);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

#endif
