#include "reg52.h"
#include "intrins.h"

sbit DIN = P3^4;
sbit CS = P3^5;
sbit DCLK = P3^6;
sbit DOUT = P3^7;

void ET2046_Write_REG(unsigned char cmd)
{
	char i = 0;
	
	DCLK = 0;
	for(i=0;i<8;i++)
	{
		if(cmd&0x80)
			DIN = 1;
		else
			DIN = 0;
		_nop_();
		DCLK = 1;
		_nop_();
		DCLK = 0;
		cmd <<= 1;
	}
	
}

//void ET2046_Write_REG(unsigned char cmd)
//{
//	char i = 0;
//	
//	DCLK = 0;
//	for(i=0; i<8; i++)
//	{
//			DIN = cmd >> 7;     //放置最高位
//			cmd <<= 1;
//			DCLK = 0;            //上升沿放置数据

//			DCLK = 1;

//	}
//	
//}

unsigned int ET2046_Write_Data(void)
{
	char i = 0;
	unsigned int dat = 0;
	DCLK = 0;
	for(i=0;i<12;i++)
	{
		dat <<= 1;
		if(DOUT)
			dat |= 1;
		else
			dat |= 0;
		_nop_();
		DCLK = 1;
		_nop_();
		DCLK = 0;
	}
	
	return dat;
	
}

//unsigned int ET2046_Write_Data(void)
//{
//	char i = 0;
//	unsigned int dat = 0;
//	
//	DCLK = 0;
//	for(i=0; i<12; i++)     //接收12位数据
//	{
//			dat <<= 1;

//			DCLK = 1;
//			DCLK = 0;

//			dat |= DOUT;

//	}
//  return dat; 

//}


unsigned int ET2046_ConvertAndRead(unsigned char cmd)
{
	unsigned int dat = 0;
	char i = 0;
	
	CS = 0;
	ET2046_Write_REG(cmd);
	for(i=6; i>0; i--);
	_nop_();
	DCLK = 1;
	_nop_();
	DCLK = 0;
	dat = ET2046_Write_Data();
	CS = 1;
	
	return dat;
	
}

//unsigned int ET2046_ConvertAndRead(unsigned char cmd)
//{
//	unsigned int dat = 0;
//	char i = 0;
//	
//	DCLK = 0;
//	CS  = 0;
//	ET2046_Write_REG(cmd);
//	for(i=6; i>0; i--);     //延时等待转换结果
//	DCLK = 1;      //发送一个时钟周期，清除BUSY
//	_nop_();
//	_nop_();
//	DCLK = 0;
//	_nop_();
//	_nop_();
//	dat=ET2046_Write_Data();
//	CS = 1;
//	return dat;
//	
//}
