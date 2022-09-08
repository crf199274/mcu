#include "pid.h"

static double   Proportion=0.6;                               //比例常数 Proportional Const
static double   Integral=0.1;                                 //积分常数 Integral Const
static double   Derivative=0;

void Motor_init(void)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_my);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void Motor_direction(void)
{
	if((GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==0&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)==0)||(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==1&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)==1))
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==0&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==1&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)==0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
	
	if((GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)==0&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)==0)||(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)==1&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)==1))
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	}
	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)==0&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	}
	else if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)==1&&GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	}
}

int PID_Calc(int NextPoint,int SetPoint) 
{ 
                             //微分常数 Derivative Const
	static int      LastError;                                //Error[-1]
	static int      PrevError;                                //Error[-2]
  int iError,Outpid;                                   //当前误差
	
  iError=SetPoint-NextPoint;                           //增量计算
  Outpid=(Proportion * iError)                   //E[k]项
              -(Integral * LastError)      //E[k-1]项
              +(Derivative * PrevError);   //E[k-2]项
              
  PrevError=LastError;                     //存储误差，用于下次计算
  LastError=iError;
  return(Outpid);                                      //返回增量值
}

#define Vertical_Kp  360
#define Vertical_Kd  1.4

int Vertical(float Med,float Angle,float gyro_Y) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  
  return PWM_out;
}

#define Velocity_Kp  80
#define Velocity_Ki    0.4

int Velocity(int Target,int encoder_left,int encoder_right)
{
  
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  
  Encoder_Err = ((encoder_left+encoder_right)-Target);
  
 
  EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last;
  EnC_Err_Lowout_last = EnC_Err_Lowout;   
  
  
  Encoder_S+=EnC_Err_Lowout;
  
  
  Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
  
  
  PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
  
  return PWM_out;
}

int Turn(int gyro_Z)
{
  int PWM_out;
  
  PWM_out = (-0.6)*gyro_Z;
  
  return PWM_out;
}

void SetMotorVoltageAndDirection(int *i16LeftVoltage,int *i16RightVoltage)
{
	 if(i16LeftVoltage<0)
    {	
			GPIO_SetBits(GPIOB, GPIO_Pin_13 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_12 );
      *i16LeftVoltage = (-*i16LeftVoltage);
    }
    else 
    {	
      GPIO_SetBits(GPIOB, GPIO_Pin_12 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 ); 
    }

    if(i16RightVoltage<0)
    {	
     	GPIO_SetBits(GPIOB, GPIO_Pin_15 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
      *i16RightVoltage = (-*i16RightVoltage);
    }
    else
    {
			GPIO_SetBits(GPIOB, GPIO_Pin_14 );				    
			GPIO_ResetBits(GPIOB, GPIO_Pin_15 );	      
    }
}
