#include "oled.h"
#include "adc.h"
#include "delay.h"
#include "key.h"
#include "time.h"

void main()
{
	P0M0 = 0;
	P0M1 = 0;
	P1M0 = 0;
	P1M1 = 0;
	P2M0 = 0;
	P2M1 = 0;
	P3M0 = 0;
	P3M1 = 0;
	P4M0 = 0;
	P4M1 = 0;
	P5M0 = 0;
	P5M1 = 0;
	
//	keyInit();
	oled_init();
	adcInit();
//	timeInit();
	
	EA = 1;
	
	while(1)
	{
		
		updata_OLED();
		
		Delay10ms();
		
	}
	
}
