#ifndef  _IIC_H
#define _IIC_H

//sbit OLED_RES = P3^0;
//sbit OLED_DC = P3^1;

void iic_init(void);
void iic_start(void);
void iic_stop(void);
unsigned char iic_wait_ack(void);
//void iic_ack(void);
//void iic_nack(void);
void iic_send_byte(unsigned char mdata);
//unsigned char iic_read_byte(unsigned char ack);

#endif