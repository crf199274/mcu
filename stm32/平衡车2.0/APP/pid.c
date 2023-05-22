#include "pid.h"
#include "Output.h"
#include "usart.h"

int Balance_Pwm=0,Velocity_Pwm=0,Turn_Pwm=0;

int Motor_L,Motor_R;

float Mechanical_angle=0; // �޳����������ƽ���ŵ�С����е��ֵ

float balance_UP_KP=1250; 	 // С��ֱ����KP
float balance_UP_KD=1.8;// 

float velocity_KP=100;
float velocity_KI=0.5;

void Motor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_my);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void MotorR_direction(uint8_t Direction)
{
	switch(Direction)
	{
		case 0: GPIO_ResetBits(GPIOB,GPIO_Pin_14);
						GPIO_SetBits(GPIOB,GPIO_Pin_15);
						break;
		case 1:	GPIO_SetBits(GPIOB,GPIO_Pin_14);
						GPIO_ResetBits(GPIOB,GPIO_Pin_15);
						break;
	}
}

void MotorL_direction(uint8_t Direction)
{
	switch(Direction)
	{
		case 0: GPIO_ResetBits(GPIOB,GPIO_Pin_12);
						GPIO_SetBits(GPIOB,GPIO_Pin_13);
						break;
		case 1:	GPIO_SetBits(GPIOB,GPIO_Pin_12);
						GPIO_ResetBits(GPIOB,GPIO_Pin_13);
						break;
	}
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM������7200 ������7000
    if(Motor_L<-3500 ) Motor_L=-3500 ;
		if(Motor_L>3500 )  Motor_L=3500 ;
	  if(Motor_R<-3500 ) Motor_R=-3500 ;
		if(Motor_R>3500 )  Motor_R=3500 ;
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
Ŀ    �ģ�����ֱ�������ٶȻ��Լ�ת�򻷼��������PWM�п���Ϊ��ֵ
					��ֻ�ܸ�����ʱ��PWM�Ĵ���ֻ������ֵ������Ҫ��PWM���о���ֵ����
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
		if(moto1<0)			MotorL_direction(0);
		else 	          MotorL_direction(1);
		TIM3->CCR3=myabs(moto1);
		if(moto2<0)	MotorR_direction(0);
		else        MotorR_direction(1);
		TIM3->CCR4=myabs(moto2);	
}

int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===���ƽ��ĽǶ���ֵ�ͻ�е���
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
		if(1==Flag_Qian)				
		{
			Movement=20;
		}
		else if(1==Flag_Hou)//���յ�����APPң������		
		{
			Movement=-20;//�趨�ٶ�
		}
		else//û�н��ܵ��κε�����
		{	
			Movement=0;
		}
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(Encoder_L+Encoder_R)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.7;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.3;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===�����޷�	
		Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI;        //===�ٶȿ���	
	  if(pitch<-40||pitch>40) 			Encoder_Integral=0;     						//===����رպ��������
	  return Velocity;
}

int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=44,Kp=20,Kd=0;     
	  //=============ң��������ת����=======================//
	  //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
  	if(1==Flag_Left||1==Flag_Right)                      
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left+encoder_right);      
			Turn_Convert=55/Encoder_temp;
			if(Turn_Convert<0.6)Turn_Convert=0.6;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}			
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 
		else Turn_Target=0;
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת��	�ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}
