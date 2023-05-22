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

#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_STOP		0x00						//ADC��ͣ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

void adcInit(void);


#endif
