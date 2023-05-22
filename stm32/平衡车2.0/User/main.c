#include "main.h"

int main()
{
	stdelay_init(72);
	usart_init(9600);
	MOTO_PWM_Init(3599,0);
	Motor_init();
	while(MPU_6050_init()==0)
	{
		printf("error !");
		stdelay_ms(1000);
	}
	
	while(mpu_dmp_init())
	{
		printf("MPU_6050_dmp_init is error !");
		stdelay_ms(1000);
	}
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	NVIC_Config();
	Timer1_Init(500,719);
//	TIM_SetCompare4(TIM3,3599);
//	TIM_SetCompare3(TIM3,3599);
//	MotorR_direction(0);
//	MotorL_direction(0);
	
	while(1)
	{
//		usartSendSpeedDataToANO(Read_Encoder(2),Read_Encoder(4));
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{
//			MPU_gXYZ_Get();
//			
//			usartSendMPU6050DataToANO(pitch,((float)gyroy/(0xffff/32)));
//			
//		}
//		  
		stdelay_ms(10);
		
	}
}
