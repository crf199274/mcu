#include "stc8h.h"
#include "LCD.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"

sbit lowPowerLED = P1^2;

unsigned int time_cnt1 = 0;

void updata_LCD(void);

void main()
{
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x04;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x40;				//设置定时初始值
	TH1 = 0xA2;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 0;				//定时器1开始计时
	ET1 = 1;
	
	adcInit();
	
	EA = 1;
	
	LCD_Init();
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	while(1)
	{
		updata_LCD();
		
		if(BAT<3.7)
		{
			TR1 = 1;				//定时器1开始计时
		}else{
			TR1 = 0;				//定时器1停止计时
		}
		
	}
}

void updata_LCD(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"VOUT: %4.1f V",VOUT);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,20);
	sprintf(buf,"IOUT: %7.2f mA",IOUT);
	LCD_ShowString(0,20,buf,BLUE,BLACK,16,0);//第二行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,20);
	sprintf(buf,"BAT: %4.1f V",BAT);
	LCD_ShowString(0,40,buf,BLUE,BLACK,16,0);//第三行 -- 8x16的显示单元显示ASCII码
	
}

void TM1_Isr() interrupt 3
{
	time_cnt1++;
	if(time_cnt1>=500)
	{
    lowPowerLED = !lowPowerLED; 
		time_cnt1 = 0;
	}		
}
