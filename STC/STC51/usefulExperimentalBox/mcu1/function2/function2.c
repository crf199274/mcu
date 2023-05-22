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
	PWMCFG = 0x00;					//ֹͣPWMģ��
	PWM0CR = 0x00;					//ʧ��PWM0 ���
	P_SW2 = 0x00;
	
}

void updata_LCD2(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"Freg: %-5d HZ",SW_Freq);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	
}

void updataPwmFreq(void)
{
	
	PWMSET = 0x01;					//ʹ��PWMģ��(������ʹ��ģ���������ò���Ч)
	
	P_SW2 = 0x80;
	PWMCKS = 0x0f;					//PWMʱ��Ϊϵͳʱ��
	PWMC = ((1500000/SW_Freq)&0x07ff);						//����PWM����Ϊ1000H ��PWMʱ��
	PWM0T1 = ((1500000/SW_Freq)&0x07ff)/2;				//�ڼ���ֵΪ0000H �ط�PWM0 ͨ������͵�ƽ
	PWM0T2 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM0 ͨ������ߵ�ƽ
	PWM0CR = 0x80;					//ʹ��PWM0 ���
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//����PWMģ��
	
}