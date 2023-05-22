#include "function2.h"

void updata_LCD2(void);
void updataPwmFreq(void);

void function2()
{
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	while(1)
	{
		
		updata_LCD2();
		
		if(falg1)
		{
			falg1 = 0;
			updataPwmFreq();
		}
		
		if(MODE!=1)
			break;
	}
	
	P_SW2 = 0x80;
	PWMCFG = 0x00;					//停止PWM模块
	PWM0CR = 0x00;					//失能PWM0 输出
	P_SW2 = 0x00;
	
}

void updata_LCD2(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"Freg: %-5d HZ",SW_Freq);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//第一行 -- 8x16的显示单元显示ASCII码
	
	
}

void updataPwmFreq(void)
{
	
	PWMSET = 0x01;					//使能PWM模块(必须先使能模块后面的设置才有效)
	
	P_SW2 = 0x80;
	PWMCKS = 0x0f;					//PWM时钟为系统时钟
	PWMC = ((1500000/SW_Freq)&0x07ff);						//设置PWM周期为1000H 个PWM时钟
	PWM0T1 = ((1500000/SW_Freq)&0x07ff)/2;				//在计数值为0000H 地方PWM0 通道输出低电平
	PWM0T2 = 0x0000;				//在计数值为0000H 地方PWM0 通道输出高电平
	PWM0CR = 0x80;					//使能PWM0 输出
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//启动PWM模块
	
}