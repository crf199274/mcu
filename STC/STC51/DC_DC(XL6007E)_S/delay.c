#include "delay.h"
#include "intrins.h"

//void Delay2us(void)		//@24.000MHz
//{
//}

void Delay2us()		//@24.000MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}

//void Delay5us(void)		//@24.000MHz
//{
//	unsigned char i;

//	_nop_();
//	i = 2;
//	while (--i);
//}

void Delay5us()		//@24.000MHz
{
	unsigned char i;

	i = 38;
	while (--i);
}

//void Delay6us(void)		//@24.000MHz
//{
//	unsigned char i;

//	_nop_();
//	i = 3;
//	while (--i);
//}

void Delay6us()		//@24.000MHz
{
	unsigned char i;

	i = 46;
	while (--i);
}

//void Delay1ms()		//@24.000MHz
//{
//	unsigned char i, j;

//	i = 4;
//	j = 225;
//	do
//	{
//		while (--j);
//	} while (--i);
//}

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 32;
	j = 40;
	do
	{
		while (--j);
	} while (--i);
}

//void Delay50ms()		//@24.000MHz
//{
//	unsigned char i, j, k;

//	_nop_();
//	i = 7;
//	j = 23;
//	k = 105;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}

void Delay500ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 61;
	j = 225;
	k = 62;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

