#include "MPU_6050.h"
#include "I2C.h"
#include "math.h"
#include "delay.h"
#include "Output.h"

float angle_pitch[8];
float angle_roll[8];
float angle_yaw[8];

void MPU_6050_IIC_init(void)
{
	iic_init();
}

uint8_t MPU_6050_init(void)
{
	int ID=0x68;
	
	MPU_6050_IIC_init();
	
	stdelay_ms(100);
	
	MPU_6050_send_byte(PWR_MGMT_1,0X80);
	
	stdelay_ms(100);
	
	MPU_6050_send_byte(PWR_MGMT_1,0X00);
	
	MPU_6050_send_byte(SMPLRT_DIV,(3<<3));
	
	MPU_6050_send_byte(ACCEL_CONFIG,(0<<3));
	
	MPU_Set_Rate(200);	
	
	MPU_6050_send_byte(INT_ENABLE,0X00);
	
	MPU_6050_send_byte(0x6A,0X00);
	
	MPU_6050_send_byte(0x23,0X00);
	
	//判断是否建立连接,如果连接成功则复位MPU6050,反之,返回失败
	if(MPU_6050_Read_byte(0x75)==ID)
	{
		MPU_6050_send_byte(PWR_MGMT_1,0x01);
	  MPU_6050_send_byte(PWR_MGMT_2,0x00);
	}
	else
	{
		return 0;
	}
	
	return 1;
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	MPU_6050_send_byte(0x1A,data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	MPU_6050_send_byte(0x19,data);	//设置数字低通滤波器
 	MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

void MPU_6050_send_byte(uint8_t addr,uint8_t data)
{
	iic_start();
	iic_send_byte(0xD0);
	iic_wait_ack();
	iic_send_byte(addr);
	iic_wait_ack();
	iic_send_byte(data);
	iic_wait_ack();
	iic_stop();
}

//
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
  iic_start(); 
	iic_send_byte((addr<<1)|0);//发送器件地址+写命令	
	if(iic_wait_ack())	//等待应答
	{
		iic_stop();		 
		return 1;		
	}
  iic_send_byte(reg);	//写寄存器地址
  iic_wait_ack();		//等待应答
	for(i=0;i<len;i++)
	{
		iic_send_byte(buf[i]);	//发送数据
		if(iic_wait_ack())		//等待ACK
		{
			iic_stop();	 
			return 1;		 
		}		
	}    
  iic_stop();	 
	return 0;	
} 

uint8_t MPU_6050_Read_byte(uint8_t addr)
{
	uint8_t data;
	iic_start();
	iic_send_byte(0xD0);
	iic_wait_ack();
	iic_send_byte(addr);
	iic_wait_ack();
	iic_start();
	iic_send_byte(0xD1);
	iic_wait_ack();
	data=iic_read_byte(0);
	iic_stop();
	return data;
}

//
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	iic_start(); 
	iic_send_byte((addr<<1)|0);//发送器件地址+写命令	
	if(iic_wait_ack())	//等待应答
	{
		iic_stop();		 
		return 1;		
	}
	iic_send_byte(reg);	//写寄存器地址
	iic_wait_ack();		//等待应答
	iic_start();
	iic_send_byte((addr<<1)|1);//发送器件地址+读命令	
	iic_wait_ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=iic_read_byte(0);//读数据,发送nACK 
		else *buf=iic_read_byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
	iic_stop();	//产生一个停止条件 
	return 0;	
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void MPU_Get_Gyroscope()
{
	uint8_t data=0;
	
	data=MPU_6050_Read_byte(0x43);
	gyrox=data<<8;
	data=MPU_6050_Read_byte(0x44);
	gyrox|=data;
	
	data=MPU_6050_Read_byte(0x45);
	gyroy=data<<8;
	data=MPU_6050_Read_byte(0x46);
	gyroy|=data;
	
	data=MPU_6050_Read_byte(0x47);
	gyroz=data<<8;
	data=MPU_6050_Read_byte(0x48);
	gyroz|=data;
	
}
