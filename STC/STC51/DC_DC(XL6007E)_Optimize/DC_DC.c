#include "DC_DC.h"

struct DC_DC DC_DC_1 = {0,0,VOL_MIN,0,0,0,0};

/* 求电压值对应的PWM值 */
/* arg 		vol			:voltage_value
/* return		pwm_value
*/
uint16_t voltageToPWM(float vol)
{
	if(vol<VOL_MIN||vol>VOL_MAX)
		return DC_DC_1.pwm;
	
	return pwm_map(1.0-(vol-VOL_LIMIT_MIN)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN));
}


/* 根据占空比计算pwm的值 */
/* arg occ :占空比 */
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

/* 加载PWM */
/* brief 		用于pwm_value更新时加载到硬件上 */
/* arg		pwm_value 		:pwm的值 */
void LoadPWM(uint16_t pwm_value)
{
	if(pwm_value>CYCLE)
		pwm_value = CYCLE;
	
	P_SW2=0X80;
	
  PWMCFG = 0x00;					//关闭PWM模块
	
	PWM0T1 = pwm_value;
	PWM0T2 = 0x0000;
	
	PWMCFG = 0x01;					//启动PWM模块
	
}


/* PWM初始化*/
/* brief		为控制XL6007E1提供一路PWM */
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
