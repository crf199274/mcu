#ifndef __MAIN_H
#define __MAIN_H

#include "fingerprint.h"
#include "reg52.h"

sbit pwm = P1^0;
sbit light1 = P2^0;
sbit light2 = P2^1;
unsigned char usart_buf[16];
unsigned short usart_cnt = 0, usart_cntPre = 0;															

#endif