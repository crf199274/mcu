#include "ws2812.h"
#include "color.h"
#include "string.h"

void delay_ms(int i);
void RCC_INIT(void);
void IO_INIT(void);
void TM0_INIT(void);
void TM1_INIT(void);
void TM2_INIT(void);
void LightCubeType0(void);
void LightCubeType1(void);
void LightCubeType2(void);
void LightCubeType3(void);
void LightCubeType4(void);
void LightCubeType5(void);
void LightCubeType6(void);
void LightCubeType7(void);
void LightCubeType8(void);
void LightCubeType9(void);
void LightCubeType10(void);
void LightStatus(void);
void LightStatusAll(void);

char flag1 = 0;
char flag2 = 0;
int time_cnt = 0;
int keyT_cnt = 0;
char key = 125;
char status = 0;
sbit key0 = P1^1;
sbit key1 = P3^7;
//sbit key2 = P1^0;
sbit key3 = P3^6;

void main()
{
	RCC_INIT();
	IO_INIT();
	delay_ms(100);
	TM0_INIT();
	TM1_INIT();
	TM2_INIT();
	ws2812b_INIT();
	
	delay_ms(100);
	
	LightCubeType0();
	
	while(1)
	{		
		switch(key)
		{
			case 0: LightStatusAll();break;
			case 1:LightStatus();break;
//			case 2:LightStatus();break;
			default : break;
		}
	}
}

void RCC_INIT(void)
{
	 //选择24MHz
    P_SW2 = 0x80;
    CLKDIV = 0x04;
    IRTRIM = T24M_ROMADDR;
    VRTRIM = VRT20M_ROMADDR;
    IRCBAND = 0x00;
    CLKDIV = 0x00;
}

void IO_INIT(void)
{
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
	
	key0 = 1;
	key1 = 1;
	key3 = 1;
	
}

void TM0_INIT(void)//1ms
{
	TMOD = 0x00;                                //模式0
	TL0 = 0x30;                                 //65536-11.0592M/12/1000
	TH0 = 0xF8;
//	TR0 = 1;                                    //启动定时器
	ET0 = 1;                                    //使能定时器中断
//	EA = 1;
}

void TM1_INIT(void)
{
	TMOD = 0x00;                                //模式0
	TL1 = 0x30;                                 //65536-11.0592M/12/1000
	TH1 = 0xF8;
//	TR1 = 1;                                    //启动定时器
	ET1 = 1;                                    //使能定时器中断
	EA = 1;
}

void TM2_INIT(void)
{
	T2L = 0x30;                                 //65536-11.0592M/12/1000
	T2H = 0xF8;
	AUXR = 0x10;                                //启动定时器
	IE2 = ET2;                                  //使能定时器中断
	EA = 1;
}

void LightCubeType0(void)
{
	char i = 0;
	WsColor Type1Color={0x00,0x00,0x00};
	for(i=0;i<125;i++)
	{
		ws2812bWriteAColor(Type1Color);
	}
	delay_ms(1);
}

void LightCubeType1(void)
{
	char i = 0,j = 1;
	char num = 1;
	WsColor Type1Color={0x00,0x00,0xff};
	WsColor LightsOut={0x00,0x00,0x00};
	for(i=0;i<125;i++)
	{
		for(j=1;j<126;j++)
		{
			if(j==num)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		num++;
		delay_ms(100);
	}
}

void LightCubeType2(void)
{
	char i = 0;
	WsColor Type1Color={0x00,0x00,0xff};
	for(i=0;i<125;i++)
	{
		ws2812bWriteAColor(Type1Color);
	}
	delay_ms(500);
}

void LightCubeType3(void)
{
	char i = 0,j = 0,k = 0;
	char num = 0;
	WsColor Type1Color={0x00,0x00,0xff};
	WsColor LightsOut={0x00,0x00,0x00};
	for(k=0;k<3;k++)
	{
		switch(num)
		{
			case 0:
				num++;
				for(i = 0;i<5;i++)
				{
					for(j=1;j<26;j++)
					{
						if(j<=5||j>=20)
							ws2812bWriteAColor(Type1Color);
						else
							ws2812bWriteAColor(LightsOut);
					}
				}
				break;
			case 1:
				num++;
				for(i = 0;i<5;i++)
				{
					for(j=1;j<26;j++)
					{
						if(j==1||j==10||j==11||j==20||j==21||j==5||j==6||j==15||j==16||j==25)
							ws2812bWriteAColor(Type1Color);
						else
							ws2812bWriteAColor(LightsOut);
					}
				}
				break;
			case 2:
				num=0;
				for(i=1;i<126;i++)
				{
					if(i<=25||i>=100)
						ws2812bWriteAColor(Type1Color);
					else
						ws2812bWriteAColor(LightsOut);
				}
				break;
		}
		delay_ms(500);
	}
	
}

void LightCubeType4(void)
{
	char i = 0,j = 1,k = 0;
	char num = 0;
	WsColor Type1Color={0x00,0x00,0xff};
	WsColor LightsOut={0x00,0x00,0x00};
	for(k=0;k<5;k++)
	{
		for(i=0;i<5;i++)
		{
			for(j=1;j<26;j++)
			{
				if((i<=num)&&(j==1||j==5||j==21||j==25))
					ws2812bWriteAColor(Type1Color);
				else if((j>=7&&j<=9)||(j>=12&&j<=14)||(j>=17&&j<=19))
					ws2812bWriteAColor(Type1Color);
				else
					ws2812bWriteAColor(LightsOut);
			}
		}
		num++;
		delay_ms(200);
	}
}

void LightCubeType5(void)
{
	char i = 0,j = 0;
	WsColor Type1Color={0x00,0x00,0xff};
	WsColor LightsOut={0x00,0x00,0x00};
	for(j=0;j<3;j++)
	{
		//1
		for(i=1;i<26;i++)
		{
			if(i==3||i==7||i==9||i==11||i==15||i==17||i==19||i==23)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//2
		for(i=1;i<26;i++)
		{
			if(i==2||i==4||i==6||i==10||i==16||i==20||i==22||i==24)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//3
		for(i=1;i<26;i++)
		{
			if(i==1||i==5||i==21||i==25)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//4
		for(i=1;i<26;i++)
		{
			if(i==2||i==4||i==6||i==10||i==16||i==20||i==22||i==24)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//5
		for(i=1;i<26;i++)
		{
			if(i==3||i==7||i==9||i==11||i==15||i==17||i==19||i==23)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		delay_ms(500);
		//1
		for(i=1;i<26;i++)
		{
			if(i==8||i==12||i==14||i==18)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//2
		for(i=1;i<26;i++)
		{
			if(i==3||i==11||i==15||i==23)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//3
		for(i=1;i<26;i++)
		{
			if(i==2||i==4||i==6||i==10||i==16||i==20||i==22||i==24)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//4
		for(i=1;i<26;i++)
		{
			if(i==3||i==11||i==15||i==23)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		//5
		for(i=1;i<26;i++)
		{
			if(i==8||i==12||i==14||i==18)
				ws2812bWriteAColor(Type1Color);
			else
				ws2812bWriteAColor(LightsOut);
		}
		delay_ms(500);
	}
}

void LightCubeType6(void)
{
	char i=0,j=0,k=0;
	for(k=0;k<30;k++)
	{
		for(i=0;i<5;i++)
		{
			if(i%2==0)
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(Type6[k*2]);
				}
			}
			else
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(Type6[k*2+1]);
				}
			}
		}
		delay_ms(500);
	}
}

void LightCubeType7(void)
{
	char i=0,j=0,k=0;
	WsColor LightsOut={0x00,0x00,0x00};
	for(k=0;k<8;k++)
	{
		for(i=0;i<5;i++)
		{
			for(j=1;j<26;j++)
			{
				switch(j)
				{
					case 1:ws2812bWriteAColor(Type6[k*5+4]);break;
					case 5:ws2812bWriteAColor(Type6[(k+1)*5+4]);break;
					case 13:ws2812bWriteAColor(Type6[(k+2)*5+4]);break;
					case 21:ws2812bWriteAColor(Type6[(k+3)*5+4]);break;
					case 25:ws2812bWriteAColor(Type6[(k+4)*5+4]);break;
					default :ws2812bWriteAColor(LightsOut);break;
				}
			}
		}
		LightCube_Reset();
		delay_ms(500);
	}
}

void LightCubeType8_layer(char layer,char num,char color)
{
	char i=0,j=0;
	WsColor LightsOut={0x00,0x00,0x00};
	if(num==1)
	{
		for(i=0;i<5;i++)
		{
			if(i==layer)
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(Type6[color*5+layer]);
				}
			}
			else
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(LightsOut);
				}
			}
		}
	}else if(num==2)
	{
		for(i=0;i<5;i++)
		{
			if(i==layer)
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(Type6[color*5+layer]);
				}
			}
			else if(i==(layer+1))
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(Type6[color*5+layer+1]);
				}
			}
			else
			{
				for(j=1;j<26;j++)
				{
					ws2812bWriteAColor(LightsOut);
				}
			}
		}
	}
}

void LightCubeType8(void)
{
	char i=0,j=0;
	time_cnt = 0;
	TR1 = 1;		//定时器1开始计时
	for(j=0;j<12;j++)
	{
		while(1)
		{
			for(i=0;i<3;i++)
			{
				switch(i)
				{
					case 0:LightCubeType8_layer(0,1,j);break;
					case 1:LightCubeType8_layer(1,2,j);break;
					case 2:LightCubeType8_layer(3,2,j);break;
				}
				LightCube_Reset();
			}
			if(flag1)
			{
				flag1 = 0;
				break;
			}
		}
	}
	TR1 = 0;		//定时器1停止计时
}

//char getHeadIndex(char head)
//{
//	char i=0;
//	char head_element[16]={1,2,3,4,5,6,15,16,25,24,23,22,21,20,11,10};
//	for (i=0; i<16; i++)
//	{
//			if (head_element[i]==head)
//				return (i+1);
//	}
//}

void getSnake(char * snake_list,char * snakeElementLayer,char snake_size)
{
	char i=0;
	xdata char head_element[16]={1,2,3,4,5,6,15,16,25,24,23,22,21,20,11,10};
	if(snake_size<12)
	{
		snake_list[snake_size]=head_element[snake_size];
	}
	else
	{
		for(i=0;i<12;i++)
		{
			switch(snake_list[i])
			{
				case 1:snake_list[i]=2;break;
				case 2:snake_list[i]=3;break;
				case 3:snake_list[i]=4;break;
				case 4:snake_list[i]=5;break;
				case 5:snake_list[i]=6;break;
				case 6:snake_list[i]=15;break;
				case 15:snake_list[i]=16;break;
				case 16:snake_list[i]=25;break;
				case 25:snake_list[i]=24;break;
				case 24:snake_list[i]=23;break;
				case 23:snake_list[i]=22;break;
				case 22:snake_list[i]=21;break;
				case 21:snake_list[i]=20;break;
				case 20:snake_list[i]=11;break;
				case 11:snake_list[i]=10;break;
				case 10:snake_list[i]=1;snakeElementLayer[i]=snakeElementLayer[i]+1;break;
			}
		}
	}
}

void LightCubeType9(void)
{
	char i=0;
	char snake_size = 0;
	xdata char snakeElementLayer[12]={0};
	xdata char snake_list[12]={1};
	xdata char snake[12]={1};
	WsColor LightsOut={0x00,0x00,0x00};
	WsColor LightsOn={0xff,0xff,0xff};
	time_cnt = 0;
	TR0 = 1;		//定时器0开始计时
	while(1)
	{
		for(i=0;i<12;i++)
		{
			snake[i]=snakeElementLayer[i]*25+snake_list[i];
		}
		for(i=1;i<126;i++)
		{
			if(i==snake[0])
			{
				ws2812bWriteAColor(Type6[0*5+4]);
				continue;
			}
			else if(i==snake[1])
			{
				ws2812bWriteAColor(Type6[1*5+4]);
				continue;
			}
			else if(i==snake[2])
			{
				ws2812bWriteAColor(Type6[2*5+4]);
				continue;
			}
			else if(i==snake[3])
			{
				ws2812bWriteAColor(Type6[3*5+4]);
				continue;
			}
			else if(i==snake[4])
			{
				ws2812bWriteAColor(Type6[4*5+4]);
				continue;
			}
			else if(i==snake[5])
			{
				ws2812bWriteAColor(Type6[5*5+4]);
				continue;
			}
			else if(i==snake[6])
			{
				ws2812bWriteAColor(Type6[6*5+4]);
				continue;
			}
			else if(i==snake[7])
			{
				ws2812bWriteAColor(Type6[7*5+4]);
				continue;
			}
			else if(i==snake[8])
			{
				ws2812bWriteAColor(Type6[8*5+4]);
				continue;
			}
			else if(i==snake[9])
			{
				ws2812bWriteAColor(Type6[9*5+4]);
				continue;
			}
			else if(i==snake[10])
			{
				ws2812bWriteAColor(Type6[10*5+4]);
				continue;
			}
			else if(i==snake[11])
			{
				ws2812bWriteAColor(Type6[11*5+4]);
				continue;
			}
			switch(i)
			{
//				case 7:ws2812bWriteAColor(LightsOn);continue;break;
//				case 8:ws2812bWriteAColor(LightsOn);continue;break;
//				case 9:ws2812bWriteAColor(LightsOn);continue;break;
//				case 12:ws2812bWriteAColor(LightsOn);continue;break;
//				case 13:ws2812bWriteAColor(LightsOn);continue;break;
//				case 14:ws2812bWriteAColor(LightsOn);continue;break;
//				case 17:ws2812bWriteAColor(LightsOn);continue;break;
//				case 18:ws2812bWriteAColor(LightsOn);continue;break;
//				case 19:ws2812bWriteAColor(LightsOn);continue;break;
				
				case 7+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 8+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 9+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 12+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 13+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 14+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 17+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 18+25:ws2812bWriteAColor(LightsOn);continue;break;
				case 19+25:ws2812bWriteAColor(LightsOn);continue;break;
				
				case 7+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 8+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 9+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 12+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 13+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 14+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 17+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 18+50:ws2812bWriteAColor(LightsOn);continue;break;
				case 19+50:ws2812bWriteAColor(LightsOn);continue;break;
				
				case 7+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 8+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 9+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 12+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 13+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 14+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 17+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 18+75:ws2812bWriteAColor(LightsOn);continue;break;
				case 19+75:ws2812bWriteAColor(LightsOn);continue;break;
				
//				case 7+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 8+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 9+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 12+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 13+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 14+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 17+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 18+100:ws2812bWriteAColor(LightsOn);continue;break;
//				case 19+100:ws2812bWriteAColor(LightsOn);continue;break;
			}
			ws2812bWriteAColor(LightsOut);
		}
		LightCube_Reset();
		if(snake_size<12)
			snake_size++;
		if(snake_list[11]==10&&snakeElementLayer[11]==4)
			break;
		getSnake(snake_list,snakeElementLayer,snake_size);
		while(flag2==0);
		flag2=0;
	}
	TR0 = 0;		//定时器0停止计时
}

void LightCubeType10(void)
{
	char i = 0,j = 1,k = 0,n=0;
	char num = 0;
//	WsColor Type1Color={0x00,0x00,0xff};
	WsColor LightsOut={0x00,0x00,0x00};
	for(n=0;n<12;n++)
	{
		for(k=0;k<5;k++)
		{
			for(i=0;i<5;i++)
			{
				for(j=1;j<26;j++)
				{
					if((i<=num)&&(j==1||j==5||j==21||j==25))
						ws2812bWriteAColor(Type6[n*5+4]);
					else if((j>=7&&j<=9)||(j>=12&&j<=14)||(j>=17&&j<=19))
						ws2812bWriteAColor(Type6[n*5+4]);
					else
						ws2812bWriteAColor(LightsOut);
				}
			}
			num++;
			delay_ms(200);
		}
		num=0;
		delay_ms(500);
	}
}

void LightStatusAll(void)
{
	while(1)
	{
		LightCubeType1();
		LightCubeType2();
		LightCubeType3();
		LightCubeType4();
		LightCubeType5();
		LightCubeType9();
		LightCubeType7();
		LightCubeType10();
		LightCubeType6();
		LightCubeType8();
	}
}

void LightStatusSEL(WsColor wsColor)
{
	char i=0;
	for(i=0;i<125;i++)
	{
		ws2812bWriteAColor(wsColor);
	}
	LightCube_Reset();
}

void LightStatus(void)
{
	status++;
	key = -1;
	
	while(1)
	{
		switch(status)
		{
			case 1: LightStatusSEL(Type6[0*5+4]);break;
			case 2: LightStatusSEL(Type6[1*5+4]);break;
			case 3: LightStatusSEL(Type6[2*5+4]);break;
			case 4: LightStatusSEL(Type6[3*5+4]);break;
			case 5: LightStatusSEL(Type6[4*5+4]);break;
			case 6: LightStatusSEL(Type6[5*5+4]);break;
			case 7: LightStatusSEL(Type6[6*5+4]);break;
			case 8: LightStatusSEL(Type6[7*5+4]);break;
			case 9: LightStatusSEL(Type6[8*5+4]);break;
			case 10: LightStatusSEL(Type6[9*5+4]);break;
			case 11: LightStatusSEL(Type6[10*5+4]);break;
			case 12: LightStatusSEL(Type6[11*5+4]);break;
		}
		while(key=='0'||key=='1'||key=='3');
		if(key==1)
		{
			status++;
			key = -1;
		}
		else if(key==0||key==3)
			break;
		if(status>=13)
			status = 1;
	}
}

char getKeyValue(void)
{
	if(!key0)
	{
		while(key0==0);
		return 0;
	}
	else if(!key1)
	{
		while(key1==0);
		if(key==0||key=='0')
			IAP_CONTR |= 0x60;
		else
			return 1;
	}
	else if(!key3)
	{
		while(key3==0);
		return 3;
	}
	else
	{
		if(key==0||key=='0')
			return '0';
		else if(key==1||key=='1')
			return '1';
		else if(key==3||key=='3')
			return '3';
	}
	
}

void Delay100us()       //@24.000MHz
{
    unsigned char i, j;

    i = 3;
    j = 82;
    do
    {
        while (--j);
    } while (--i);
}

void delay_1ms()
{
	int i = 10;
	while(i--)
	{
		Delay100us();
	}
}

void delay_ms(int i)
{
	while(i--)
	{
		delay_1ms();
	}
}

void TM0_Isr() interrupt 1
{
  time_cnt++;
//	TL0 = 0x30;		//设置定时初始值
//	TH0 = 0xF8;		//设置定时初始值
	if(time_cnt>=75)
	{
		time_cnt = 0;
		flag2 = 1;
	}
}

void TM1_Isr() interrupt 3
{
	time_cnt++;
//	TL1 = 0x30;		//设置定时初始值
//	TH1 = 0xF8;		//设置定时初始值
	if(time_cnt>=500)
	{
		time_cnt = 0;
		flag1 = 1;
	}
	
}

void TM2_Isr() interrupt 12
{
	EA = 0;
  keyT_cnt++;
	if(keyT_cnt>=50)
	{
		keyT_cnt = 0;
		key = getKeyValue();
	}
	EA = 1;
}
