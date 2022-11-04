#ifndef __FingerPrint_H
#define __FingerPrint_H

#include "reg52.h"

//ACK宏定义
#define 							ACK_SUCCESS 										0x00 														//执行成功
#define 							ACK_FAIL 															0x01 														//执行失败
#define 							ACK_FULL 															0x04 														//数据库满
#define 							ACK_NOUSER 												0x05 														//没有这个用户
#define 							ACK_USER_EXIST 								0x07 														//用户已存在
#define 							ACK_TIMEOUT 											0x08 														//指纹图像采集超时
#define 							ACK_HARDWAREERROR 	0x0A 													//硬件错误
#define 							ACK_IMAGEERROR 						0x10 														//图像错误
#define 							ACK_BREAK 													0x18 														//终止当前指令
#define 							ACK_ALGORITHMFAIL 				0x11 														//贴膜攻击检测
#define 							ACK_HOMOLOGYFAIL 				0x12 														//同源性校验错误

extern unsigned int flag1;
extern unsigned int flag2;
extern unsigned int flag3;

void UserCompare(void);
void Color(void);
void CloseColor(void);
void UserInterrupt(void);

#endif
