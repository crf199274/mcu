#ifndef __FLASH_OPS_H
#define __FLASH_OPS_H

#include "stm32f10x.h"

#define STM32_FLASH_SIZE 64 	 		/* ��ѡSTM32��FLASH������С(��λΪK) */

/* FLASH��ʼ��ַ */
#define STM32_FLASH_BASE 0x08000000 	/* STM32 FLASH����ʼ��ַ */


uint16_t STMFLASH_ReadHalfWord(uint32_t faddr); /* ��ȡָ����ַ�İ���(16λ����) */
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead); /* ��ָ����ַ��ʼ����ָ�����ȵ����� */
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite); /* ������д�� */
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite); /* ��ָ����ַ��ʼд��ָ�����ȵ����� */

#endif
