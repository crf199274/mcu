#include "delay.h"
#include "intrins.h"
#include "stc15.h"

void Delay2us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}

void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

void Delay6us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

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

void Delay10ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 234;
	j = 115;
	do
	{
		while (--j);
	} while (--i);
}


void Delay500ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

