#include "function1.h"

void updata_LCD1(void);

void function1()
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	while(1)
	{
		
		updata_LCD1();
		
		if(falg3||falg4||falg5)
		{
			falg1 = 0;
			falg2 = 0;
			falg3 = 0;
			ws2812bWriteAColor(RGB1);
			LightCube_Reset();
		}
		
		if(MODE!=0)
			break;
	}
}

void updata_LCD1(void)
{
	char buf[20];
	
	memset(buf,0,20);
	sprintf(buf,"RGB_R: %-3d",(int)RGB1.R);
	LCD_ShowString(0,0,buf,BLUE,BLACK,16,0);//第一行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,20);
	sprintf(buf,"RGB_G: %-3d",(int)RGB1.G);
	LCD_ShowString(0,20,buf,BLUE,BLACK,16,0);//第二行 -- 8x16的显示单元显示ASCII码
	
	memset(buf,0,20);
	sprintf(buf,"RGB_B: %-3d",(int)RGB1.B);
	LCD_ShowString(0,40,buf,BLUE,BLACK,16,0);//第三行 -- 8x16的显示单元显示ASCII码
	
}
