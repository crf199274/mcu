#ifndef __RGBTFT_H
#define __RGBTFT_H

#include "stm32f10x.h"

//移植不同的LCD时只需要把LCD_W和LCD_H的分辨率修改以及IO的宏，还有LCD_GPIO_Init(),LCD_Init()修改即可。
//注意:我使用的LCD是没有片选CS脚的,如果你的有则需要加上。

#define USE_HORIZONTAL 3  //0或1为横屏		2或3为竖屏

#define ENABLE_CS   	 1  //1-启用 0-关闭


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else

#define LCD_W 160
#define LCD_H 128

#endif

//配置IO以及时钟的宏定义:
#define rgbTFT_Clock																	RCC_APB2Periph_GPIOC
																											

#define LCD_BLK_Port        													GPIOC
#define LCD_BLK        																GPIO_Pin_0

#if ENABLE_CS == 1
	#define LCD_CS_Port        													GPIOC
	#define LCD_CS          														GPIO_Pin_1
#endif

#define LCD_RST_Port        													GPIOC
#define LCD_RST     																	GPIO_Pin_3

#define LCD_DC_Port        														GPIOC
#define LCD_DC        																GPIO_Pin_2

#define LCD_SCL_Port        													GPIOC
#define LCD_SCL         															GPIO_Pin_4

#define LCD_SDA_Port        													GPIOC
#define LCD_SDA         															GPIO_Pin_5

//液晶控制IO置1操作语句宏定义
#define LCD_RST_SET()																	GPIO_SetBits(LCD_RST_Port,LCD_RST)
#define LCD_BLK_SET()																	GPIO_SetBits(LCD_BLK_Port,LCD_BLK)
#define LCD_DC_SET()																	GPIO_SetBits(LCD_DC_Port,LCD_DC)
#define LCD_SCL_SET()																	GPIO_SetBits(LCD_SCL_Port,LCD_SCL)
#define LCD_SDA_SET()																	GPIO_SetBits(LCD_SDA_Port,LCD_SDA)

//液晶控制IO置0操作语句宏定义
#define LCD_RST_CLR()																	GPIO_ResetBits(LCD_RST_Port,LCD_RST)
#define LCD_BLK_CLR()																	GPIO_ResetBits(LCD_BLK_Port,LCD_BLK)
#define LCD_DC_CLR()																	GPIO_ResetBits(LCD_DC_Port,LCD_DC)
#define LCD_SCL_CLR()																	GPIO_ResetBits(LCD_SCL_Port,LCD_SCL)
#define LCD_SDA_CLR()																	GPIO_ResetBits(LCD_SDA_Port,LCD_SDA)

#if ENABLE_CS == 1
	#define LCD_CS_SET()																	GPIO_SetBits(LCD_CS_Port,LCD_CS)
	#define LCD_CS_CLR()																	GPIO_ResetBits(LCD_CS_Port,LCD_CS)
#endif

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//初始化模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
void LCD_Init2(void);

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//在指定位置画点
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条直线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一条矩形
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//显示图片

void LCD_ShowUC12x12(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC16x16(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC24x24(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC32x32(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC48x48(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC64x64(uint16_t x,uint16_t y,uint8_t buf[],uint16_t fc,uint16_t bc);
void LCD_ShowUC16x8(uint16_t x,uint16_t y,uint8_t buf,uint16_t fc);
void LCD_ShowStr(uint16_t x,uint16_t y,uint8_t *str,uint8_t n);

#endif
