#ifndef _MPU_6050_H
#define _MPU_6050_H

#include "stm32f10x.h"

#define SMPLRT_DIV 								0x19       
#define CONFIG 										0x1A                
#define GYRO_CONFIG 							0x1B   
#define ACCEL_CONFIG 							0x1C 
#define ACCEL_XOUT_H 							0x3B
#define ACCEL_XOUT_L 							0x3C
#define ACCEL_YOUT_H 							0x3D
#define ACCEL_YOUT_L 							0x3E 
#define ACCEL_ZOUT_H 							0x3F
#define ACCEL_ZOUT_L 							0x40
#define TEMP_OUT_H 								0x41
#define TEMP_OUT_L 								0x42
#define GYRO_XOUT_H 							0x43
#define GYRO_XOUT_L 							0x44
#define GYRO_YOUT_H 							0x45
#define GYRO_YOUT_L 							0x46
#define GYRO_ZOUT_H 							0x47
#define GYRO_ZOUT_L 							0x48
#define PWR_MGMT_1 								0x6B
#define PWR_MGMT_2 								0x6C
#define INT_ENABLE								0x38

#define incise 8

uint8_t MPU_6050_init(void);
void MPU_6050_send_byte(uint8_t addr,uint8_t data);
uint8_t MPU_6050_Read_byte(uint8_t addr);
void MPU_6050_IIC_init(void);
void average(float *pitch,float *roll,float *yaw);
void MPU6050_calibrate(void);
void MPU_gXYZ_Get(void);
void MPU_Get_Gyroscope(void);
void MPU_Set_Rate(uint16_t rate);

uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);

#endif
