#include "pid.h"
#include "Output.h"
#include "usart.h"

int Balance_Pwm=0,Velocity_Pwm=0,Turn_Pwm=0;

int Motor_L,Motor_R;

float Mechanical_angle=0; // 无超声波、电池平躺着的小车机械中值

float balance_UP_KP=1250; 	 // 小车直立环KP
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
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM满幅是7200 限制在7000
    if(Motor_L<-3500 ) Motor_L=-3500 ;
		if(Motor_L>3500 )  Motor_L=3500 ;
	  if(Motor_R<-3500 ) Motor_R=-3500 ;
		if(Motor_R>3500 )  Motor_R=3500 ;
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
目    的：经过直立环和速度环以及转向环计算出来的PWM有可能为负值
					而只能赋给定时器PWM寄存器只能是正值。故需要对PWM进行绝对值处理
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
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
	 Bias=Angle-Mechanical_balance;    							 //===求出平衡的角度中值和机械相关
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
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
		else if(1==Flag_Hou)//接收到蓝牙APP遥控数据		
		{
			Movement=-20;//设定速度
		}
		else//没有接受到任何的数据
		{	
			Movement=0;
		}
   //=============速度PI控制器=======================//	
		Encoder_Least =(Encoder_L+Encoder_R)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.7;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.3;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===积分限幅	
		Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI;        //===速度控制	
	  if(pitch<-40||pitch>40) 			Encoder_Integral=0;     						//===电机关闭后清除积分
	  return Velocity;
}

int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=44,Kp=20,Kd=0;     
	  //=============遥控左右旋转部分=======================//
	  //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
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
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向	速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}
