#include "key.h"
#include "OLED.h"

void keyInit(void)
{
	IT0 = 1;                                    //ʹ��INT0�½����ж�
	EX0 = 1;                                    //ʹ��INT0�ж�
	
	IT1 = 1;                                    //ʹ��INT1�½����ж�
	EX1 = 1;                                    //ʹ��INT1�ж�
	
	INT_CLKO |= 0x30;
	
}

//-----------------------------------------
//�жϷ������
//SUB
void exint0() interrupt 0       //INT0�ж����
{
	T2=0;
	while(INT0==0);
}

//-----------------------------------------
//�жϷ������
//ADD
void exint1() interrupt 2       //INT1�ж����
{
	T3=0;
	while(INT1==0);
}

//-----------------------------------------------
//�жϷ������
//FUN2
void exint2() interrupt 10          //INT2�ж����
{
	T1=0;
	while(INT2==0);
}

//-----------------------------------------------
//�жϷ������
//FUN1
//void exint3() interrupt 11          //INT3�ж����
//{
//	mode_key = 4;
//	while(INT3==0);
//}
