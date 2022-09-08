#include "Output.h"
#include "pid.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "MPU_6050.h"
#include "timer_init.h"
#include "usart.h"
#include "stdio.h"

float pitch,roll,yaw;
int gyrox,gyroy,gyroz;
int aacx,aacy,aacz;
//u8 data;

int Vertical_output,Velocity_output,Turn_out;

int Encoder_L,Encoder_R;

int PWM_Out;

int Motor_L,Motor_R;

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)   //时间到了
	{
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//清中断
		
//		Velocity_output=Velocity(0,Encoder_L,Encoder_R);
		Vertical_output=Vertical(Velocity_output+0,pitch,gyroy);
		Turn_out=Turn(0);
		
		PWM_Out=Vertical_output;
		
		printf("PWM_Out=%d",PWM_Out);
		
		Motor_L=PWM_Out-Turn_out;
		Motor_R=PWM_Out+Turn_out;
		
		SetMotorVoltageAndDirection(&Motor_L,&Motor_R);
		
		if(Motor_L>3500)
			Motor_L=3500;
		if(Motor_R>3500)
			Motor_R=3500;
		
		MOTO_PWM_Out(Motor_L,Motor_R);
		
	}
}
