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
	if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)   //ʱ�䵽��
	{
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//���ж�
		
//		TIM_Cmd(TIM1, DISABLE);//ֹͣ��ʱ�� 
		
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		
		MPU_Get_Gyroscope();
		
		Balance_Pwm = balance_UP(pitch,Mechanical_angle,(short)gyroy);
		
		Encoder_L = Read_Encoder(2);
		Encoder_R = -Read_Encoder(4);
		
		Velocity_Pwm=velocity(Encoder_L,Encoder_R);       //===�ٶȻ�PID����	 
		if(1==Flag_Left||1==Flag_Right)    
		Turn_Pwm =turn(Encoder_L,Encoder_R,(short)gyroz);        //===ת��PID����
		else Turn_Pwm=(0.5)*(short)gyroz;																	 //===����У��С����ԭ��תȦ������
		Motor_L=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===�������ֵ������PWM
		Motor_R=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===�������ֵ������PWM
	  Xianfu_Pwm();  																					 //===PWM�޷�
//		Turn_Off(pitch);																         //===���Ƕ��Ƿ�����
		Set_Pwm(Motor_L,Motor_R);                                    //===��ֵ��PWM�Ĵ���
		
//		TIM_Cmd(TIM1, ENABLE);//������ʱ�� 
	}
}
