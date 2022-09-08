#include "rgbTFT.h"
#include "ascll.h"
#include "stdelay.h"

/******************************************************************************
函数名: LCD_GPIO_Init
功能: LCDIO初始化
入口参数: 无
返回值: 无
******************************************************************************/
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd(rgbTFT_Clock,ENABLE);
	
	//SDI--PF10  SCL--PF9  
	GPIO_InitStructure.GPIO_Pin =  LCD_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_SDA_Port, &GPIO_InitStructure);
	GPIO_SetBits(LCD_SDA_Port,LCD_SDA);
	
	GPIO_InitStructure.GPIO_Pin =  LCD_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_SCL_Port, &GPIO_InitStructure);
	GPIO_SetBits(LCD_SCL_Port,LCD_SCL);
	
		//DC--PB1
	GPIO_InitStructure.GPIO_Pin =  LCD_DC;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_DC_Port, &GPIO_InitStructure);
	GPIO_SetBits(LCD_DC_Port,LCD_DC);

		//RST--PG0
	GPIO_InitStructure.GPIO_Pin =  LCD_RST;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_RST_Port, &GPIO_InitStructure);	
	GPIO_SetBits(LCD_RST_Port,LCD_RST);
	
		// CS--PD4  BLK--PD15 
	#if ENABLE_CS == 1
		GPIO_InitStructure.GPIO_Pin =  LCD_CS;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LCD_CS_Port, &GPIO_InitStructure);
	#endif
	
	GPIO_InitStructure.GPIO_Pin =  LCD_BLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_BLK_Port, &GPIO_InitStructure);
	GPIO_SetBits(LCD_BLK_Port,LCD_BLK);
	
}

/******************************************************************************
函数名: LCD_Writ_Bus
功能: 向LCD写入一个字节数据
入口参数: uint8_t dat 需要写入的数据
返回值: 无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		LCD_SCL_CLR();
		if(dat&0x80)
		{
		   LCD_SDA_SET();
		}
		else
		{
		   LCD_SDA_CLR();
		}
		LCD_SCL_SET();
		dat<<=1;
	}	
}

/******************************************************************************
函数名: LCD_WR_DATA8
功能: 向LCD写入一个字节数据(对LCD_Writ_Bus进一步封装)
入口参数: uint8_t dat 需要写入的数据
返回值: 无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	#if ENABLE_CS == 1
		LCD_CS_CLR();
	#endif
	LCD_DC_SET();
	LCD_Writ_Bus(dat);
	#if ENABLE_CS == 1
		LCD_CS_SET();
	#endif
}

/******************************************************************************
函数名: LCD_WR_DATA
功能: 向LCD写入两个字节数据(对LCD_Writ_Bus进一步封装)
入口参数: uint16_t dat 需要写入的数据
返回值: 无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	#if ENABLE_CS == 1
		LCD_CS_CLR();
	#endif
	LCD_DC_SET();
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
	#if ENABLE_CS == 1
		LCD_CS_SET();
	#endif
}

/******************************************************************************
函数名: LCD_WR_REG
功能: 向LCD写入一个字节CMD数据(对LCD_Writ_Bus进一步封装)
入口参数: uint8_t dat 需要写入的数据
返回值: 无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	#if ENABLE_CS == 1
		LCD_CS_CLR();
	#endif
	LCD_DC_CLR();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_SET();//写数据
	#if ENABLE_CS == 1
		LCD_CS_SET();
	#endif
}

/******************************************************************************
函数名: LCD_Address_Set
功能: 设置显示区域
入口参数: uint16_t x1 uint16_t y1 起始坐标，uint16_t x2 uint16_t y2 结束坐标
返回值: 无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
/******************************************************************************
函数名: LCD_Init
功能: 初始化LCD
入口参数: 无
返回值: 无
******************************************************************************/
void LCD_Init(void)
{
	  LCD_GPIO_Init();//初始化GPIO
		LCD_RST_CLR();  //复位
		stdelay_ms(20);
		LCD_RST_SET();
		stdelay_ms(20);
		LCD_BLK_SET();//打开背光
	
		
	  LCD_WR_REG(0X36);// Memory Access Control
		if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
		else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
		else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
		else LCD_WR_DATA8(0xA0);
		LCD_WR_REG(0x3A); 
		LCD_WR_DATA8(0x05);

		LCD_WR_REG(0xB2);
		LCD_WR_DATA8(0x0C);
		LCD_WR_DATA8(0x0C);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x33);
		LCD_WR_DATA8(0x33);

		LCD_WR_REG(0xB7); 
		LCD_WR_DATA8(0x35);  

		LCD_WR_REG(0xBB);
		LCD_WR_DATA8(0x19);

		LCD_WR_REG(0xC0);
		LCD_WR_DATA8(0x2C);

		LCD_WR_REG(0xC2);
		LCD_WR_DATA8(0x01);

		LCD_WR_REG(0xC3);
		LCD_WR_DATA8(0x12);   

		LCD_WR_REG(0xC4);
		LCD_WR_DATA8(0x20);  

		LCD_WR_REG(0xC6); 
		LCD_WR_DATA8(0x0F);    

		LCD_WR_REG(0xD0); 
		LCD_WR_DATA8(0xA4);
		LCD_WR_DATA8(0xA1);

		LCD_WR_REG(0xE0);
		LCD_WR_DATA8(0xD0);
		LCD_WR_DATA8(0x04);
		LCD_WR_DATA8(0x0D);
		LCD_WR_DATA8(0x11);
		LCD_WR_DATA8(0x13);
		LCD_WR_DATA8(0x2B);
		LCD_WR_DATA8(0x3F);
		LCD_WR_DATA8(0x54);
		LCD_WR_DATA8(0x4C);
		LCD_WR_DATA8(0x18);
		LCD_WR_DATA8(0x0D);
		LCD_WR_DATA8(0x0B);
		LCD_WR_DATA8(0x1F);
		LCD_WR_DATA8(0x23);

		LCD_WR_REG(0xE1);
		LCD_WR_DATA8(0xD0);
		LCD_WR_DATA8(0x04);
		LCD_WR_DATA8(0x0C);
		LCD_WR_DATA8(0x11);
		LCD_WR_DATA8(0x13);
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x3F);
		LCD_WR_DATA8(0x44);
		LCD_WR_DATA8(0x51);
		LCD_WR_DATA8(0x2F);
		LCD_WR_DATA8(0x1F);
		LCD_WR_DATA8(0x1F);
		LCD_WR_DATA8(0x20);
		LCD_WR_DATA8(0x23);

		LCD_WR_REG(0x21); 

		LCD_WR_REG(0x11); 
		//Delay (120); 

		LCD_WR_REG(0x29); 
}

void LCD_Init2(void)
{
	  LCD_GPIO_Init();//初始化GPIO
		LCD_RST_CLR();  //复位
		stdelay_ms(20);
		LCD_RST_SET();
		stdelay_ms(20);
		LCD_BLK_SET();//打开背光
	
		
	  LCD_WR_REG(0x11);
		stdelay_ms(120);
	
		
		LCD_WR_REG(0xB1); 
		LCD_WR_DATA8(0x01);
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x2D);

		LCD_WR_REG(0xB2);
		LCD_WR_DATA8(0x01);
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x2D);

		LCD_WR_REG(0xB3); 
		LCD_WR_DATA8(0x01);  
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x2D);
		LCD_WR_DATA8(0x01);
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x2D);

		LCD_WR_REG(0xB4);
		LCD_WR_DATA8(0x07);

		LCD_WR_REG(0xC0);
		LCD_WR_DATA8(0xA2);
		LCD_WR_DATA8(0x02);
		LCD_WR_DATA8(0x84);
		LCD_WR_REG(0xC1);
		LCD_WR_DATA8(0xC5);

		LCD_WR_REG(0xC2);
		LCD_WR_DATA8(0x0A);   
		LCD_WR_DATA8(0x00);

		LCD_WR_REG(0xC3);
		LCD_WR_DATA8(0x8A);  
		LCD_WR_DATA8(0x2A); 
		LCD_WR_REG(0xC4); 
		LCD_WR_DATA8(0x8A);    
		LCD_WR_DATA8(0xEE);

		LCD_WR_REG(0xC5); 
		LCD_WR_DATA8(0x0E);
		
		LCD_WR_REG(0x36); 
		if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
		else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
		else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
		else LCD_WR_DATA8(0xA0);

		LCD_WR_REG(0xE0);
		LCD_WR_DATA8(0x0F);
		LCD_WR_DATA8(0x1A);
		LCD_WR_DATA8(0x0F);
		LCD_WR_DATA8(0x18);
		LCD_WR_DATA8(0x2F);
		LCD_WR_DATA8(0x28);
		LCD_WR_DATA8(0x20);
		LCD_WR_DATA8(0x22);
		LCD_WR_DATA8(0x1F);
		LCD_WR_DATA8(0x1B);
		LCD_WR_DATA8(0x23);
		LCD_WR_DATA8(0x37);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x07);
		LCD_WR_DATA8(0x02);
		LCD_WR_DATA8(0x10);

		LCD_WR_REG(0xE1);
		LCD_WR_DATA8(0x0F);
		LCD_WR_DATA8(0x1B);
		LCD_WR_DATA8(0x0F);
		LCD_WR_DATA8(0x17);
		LCD_WR_DATA8(0x33);
		LCD_WR_DATA8(0x2C);
		LCD_WR_DATA8(0x29);
		LCD_WR_DATA8(0x2E);
		LCD_WR_DATA8(0x30);
		LCD_WR_DATA8(0x39);
		LCD_WR_DATA8(0x3F);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x07);
		LCD_WR_DATA8(0x03);
		LCD_WR_DATA8(0x10);

		LCD_WR_REG(0x2A); 
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x7F);

		LCD_WR_REG(0x2B); 
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x9F);

		LCD_WR_REG(0xF0); 
		LCD_WR_DATA8(0x01);
		LCD_WR_REG(0xF6); 
		LCD_WR_DATA8(0x00);
		
		LCD_WR_REG(0x3A); 
		LCD_WR_DATA8(0x05);
		
		LCD_WR_REG(0x29); 
		
}

//以下是LCD显示功能拓展函数
/*************************************************
函数名: LCD_Fill
功能: 在指定位置填充颜色
入口参数: xsta,ysta:		起始坐标		xend,yend:	终止坐标		color:	填充颜色
返回值: 无
*************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/*************************************************
函数名: LCD_DrawPoint
功能: 画一个点
入口参数: 无
返回值: 无
*************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
}

/*************************************************
函数名: LCD_DrawLine
功能: 画线
入口参数: x1,y1:		起始坐标		x2,y2:	终止坐标		color:	线的颜色
返回值: 无
*************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算增量坐标
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if (delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/*************************************************
函数名: LCD_DrawRectangle
功能: 画矩形
入口参数: x1,y1:		起始坐标		x2,y2:	终止坐标		color:	线的颜色
返回值: 无
*************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/*************************************************
函数名: Draw_Circle
功能: 画矩形
入口参数: x0,y0:		圆心坐标		r:	半径		color:	线的颜色
返回值: 无
*************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过圆
		{
			b--;
		}
	}         
}

/*************************************************
函数名: LCD_ShowPicture
功能: 显示图片
入口参数: x,y:		显示坐标		length:	图片长度		width:	图片宽度		pic[]:	图片数组
返回值: 无
*************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

/*************************************************
函数名: LCD_ShowUC16x8
功能: 显示一个16x8的点阵字符(实际占用分辨率为12x16)
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色
返回值: 无
*************************************************/
void LCD_ShowUC16x8(uint16_t x,uint16_t y,uint8_t buf,uint16_t fc)
{
	uint8_t j=0,k=0,s=0;
	
	LCD_Address_Set(x,y,x+8-1,y+16-1);
	for (j = 0; j < 16; j++)
	{
		s=asc2_1608[buf-32][j];
		for (k = 0; k < 8; k++)
		{
			if(s&0x01)
			{
				LCD_WR_DATA(fc);
			}else{
					LCD_WR_DATA(0xffff);
			}
			s>>=1; 
		}
	}
}

/*************************************************
函数名: LCD_ShowUC12x12
功能: 显示一个12x12的点阵字符(实际占用分辨率为12x16)
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC12x12(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+15,y+11);
	for (j = 0; j < 12; j++)
	{
		for (i = 0; i < 2; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 2 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC16x16
功能: 显示一个16x16的点阵字符
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC16x16(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+15,y+15);
	for (j = 0; j < 16; j++)
	{
		for (i = 0; i < 2; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 2 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC24x24
功能: 显示一个24x24的点阵字符
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC24x24(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+23,y+23);
	for (j = 0; j < 24; j++)
	{
		for (i = 0; i < 3; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 3 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC32x32
功能: 显示一个32x32的点阵字符
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC32x32(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+31,y+31);
	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 4; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 4 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC48x48
功能: 显示一个48x48的点阵字符
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC48x48(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+47,y+47);
	for (j = 0; j < 48; j++)
	{
		for (i = 0; i < 6; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 6 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC64x64
功能: 显示一个64x64的点阵字符
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[] 点阵字符数据 uint16_t fc 字体颜色 uint16_t bc 背景颜色
返回值: 无
*************************************************/
void LCD_ShowUC64x64(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc)
{
	int i=0,j=0,k=0;
	
	LCD_Address_Set(x,y,x+63,y+63);
	for (j = 0; j < 64; j++)
	{
		for (i = 0; i < 8; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((buf[j * 8 + i] & (0x80 >> k)) == (0x80 >> k))
				{
					LCD_WR_DATA(fc);
				}else{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
}

/*************************************************
函数名: LCD_ShowUC64x64
功能: 显示字符串
入口参数: uint16_t x uint16_t y 起始坐标 uint8_t buf[]
返回值: 无
*************************************************/
void LCD_ShowStr(uint16_t x,uint16_t y,uint8_t *str,uint8_t n)
{
	uint8_t i = 0,pos_x = 0;
	
	for(i=0;i<n;i++)
	{
		LCD_ShowUC16x8(x+pos_x,y,*str,0x19);
		str++;
		pos_x += 8;
	}
}

