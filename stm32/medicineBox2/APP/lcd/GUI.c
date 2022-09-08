#include "GUI.h"
#include "rgbTFT.h"
#include "ff.h"
#include <stdlib.h>

FATFS fs_GUI;																/* Filesystem object */
FIL fil_GUI;																/* File object */
FRESULT res_GUI;														/* API result code */
uint8_t	readfile_GUI[LCD_W*2]={0};
UINT	bytesRead_GUI;
uint16_t GUI_j=0,GUI_k=0;

/*************************************************
函数名: UTF8TansformUnicode
功能: UTF-8转UINICODE
入口参数: uint8_t *s1 UTF-8字节流 uint16_t *s2 UINICODE
返回值: 无
*************************************************/
void UTF8TansformUnicode(uint8_t *s1,uint16_t *s2,uint8_t n)
{
	while(*s1!='\0'&&n--)
	{
		if((*s1&0x80)==0x00)
		{
			*s2=*s1;
			s2++;
			s1++;
		}
		else if((*s1&0xE0)==0xC0)
		{
			*s2=(*s1&0x1f)<<6;
			s1++;
			*s2|=(*s1&0x3f);
			s2++;
			s1++;
		}
		else if((*s1&0xF0)==0xE0)
		{
			*s2=(*s1&0x0F)<<12;
			s1++;
			*s2|=((*s1&0x3F)<<6);
			s1++;
			*s2|=(*s1&0x3F);
			s2++;
			s1++;
		}
	}
}

/*************************************************
函数名: GUI_Backgroud
功能: 设置LCD背景
入口参数: uint8_t *s 背景文件名(格式是的字符串)
返回值: GUI_ERROR类型
*************************************************/
GUI_ERROR GUI_Backgroud(uint8_t *s)
{
	uint32_t size = 0;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	
	if(res_GUI == FR_OK)
	{
		LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
		res_GUI = f_open(&fil_GUI,(const TCHAR *)s,FA_OPEN_EXISTING | FA_READ);
		if(res_GUI != FR_OK)
		{
			return GUI_NOT_OPEN;
		}
		
		size = f_size(&fil_GUI);
		
		for(GUI_j=0;GUI_j<((size/2)/LCD_W);GUI_j++)
		{
			res_GUI = f_read(&fil_GUI,readfile_GUI,sizeof(readfile_GUI),&bytesRead_GUI);
			if(res_GUI != FR_OK)
			{
				return GUI_NOT_READ;
			}
			for(GUI_k=0;GUI_k<LCD_W;GUI_k++)
			{
				LCD_WR_DATA8(readfile_GUI[GUI_k*2]);
				LCD_WR_DATA8(readfile_GUI[GUI_k*2+1]);
			}
		}
		
		for(GUI_j=0;GUI_j<(LCD_H-((size/2)/LCD_W));GUI_j++)
		{
			for(GUI_k=0;GUI_k<LCD_W;GUI_k++)
			{
				LCD_WR_DATA8(0xff);
				LCD_WR_DATA8(0xff);
			}
		}
		
	}else{
		return GUI_NOT_MOUNT;
	}
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	return GUI_OK;
	
}

/*************************************************
函数名: GUI_WIFI_LOGO
功能: 设置WIFI图标
入口参数: uint8_t *s 背景文件名(格式是的字符串)
返回值: GUI_ERROR类型
*************************************************/
GUI_ERROR GUI_WIFI_LOGO(uint8_t *s)
{
	uint8_t W=0,H=0;
	uint8_t num[2];
	
	num[0] = s[2];
	num[1] = s[3];
	
	W = atoi((const char *)num);
	
	num[0] = s[5];
	num[1] = s[6];
	
	H = atoi((const char *)num);
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	
	if(res_GUI == FR_OK)
	{
		LCD_Address_Set(LCD_W-W,0,LCD_W-1,H-1);
		res_GUI = f_open(&fil_GUI,(const TCHAR *)s,FA_OPEN_EXISTING | FA_READ);
		if(res_GUI != FR_OK)
		{
			return GUI_NOT_OPEN;
		}
		for(GUI_j=0;GUI_j<H;GUI_j++)
		{
			res_GUI = f_read(&fil_GUI,readfile_GUI,W*2,&bytesRead_GUI);
			if(res_GUI != FR_OK)
			{
				return GUI_NOT_READ;
			}
			for(GUI_k=0;GUI_k<W;GUI_k++)
			{
				LCD_WR_DATA8(readfile_GUI[GUI_k*2]);
				LCD_WR_DATA8(readfile_GUI[GUI_k*2+1]);
			}
		}
	}else{
		return GUI_NOT_MOUNT;
	}
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	return GUI_OK;
	
}

/*************************************************
函数名: ONENET_LOGO
功能: 设置ONENET图标
入口参数: uint8_t *s 背景文件名(格式是的字符串)
返回值: GUI_ERROR类型
*************************************************/
GUI_ERROR ONENET_LOGO(uint8_t *s)
{
	uint8_t H = 12;
	uint8_t W = 19;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	
	if(res_GUI == FR_OK)
	{
		LCD_Address_Set((LCD_W-16-W),0,(LCD_W-16-1),H-1);
		res_GUI = f_open(&fil_GUI,(const TCHAR *)s,FA_OPEN_EXISTING | FA_READ);
		if(res_GUI != FR_OK)
		{
			return GUI_NOT_OPEN;
		}
		for(GUI_j=0;GUI_j<H;GUI_j++)
		{
			res_GUI = f_read(&fil_GUI,readfile_GUI,W*2,&bytesRead_GUI);
			if(res_GUI != FR_OK)
			{
				return GUI_NOT_READ;
			}
			for(GUI_k=0;GUI_k<W;GUI_k++)
			{
				LCD_WR_DATA8(readfile_GUI[GUI_k*2]);
				LCD_WR_DATA8(readfile_GUI[GUI_k*2+1]);
			}
		}
	}else{
		return GUI_NOT_MOUNT;
	}
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	return GUI_OK;
	
}

/*************************************************
函数名: GUI_ShowUC_12x12
功能: 显示一个12x12的点阵字符(对LCD_ShowUC12x12进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_12x12(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[24]={0};
	
	unicode = unicode*24;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC12x12.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC12x12(x+4,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUC_16x16
功能: 显示一个16x16的点阵字符(对LCD_ShowUC16x16进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_16x16(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[32]={0};
	
	unicode = unicode*32;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC16x16.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC16x16(x,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUC_24x24
功能: 显示一个24x24的点阵字符(对LCD_ShowUC24x24进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_24x24(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[72]={0};
	
	unicode = unicode*72;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC24x24.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC24x24(x,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUC_32x32
功能: 显示一个32x32的点阵字符(对LCD_ShowUC32x32进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_32x32(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[128]={0};
	
	unicode = unicode*128;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC32x32.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC32x32(x,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUC_48x48
功能: 显示一个48x48的点阵字符(对LCD_ShowUC48x48进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_48x48(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[288]={0};
	
	unicode = unicode*288;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC48x48.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC48x48(x,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUC_64x64
功能: 显示一个64x64的点阵字符(对LCD_ShowUC64x64进一步封装)
入口参数: uint32_t unicode UNICODE的字符对照表的值(两个字节的UNICODE对照表)   uint16_t x uint16_t y 起始坐标 
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUC_64x64(uint32_t unicode,uint16_t x,uint16_t y)
{
	uint8_t buf[512]={0};
	
	unicode = unicode*512;
	
	/* 挂载文件系统 */
	res_GUI = f_mount(&fs_GUI,"0:", 1);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_open(&fil_GUI,"UC64x64.Dzk",FA_OPEN_EXISTING | FA_READ);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_lseek(&fil_GUI,unicode);
	if(res_GUI!=GUI_OK)
		return (uint8_t)GUI_ERROR_INFO;
	res_GUI = f_read(&fil_GUI,buf,sizeof(buf),&bytesRead_GUI);
	
	f_close(&fil_GUI);
	
	/*卸载文件系统*/
	f_mount(0, "0:", 1);//取消挂载
	
	LCD_ShowUC64x64(x,y,buf,0x19,0xffff);
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_ShowUCString
功能: 显示点阵字符字符串
入口参数: uint8_t *s  字符串(只解析ascll)  uint16_t x uint16_t y 起始坐标 uint8_t Ssize 字体大小(12,16,24,32,48,64)
返回值: GUI_OK_INFO,GUI_ERROR_INFO
*************************************************/
uint8_t GUI_ShowUCString(uint16_t *s,uint16_t x,uint16_t y,uint8_t Ssize)
{
	uint8_t i=0;
	uint8_t k=LCD_W/Ssize;
	
	if(Ssize!=12&&Ssize!=16&&Ssize!=24&&Ssize!=32&&Ssize!=48&&Ssize!=64)
		return (uint8_t)GUI_ERROR_INFO;
	
	for(i=0;i<k;i++)
	{
		if(*s=='\0')
			break;
		if(Ssize==12)
			GUI_ShowUC_12x12(*s,x,y);
		else if(Ssize==16)
			GUI_ShowUC_16x16(*s,x,y);
		else if(Ssize==24)
			GUI_ShowUC_24x24(*s,x,y);
		else if(Ssize==32)
			GUI_ShowUC_32x32(*s,x,y);
		else if(Ssize==48)
			GUI_ShowUC_48x48(*s,x,y);
		else
			GUI_ShowUC_64x64(*s,x,y);
		s++;
		x+=Ssize;
	}
	
	return (uint8_t)GUI_OK_INFO;
}

/*************************************************
函数名: GUI_realtime
功能: 把时间显示在LCD上
入口参数: uint16_t *date  年月日   uint16_t *time  小时分钟秒
返回值: 无
*************************************************/
void GUI_realtime(uint8_t *date,uint8_t *time)
{
	
	LCD_ShowStr(5,4,date,10);//年月日
	LCD_ShowStr(5,24,time,8);//小时分钟
	
}

/*************************************************
函数名: GUI_INFOPRINT
功能: 把INFO显示在LCD上
入口参数: uint8_t *s INFO
返回值: 无
*************************************************/
void GUI_INFOPRINT(uint8_t *s)
{
	uint16_t unicode[LCD_W/INFO+1]={0};
	
	UTF8TansformUnicode(s,unicode,LCD_W/INFO);
	
	LCD_Fill(0,LCD_H-48,LCD_W-1,LCD_H-48+INFO-1,0xffff);
	
	GUI_ShowUCString(unicode,0,LCD_H-48,INFO);
	
}

/*************************************************
函数名: GUI_INFOPRINT2
功能: 把INFO显示在LCD上
入口参数: uint8_t *s INFO
返回值: 无
*************************************************/
void GUI_INFOPRINT2(uint8_t *s)
{
	uint16_t unicode[LCD_W/INFO2+1]={0};
	
	UTF8TansformUnicode(s,unicode,LCD_W/INFO2);
	
	LCD_Fill(0,LCD_H-INFO2,LCD_W-1,LCD_H-1,0xffff);
	
	GUI_ShowUCString(unicode,0,LCD_H-INFO2,INFO2);
	
}

