#include "functionSwitching.h"

sbit INT1 = P3^3;
sbit INT0 = P3^2;

unsigned char MODE = 0;

unsigned char MODE2 = 0;

void functionSwitchingInit()
{
	
	IT1 = 1;                                    //使能INT1下降沿中断
  EX1 = 1;                                    //使能INT1中断
	INT1 = 1;
	IT0 = 1;
	EX0 = 1;
	INT0 = 1;
	
}

void INT1_Isr() interrupt 2
{
	
	if(++MODE>2)
		MODE = 0;
  while(!INT1);
	
}

void INT0_Isr() interrupt 0
{
	
	MODE2 = 1 - MODE2;
  while(!INT0);
}
