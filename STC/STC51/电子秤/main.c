#include "stc8.h"
#include <stdio.h>
#include "delay.h"
#include "OLED.h"

sbit HX711_DOUT = P3^2; 	//����P3^2�ڽ�HX711������
sbit HX711_SCK = P3^3;		//����P3^3�ڽ�HX711ʱ����
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
	unsigned long count; 	//��adֵ
	unsigned char i; 
	
	HX711_DOUT = 1;				//���ݶ˿����ߣ�����ʱ��ͼд
  HX711_SCK = 0; 								
	count = 0;
	while(HX711_DOUT);
	
  for(i=0; i<24; i++)		//ѭ��24�δ����ݣ���λ��������
	{ 
	  	HX711_SCK = 1; 		//׼��ת������
			delay_07us(2);//1us
	  	count = count << 1; 	//������λ�������λ��0���ճ����λ���д�ֵ��������1<<1Ϊ20
			HX711_SCK = 0; 		//ʱ�������½��ӣ����ݿ�ʼת��
			delay_07us(2);//1us
	  	if(HX711_DOUT)
			count++; 		//���ݶ�Ϊ�������λ+1��Ϊ1�����ݶ�Ϊ�������λ���䣬Ϊ0�����ư����λ�ճ���
	} 
 	HX711_SCK = 1; 		//��25��������
  count = count^0x800000;		//������ԭ��
	count = count>>8;		//������ԭ��
	delay_07us(2);//1us
	HX711_SCK = 0;		//����adת��
	delay_07us(2);//1us
	return(count);		//����adֵ
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
