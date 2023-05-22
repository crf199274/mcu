#include "fingerprint.h"

unsigned int flag1 = 0;
unsigned int flag2 = 0;
unsigned int flag3 = 0;

void UserCompare()
{
	if(flag1==0)
	{
		SBUF=0xF5;
		while(TI==0);TI=0;
		SBUF=0x0C;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x0C;
		while(TI==0);TI=0;
		SBUF=0xF5;
		while(TI==0);TI=0;
		
		flag1 = 100;
		
	}
	
}

void Color()
{
	if(flag2==0)
	{
		//---红蓝渐变
		SBUF=0xF5;
		while(TI==0);TI=0;
		SBUF=0xC3;
		while(TI==0);TI=0;
		SBUF=0x03;
		while(TI==0);TI=0;
		SBUF=0x06;
		while(TI==0);TI=0;
		SBUF=0x96;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x50;
		while(TI==0);TI=0;
		SBUF=0xF5;
		while(TI==0);TI=0;
		
		flag2 = 10;
		
	}
}

void CloseColor()
{
	if(flag3==0)
	{
		//---关灯
		SBUF=0xF5;
		while(TI==0);TI=0;
		SBUF=0xC3;
		while(TI==0);TI=0;
		SBUF=0x07;
		while(TI==0);TI=0;
		SBUF=0x07;
		while(TI==0);TI=0;
		SBUF=0x96;
		while(TI==0);TI=0;
		SBUF=0x00;
		while(TI==0);TI=0;
		SBUF=0x55;
		while(TI==0);TI=0;
		SBUF=0xF5;
		while(TI==0);TI=0;
		
		flag3 = 10;
		
	}
}

void UserInterrupt(void)
{
	
	SBUF=0xF5;
	while(TI==0);TI=0;
	SBUF=0xFE;
	while(TI==0);TI=0;
	SBUF=0x00;
	while(TI==0);TI=0;
	SBUF=0x00;
	while(TI==0);TI=0;
	SBUF=0x00;
	while(TI==0);TI=0;
	SBUF=0x00;
	while(TI==0);TI=0;
	SBUF=0xFE;
	while(TI==0);TI=0;
	SBUF=0xF5;
	while(TI==0);TI=0;
	
}

