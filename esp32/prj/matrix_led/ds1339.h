#ifndef __ds1339_H
#define __ds1339_H

#include "arduino.h"
#include "Wire.h"

/*需要首次初始化时，配置以下寄存器*/
#define     REGADDR_SECONDS                         0x00   /* 秒   寄存器 */
#define     REGADDR_MINUTES                         0x01   /* 分   寄存器 */
#define     REGADDR_HOURS                           0x02   /* 时   寄存器 */
#define     REGADDR_DAY                             0x03   /* 周日 寄存器 */
#define     REGADDR_DATA                            0x04   /* 日   寄存器 */
#define     REGADDR_MONTH_CENTURY                   0x05   /* 月   寄存器 */
#define     REGADDR_YEAR                            0x06   /* 年   寄存器 */
/*需要设置闹钟，可以配置以下寄存器，否则默认即可*/          
#define     REGADDR_ALARM1SECONDS                   0x07   /* 闹钟1-秒   寄存器 */
#define     REGADDR_ALARM1MINUTES                   0x08   /* 闹钟1-分   寄存器 */
#define     REGADDR_ALARM1HOURS                     0x09   /* 闹钟1-时   寄存器 */
#define     REGADDR_ALARM1DAY_ALARM1DATE            0x0A   /* 闹钟1-周日-日 寄存器 */
                               
#define     REGADDR_ALARM2MINUTES                   0x0B   /* 闹钟2-分   寄存器 */
#define     REGADDR_ALARM2HOURS                     0x0C   /* 闹钟2-时   寄存器 */
#define     REGADDR_ALARM2DAY_ALARM2DATE            0x0D   /* 闹钟2-周日-日 寄存器 */
/*以下寄存器默认即可*/                                      
#define     REGADDR_CONTROL                         0x0E   /* 控制寄存器，用于设置晶振频率 */
#define     REGADDR_STATUS                          0x0F   /* 状态 */
#define     REGADDR_TRICKLECHARGER                  0x10

/*DS1339时间信息结构体*/
typedef struct T_DS1339TIME_  /* 24小时制 */
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t weekday;
    uint8_t day;
    uint8_t month;
    uint8_t year; /* 00-99 */
} T_DS1339TIME;

void ds1339_init(void);
uint8_t ds1339_write_time(const T_DS1339TIME *time);
uint8_t ds1339_read_time(T_DS1339TIME *time);
void ds1339_set_time_by_serial(void);

#endif
