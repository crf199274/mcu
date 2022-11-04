#include "main.h"
#include "string.h"

void delay(int i);
void delay_ms(int i);
void delay_s(int i);

void init(void);
void USARTinit(void);
unsigned char usart_WaitRecive(void);
void cleanUsartBuf(void);

void main()
{
	init();
	USARTinit();
	while(1)
	{
		CloseColor();
		delay_ms(10);
		if(usart_WaitRecive())
		{
			if(usart_buf[1]==0xC3&&usart_buf[2]==0x00)
			{
				flag3 = 0;
				break;
			}
			cleanUsartBuf();
		}
		flag3--;
	}
	
	while(1)
	{
		UserCompare();
		delay_ms(10);
		if(usart_WaitRecive())
		{
			if(usart_buf[1]==0x0C&&usart_buf[3]!=0x00)
			{
				while(1)
				{
					Color();
					delay_ms(10);
					if(usart_WaitRecive())
					{
						if(usart_buf[1]==0xC3&&usart_buf[2]==0x00)
						{
							while(1)
							{
								pwm = 1;
								delay(50);
								pwm = 0;
								delay(1950);
								if(light1==0)
								{
									delay_s(3);
									while(1)
									{
										pwm = 1;
										delay(250);
										pwm = 0;
										delay(1750);
										if(light2==1)
											break;
									}
									break;
								}
							}
							
							while(1)
							{
								CloseColor();
								delay_ms(10);
								if(usart_WaitRecive())
								{
									if(usart_buf[1]==0xC3&&usart_buf[2]==0x00)
									{
										flag3 = 0;
										break;
									}
									cleanUsartBuf();
								}
								flag3--;
							}
							
							flag2 = 0;
							break;
						}
						cleanUsartBuf();
					}
					flag2--;
				}
				flag1 = 0;
			}
			cleanUsartBuf();
		}
		flag1--;
	} 
}

unsigned char usart_WaitRecive()
{
		if(usart_cnt == 0)
			return 0;
		
		if(usart_cnt == usart_cntPre)
		{
			usart_cnt = 0;
			return 1;	
		}
		
		usart_cntPre = usart_cnt;
		
		return 0;
}

void cleanUsartBuf()
{
	memset(usart_buf,0,sizeof(usart_buf));
	usart_cnt = 0;
}

void Usart() interrupt 4
{
	if(RI==1)
	{
		if(usart_cnt>=sizeof(usart_buf))
			usart_cnt = 0;
		usart_buf[usart_cnt++] = SBUF;
		RI = 0;
	}
}

void USARTinit()
{
	TMOD=0x20;
	TH1=0xFA;
	TL1=0xFA;
	TR1=1;
	PCON|=0x80;
	SCON|=0x50;
	ES=1;
	EA=1;

}

void delay(int i)
{
	while(i--);
}

void delay_ms(int i)
{
	while(i--)
	{
		delay(100);
	}
}

void delay_s(int i)
{
	while(i--)
	{
		delay_ms(1000);
	}
}


void init()
{
	pwm = 0;
	light1 = 1;
	light2 = 1;
}
