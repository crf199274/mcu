#ifndef __ADC_H
#define __ADC_H

#include "stc8a8k64d4.h"
#include "ws2812.h"

//#define SW_FREQ					(5/4096.0)
//#define M_FREQ					(5/4096.0)
//#define RGB_R						(5/4096.0)
//#define RGB_G						(5/4096.0)
//#define RGB_B						(5/4096.0)

extern WsColor RGB1;
extern unsigned int SW_Freq;
extern unsigned int M_Freq;

extern unsigned char falg1;
extern unsigned char falg2;
extern unsigned char falg3;
extern unsigned char falg4;
extern unsigned char falg5;

void adcInit();

#endif
