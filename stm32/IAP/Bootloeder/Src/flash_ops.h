#ifndef __FLASH_OPS_H
#define __FLASH_OPS_H

#include "stm32f10x.h"

#define STM32_FLASH_SIZE 64 	 		/* 所选STM32的FLASH容量大小(单位为K) */

/* FLASH起始地址 */
#define STM32_FLASH_BASE 0x08000000 	/* STM32 FLASH的起始地址 */


uint16_t STMFLASH_ReadHalfWord(uint32_t faddr); /* 读取指定地址的半字(16位数据) */
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead); /* 从指定地址开始读出指定长度的数据 */
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite); /* 不检查的写入 */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite); /* 从指定地址开始写入指定长度的数据 */

#endif
