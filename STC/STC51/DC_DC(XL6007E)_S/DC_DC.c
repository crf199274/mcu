#include "DC_DC.h"
#include "delay.h"
#include "oled.h"

uint16_t     pwm;                //定义PWM输出高电平的时间的变量。用户操作PWM的变量。

float VOL = VOL_MIN;

int pwm_error = 0;

uint16_t VOL_OCC(float vol)
{
	if(vol<VOL_MIN||vol>VOL_MAX)
		return pwm;
	
	return pwm_map(1.0-(vol-VOL_LIMIT_MIN)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN));
}


//ooc>=0 | occ<=1
uint16_t pwm_map(float occ)
{
	if(occ>=1)
		return 0;
	else if(occ<=0)
		return CYCLE;
	else
		return CYCLE*occ;
		
}

void LoadPWM(uint16_t i)
{
	if(i>CYCLE)
		i = CYCLE;
	
	P_SW2=0X80;
	
  PWMCFG = 0x00;					//关闭PWM模块
	
	PWM0T1 = i;
	PWM0T2 = 0x0000;
	
	PWMCFG = 0x01;					//启动PWM模块
	
}


void DC_DC_PWM_Init(void)
{
	PWMSET = 0x01;					//使能PWM模块(必须先使能模块后面的设置才有效)
	
	P_SW2 = 0x80;
	PWMCKS = 0x00;					//PWM时钟为系统时钟
	PWMC = CYCLE;						//设置PWM周期为1000H 个PWM时钟
	PWM0T1 = 0x0000;				//在计数值为0000H 地方PWM0 通道输出低电平
	PWM0T2 = 0x0000;				//在计数值为0000H 地方PWM0 通道输出高电平
	PWM0CR = 0x80;					//使能PWM0 输出
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//启动PWM模块

}
