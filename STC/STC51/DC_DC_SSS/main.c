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


#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_STOP		0x00						//ADC暂停
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

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

uint8_t ch = 0;                        //ADC通道号

#define     MAIN_Fosc       24000000UL      //定义主时钟

#define     PWM_DUTY        10000            //定义PWM的周期，数值为时钟周期数，假如使用24.576MHZ的主频，则PWM频率为6000HZ。

#define     PWM_HIGH_MIN    32              //限制PWM输出的最小占空比。用户请勿修改。
#define     PWM_HIGH_MAX    (PWM_DUTY-PWM_HIGH_MIN) //限制PWM输出的最大占空比。用户请勿修改。

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

//sfr P3M1  = 0xB1;   //P3M1.n,P3M0.n     =00--->Standard,    01--->push-pull
//sfr P3M0  = 0xB2;   //                  =10--->pure input,  11--->open drain
//sfr AUXR  = 0x8E;
//sfr INT_CLKO = 0x8F;

sbit    P_PWM = P3^5;       //定义PWM输出引脚。
//sbit  P_PWM = P1^4;       //定义PWM输出引脚。STC15W204S

int     pwm = 0;                //定义PWM输出高电平的时间的变量。用户操作PWM的变量。

u16     PWM_high,PWM_low;   //中间变量，用户请勿修改。

//void    delay_ms(unsigned char ms);
void    LoadPWM(u16 i);

void updata_LCD(void)
{
	char buf[16];
	
	memset(buf,0,16);
	sprintf(buf,"VIN: %4.1f V",VIN);
	OLED_P8x16Str(0,0,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"VOUT: %4.1f V",VOUT);
	OLED_P8x16Str(0,2,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
//	memset(buf,0,16);
//	sprintf(buf,"pwm: %5d",pwm);
//	OLED_P8x16Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	OCC = pwm/(PWM_DUTY*1.0)*100;
	sprintf(buf,"OCC: %7.3f %%",OCC);
	OLED_P8x16Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
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
	
	InitADC();                      //初始化ADC
	IE = 0xa0;                      //使能ADC中断
																	//开始AD转换
	P_PWM = 0;
	P3M1 &= ~(1 << 5);  //P3.5 设置为推挽输出
	P3M0 |=  (1 << 5);

//  P1M1 &= ~(1 << 4);  //P1.4 设置为推挽输出   STC15W204S
//  P1M0 |=  (1 << 4);

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

	INT0 = 1;
	IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	EX0 = 1;                    //使能INT0中断
	INT1 = 1;
	IT1 = 1;                    //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	EX1 = 1;                    //使能INT1中断
//	INT_CLKO |= 0x10;           //(EX2 = 1)使能INT2中断
//	INT_CLKO |= 0x20;           //(EX3 = 1)使能INT3中断
	INT_CLKO |= 0x30;
	EA = 1;
	
	OLED_Init(); //OLED初始化
	
	getV_MAX_MIN();
	
	SetV_Value(V_INIT);
	
//	sprintf(buf,"PB:%5dPS:%5d",V_MAX_PWM,V_MIN_PWM);
//	OLED_P8x16Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"B:%4.1fV S:%4.1fV",V_MAX,V_MIN);
	OLED_P8x16Str(0,6,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
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
	LoadPWM(pwm);           //计算PWM重装值
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
			LoadPWM(pwm);           //计算PWM重装值
			Delay10ms();
			ADC_OK=1;
		}
		if(pwm<=0)
			break;
	}
	
	pwm = 6000;
	LoadPWM(pwm);           //计算PWM重装值
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
			LoadPWM(pwm);           //计算PWM重装值
			Delay10ms();
			ADC_OK=1;
		}
		if(pwm>=7000)
			break;
	}
	
}

/*----------------------------
ADC中断服务程序
----------------------------*/
void adc_isr() interrupt 5
{
    ADC_CONTR &= !ADC_FLAG;         //清除ADC中断标志
		
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
初始化ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
    Delay_ADC(2);                       //ADC上电并延时
}

/*----------------------------
软件延时
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
		LoadPWM(pwm);  //计算PWM重装值
		
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
	LoadPWM(pwm);  //计算PWM重装值
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
			LoadPWM(pwm);  //计算PWM重装值
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
//中断服务程序
void exint0() interrupt 0       //INT0中断入口
{
    SUB = 1;
		while(INT0==0);
}

//-----------------------------------------
//中断服务程序
void exint1() interrupt 2       //INT1中断入口
{
    ADD = 1;
		while(INT1==0);
}

//-----------------------------------------------
//中断服务程序
void exint2() interrupt 10          //INT2中断入口
{
	OCC_ADD = 1;
	while(INT2==0);
}

//-----------------------------------------------
//中断服务程序
void exint3() interrupt 11          //INT3中断入口
{
	OCC_SUB = 1;
	while(INT3==0);
}

//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
//void  delay_ms(unsigned char ms)
//{
//     unsigned int i;
//     do{
//          i = MAIN_Fosc / 13000;
//          while(--i)    ;
//     }while(--ms);
//}

/**************** 计算PWM重装值函数 *******************/
void    LoadPWM(u16 i)
{
    u16 j;

    if(i > PWM_HIGH_MAX)        i = PWM_HIGH_MAX;   //如果写入大于最大占空比数据，则强制为最大占空比。
    if(i < PWM_HIGH_MIN)        i = PWM_HIGH_MIN;   //如果写入小于最小占空比数据，则强制为最小占空比。
    j = 65536UL - PWM_DUTY + i; //计算PWM低电平时间
    i = 65536UL - i;            //计算PWM高电平时间
    EA = 0;
    PWM_high = i;   //装载PWM高电平时间
    PWM_low  = j;   //装载PWM低电平时间
    EA = 1;
}

/********************* Timer0中断函数************************/
void timer0_int (void) interrupt 1
{
    if(P_PWM)
    {
        TH0 = (u8)(PWM_low >> 8);   //如果是输出高电平，则装载低电平时间。
        TL0 = (u8)PWM_low;
    }
    else
    {
        TH0 = (u8)(PWM_high >> 8);  //如果是输出低电平，则装载高电平时间。
        TL0 = (u8)PWM_high;
    }
}

