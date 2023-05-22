#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stc8h.h"
#include "delay.h"

#define X_OFFSET 26
#define Y_OFFSET 1

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif

sbit LCD_SCL=P3^4;//SCLK
sbit LCD_SDA=P3^5;//MOSI
sbit LCD_RES=P3^7;//RES
sbit LCD_DC =P3^6;//DC
sbit LCD_CS =P3^3; //CS
sbit LCD_BLK=P5^4; //BLK

//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() LCD_SCL=0//SCL=SCLK
#define LCD_SCLK_Set() LCD_SCL=1

#define LCD_MOSI_Clr() LCD_SDA=0//SDA=MOSI
#define LCD_MOSI_Set() LCD_SDA=1

#define LCD_RES_Clr() LCD_RES=0//RES
#define LCD_RES_Set() LCD_RES=1

#define LCD_DC_Clr() LCD_DC=0//DC
#define LCD_DC_Set() LCD_DC=1

#define LCD_CS_Clr()  LCD_CS=0//CS
#define LCD_CS_Set()  LCD_CS=1

#define LCD_BLK_Clr()  LCD_BLK=1//BLK
#define LCD_BLK_Set()  LCD_BLK=0



void LCD_Writ_Bus(unsigned char dat);//模拟SPI时序
void LCD_WR_DATA8(unsigned char dat);//写入一个字节
void LCD_WR_DATA(unsigned int dat);//写入两个字节
void LCD_WR_REG(unsigned char dat);//写入一个指令
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




