#include "function3.h"

void updata_LCD3(void);
void updataPwmOcc(unsigned int i);

void function3()
{
	PWMSET = 0x01;					//ʹ��PWMģ��(������ʹ��ģ���������ò���Ч)
	
	P_SW2 = 0x80;
	PWMCKS = 0x00;					//PWMʱ��Ϊϵͳʱ��
	PWMC = 1000;						//����PWM����Ϊ1000H ��PWMʱ��
	PWM1T1 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM1 ͨ������͵�ƽ
	PWM1T2 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM1 ͨ������ߵ�ƽ
	PWM2T1 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM2 ͨ������͵�ƽ
	PWM2T2 = 0x0000;				//�ڼ���ֵΪ0000H �ط�PWM2 ͨ������ߵ�ƽ
	PWM1CR = 0x80;					//ʹ��PWM1 ���
	PWM2CR = 0x80;					//ʹ��PWM2 ���
	P_SW2 = 0x00;
	
	PWMCFG = 0x01;					//����PWMģ��
	
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
	PWMCFG = 0x00;					//ֹͣPWMģ��
	PWM1CR = 0x00;					//ʧ��PWM1 ���
	PWM2CR = 0x00;					//ʧ��PWM2 ���
	P_SW2 = 0x00;
	
}

void updata_LCD3(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"OCC: %-3d%%",M_Freq);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
}

void updataPwmOcc(unsigned int i)
{
	
	P_SW2=0X80;
	
	PWMCFG = 0x00;					//�ر�PWMģ��
	
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
	
	PWMCFG = 0x01;					//����PWMģ��
}
