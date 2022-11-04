#include "reg52.h"
#include "string.h"
#include "stdio.h"
#include "intrins.h"

sbit LS7438_A=P2^2;
sbit LS7438_B=P2^3;
sbit LS7438_C=P2^4;

code char seg_7_code[12]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80,0x00};

void Delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}

void Delay_100us(int n100us)
{
	while(n100us--)
	{
		Delay100us();
	}
}

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(int nms)
{
	while(nms--)
	{
		Delay1ms();
	}
}

void display_NixieTube(float num)
{
	char i=0,buf[10]={0},sort=0,flag=0;
	
	sprintf(buf,"%4.1f",(float)num);
		
	
	for(i=0;i<strlen(buf);i++)
	{
		
		if(buf[i]=='.')
			flag = 1;
		
		if(flag)
			sort = i-1 ;
		else
			sort = i;
		
		switch((strlen(buf)-sort-2))
		{
			case 0:LS7438_C=0;LS7438_B=0;LS7438_A=0;break;
			case 1:LS7438_C=0;LS7438_B=0;LS7438_A=1;break;
			case 2:LS7438_C=0;LS7438_B=1;LS7438_A=0;break;
		}
		
		switch(buf[i])
		{
			case '0':P0=seg_7_code[0];break;
			case '1':P0=seg_7_code[1];break;
			case '2':P0=seg_7_code[2];break;
			case '3':P0=seg_7_code[3];break;
			case '4':P0=seg_7_code[4];break;
			case '5':P0=seg_7_code[5];break;
			case '6':P0=seg_7_code[6];break;
			case '7':P0=seg_7_code[7];break;
			case '8':P0=seg_7_code[8];break;
			case '9':P0=seg_7_code[9];break;
			case '.':P0=seg_7_code[10];break;
		}
		
		Delay_100us(5);
		
		P0 = seg_7_code[11];
		
	}
}