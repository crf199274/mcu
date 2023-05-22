#include "OLED.h"
#include "IIC.h"
#include "delay.h"
#include "ascll.h"

void oled_write_cmd(unsigned char cmd)
{
	iic_start();
	iic_send_byte(0x78);
	iic_wait_ack();
	iic_send_byte(0x00);
	iic_wait_ack();
	iic_send_byte(cmd);
	iic_wait_ack();
	iic_stop();
}

void oled_write_data(unsigned char mdata)
{
	iic_start();
	iic_send_byte(0x78);
	iic_wait_ack();
	iic_send_byte(0x40);
	iic_wait_ack();
	iic_send_byte(mdata);
	iic_wait_ack();
	iic_stop();
}

//void OLED_OFF(void)
//{
//	oled_write_cmd(0X8D);  //设置电荷泵
//	oled_write_cmd(0X10);  //关闭电荷泵
//	oled_write_cmd(0XAE);  //OLED休眠
//}

void OLED_ON(void)
{
	oled_write_cmd(0X8D);  //设置电荷泵
	oled_write_cmd(0X14);  //开启电荷泵
	oled_write_cmd(0XAF);  //OLED唤醒
}

void oled_init(void)
{
	delay_ms(500);
	
	oled_write_cmd(0xAE); //display off
	oled_write_cmd(0x20);	//Set Memory Addressing Mode	
	oled_write_cmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	oled_write_cmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	oled_write_cmd(0xc8);	//Set COM Output Scan Direction
	oled_write_cmd(0x00); //---set low column address
	oled_write_cmd(0x10); //---set high column address
	oled_write_cmd(0x40); //--set start line address
	oled_write_cmd(0x81); //--set contrast control register
	oled_write_cmd(0xff); //亮度调节 0x00~0xff
	oled_write_cmd(0xa1); //--set segment re-map 0 to 127
	oled_write_cmd(0xa6); //--set normal display
	oled_write_cmd(0xa8); //--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3F); //
	oled_write_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	oled_write_cmd(0xd3); //-set display offset
	oled_write_cmd(0x00); //-not offset
	oled_write_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0xf0); //--set divide ratio
	oled_write_cmd(0xd9); //--set pre-charge period
	oled_write_cmd(0x22); //
	oled_write_cmd(0xda); //--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xdb); //--set vcomh
	oled_write_cmd(0x20); //0x20,0.77xVcc
	oled_write_cmd(0x8d); //--set DC-DC enable
	oled_write_cmd(0x14); //
	oled_write_cmd(0xaf); //--turn on oled panel
	
}

void oled_screen_fill(unsigned char cmd)
{
	unsigned int i,j;
	for(i=0;i<8;i++)
	{
		oled_write_cmd(0xb0+i);
		oled_write_cmd(0x00);
		oled_write_cmd(0x10);
		for(j=0;j<128;j++)
		{
			oled_write_data(cmd);
		}
	}
}

void oled_screen_init(void)
{
	OLED_ON();
	oled_screen_fill(0xff);
//	delay_ms(3000);
//	oled_screen_fill(0x00);
//	delay_ms(3000);
}

void oled_Set_pos(unsigned char x,unsigned char y)
{
	oled_write_cmd(0xb0+y);
	oled_write_cmd(((x&0xf0)>>4)|0x10);
	oled_write_cmd((x&0x0f)|0x01);
}

void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch)
{
	unsigned char i=0,j=0;
	unsigned int N=0;
	unsigned int addr=0;
	while(ch[i]!='\0')
	{
		N++;
		i++;
	}
	
	for(j=0;j<N;j++)
	{
		addr=(ch[j]-32)*16;
		if(x>120)
		{
			x=0;
			y++;
		}
		oled_Set_pos(x,y);
		for(i=0;i<8;i++)
		{
			oled_write_data(F8X16[addr]);
			addr=addr+1;
		}
		oled_Set_pos(x,y+1);
		for(i=0;i<8;i++)
		{
			oled_write_data(F8X16[addr]);
			addr=addr+1;
		}
		x+=8;
	}
}

