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
	
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x40;				//���ö�ʱ��ʼֵ
	TH1 = 0xA2;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 0;				//��ʱ��1��ʼ��ʱ
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
			TR1 = 1;				//��ʱ��1��ʼ��ʱ
		}else{
			TR1 = 0;				//��ʱ��1ֹͣ��ʱ
		}
		
	}
}

void updata_LCD(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"VOUT: %4.1f V",VOUT);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	sprintf(buf,"IOUT: %7.2f mA",IOUT);
	LCD_ShowString(0,20,buf,BLUE,BLACK,16,0);//�ڶ��� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	sprintf(buf,"BAT: %4.1f V",BAT);
	LCD_ShowString(0,40,buf,BLUE,BLACK,16,0);//������ -- 8x16����ʾ��Ԫ��ʾASCII��
	
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
