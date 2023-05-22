#include "key.h"

uint8_t mode_key = 0;

void keyInit(void)
{
	INT0 = 1;
	IT0 = 1;                    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
	EX0 = 1;                    //ʹ��INT0�ж�
	INT1 = 1;
	IT1 = 1;                    //����INT1���ж����� (1:���½��� 0:�����غ��½���)
	EX1 = 1;                    //ʹ��INT1�ж�
	
	INT_CLKO |= 0x30;
	
}

//-----------------------------------------
//�жϷ������
//SUB
void exint0() interrupt 0       //INT0�ж����
{
    mode_key = 1;
		while(INT0==0);
}

//-----------------------------------------
//�жϷ������
//ADD
void exint1() interrupt 2       //INT1�ж����
{
    mode_key = 2;
		while(INT1==0);
}

//-----------------------------------------------
//�жϷ������
//FUN2
void exint2() interrupt 10          //INT2�ж����
{
	mode_key = 3;
	while(INT2==0);
}

//-----------------------------------------------
//�жϷ������
//FUN1
void exint3() interrupt 11          //INT3�ж����
{
	mode_key = 4;
	while(INT3==0);
}
