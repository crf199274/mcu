#include "main.h"
#include "OLED.h"
#include "DC_DC.h"
#include "adc.h"
#include "key.h"
#include "delay.h"
#include "math.h"

void main()
{
	//初始化IO
	IO_Init();
	
	//初始OLED
	oled_init();
	
	//初始化按键
	keyInit();
	
	//初始化ADC
	adcInit();
	
	//初始化DC_DC
	DC_DC_PWM_Init();
	
	//电压校准并设置初始电压
	voltageVerify();
	
	//主循环
	while(1)
	{
		mainTask();
	}
	
}

/*						IO模式
*		PxM0				PxM1		MODE
*		1						1				开漏输出
*		1						0				高阻输入
*		0						1				推挽输出
*		0						0				准双向
*/

//IO口初始化函数
void IO_Init(void)
{
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x01;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
}

//计算误差设计初始电压
void voltageVerify(void)
{
	tip_OLED("Voltage verify");
	
	DC_DC_1.pwm = voltageToPWM(DC_DC_1.vol);
	LoadPWM(DC_DC_1.pwm);
	
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	Delay500ms();
	
	if(DC_DC_1.Vout<DC_DC_1.vol)
	{
		DC_DC_1.pwm_error = -((fabs(DC_DC_1.vol-DC_DC_1.Vout)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN))*CYCLE);
	}else{
		
		DC_DC_1.pwm_error = fabs(DC_DC_1.vol-DC_DC_1.Vout)/(VOL_LIMIT_MAX-VOL_LIMIT_MIN)*CYCLE;
	}
	
	oled_screen_fill(0x00);
	tip_OLED("OK");
	
	Delay500ms();
	Delay500ms();
	
	oled_screen_fill(0x00);
	
	DC_DC_1.pwm = voltageToPWM(DC_DC_1.vol) + DC_DC_1.pwm_error;
	LoadPWM(DC_DC_1.pwm);
	
}

//OLED显示电流电压信息
void updata_OLED(void)
{
	char buf[16];
	
	memset(buf,0,16);
	sprintf(buf,"VIN: %5.2f V",DC_DC_1.Vin);
	oled_write_size_16x8(0,0,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"VOUT: %5.2f V",DC_DC_1.Vout);
	oled_write_size_16x8(0,2,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"IIN: %5.2f A",DC_DC_1.Iin);
	oled_write_size_16x8(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"IOUT: %5.2f A",DC_DC_1.Iout);
	oled_write_size_16x8(0,6,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
}

//显示提示信息
void tip_OLED(char str[])
{
	oled_write_size_16x8(0,0,str);//第一行 -- 8x16的显示单元显示ASCII码
}

//执行主体
void mainTask(void)
{
	while(1)
	{
		
		//按键是否被触发
		if(key_msg_1.mode_key!=0)
		{
			//哪个按键被按下
			switch(key_msg_1.mode_key)
			{
				case 1:
					if(key_msg_1.key_RR)
						DC_DC_1.vol -= 1;
					else
						DC_DC_1.vol -= 0.1;
					if(DC_DC_1.vol<VOL_MIN)
						DC_DC_1.vol = VOL_MIN;
					break;
				case 2:
					if(key_msg_1.key_RR)
						DC_DC_1.vol += 1;
					else
						DC_DC_1.vol += 0.1;
					if(DC_DC_1.vol>VOL_MAX)
						DC_DC_1.vol = VOL_MAX;
					break;
				case 3:
					if(key_msg_1.key_EV==0)
					{
						DC_DC_1.vol = VOL_MIN;
						key_msg_1.key_EV = 1 - key_msg_1.key_EV;
					}
					else
					{
						DC_DC_1.vol = VOL_MAX;
						key_msg_1.key_EV = 1 - key_msg_1.key_EV;
					}
					break;
				case 4:
					key_msg_1.key_RR = 1 - key_msg_1.key_RR;
					break;
			}
			
			//计算结果加上误差
			DC_DC_1.pwm = voltageToPWM(DC_DC_1.vol) + DC_DC_1.pwm_error;
			
			//加载pwm
			LoadPWM(DC_DC_1.pwm);
			
			//按键清除
			key_msg_1.mode_key = 0;
			
		}
		
		//更新OLED显示
		updata_OLED();
		
		//延时
		Delay1ms();
		
	}
}
