#pragma once
#include "config.h"

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
void MPU9250_INIT(void); //mpu 각종 설정 수행
void MPU9250_GET_GYRO(int16_t * destination); //자이로 값 읽기
void MPU9250_GET_ACCEL(int16_t * destination); //가속도 값 읽기
void find_gyro_bias(); // 자이로  bias 잡기 초반 100개 평균을 낸다.
void get_MPU_f(float * gyro_f, float * accel_f); //자이로 가속도 값을 스케일 변환한다. bias잡은 것도 적용한다.
void get_MPU_d(int16_t * gyro, int16_t * accel); //자이로 가속도 값을 읽은 정수형 그대로 저장한다. bias잡은 것도 적용한다.


// 자이로계 측정범위
// FS_SEL : Full-Scale Range, (Sensitivity Scale Factor)
// 0 : +/-250 degree/s, (131.0 LSB/(degree/s))
// 1 : +/-500 degree/s, (65.5 LSB/(degree/s))
// 2 : +/-1000 degree/s, (32.8 LSB/(degree/s))
// 3 : +/-2000 degree/s, (16.4 LSB/(degree/s))
#if FS_SEL == 0
#define GYRO_SCALE_FACTOR 131.0
#elif FS_SEL == 1
#define GYRO_SCALE_FACTOR 65.5
#elif FS_SEL == 2
#define GYRO_SCALE_FACTOR 32.8
#elif FS_SEL == 3
#define GYRO_SCALE_FACTOR 16.4
#endif

// 가속도계 측정범위
// AFS_SEL : Full-Scale Range, (Sensitivity Scale Factor)
// 0 : +/-2 g, (16384.0 LSB/g)
// 1 : +/-4 g, (8192.0 LSB/g)
// 2 : +/-8 g, (4096.0 LSB/g)
// 3 : +/-16 g, (2048.0 LSB/g)
#if AFS_SEL == 0
#define ACCEL_SCALE_FACTOR 16384.0
#elif AFS_SEL == 1
#define ACCEL_SCALE_FACTOR 8192.0
#elif AFS_SEL == 2
#define ACCEL_SCALE_FACTOR 4096.0
#elif AFS_SEL == 3
#define ACCEL_SCALE_FACTOR 2048.0
#endif
