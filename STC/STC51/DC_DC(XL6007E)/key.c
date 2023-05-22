#include "key.h"

uint8_t mode_key = 0;

void keyInit(void)
{
	INT0 = 1;
	IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	EX0 = 1;                    //使能INT0中断
	INT1 = 1;
	IT1 = 1;                    //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
	EX1 = 1;                    //使能INT1中断
	
	INT_CLKO |= 0x30;
	
}

//-----------------------------------------
//中断服务程序
//SUB
void exint0() interrupt 0       //INT0中断入口
{
    mode_key = 1;
		while(INT0==0);
}

//-----------------------------------------
//中断服务程序
//ADD
void exint1() interrupt 2       //INT1中断入口
{
    mode_key = 2;
		while(INT1==0);
}

//-----------------------------------------------
//中断服务程序
//FUN2
void exint2() interrupt 10          //INT2中断入口
{
	mode_key = 3;
	while(INT2==0);
}

//-----------------------------------------------
//中断服务程序
//FUN1
void exint3() interrupt 11          //INT3中断入口
{
	mode_key = 4;
	while(INT3==0);
}
