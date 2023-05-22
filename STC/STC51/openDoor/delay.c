#include "delay.h"

void delay(unsigned int i)
{
	while(i--);
}

void delay_ms(unsigned int i)
{
	while(i--)
	{
		delay(100);
	}
}

void delay_s(unsigned int i)
{
	while(i--)
	{
		delay_ms(1000);
	}
}