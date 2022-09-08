#include "stepper.h"            
#include "stdelay.h" 

/**
  * @brief  配置STEPPER_TIMx输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void STEPPER_TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(STEPPER_TIM_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = STEPPER_TIM_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(STEPPER_TIM_GPIO_PORT,&GPIO_InitStructure);

}

/**
  * @brief  配置STEPPER_TIMx输出的PWM信号的模式，如周期、极性,1ms一个PWM周期
  * @param  无
  * @retval 无
  */
static void STEPPER_TIMx_Mode_Config(void)
{
	RCC_APB1PeriphClockCmd(STEPPER_TIM_CLK, ENABLE);							//TIM时钟使能
	
	TIM_InternalClockConfig(STEPPER_TIMx);												//内部时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;										//ARR,当定时器从0计数到100，为一个定时周期
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;							  	//PSC,设置预分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(STEPPER_TIMx, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);													//给结构体赋初值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;								//配置为PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				//当定时器计数值小于CCR_Val时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//使能输出
	TIM_OCInitStructure.TIM_Pulse = 500;														//CCR设置	
	TIM_OC1Init(STEPPER_TIMx, &TIM_OCInitStructure);								//TIMx通道1初始化PWM
	
	TIM_Cmd(STEPPER_TIMx, ENABLE);				//使能定时器
}

/**
  * @brief  PWM占空比设置
  * @param  占空比值：0-100
  * @retval 无
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}


/**
  * @brief  STEPPER_PWM初始化
  * @param  无
  * @retval 无
  */
void STEPPER_PWM_Init(void)
{
	STEPPER_TIMx_GPIO_Config();
	STEPPER_TIMx_Mode_Config();	
}

/**
  * @brief  STEPPER引脚配置
  * @param  无
  * @retval 无
  */
void STEPPER_GPIO_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
		//ENABLE与DIR引脚初始化
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = ENABLE_Pin | DIR_Pin;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(ENABLE_GPIO_Port,&GPIO_InitStructure);
	
		//MS1,2,3引脚初始化
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = MS1_Pin | MS2_Pin | MS3_Pin;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(MS2_GPIO_Port,&GPIO_InitStructure);
	
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC,&GPIO_InitStructure);
}

/**
  * @brief  STEPPER引脚初始化，16细分
  * @param  无
  * @retval 无
  */
void STEPPER_GPIO_Init(void)
{
	STEPPER_GPIO_Config();
	MS1_HIGHT;
	MS2_HIGHT;
	MS3_HIGHT;
	DIR_CW;
	A4988_DISABLE;
}

/**
  * @brief  旋转脚设置
  * @param  45度的倍数
  * @retval 无
  */
void STEPPER_Rotation(uint16_t angle)
{
	uint16_t time;
	time = 16*angle/1.8;
	A4988_ENABLE;
	stdelay_ms(time);
	A4988_DISABLE;
}

void STEPPER_Stop(void)
{
	A4988_DISABLE;
}

void STEPPER_INIT(void)
{
	STEPPER_PWM_Init();
	
	STEPPER_GPIO_Init();
}

