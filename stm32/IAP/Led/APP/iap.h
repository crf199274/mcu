#ifndef __IAP_H
#define __IAP_H

#include "stm32f10x.h"

typedef        void (*pFunction)(void);

#define       BootloaderAddress       0x08000000

void iap_load_bootloader(uint32_t appxaddr); /* 跳转到应用程序段 */

#endif
