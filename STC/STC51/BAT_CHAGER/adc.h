#ifndef __ADC_H
#define __ADC_H

#include "stc15.h"
#include "typedef.h"

#define VOUT1_K			(5/1024.0)*11
#define VOUT2_K			(5/1024.0)*11
#define VOUT3_K			(5/1024.0)*11

#define IOUT1_K			5/(1024*0.005*50)
#define IOUT2_K			5/(1024*0.005*50)
#define IOUT3_K			5/(1024*0.005*50)

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_STOP		0x00						//ADC暂停
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

void adcInit(void);


#endif
