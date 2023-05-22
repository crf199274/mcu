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

int Encoder_L,Encoder_R;

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)   //时间到了
	{
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//清中断
		
//		TIM_Cmd(TIM1, DISABLE);//停止定时器 
		
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		
		MPU_Get_Gyroscope();
		
		Balance_Pwm = balance_UP(pitch,Mechanical_angle,(short)gyroy);
		
		Encoder_L = Read_Encoder(2);
		Encoder_R = -Read_Encoder(4);
		
		Velocity_Pwm=velocity(Encoder_L,Encoder_R);       //===速度环PID控制	 
		if(1==Flag_Left||1==Flag_Right)    
		Turn_Pwm =turn(Encoder_L,Encoder_R,(short)gyroz);        //===转向环PID控制
		else Turn_Pwm=(0.5)*(short)gyroz;																	 //===用来校正小车在原地转圈的现象
		Motor_L=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===计算左轮电机最终PWM
		Motor_R=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===计算右轮电机最终PWM
	  Xianfu_Pwm();  																					 //===PWM限幅
//		Turn_Off(pitch);																         //===检查角度是否正常
		Set_Pwm(Motor_L,Motor_R);                                    //===赋值给PWM寄存器
		
//		TIM_Cmd(TIM1, ENABLE);//启动定时器 
	}
}
