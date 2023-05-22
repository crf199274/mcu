#include "key.h"
#include "OLED.h"

void keyInit(void)
{
	IT0 = 1;                                    //使能INT0下降沿中断
	EX0 = 1;                                    //使能INT0中断
	
	IT1 = 1;                                    //使能INT1下降沿中断
	EX1 = 1;                                    //使能INT1中断
	
	INT_CLKO |= 0x30;
	
}

//-----------------------------------------
//中断服务程序
//SUB
void exint0() interrupt 0       //INT0中断入口
{
	T2=0;
	while(INT0==0);
}

//-----------------------------------------
//中断服务程序
//ADD
void exint1() interrupt 2       //INT1中断入口
{
	T3=0;
	while(INT1==0);
}

//-----------------------------------------------
//中断服务程序
//FUN2
void exint2() interrupt 10          //INT2中断入口
{
	T1=0;
	while(INT2==0);
}

//-----------------------------------------------
//中断服务程序
//FUN1
//void exint3() interrupt 11          //INT3中断入口
//{
//	mode_key = 4;
//	while(INT3==0);
//}
