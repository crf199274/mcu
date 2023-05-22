#include "stc15.h"
#include "lcd_init.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

#define I_Value				0.03223
#define VCC_I					0.03545
#define VCC_O					0.03545
#define DA						0.00645
#define DS						0.00645

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

int AD_Value = 0;

void updata_LCD(void);
void Delay_ADC(uint16_t n);
void InitADC(void);

void Delay1ms();		//@24.000MHz

//����
float I = 0;
//�����ѹ
float VIN = 0;
//�����ѹ
float VOUT = 0;
//D+
float D_A = 0;
//D-
float D_S = 0;
//����
float PW = 0;

uint8_t ch = 0;                        //ADC�����̨�o?

void main()
{
	P0M0=0;
	P0M1=0;
	P1M0=0;
	P1M1=0;
	P2M0=0;
	P2M1=0;
	P3M0=0;
	P3M1=0;
	P4M0=0;
	P4M1=0;
	P5M0=0;
	P5M1=0;
	
	LCD_Init();//LCD��ʼ��
	
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	InitADC();
	IE = 0xa0;                      //ʹ��ADC�ж�
																	//��ʼADת��
	
	while(1)
	{
		updata_LCD();
		Delay1ms();
	}	
}

void CP_GET(char *p)
{
	memset(p,0,20);
	
	if(fabs(D_A-D_S)<=0.1)
	{
		if(D_A>=0.4&&D_A<=1.0)//12V
			sprintf(p,"%-6.5s","QC2.0");
		else if(D_A>=0.3)
			sprintf(p,"%-6.5s","DCP");
		else
			sprintf(p,"%-6.5s","???");
	}
	else if(D_A>3.0&&(D_S>0.4&&D_S<1.0))//9V
		sprintf(p,"%-6.5s","QC2.0");
	else if(D_A>3.0&&D_S>3.0)//20V
		sprintf(p,"%-6.5s","QC2.0");
	else if((D_A>0.4&&D_A<1.0)&&(D_S>0.0&&D_S<0.1))//5V
		sprintf(p,"%-6.5s","QC2.0");
	else if((D_A>0.4&&D_A<1.0)&&D_S>3.0)
		sprintf(p,"%-6.5s","QC3.0");
	else
		sprintf(p,"%-6.5s","???");
	
}

void updata_LCD(void)
{
	char buf[20];
	
	CP_GET(buf);
	LCD_ShowString(104,0,buf,YELLOW,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	sprintf(buf,"VIN: %4.1f V",VIN);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	sprintf(buf,"VOUT: %4.1f V",VOUT);
	LCD_ShowString(0,20,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	PW=VIN*I;
	sprintf(buf,"I: %4.1f A P: %4.1f W",I,PW);
	LCD_ShowString(0,40,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
	memset(buf,0,20);
	sprintf(buf,"DA: %3.1f V DS: %3.1f V",D_A,D_S);
	LCD_ShowString(0,60,buf,BLUE,BLACK,16,0);//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
	
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

/*----------------------------
��ʼ��ADC
----------------------------*/
void InitADC(void)
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
    Delay_ADC(2);                       //ADC�ϵ粢��ʱ
}

/*----------------------------
ADC�жϷ������
----------------------------*/
void adc_isr() interrupt 5
{
    ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
		
		AD_Value = ADC_RES<<2;
		AD_Value = AD_Value|(ADC_RESL&0x03);
	
		if(ch==0)
			I = AD_Value*I_Value;
		else if(ch==1)
			VIN = AD_Value*VCC_I;
		else if(ch==2)
			VOUT = AD_Value*VCC_O;
		else if(ch==3)
			D_A = AD_Value*DA;
		else if(ch==4)
			D_S = AD_Value*DS;
	
		if(++ch>4)
			ch = 0;
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;

    
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
