#ifndef __FINGERPRINT_H
#define __FINGERPRINT_H

#include "stm32f10x.h"

#define DEBUG				0

#define ACK_SUCCESS 0x00 //操作成功
#define ACK_FAIL 0x01 //操作失败
#define ACK_FULL 0x04 //指纹数据库已满
#define ACK_NOUSER 0x05 //无此用户
#define ACK_USER_OCCUPIED 0x06 //用户已存在
#define ACK_FINGER_OCCUPIED 0x07 //指纹已存在
#define ACK_TIMEOUT 0x08 //采集超时
//CHK：校验和，为第 2 字节到第 6 字节的异或值

#define fingerprintSleepIOClock																					RCC_APB2Periph_GPIOA
#define fingerprintSleepIOPort																					GPIOA
#define fingerprintSleepIO																							GPIO_Pin_5

#define fingerprintWAKEIOClock																					RCC_APB2Periph_GPIOA
#define fingerprintWAKEIOPort																						GPIOA
#define fingerprintWAKEIO																								GPIO_Pin_4

void fingerprintStaus(uint8_t staus);
void fingerprintINIT(uint32_t BaudRate);
uint8_t fingerprint(void);
void getfingerprintNotUsedID(void);
uint8_t fingerprintDEL(uint8_t HID,uint8_t LID);
uint8_t fingerprintDELALL(void);
uint8_t fingerprintCompare(void);
uint8_t fingerprintDownloadEigenvalue(uint8_t Eigenvalue[],uint8_t HID,uint8_t LID,uint8_t len);
uint8_t fingerprintInstallEigenvalue(uint8_t Eigenvalue[]);
uint8_t getUserCount(void);


#endif
