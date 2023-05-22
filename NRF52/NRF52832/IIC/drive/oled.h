#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include <stdbool.h>

//I2C引脚定义
#define TWI_SCL_M           31         //I2C SCL引脚
#define TWI_SDA_M           30         //I2C SDA引脚

#define OLED_ADDRESS				(0x78>>1)

void twi_master_init(void);

void oled_init(void);

void oled_screen_fill(unsigned char cmd);

void oled_show_line(const char *const buf,uint8_t line,uint8_t size);

#endif
