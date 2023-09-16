#ifndef __IAP_H
#define __IAP_H

#include "flash_ops.h"

typedef        void (*pFunction)(void);

#define       ApplicationAddress       0x08001400

void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize); /* 把升级数据写入App存储空间 */
void iap_load_app(u32 appxaddr); /* 跳转到应用程序段 */

#endif
