#ifndef __INC_MPU6050_H__
#define __INC_MPU6050_H__

#include "i2c.h"

#define MPU_ADDRESS 0x68U
#define MPU_REG_PWR_MGMT_1 0x6BU
#define MPU_REG_SMPLRT_DIV 0x19U
#define MPU_REG_CONFIG 0x1AU
#define MPU_REG_GYRO_CONFIG 0x1BU
#define MPU_REG_ACCEL_CONFIG 0x1CU
#define MPU_REG_ACCEL_XOUT_H 0x3BU
#define MPU_REG_GYRO_XOUT_H 0x43U

void MPU_Init();
uint8_t MPU_Whoami();

void MPU_Read_Accel(float *arr);
void MPU_Read_Gyro(float *arr);

#endif