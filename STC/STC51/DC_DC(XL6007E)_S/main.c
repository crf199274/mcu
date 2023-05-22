#include "DC_DC.h"
#include "key.h"
#include "adc.h"
#include "delay.h"
#include "OLED.h"
#include "math.h"

uint8_t mode1 = 1;
uint8_t mode2 = 0;

void main()
{
	
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x01;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	keyInit();
	oled_init();
	adcInit();
	DC_DC_PWM_Init();
	
	tip_OLED("Voltage verify");
	
	pwm = VOL_OCC(VOL);
	LoadPWM(pwm);
	
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	
	if(VOUT<VOL)
	{
		pwm_error = -((fabs(VOL-VOUT)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN))*CYCLE);
	}else{
		
		pwm_error = fabs(VOL-VOUT)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN)*CYCLE;
	}
	
	oled_screen_fill(0x00);
	tip_OLED("OK");
	
	Delay500ms();
	Delay500ms();
	
	oled_screen_fill(0x00);
	
	pwm = VOL_OCC(VOL) + pwm_error;
	LoadPWM(pwm);
	
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
			
			pwm = VOL_OCC(VOL) + pwm_error;
			
			LoadPWM(pwm);
			
			mode_key = 0;
			
		}
		
		updata_OLED();
		
		Delay1ms();
		
	}
	
}
