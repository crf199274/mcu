#include "MPU_6050.h"
#include "usart.h"
#include "timer_init.h"
#include "pid.h"
#include "stdio.h"
#include "stdalay.h"
#include "Output.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

extern float pitch,roll,yaw;
extern int gyrox,gyroy,gyroz;
extern int aacx,aacy,aacz;
u8 data;

extern int Encoder_L,Encoder_R;

int main()
{
	stdalay_init(72);
	my_usart_init(38400);
	MOTO_PWM_Init(3599,0);
	Motor_init();
	Motor_direction();
	MPU_6050_IIC_init();
	while(MPU_6050_init()==0)
	{
		printf("error !");
		stdalay_ms(1000);
	}
	
	while(mpu_dmp_init())
	{
		printf("MPU_6050_dmp_init is error !");
		stdalay_ms(1000);
	}
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	NVIC_Config();
	Timer1_Init(1499,7199);
	stdalay_ms(100);
	printf("running !");
	
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
//			data=MPU_6050_Read_byte(0x3B);
//		  aacx=data<<8;
//		  data=MPU_6050_Read_byte(0x3C);
//		  aacx|=data;
//		
//		  data=MPU_6050_Read_byte(0x3D);
//		  aacy=data<<8;
//		  data=MPU_6050_Read_byte(0x3E);
//		  aacy|=data;
//		
//		  data=MPU_6050_Read_byte(0x3F);
//		  aacz=data<<8;
//		  data=MPU_6050_Read_byte(0x40);
//		  aacz|=data;
		
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
			
		 Encoder_L=Read_Encoder(2);
		 Encoder_R=Read_Encoder(4);
		
		if((gyroy&0x8000)==0)
			gyroy=gyroy+0;
		else
			gyroy=-((~gyroy+1)&0x7FFF);
		
		printf("gyrox=%d,gyroy=%d,gyroz=%d\r\n",gyrox,gyroy,gyroz);
//		printf("aacx=%d,aacy=%d,aacz=%d\r\n",aacx,aacy,aacz);
		printf("pitch=%f,roll=%f,yaw=%f\r\n",pitch,roll,yaw);
		printf("Encoder_L=%d,Encoder_R=%d\r\n",Encoder_L,Encoder_R);
		
		
		}
		stdalay_ms(20);
	}
}
