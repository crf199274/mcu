#ifndef __KEY_H
#define __KEY_H

#include "stc8a8k64d4.h"
#include "typedef.h"

sbit INT1 = P3^3;
sbit INT0 = P3^2;
sbit INT2 = P3^6;
sbit INT3 = P3^7;

struct key_msg{
	
	/* ��¼�ĸ����������� */
	uint8_t mode_key;
	/* ��¼�л��ֱ��ʰ�����״̬ */
	uint8_t key_RR;
	/* ��¼�л���ѹ��ֵ������״̬ */
	uint8_t key_EV;
};

extern struct key_msg key_msg_1;

extern uint8_t mode_key;

void keyInit(void);

#endif
