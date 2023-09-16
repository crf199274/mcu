#ifndef __ADC_H
#define __ADC_H

#include "stc8a8k64d4.h"
#include "typedef.h"
#include "delay.h"
#include "intrins.h"
#include "DC_DC.h"

struct adc_msg{
	
	/* adc��ͨ���� */
	uint8_t ch;
	
	/* adc��ͨ������ֵ */
	uint16_t AD_Value;
};

extern struct adc_msg adc_msg_1;

#define VOUT_K					(5/4096.0)*21
#define VIN_K						(5/4096.0)*3
#define IIN_K						 5/(4096*0.005*50)
#define IOUT_K					 5/(4096*0.005*50)

void adcInit(void);


#endif
