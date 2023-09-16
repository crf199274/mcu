#ifndef __IAP_H
#define __IAP_H

#include "flash_ops.h"

typedef        void (*pFunction)(void);

#define       ApplicationAddress       0x08001400

void iap_write_appbin(uint32_t appxaddr,u8 *appbuf,uint32_t appsize); /* 把升级数据写入App存储空间 */
void iap_load_app(uint32_t appxaddr); /* 跳转到应用程序段 */

#endif
