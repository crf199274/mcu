#include "timer_init.h"

void MOTO_PWM_Init(u32 arr, int psc)
{
	TIM_OCInitTypeDef TIM_OCInitSructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = arr-1;                   //自动重新装载寄存器周期的值澹ㄥ计数值澹)
	TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //时钟分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //对外部时钟进行采样的时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);              //参数初始化

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//配置pwm输出端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0| GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	//设置通道3 pwm参数
	TIM_OCInitSructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitSructure.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OCInitSructure.TIM_Pulse = 0;//占空比
	TIM_OCInitSructure.TIM_OCPolarity = TIM_OCPolarity_High;//当定时器计数值小于CCR1_Val时为高电平
	TIM_OC3Init(TIM3, &TIM_OCInitSructure);//参数初始化
	TIM_OC3PolarityConfig(TIM3, TIM_OCPreload_Enable);//开始输出pwm

	//设置通道4 pwm参数
	TIM_OCInitSructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitSructure.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OCInitSructure.TIM_Pulse = 0;//占空比
	TIM_OCInitSructure.TIM_OCPolarity = TIM_OCPolarity_High;//当定时器计数值小于CCR1_Val时为高电平
	TIM_OC4Init(TIM3, &TIM_OCInitSructure);//参数初始化
	TIM_OC4PolarityConfig(TIM3, TIM_OCPreload_Enable);//开始输出pwm

	TIM_ARRPreloadConfig(TIM3, ENABLE);//启动自动重装
	TIM_Cmd(TIM3, ENABLE);//启动定时	
}


//占空比 = TIMx_CCRx / TIMx_ARR
//moto_r：右轮电机，moto_l：左轮电机.   数值 0-3599
void MOTO_PWM_Out(u16 moto_l, u16 moto_r)
{
	
	TIM_OCInitTypeDef TIM_OCInitSructure;
	
	TIM_OCInitSructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitSructure.TIM_OutputState= TIM_OutputState_Enable;
	//CH3 右电机
	TIM_OCInitSructure.TIM_Pulse = moto_l;//占空比= ccr/3599
	TIM_OC3Init(TIM3, &TIM_OCInitSructure);//参数初始化
	TIM_OC3PolarityConfig(TIM3, TIM_OCPreload_Enable);//开始输出pwm
	
	//CH4 左电机
	TIM_OCInitSructure.TIM_Pulse = moto_r;//占空比= ccr /3599
	TIM_OC4Init(TIM3, &TIM_OCInitSructure);//参数初始化
	TIM_OC4PolarityConfig(TIM3, TIM_OCPreload_Enable);//开始输出pwm
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);//启动自动重装
	 
}	

//***************************定时器1初始化 系统每10ms处理一次中断，更新数据，更新pwm等***************************//
void Timer1_Init(int per,int pers)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = per;//自动重新装载寄存器周期的值澹ㄥ计数值澹)
	TIM_TimeBaseStructure.TIM_Prescaler = pers;//时钟分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//对外部时钟进行采样的时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //高级定时器1是用定时器功能配置这个才可以是正常的计数频率一开始的72mhz 值得注意的地方
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//参数初始化

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM1, ENABLE);//启动定时器 
}

void NVIC_TIM1_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void NVIC_Config(void)
{
	NVIC_TIM1_Config();
}

//***************************定时器2初始化 ，使用编码器功能***************************//
//左电机编码器计数
//PA0----接 编码器A相 或者电机驱动的BM3标识
//PA1----接 编码器B相 或者电机驱动的BM4标识
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}



//***************************定时器4初始化 ，使用编码器功能***************************//
//右电机编码器计数
//PB6----接 编码器A相 或者电机驱动的BM1标识
//PB7----接 编码器B相 或者电机驱动的BM2标识
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
	
	
}

int Read_Encoder(char num)
{
    int Encoder_TIM;    
    switch(num)
		{
			case 2:Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
			case 4:Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
			default:  Encoder_TIM=0;
		}
		return Encoder_TIM;
}

void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
