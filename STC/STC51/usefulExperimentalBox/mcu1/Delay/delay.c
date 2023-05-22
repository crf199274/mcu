#include "delay.h"

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 13;
	j = 45;
	k = 214;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay120ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 15;
	j = 157;
	k = 53;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
