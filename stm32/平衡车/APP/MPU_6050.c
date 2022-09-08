#include "MPU_6050.h"
#include "IIC.h"
#include "math.h"
#include "stdalay.h"

float angle_pitch[8];
float angle_roll[8];
float angle_yaw[8];

void MPU_6050_IIC_init(void)
{
	iic_init();
}

u8 MPU_6050_init(void)
{
	int ID=0x68;
	
	iic_init();
	
	stdalay_ms(1000);
	
//	MPU_6050_send_byte(0x6B,0x80);
//	stdalay_ms(1000);
	MPU_6050_send_byte(0x6B,0x00);
	stdalay_ms(1000);
	
//	MPU_6050_send_byte(0x6B,0x09);
	
	MPU_6050_send_byte(0x1B,0x18);//(0x03<<3)
	
	MPU_6050_send_byte(0x1C,0x01);//(0x03<<3)
	
//	MPU_6050_send_byte(0x19,0x19);
//	MPU_6050_send_byte(0x1A,0x04);
	
//	MPU_6050_send_byte(0x23,0x00);
	
//	MPU_6050_send_byte(0x37,0x80);
	
//	MPU_6050_send_byte(0x38,0x00);
	
//	MPU_6050_send_byte(0x6A,0x00);
	
	if(MPU_6050_Read_byte(0x75)==ID)
	{
//		MPU_6050_send_byte(0x6B,0x01);
//		MPU_6050_send_byte(0x6C,0x00);
		MPU_6050_send_byte(0x19,0x07);
	  MPU_6050_send_byte(0x1A,0x04);
	}
	else
		return 0;
	return 1;
}

void MPU_6050_send_byte(u8 addr,u8 data)
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
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
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

u8 MPU_6050_Read_byte(u8 addr)
{
	u8 data;
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
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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

void average(float *pitch,float *roll,float *yaw)
{
	int i;
	for(i=incise-1;i>0;i--)
	{
		angle_pitch[i]=angle_pitch[i-1];
		angle_roll[i]=angle_roll[i-1];
		angle_yaw[i]=angle_yaw[i-1];
	}
	angle_pitch[0]=*pitch;
	angle_roll[0]=*roll;
	angle_yaw[0]=*yaw;
	
	for(i=0;i<incise;i++)
	{
		*pitch=*pitch+angle_pitch[i];
		*roll=*roll+angle_roll[i];
		*yaw=*yaw+angle_yaw[i];
	}
	*pitch=*pitch/incise;
	*roll=*roll/incise;
	*yaw=*yaw/incise;
}
