#pragma once

//mpu9250 register
#define MPU9250_ADDRESS (0x68<<1) //0b11010000
#define WHO_AM_I 0x75
#define FIFO_EN 0x23
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define ACCEL_XOUT_H 0x3B
#define TEMP_OUT_H 0x41
#define GYRO_XOUT_H 0x43

#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define INT_PIN_CFG 0x37

#define AK8963_ADDRESS (0x0C<<1) // 0b00001100
#define WHO_AM_I_AK8963 0x00 // should return 0x48

extern I2C i2c;
extern Serial pc;

void MPU9250_RESET(void);
void MPU9250_INIT(void);
void MPU9250_GET_GYRO(int16_t * destination);
void MPU9250_GET_ACCEL(int16_t * destination);
void gyro_bias();
void get_MPU(float * gyro_f, float * accel_f);
