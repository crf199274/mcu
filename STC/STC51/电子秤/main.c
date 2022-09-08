#include "stc8.h"
#include <stdio.h>
#include "delay.h"
#include "OLED.h"

sbit HX711_DOUT = P3^2; 	//声明P3^2口接HX711数据线
sbit HX711_SCK = P3^3;		//声明P3^3口接HX711时钟线
sbit LED = P5^4;

unsigned int HX711_Read(void);

unsigned int HX711_Mean(void);

void InitUART(void);

void UART1_SendData(char dat);

void main(void)
{
	unsigned int ad=0;
	
	float Mweight = 0;
	float weight = 0;
	
	char i = 0 ;
	
	char buf[16]={0};
	
	P3M0 = 0x00;
	P3M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	oled_init();
	
	oled_screen_init();
	
	for(i=0;i<2;i++)
	{
		ad=HX711_Mean();
	
		Mweight = (float)ad/0.4;
		
	}
	
	sprintf(buf,"weight = %5.1f g",(float)weight);
	oled_write_size_16x8(0,0,buf);
	sprintf(buf,"weight = %4.1f kg",(float)weight);
	oled_write_size_16x8(0,2,buf);
	oled_write_size_16x8(0,4,"FS : 20 kg");
	
	while(1)
	{
		
		ad=HX711_Mean();
		
		//weight = (unsigned int)((float)ad/207374.1824);
		weight = Mweight - ((float)ad/0.4);
		
		if(weight>1000)
		{
			oled_write_size_16x8(0,0,"weight = >1000 g");
			sprintf(buf,"weight = %4.1f kg",(float)(weight/1000.));
			oled_write_size_16x8(0,2,buf);
			
		}else
		{
			sprintf(buf,"weight = %5.1f g",(float)weight);
			oled_write_size_16x8(0,0,buf);
			sprintf(buf,"weight = %4.1f kg",(float)(weight/1000.));
			oled_write_size_16x8(0,2,buf);
		}
		
	}
	
}

unsigned int HX711_Read(void)
{
	unsigned long count; 	//存ad值
	unsigned char i; 
	
	HX711_DOUT = 1;				//数据端口拉高，参照时序图写
  HX711_SCK = 0; 								
	count = 0;
	while(HX711_DOUT);
	
  for(i=0; i<24; i++)		//循环24次传数据，按位接收数据
	{ 
	  	HX711_SCK = 1; 		//准备转换数据
			delay_07us(2);//1us
	  	count = count << 1; 	//向左移位，即向低位补0，空出最低位进行传值操作，如1<<1为20
			HX711_SCK = 0; 		//时钟脉冲下降延，数据开始转换
			delay_07us(2);//1us
	  	if(HX711_DOUT)
			count++; 		//数据端为高则最低位+1，为1，数据端为低则最低位不变，为0（左移把最低位空出）
	} 
 	HX711_SCK = 1; 		//第25个脉冲延
  count = count^0x800000;		//异或输出原码
	count = count>>8;		//异或输出原码
	delay_07us(2);//1us
	HX711_SCK = 0;		//结束ad转换
	delay_07us(2);//1us
	return(count);		//传出ad值
}

unsigned int HX711_Mean(void)
{
	unsigned int adMean=0,adMax=0,adMin=0;
	
	unsigned int ads[20] = {0};
	
	char i = 0,j = 0;
	
	for(i=0;i<20;i++)
	{
		ads[i] = HX711_Read();
	}
	
	adMax = ads[0];
	adMin = ads[0];
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<20;i++)
		{
			if(ads[i]!=0)
			{
				if(ads[i]>adMax)
				{
					adMax = ads[i];
				}
				
				if(ads[i]<adMin)
				{
					adMin = ads[i];
				}
			}
		}
		
		for(i=0;i<20;i++)
		{
			if(ads[i]==adMax)
			{
				ads[i] = 0;
				break;
			}
			
		}
		
		for(i=0;i<20;i++)
		{
			if(ads[i]==adMin)
			{
				ads[i] = 0;
				break;
			}
		}
		
		for(i=0;i<20;i++)
		{
			if(ads[i]!=0)
			{
				adMax = ads[i];
				adMin = ads[i];
				break;
			}
		}
		
	}
	
	for(i=0;i<20;i++)
	{
		adMean = adMean + ads[i];
	}
	
	adMean = adMean/12;
	
	return adMean;
	
}
