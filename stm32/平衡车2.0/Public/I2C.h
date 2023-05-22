#ifndef _I2C_H
#define	_I2C_H

#include "stm32f10x.h"

#define IIC_SCL_PORT   										GPIOB
#define IIC_SCL_PIN        								GPIO_Pin_10
#define IIC_SCL_PORT_RCC									RCC_APB2Periph_GPIOB

#define IIC_SDA_PORT   										GPIOB
#define IIC_SDA_PIN        								GPIO_Pin_11
#define IIC_SDA_PORT_RCC									RCC_APB2Periph_GPIOB

#define IIC_SCL_High											GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN)
#define IIC_SCL_Low 											GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN)
#define IIC_SDA_IN												GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN)
#define IIC_SDA_OUT_High									GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN)
#define IIC_SDA_OUT_Low										GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN)

void iic_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t data);
uint8_t iic_read_byte(uint8_t ack);
uint8_t iic_read_byte(uint8_t ack);

#endif
