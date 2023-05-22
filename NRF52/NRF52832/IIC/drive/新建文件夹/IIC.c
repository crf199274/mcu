#include "IIC.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

void IIC_SDA_STATE(bool state)
{
	nrf_gpio_cfg_output(SDA_PIN);
	
	if(state)
	{
		nrf_gpio_pin_set(SDA_PIN);
	}else{
		nrf_gpio_pin_clear(SDA_PIN);
	}
}

void IIC_SCL_STATE(bool state)
{
	nrf_gpio_cfg_output(SCL_PIN);
	
	if(state)
	{
		nrf_gpio_pin_set(SCL_PIN);
	}else{
		nrf_gpio_pin_clear(SCL_PIN);
	}
}

bool IIC_SDA_GET_STATE(void)
{
	return nrf_gpio_pin_read(SDA_PIN);
}

//起始信号
void iic_start(void)
{
	IIC_SDA_STATE(HIGH);
	IIC_SCL_STATE(HIGH);
	nrf_delay_us(5);//5us
	IIC_SDA_STATE(LOW);
	nrf_delay_us(6);//6us
	IIC_SCL_STATE(LOW);
}

//停止信号
void iic_stop(void)
{
	IIC_SCL_STATE(LOW);
	IIC_SDA_STATE(LOW);
	IIC_SCL_STATE(HIGH);
	nrf_delay_us(6);//6us
	IIC_SDA_STATE(HIGH);
	nrf_delay_us(6);//6us
}

//等待应答
unsigned char iic_wait_ack(void)
{
	unsigned char temptime=0;
	IIC_SDA_STATE(HIGH);
	nrf_delay_us(1);//1us
	IIC_SCL_STATE(HIGH);
	nrf_delay_us(1);//1us
	
	nrf_gpio_cfg_input(SDA_PIN,NRF_GPIO_PIN_PULLUP);
	nrf_delay_us(1);//1us
	
	while(IIC_SDA_GET_STATE())
	{
		temptime++;
		if(temptime>250)
		{
			iic_stop();
			return 1;
		}
	}
	IIC_SCL_STATE(LOW);
	return 0;
}

//应答
//void iic_ack(void)
//{
//	IIC_SCL_STATE(LOW);
//	IIC_SDA_STATE(LOW);
//	delay_07us(3);//2us
//	IIC_SCL_STATE(HIGH);
//	nrf_delay_us(5);//5us
//	IIC_SCL_STATE(LOW);
//}

//非应答
//void iic_nack(void)
//{
//	IIC_SCL_STATE(LOW);
//	IIC_SDA_STATE(HIGH);
//	delay_07us(3);//2us
//	IIC_SCL_STATE(HIGH);
//	nrf_delay_us(5);//5us
//	IIC_SCL_STATE(LOW);
//}

//数据发送
void iic_send_byte(unsigned char mdata)
{
	unsigned char t;
	IIC_SCL_STATE(LOW);
	for(t=0;t<8;t++)
	{
		if((mdata&0x80)>0)
			IIC_SDA_STATE(HIGH);
		else
			IIC_SDA_STATE(LOW);
		mdata<<=1;
		nrf_delay_us(2);//2us
		IIC_SCL_STATE(HIGH);
		nrf_delay_us(2);//2us
		IIC_SCL_STATE(LOW);
		nrf_delay_us(2);//2us
	}
}

//数据读取
//unsigned char iic_read_byte(unsigned char ack)
//{
//	unsigned char i=0,mdata=0;
//	for(i=0;i<8;i++)
//	{
//		IIC_SCL_STATE(LOW);
//		delay_07us(3);//2us
//		IIC_SCL_STATE(HIGH);
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
