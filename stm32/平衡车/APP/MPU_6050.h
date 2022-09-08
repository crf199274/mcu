#ifndef _MPU_6050_H
#define _MPU_6050_H

#include "stm32f10x.h"

#define SMPLRT_DIV 0x19       
#define CONFIG 0x1A                
#define GYRO_CONFIG 0x1B   
#define ACCEL_CONFIG 0x1C 
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E 
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define PWR_MGMT_1 0x6B

//#define pi        3.14159265f
//#define kp       0.8f
//#define ki        0.001f
//#define halfT   0.01f 
#define incise 8

//extern float q0,q1,q2,q3;

u8 MPU_6050_init(void);
void MPU_6050_send_byte(u8 addr,u8 data);
u8 MPU_6050_Read_byte(u8 addr);
//void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
//void show(int data1,int data2,int data3);
void MPU_6050_IIC_init(void);
void average(float *pitch,float *roll,float *yaw);

//
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

#endif
