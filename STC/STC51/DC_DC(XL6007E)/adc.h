#ifndef __ADC_H
#define __ADC_H

#include "stc15.h"
#include "typedef.h"

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
