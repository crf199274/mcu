#ifndef _pid_H
#define _pid_H

#include "stm32f10x.h"

void Motor_init(void);
void Motor_direction(void);
int PID_Calc(int NextPoint,int SetPoint);
int Vertical(float Med,float Angle,float gyro_Y) ;
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);
void SetMotorVoltageAndDirection(int *i16LeftVoltage,int *i16RightVoltage);

#endif
