#ifndef __sccb_H
#define __sccb_H

#include "esp32.h"

#define SCCB_SDA              26
#define SCCB_SCL              27
#define SCCB_SDA_OUT()        pinMode(SCCB_SDA,OUTPUT)
#define SCCB_SDA_IN()         pinMode(SCCB_SDA,INPUT)
#define SCCB_SCL_OUT()        pinMode(SCCB_SCL,OUTPUT)

#define SCCB_SDA_SET()        digitalWrite(SCCB_SDA,HIGH)
#define SCCB_SDA_CLR()        digitalWrite(SCCB_SDA,LOW)
#define SCCB_SDA_READ()       digitalRead(SCCB_SDA)

#define SCCB_SCL_SET()        digitalWrite(SCCB_SCL,HIGH)
#define SCCB_SCL_CLR()        digitalWrite(SCCB_SCL,LOW)

void SCCB_Start(void);
void SCCB_Stop(void)
void SCCB_NA(void);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Byte(u8 dat);

#endif
