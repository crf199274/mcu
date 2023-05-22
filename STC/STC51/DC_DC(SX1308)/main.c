#include "stc8g.h"
#include "dc_dc.h"
#include "pwm.h"


void main()
{
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;

	DC_DC_INIT();
	
	PWM_OCC_Load(0);
	
	while(1)
	{
		
	}
}
