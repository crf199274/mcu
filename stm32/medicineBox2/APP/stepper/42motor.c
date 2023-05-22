#include "42motor.h"
#include "stdelay.h"
#include "tim_interrupt.h"

/*************************************************
������: _42motorPinConfig()
����: ��ʼ���������IO
��ڲ���: _Bool MS1,_Bool MS2,_Bool MS3 ����ϸ��
����ֵ: ��  
*************************************************/
void _42motorPinConfig(_Bool MS1,_Bool MS2,_Bool MS3)
{
	if(MS1)
	{
		GPIO_SetBits(motorGPIO_Port,motorPin_MS1);
	}else{
		GPIO_ResetBits(motorGPIO_Port,motorPin_MS1);
	}
	
	if(MS2)
	{
		GPIO_SetBits(motorGPIO_Port,motorPin_MS2);
	}else{
		GPIO_ResetBits(motorGPIO_Port,motorPin_MS2);
	}
	
	if(MS3)
	{
		GPIO_SetBits(motorGPIO_Port,motorPin_MS3);
	}else{
		GPIO_ResetBits(motorGPIO_Port,motorPin_MS3);
	}
	
//	GPIO_ResetBits(motorGPIO_Port,motorPin_ENABLE);
	GPIO_SetBits(motorGPIO_Port,motorPin_ENABLE);
	
	GPIO_SetBits(motorGPIO_Port,motorPin_DIR);
	
	//ֹͣPWM
//	TIM_SelectOCxM(TIM3,TIM_Channel_1,TIM_ForcedAction_Active);
//	TIM_Cmd(TIM3,ENABLE);
	
}

/*************************************************
������: _42motorInit()
����: ��ʼ���������
��ڲ���: _Bool MS1,_Bool MS2,_Bool MS3 ����ϸ��
����ֵ: ��  
*************************************************/
void _42motorInit(_Bool MS1,_Bool MS2,_Bool MS3)
{
	RCC_APB2PeriphClockCmd(motorClock,ENABLE);
	
	GPIO_InitTypeDef  _42motor;
	_42motor.GPIO_Speed=GPIO_Speed_50MHz;
	_42motor.GPIO_Mode=GPIO_Mode_Out_PP;
	_42motor.GPIO_Pin=motorPin_DIR|motorPin_MS1|motorPin_MS2|motorPin_MS3|motorPin_ENABLE;
	GPIO_Init(motorGPIO_Port,&_42motor);
	
	PWM_init(200,720);
	
	_42motorPinConfig(MS1,MS2,MS3);
	
}

/*************************************************
������: step90()
����: ������90��
��ڲ���: uint8_t step  �������ٲ�
����ֵ: uint8_t  
*************************************************/
uint8_t step90T(uint8_t step)
{
	step = step*16;
	
	if(step>64)
	{
		return 1;
	}
	
	GPIO_SetBits(motorGPIO_Port,motorPin_DIR);
	
	GPIO_ResetBits(motorGPIO_Port,motorPin_ENABLE);
//	TIM_SelectOCxM(TIM3,TIM_Channel_1,TIM_OCMode_PWM1);
//	TIM_Cmd(TIM3,ENABLE);
	stdelay_100ms(step);
//	TIM_SelectOCxM(TIM3,TIM_Channel_1,TIM_ForcedAction_Active);
//	TIM_Cmd(TIM3,ENABLE);
	GPIO_SetBits(motorGPIO_Port,motorPin_ENABLE);
	
	return 0;
}

/*************************************************
������: step90F()
����: ������90��
��ڲ���: uint8_t step  �������ٲ�
����ֵ: uint8_t  
*************************************************/
uint8_t step90F(uint8_t step)
{
	step = step*16;
	
	if(step>64)
	{
		return 1;
	}
	
	GPIO_ResetBits(motorGPIO_Port,motorPin_DIR);
	
	GPIO_ResetBits(motorGPIO_Port,motorPin_ENABLE);
//	TIM_SelectOCxM(TIM3,TIM_Channel_1,TIM_OCMode_PWM1);
//	TIM_Cmd(TIM3,ENABLE);
	stdelay_100ms(step);
//	TIM_SelectOCxM(TIM3,TIM_Channel_1,TIM_ForcedAction_Active);
//	TIM_Cmd(TIM3,ENABLE);
	GPIO_SetBits(motorGPIO_Port,motorPin_ENABLE);
	
	return 0;
	
}
