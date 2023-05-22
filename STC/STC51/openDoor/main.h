#ifndef __MAIN_H
#define __MAIN_H

#include "fingerprint.h"
#include "reg52.h"

sbit FEN_MOTOR1 = P1^0;
sbit FEN_MOTOR2 = P1^1;
sbit REN_MOTOR1 = P1^2;
sbit REN_MOTOR2 = P1^3;

sbit EN_MOTOR = P1^4;

sbit TDP1 = P2^0;
sbit TDP2 = P2^1;
unsigned char usart_buf[16];
unsigned short usart_cnt = 0, usart_cntPre = 0;															

#endif