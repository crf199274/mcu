#ifndef __IAP_H
#define __IAP_H

#include "stm32f10x.h"

typedef        void (*pFunction)(void);

#define       BootloaderAddress       0x08000000

void iap_load_bootloader(uint32_t appxaddr); /* ��ת��Ӧ�ó���� */

#endif
