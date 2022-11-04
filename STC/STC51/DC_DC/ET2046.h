#ifndef __ET2046_H
#define __ET2046_H

#define AIN0 0x94 
#define AIN1 0xD4
#define AIN2 0xA4
#define AIN3 0xE4

unsigned int ET2046_ConvertAndRead(unsigned char cmd);

#endif
