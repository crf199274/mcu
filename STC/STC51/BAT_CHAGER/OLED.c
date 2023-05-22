#include "OLED.h"
#include "IIC.h"
#include "delay.h"
#include "ascll.h"
#include "stdio.h"
#include "string.h"

float VOUT1=0;
float VOUT2=0;
float VOUT3=0;
float IOUT1=0;
float IOUT2=0;
float IOUT3=0;

uint8_t T1 = 0;
uint8_t T2 = 0;
uint8_t T3 = 0;

uint16_t MIN1 = 0;
uint16_t MIN2 = 0;
uint16_t MIN3 = 0;

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

void oled_init(void)
{
	Delay500ms();
	
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
	
	oled_screen_fill(0x00);
	
}

//void oled_screen_init(void)
//{
//	OLED_ON();
//	oled_screen_fill(0xff);
////	delay_ms(3000);
////	oled_screen_fill(0x00);
////	delay_ms(3000);
//}

void oled_Set_pos(unsigned char x,unsigned char y)
{
	oled_write_cmd(0xb0+y);
	oled_write_cmd(((x&0xf0)>>4)|0x10);
	oled_write_cmd((x&0x0f)|0x01);
}

void OLED_P6x8Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		oled_Set_pos(x,y);
		for(i=0;i<6;i++)
		oled_write_data(F6x8[c][i]);
		x+=6;
		j++;
	}
}

//void oled_write_size_16x8(unsigned char x,unsigned char y,const char *const ch)
//{
//	unsigned char i=0,j=0;
//	unsigned int N=0;
//	unsigned int addr=0;
//	while(ch[i]!='\0')
//	{
//		N++;
//		i++;
//	}
//	
//	for(j=0;j<N;j++)
//	{
//		addr=(ch[j]-32)*16;
//		if(x>120)
//		{
//			x=0;
//			y++;
//		}
//		oled_Set_pos(x,y);
//		for(i=0;i<8;i++)
//		{
//			oled_write_data(F8X16[addr]);
//			addr=addr+1;
//		}
//		oled_Set_pos(x,y+1);
//		for(i=0;i<8;i++)
//		{
//			oled_write_data(F8X16[addr]);
//			addr=addr+1;
//		}
//		x+=8;
//	}
//}

void updata_OLED(void)
{
	
	char buf[21];
//	float M1,M2,M3,H1,H2,H3;
		
	IOUT1 = IOUT1*1000;
//	if(IOUT1>=100)
//	{
//		T1=1;
//		MIN1=0;
//	}
	memset(buf,0,21);
	sprintf(buf,"VO1:%5.2fV IO1:%4dmA",VOUT1,(int)IOUT1);
	OLED_P6x8Str(0,0,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	IOUT2 = IOUT2*1000;
//	if(IOUT2>=100)
//	{
//		T2=1;
//		MIN2=0;
//	}
	memset(buf,0,21);
	sprintf(buf,"VO2:%5.2fV IO2:%4dmA",VOUT2,(int)IOUT2);
	OLED_P6x8Str(0,1,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	IOUT3 = IOUT3*1000;
//	if(IOUT3>=100)
//	{
//		T3=1;
//		MIN3=0;
//	}
	memset(buf,0,21);
	sprintf(buf,"VO3:%5.2fV IO3:%4dmA",VOUT3,(int)IOUT3);
	OLED_P6x8Str(0,2,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
//	M1 = MIN1/60.0;
//	H1 = MIN1/3600.0;
//	memset(buf,0,21);
//	sprintf(buf,"T1:%6.1fmin %4.1fh",M1,H1);
//	OLED_P6x8Str(0,3,buf);//第一行 -- 8x16的显示单元显示ASCII码
//	
//	M2 = MIN2/60.0;
//	H2 = MIN2/3600.0;
//	memset(buf,0,21);
//	sprintf(buf,"T2:%6.1fmin %4.1fh",M2,H2);
//	OLED_P6x8Str(0,4,buf);//第一行 -- 8x16的显示单元显示ASCII码
//	
//	M3 = MIN3/60.0;
//	H3 = MIN3/3600.0;
//	memset(buf,0,21);
//	sprintf(buf,"T3:%6.1fmin %4.1fh",M3,H3);
//	OLED_P6x8Str(0,5,buf);//第一行 -- 8x16的显示单元显示ASCII码
	
	
}
