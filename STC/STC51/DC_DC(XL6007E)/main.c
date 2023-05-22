#include "DC_DC.h"
#include "key.h"
#include "lcd.h"
#include "adc.h"
#include "delay.h"
#include "DC_DC.h"

uint8_t mode1 = 1;
uint8_t mode2 = 0;

void main()
{
	
	lcd_init();
	adcInit();
	DC_DC_PWM_Init();
	keyInit();
	
	EA = 1;
	
//	LoadPWM(PWM_HIGH_MIN);
//	LoadPWM(PWM_HIGH_MAX);
	
	while(1)
	{
		if(mode_key!=0)
		{
			switch(mode_key)
			{
				case 1:
					if(mode1)
						VOL -= 1;
					else
						VOL -= 0.1;
					if(VOL<VOL_MIN)
						VOL = VOL_MIN;
					break;
				case 2:
					if(mode1)
						VOL += 1;
					else
						VOL += 0.1;
					if(VOL>VOL_MAX)
						VOL = VOL_MAX;
					break;
				case 3:
					if(mode2==0)
					{
						VOL = VOL_MIN;
						mode2 = 1 - mode2;
					}
					else
					{
						VOL = VOL_MAX;
						mode2 = 1 - mode2;
					}
					break;
				case 4:
					mode1 = 1 - mode1;
					break;
			}
			
			pwm = VOL_OCC(VOL);
			
			LoadPWM(pwm);
			
			mode_key = 0;
			
		}
		
		updata_LCD();
		
		Delay1ms();
		
	}
	
}
