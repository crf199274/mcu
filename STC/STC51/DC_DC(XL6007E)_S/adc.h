#ifndef __ADC_H
#define __ADC_H

#include "stc8a8k64d4.h"
#include "typedef.h"

#define VOUT_K					(5/4096.0)*21
#define VIN_K						(5/4096.0)*3
#define BAT_I_K					5/(4096*0.005*50)
#define VOUT_I_K				5/(4096*0.005*50)

extern uint16_t AD_Value;

void adcInit(void);


#endif
