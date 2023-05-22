#ifndef _pid_H
#define _pid_H

#include "stm32f10x.h"

extern int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
extern int Motor_L,Motor_R;
extern float Mechanical_angle; // �޳����������ƽ���ŵ�С����е��ֵ

void Motor_init(void);
void MotorR_direction(uint8_t Direction);
void MotorL_direction(uint8_t Direction);
void Xianfu_Pwm(void);
void Set_Pwm(int moto1,int moto2);
int balance_UP(float Angle,float Mechanical_balance,float Gyro);
int velocity(int encoder_left,int encoder_right);
int turn(int encoder_left,int encoder_right,float gyro);//ת�����

#endif
