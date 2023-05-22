#include "pwm.h"

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

void PWM_INIT(void)
{
	CCON = 0x00;
	CMOD = 0x08;                                //PCAʱ��Ϊϵͳʱ��
	CL = 0x00;
	CH = 0x00;
	
	//����10λPWM����
	CCAPM1 = 0x42;                              //PCAģ��2ΪPWM����ģʽ
	PCA_PWM1 = 0xfc;                            //PCAģ��2���10λPWM
	CCAP1L = 0xff;                              //PWMռ�ձ�Ϊ96.875%[(400H-20H)/400H]
	CCAP1H = 0xff;
	CR = 1;                                     //����PCA��ʱ�� 

}

void PWM_OCC_Load(float occ)
{
	uint8_t pwmL,pwmH;
	uint16_t pwm;
	
	CR = 0;
	
	CL = 0x00;
	CH = 0x00;
	
	if(occ<0)
		occ=0;
	else if(occ>1)
		occ=1;
	
	pwm = 1024-1024*occ;
	
	pwmL = pwm;
	pwmH = pwm>>8;
	
	PCA_PWM1=0xc0|(pwmH<<4)|(pwmH<<2);
	CCAP1L=pwmL;
	CCAP1H=pwmL;
	
	CR = 1;
	
}