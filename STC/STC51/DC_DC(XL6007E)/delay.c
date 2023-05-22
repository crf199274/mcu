#include "delay.h"
#include "intrins.h"

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

//void Delay100ms()		//@24.000MHz
//{
//	unsigned char i, j, k;

//	_nop_();
//	_nop_();
//	i = 10;
//	j = 31;
//	k = 147;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}
