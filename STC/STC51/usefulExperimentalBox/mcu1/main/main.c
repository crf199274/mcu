#include "stc8a8k64d4.h"
#include "functionSwitching.h"
#include "function1.h"
#include "function2.h"
#include "function3.h"

void main()
{
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x07;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	ws2812b_INIT();
	
	LCD_Init();
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	functionSwitchingInit();
	
	adcInit();
	
	EA = 1;
	
	while(1)
	{
		switch(MODE)
		{
			case 0:function1();break;
			case 1:function2();break;
			case 2:function3();break;
		}
	}
}