#ifndef __IAP_H
#define __IAP_H

#include "flash_ops.h"

typedef        void (*pFunction)(void);

#define       ApplicationAddress       0x08001400

void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize); /* ����������д��App�洢�ռ� */
void iap_load_app(u32 appxaddr); /* ��ת��Ӧ�ó���� */

#endif
