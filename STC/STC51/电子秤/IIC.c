#include "IIC.h"
#include "delay.h"
#include "stc8.h"

sbit OLED_SDA = P5^4;
sbit OLED_SCL = P5^5;

//起始信号
void iic_start(void)
{
	OLED_SDA = 1;
	OLED_SCL = 1;
	delay_07us(7);//5us
	OLED_SDA = 0;
	delay_07us(9);//6us
	OLED_SCL = 0;
}

//停止信号
void iic_stop(void)
{
	OLED_SCL = 0;
	OLED_SDA = 0;
	OLED_SCL = 1;
	delay_07us(9);//6us
	OLED_SDA = 1;
	delay_07us(9);//6us
}

//等待应答
unsigned char iic_wait_ack(void)
{
	unsigned char temptime=0;
	OLED_SDA = 1;
	delay_07us(2);//1us
	OLED_SCL = 1;
	delay_07us(2);//1us
	while(OLED_SDA)
	{
		temptime++;
		if(temptime>250)
		{
			iic_stop();
			return 1;
		}
	}
	OLED_SCL = 0;
	return 0;
}

//应答
//void iic_ack(void)
//{
//	OLED_SCL = 0;
//	OLED_SDA = 0;
//	delay_07us(3);//2us
//	OLED_SCL = 1;
//	delay_07us(7);//5us
//	OLED_SCL = 0;
//}

//非应答
//void iic_nack(void)
//{
//	OLED_SCL = 0;
//	OLED_SDA = 1;
//	delay_07us(3);//2us
//	OLED_SCL = 1;
//	delay_07us(7);//5us
//	OLED_SCL = 0;
//}

//数据发送
void iic_send_byte(unsigned char mdata)
{
	unsigned char t;
	OLED_SCL = 0;
	for(t=0;t<8;t++)
	{
		if((mdata&0x80)>0)
			OLED_SDA = 1;
		else
			OLED_SDA = 0;
		mdata<<=1;
		delay_07us(3);//2us
		OLED_SCL = 1;
		delay_07us(3);//2us
		OLED_SCL = 0;
		delay_07us(3);//2us
	}
}

//数据读取
//unsigned char iic_read_byte(unsigned char ack)
//{
//	unsigned char i=0,mdata=0;
//	for(i=0;i<8;i++)
//	{
//		OLED_SCL = 0;
//		delay_07us(3);//2us
//		OLED_SCL = 1;
//		mdata<<=1;
//		if(OLED_SDA)
//			mdata++;
//		delay_07us(2);//2us
//	}
//	if(!ack)
//		iic_nack();
//	else
//		iic_ack();
//	return mdata;
//}
