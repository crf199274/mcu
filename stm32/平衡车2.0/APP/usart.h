#ifndef  _usart_H
#define _usart_H

#include "stm32f10x.h"

extern int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //����ң����صı���

void usart_init(uint32_t BaudRate);
void usartSendMPU6050DataToANO(float pitch,float gyroy);
void usartSendSpeedDataToANO(uint16_t Speed_L,uint16_t Speed_R);

#endif
