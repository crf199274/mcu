#ifndef __OLED_H
#define __OLED_H

void oled_init(void);
void oled_write_cmd(unsigned char cmd);
void oled_write_data(unsigned char mdata);
void oled_screen_fill(unsigned char cmd);
void oled_Set_pos(unsigned char x,unsigned char y);
void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch);
//void OLED_OFF(void);
//void OLED_ON(void);
void oled_screen_init(void);

#endif
