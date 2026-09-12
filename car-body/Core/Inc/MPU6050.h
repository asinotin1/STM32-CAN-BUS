#ifndef MPU6050_H
#define MPU6050_H
#include "main.h"
#include "math.h"
extern I2C_HandleTypeDef hi2c1;
extern float AX, AY, AZ;
extern float GX, GY, GZ;


#define MPU6050_ADDR         0xD0
#define SMPLRT_DIV_REG      0x19
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C
#define ACCEL_XOUT_H_REG    0x3B
#define TEMP_OUT_H_REG      0x41
#define GYRO_XOUT_H_REG     0x43
#define PWR_MGMT_1_REG      0x6B
#define WHO_AM_I_REG        0x75

void MPU6050_Init(void);
void MPU6050_read_accel(void);
void MPU6050_read_gyro(void);

float MPU6050_vibrate(void);
#endif