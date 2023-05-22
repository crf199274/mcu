#include "delay.h"

void delay_07us(int i)
{
	while(i--);
}

void delay_ms(int i)
{
	while(i--)
		delay_07us(1458);
}
