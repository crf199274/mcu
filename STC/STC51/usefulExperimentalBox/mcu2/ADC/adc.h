#ifndef __ADC_H
#define __ADC_H

#include "stc8h.h"

#define VOUT_K					(5/1024.0)*21
#define VIN_K						(5/1024.0)*3
#define VOUT_I_K				5/(1024*0.005*50)

extern float VOUT;
extern float IOUT;
extern float BAT;

void adcInit();

#endif
