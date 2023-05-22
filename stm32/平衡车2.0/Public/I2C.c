#include "I2C.h"
#include "delay.h"

//初始化
void iic_init(void)
{
	GPIO_InitTypeDef GPIO_Init_my;
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=IIC_SCL_PIN;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL_PORT,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_Init_my);
	
	IIC_SCL_High;
	IIC_SDA_OUT_High;
}
//SDA输出
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT,&GPIO_Init_my);
}
//SDA输入
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init_my.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA_PORT,&GPIO_Init_my);
}
//起始信号
void iic_start(void)
{
	SDA_OUT();
	IIC_SDA_OUT_High;
	IIC_SCL_High;
	stdelay_us(5);
	IIC_SDA_OUT_Low;
	stdelay_us(6);
	IIC_SCL_Low;
}
//停止信号
void iic_stop(void)
{
	SDA_OUT();
	IIC_SCL_Low;
	IIC_SDA_OUT_Low;
	IIC_SCL_High;
	stdelay_us(6);
	IIC_SDA_OUT_High;
	stdelay_us(6);
}
//等待应答
uint8_t iic_wait_ack(void)
{
	uint8_t temptime=0;
	SDA_IN();
	IIC_SDA_OUT_High;
	stdelay_us(1);
	IIC_SCL_High;
	stdelay_us(1);
	while(IIC_SDA_IN)
	{
		temptime++;
		if(temptime>250)
		{
			iic_stop();
			return 1;
		}
	}
	IIC_SCL_Low;
	return 0;
}
//应答
void iic_ack(void)
{
	IIC_SCL_Low;
	SDA_OUT();
	IIC_SDA_OUT_Low;
	stdelay_us(2);
	IIC_SCL_High;
	stdelay_us(5);
	IIC_SCL_Low;
}
//非应答
void iic_nack(void)
{
	IIC_SCL_Low;
	SDA_OUT();
	IIC_SDA_OUT_High;
	stdelay_us(2);
	IIC_SCL_High;
	stdelay_us(5);
	IIC_SCL_Low;
}
//数据发送
void iic_send_byte(uint8_t data)
{
	uint8_t t;
	SDA_OUT();
	IIC_SCL_Low;
	for(t=0;t<8;t++)
	{
		if((data&0x80)>0)
			IIC_SDA_OUT_High;
		else
			IIC_SDA_OUT_Low;
		data<<=1;
		stdelay_us(2);
		IIC_SCL_High;
		stdelay_us(2);
		IIC_SCL_Low;
		stdelay_us(2);
	}
}
//数据读取
uint8_t iic_read_byte(uint8_t ack)
{
	uint8_t i=0,data=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL_Low;
		stdelay_us(2);
		IIC_SCL_High;
		data<<=1;
		if(IIC_SDA_IN)
			data++;
		stdelay_us(1);
	}
	if(!ack)
		iic_nack();
	else
		iic_ack();
	return data;
}
