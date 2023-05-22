#include "stc15.h"
#include "intrins.h"
#include "codetab.h"
#include "LQ12864.h"
#include "stdio.h"
#include "string.h"

sbit INT1 = P3^3;
sbit INT0 = P3^2;
sbit INT2 = P3^6;
sbit INT3 = P3^7;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;


#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_STOP		0x00						//ADC��ͣ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

float VIN = 0;
float VOUT = 0;
float OCC = 0;
int AD_Value = 0;
char SUB = 0;
char ADD = 0;
char OCC_ADD = 0;
char OCC_SUB = 0;
char ADC_OK = 0;
float V_MAX = 0;
int V_MAX_PWM = 0;
float V_MIN = 100;
int V_MIN_PWM = 0;

float V_INIT = 5;
float V_NOW = 0;
float V_OLD = 0;

void Delay1ms();		//@24.000MHz
void Delay10ms();		//@24.000MHz
//void Delay100ms();		//@24.000MHz
void Delay_ADC(uint16_t n);
void InitADC();

void getV_MAX_MIN();
void SetV_Value(float V_standard);
void ADDOrSUB_1V(char AB);

uint8_t ch = 0;                        //ADCͨ����

#define     MAIN_Fosc       24000000UL      //������ʱ��

#define     PWM_DUTY        10000            //����PWM�����ڣ���ֵΪʱ��������������ʹ��24.576MHZ����Ƶ����PWMƵ��Ϊ6000HZ��

#define     PWM_HIGH_MIN    32              //����PWM�������Сռ�ձȡ��û������޸ġ�
#define     PWM_HIGH_MAX    (PWM_DUTY-PWM_HIGH_MIN) //����PWM��������ռ�ձȡ��û������޸ġ�

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//sfr P3M1  = 0xB1;   //P3M1.n,P3M0.n     =00--->Standard,    01--->push-pull
//sfr P3M0  = 0xB2;   //                  =10--->pure input,  11--->open drain
//sfr AUXR  = 0x8E;
//sfr INT_CLKO = 0x8F;

sbit    P_PWM = P3^5;       //����PWM������š�
//sbit  P_PWM = P1^4;       //����PWM������š�STC15W204S

int     pwm = 0;                //����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����

u16     PWM_high,PWM_low;   //�м�������û������޸ġ�

//void    delay_ms(unsigned char ms);
void    LoadPWM(u16 i);

void updata_LCD(void)
{
	char buf[16];
	
	memset(buf,0,16);
	sprintf(buf,"VIN: %4.1f V",VIN);
	OLED_P8x16Str(0,0,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,16);
	sprintf(buf,"VOUT: %4.1f V",VOUT);
	OLED_P8x16Str(0,2,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
//	memset(buf,0,16);
//	sprintf(buf,"pwm: %5d",pwm);
//	OLED_P8x16Str(0,4,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,16);
	OCC = pwm/(PWM_DUTY*1.0)*100;
	sprintf(buf,"OCC: %7.3f %%",OCC);
	OLED_P8x16Str(0,4,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
}

void main()
{
	char buf[32];
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	InitADC();                      //��ʼ��ADC
	IE = 0xa0;                      //ʹ��ADC�ж�
																	//��ʼADת��
	P_PWM = 0;
	P3M1 &= ~(1 << 5);  //P3.5 ����Ϊ�������
	P3M0 |=  (1 << 5);

//  P1M1 &= ~(1 << 4);  //P1.4 ����Ϊ�������   STC15W204S
//  P1M0 |=  (1 << 4);

	TR0 = 0;        //ֹͣ����
	ET0 = 1;        //�����ж�
	PT0 = 1;        //�����ȼ��ж�
	TMOD &= ~0x03;  //����ģʽ,0: 16λ�Զ���װ
	AUXR |=  0x80;  //1T
	TMOD &= ~0x04;  //��ʱ
	INT_CLKO |=  0x01;  //���ʱ��

	TH0 = 0;
	TL0 = 0;
	TR0 = 1;    //��ʼ����

	INT0 = 1;
	IT0 = 1;                    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
	EX0 = 1;                    //ʹ��INT0�ж�
	INT1 = 1;
	IT1 = 1;                    //����INT1���ж����� (1:���½��� 0:�����غ��½���)
	EX1 = 1;                    //ʹ��INT1�ж�
//	INT_CLKO |= 0x10;           //(EX2 = 1)ʹ��INT2�ж�
//	INT_CLKO |= 0x20;           //(EX3 = 1)ʹ��INT3�ж�
	INT_CLKO |= 0x30;
	EA = 1;
	
	OLED_Init(); //OLED��ʼ��
	
	getV_MAX_MIN();
	
	SetV_Value(V_INIT);
	
//	sprintf(buf,"PB:%5dPS:%5d",V_MAX_PWM,V_MIN_PWM);
//	OLED_P8x16Str(0,4,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,16);
	sprintf(buf,"B:%4.1fV S:%4.1fV",V_MAX,V_MIN);
	OLED_P8x16Str(0,6,buf);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	V_NOW = V_INIT;
	
	while(1)
	{
		updata_LCD();
		if(OCC_ADD==1)
		{
			OCC_ADD = 0;
			V_OLD=V_NOW;
			V_NOW=5;
			SetV_Value(V_NOW);
		}
		if(OCC_SUB==1)
		{
			OCC_SUB = 0;
			V_OLD=V_NOW;
			V_NOW=14;
			SetV_Value(V_NOW);
		}
		if(SUB==1)
		{
			SUB=0;
			ADDOrSUB_1V(1);
		}
		if(ADD==1)
		{
			ADD=0;
			ADDOrSUB_1V(0);
		}
		Delay1ms();
	}
}

void getV_MAX_MIN()
{
	pwm = 500;
	LoadPWM(pwm);           //����PWM��װֵ
	Delay10ms();
	ADC_OK=1;
	
	while(1)
	{
		if(ADC_OK==0)
		{
			if(VOUT>V_MAX)
			{
				V_MAX = VOUT;
				V_MAX_PWM = pwm;
			}
			pwm-=10;
			LoadPWM(pwm);           //����PWM��װֵ
			Delay10ms();
			ADC_OK=1;
		}
		if(pwm<=0)
			break;
	}
	
	pwm = 6000;
	LoadPWM(pwm);           //����PWM��װֵ
	Delay10ms();
	ADC_OK=1;
	
	while(1)
	{
		if(ADC_OK==0)
		{
			if(VOUT<V_MIN)
			{
				V_MIN = VOUT;
				V_MIN_PWM = pwm;
			}
			pwm+=10;
			LoadPWM(pwm);           //����PWM��װֵ
			Delay10ms();
			ADC_OK=1;
		}
		if(pwm>=7000)
			break;
	}
	
}

/*----------------------------
ADC�жϷ������
----------------------------*/
void adc_isr() interrupt 5
{
    ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
		
		AD_Value = ADC_RES<<2;
		AD_Value = AD_Value|(ADC_RESL&0x03);
	
		if(ADC_OK)
			ADC_OK=0;
	
		if(ch==0)
			VOUT = AD_Value*(5/1024.0)*4;
		else
			VIN = AD_Value*(5/1024.0)*4;
	
		if(++ch>1)
			ch = 0;
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;

    
}

/*----------------------------
��ʼ��ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
    Delay_ADC(2);                       //ADC�ϵ粢��ʱ
}

/*----------------------------
�����ʱ
----------------------------*/
void Delay_ADC(uint16_t n)
{
    uint16_t x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

void Delay10ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 234;
	j = 115;
	do
	{
		while (--j);
	} while (--i);
}

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void SetV_Value(float V_standard)
{
	if(V_standard<5||V_standard>14)
		return;
	
	if(V_standard<V_OLD)
	{
		pwm = V_MIN_PWM;
		LoadPWM(pwm);  //����PWM��װֵ
		
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
	}
	
	pwm = V_MIN_PWM - (V_standard-V_MIN)/((V_MAX-V_MIN)/(V_MIN_PWM-V_MAX_PWM));
	LoadPWM(pwm);  //����PWM��װֵ
	Delay10ms();
	ADC_OK=1;
	
	while(1)
	{
		if(ADC_OK==0)
		{
			if((VOUT>=(V_standard-0.05))&&(VOUT<=(V_standard+0.05)))
				break;
			else if(VOUT>V_standard)
				pwm += 10;
			else if(VOUT<V_standard)
				pwm -= 10;
			if((pwm>=(V_MIN_PWM-10))||(pwm<=(V_MAX_PWM+10)))
				break;
			LoadPWM(pwm);  //����PWM��װֵ
			Delay10ms();
			ADC_OK=1;
		}
	}
	
}

void ADDOrSUB_1V(char AB)
{
	if(AB)
	{
		V_OLD=V_NOW;
		V_NOW -= 1;
		if(V_NOW<5)
			V_NOW=5;
	}
	else
	{
		V_OLD=V_NOW;
		V_NOW += 1;
		if(V_NOW>14)
			V_NOW=14;
	}
	
	SetV_Value(V_NOW);
	
}

//-----------------------------------------
//�жϷ������
void exint0() interrupt 0       //INT0�ж����
{
    SUB = 1;
		while(INT0==0);
}

//-----------------------------------------
//�жϷ������
void exint1() interrupt 2       //INT1�ж����
{
    ADD = 1;
		while(INT1==0);
}

//-----------------------------------------------
//�жϷ������
void exint2() interrupt 10          //INT2�ж����
{
	OCC_ADD = 1;
	while(INT2==0);
}

//-----------------------------------------------
//�жϷ������
void exint3() interrupt 11          //INT3�ж����
{
	OCC_SUB = 1;
	while(INT3==0);
}

//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
//void  delay_ms(unsigned char ms)
//{
//     unsigned int i;
//     do{
//          i = MAIN_Fosc / 13000;
//          while(--i)    ;
//     }while(--ms);
//}

/**************** ����PWM��װֵ���� *******************/
void    LoadPWM(u16 i)
{
    u16 j;

    if(i > PWM_HIGH_MAX)        i = PWM_HIGH_MAX;   //���д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
    if(i < PWM_HIGH_MIN)        i = PWM_HIGH_MIN;   //���д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
    j = 65536UL - PWM_DUTY + i; //����PWM�͵�ƽʱ��
    i = 65536UL - i;            //����PWM�ߵ�ƽʱ��
    EA = 0;
    PWM_high = i;   //װ��PWM�ߵ�ƽʱ��
    PWM_low  = j;   //װ��PWM�͵�ƽʱ��
    EA = 1;
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt 1
{
    if(P_PWM)
    {
        TH0 = (u8)(PWM_low >> 8);   //���������ߵ�ƽ����װ�ص͵�ƽʱ�䡣
        TL0 = (u8)PWM_low;
    }
    else
    {
        TH0 = (u8)(PWM_high >> 8);  //���������͵�ƽ����װ�ظߵ�ƽʱ�䡣
        TL0 = (u8)PWM_high;
    }
}

