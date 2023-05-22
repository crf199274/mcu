#include "codetab.h"
#include "LQ12864.h"
#include "DC_DC.h"
#include "string.h"
#include "stdio.h"

float VIN = 0;
float VOUT = 0;
float OCC = 0;

//float DC_VOL_MAX = 0;//极限最大电压
//float DC_VOL_MIN = 0;//极限最小电压
//float DC_VOL_Section = 0;

void lcd_init(void)
{
	OLED_Init(); //OLED初始化
}

void updata_LCD(void)
{
	char buf[16];
	
	memset(buf,0,16);
	sprintf(buf,"VIN: %5.2f V",VIN);
	OLED_P8x16Str(0,0,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"VOUT: %5.2f V",VOUT);
	OLED_P8x16Str(0,2,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
//	memset(buf,0,16);
//	sprintf(buf,"pwm: %5d",pwm);
//	OLED_P8x16Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	OCC = pwm/(PWM_HIGH_MAX*1.0)*100;
	sprintf(buf,"OCC: %7.3f %%",OCC);
	OLED_P8x16Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,16);
	sprintf(buf,"B: %2d V S: %2d V",(int)VOL_MAX,(int)VOL_MIN);
	OLED_P8x16Str(0,6,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
}