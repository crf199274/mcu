#include "DC_DC.h"
#include "delay.h"
#include "lcd.h"

uint16_t     pwm;                //定义PWM输出高电平的时间的变量。用户操作PWM的变量。

sbit    P_PWM = P3^5;       //定义PWM输出引脚。

uint16_t     PWM_high,PWM_low;   //中间变量，用户请勿修改。

float VOL = VOL_MIN;

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
		return PWM_HIGH_MIN;
	else if(occ<=0)
		return PWM_HIGH_MAX;
	else
		return PWM_HIGH_MAX*occ;
		
}

void LoadPWM(uint16_t i)
{
    uint16_t j;

    if(i > PWM_HIGH_MAX)        i = PWM_HIGH_MAX;   //如果写入大于最大占空比数据，则强制为最大占空比。
    if(i < PWM_HIGH_MIN)        i = PWM_HIGH_MIN;   //如果写入小于最小占空比数据，则强制为最小占空比。
    j = 65536UL - PWM_DUTY + i; //计算PWM低电平时间
    i = 65536UL - i;            //计算PWM高电平时间
    EA = 0;
    PWM_high = i;   //装载PWM高电平时间
    PWM_low  = j;   //装载PWM低电平时间
    EA = 1;
}


void DC_DC_PWM_Init(void)
{
	P_PWM = 0;
	P3M1 &= ~(1 << 5);  //P3.5 设置为推挽输出
	P3M0 |=  (1 << 5);

	
	TR0 = 0;        //停止计数
	ET0 = 1;        //允许中断
	PT0 = 1;        //高优先级中断
	TMOD &= ~0x03;  //工作模式,0: 16位自动重装
	AUXR |=  0x80;  //1T
	TMOD &= ~0x04;  //定时
	INT_CLKO |=  0x01;  //输出时钟

	
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;    //开始运行
	
	pwm = VOL_OCC(VOL);
	LoadPWM(pwm);

}

void timer0_int (void) interrupt 1
{
    if(P_PWM)
    {
        TH0 = (uint8_t)(PWM_low >> 8);   //如果是输出高电平，则装载低电平时间。
        TL0 = (uint8_t)PWM_low;
    }
    else
    {
        TH0 = (uint8_t)(PWM_high >> 8);  //如果是输出低电平，则装载高电平时间。
        TL0 = (uint8_t)PWM_high;
    }
}
