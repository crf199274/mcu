#include "function3.h"

void updata_LCD3(void);
void updataPwmOcc(unsigned int i);

void function3()
{
	PWMSET = 0x01;					//使能PWM模块(必须先使能模块后面的设置才有效)
	
	P_SW2 = 0x80;
	PWMCKS = 0x00;					//PWM时钟为系统时钟
	PWMC = 1000;						//设置PWM周期为1000H 个PWM时钟
	PWM1T1 = 0x0000;				//在计数值为0000H 地方PWM1 通道输出低电平
	PWM1T2 = 0x0000;				//在计数值为0000H 地方PWM1 通道输出高电平
	PWM2T1 = 0x0000;				//在计数值为0000H 地方PWM2 通道输出低电平
	PWM2T2 = 0x0000;				//在计数值为0000H 地方PWM2 通道输出高电平
	PWM1CR = 0x80;					//使能PWM1 输出
	PWM2CR = 0x80;					//使能PWM2 输出
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//启动PWM模块
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	while(1)
	{
		
		updata_LCD3();
		
		if(falg2)
		{
			falg2 = 0;
			updataPwmOcc(M_Freq);
		}
		
		if(MODE!=2)
			break;
	}
	
	P_SW2 = 0x80;
	PWMCFG = 0x00;					//停止PWM模块
	PWM1CR = 0x00;					//失能PWM1 输出
	PWM2CR = 0x00;					//失能PWM2 输出
	P_SW2 = 0x00;
	
}

void updata_LCD3(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"OCC: %-3d%%",M_Freq);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//第一行 -- 8x16的显示单元显示ASCII码
	
}

void updataPwmOcc(unsigned int i)
{
	
	P_SW2=0X80;
	
	PWMCFG = 0x00;					//关闭PWM模块
	
	if(MODE2)
	{
		PWM1T1 = (int)(1000*(i/100.0));
		PWM1T2 = 0x0000;
		
		PWM2T1 = 0x0000;
		PWM2T2 = 0x0000;
	}else{
		PWM1T1 = 0x0000;
		PWM1T2 = 0x0000;
		
		PWM2T1 = (int)(1000*(i/100.0));
		PWM2T2 = 0x0000;
	}
	
	PWMCFG = 0x01;					//启动PWM模块
}
