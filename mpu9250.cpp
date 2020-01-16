#include "mbed.h"
#include "config.h"
#include "mpu9250.h"

I2C i2c(MPU_SDA, MPU_SCL);
DigitalOut led1(LED1);

static float gyro_bias_f[3];

void MPU9250_RESET(void) {
    char cmd[2];
    cmd[0] = PWR_MGMT_1; //status
    cmd[1] = 0x80;
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);
}

void MPU9250_INIT(void) {
    char cmd[3];
    cmd[0] = PWR_MGMT_1; //reset
    cmd[1] = 0x80;
    i2c.write(MPU9250_ADDRESS, cmd, 2);
    wait(0.1);
 
    cmd[0] = PWR_MGMT_1; // Auto selects the best available clock source PLL if ready, else use the Internal oscillator
    cmd[1] = 0x01;
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    cmd[0] = CONFIG;
    cmd[1] = 0x03;// 41Hz gyro bandwidth, 1kHz internal sampling
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    // sample rate divisor for all sensors, 1000/(1+4)=200 Hz for Gyro 
    cmd[0] = SMPLRT_DIV;
    cmd[1] = 0x04;//0x04 
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    cmd[0] = GYRO_CONFIG;
    cmd[1] = 0b00010000;// Gyro full scale 1000 deg/sec; Gyro DLPF Enable
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    // Set accelerometer configuration
    // Accel fulll sacle range +/- 16g
    cmd[0] = ACCEL_CONFIG;
    cmd[1] = 0b00011000;// Accel 
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    cmd[0] = ACCEL_CONFIG2;
    cmd[1] = 0b00001100;
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);
    
    // XYZ Gyro accel enable (default)
    cmd[0] = PWR_MGMT_2;
    cmd[1] = 0x00;
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    //I2c bypass mode
    cmd[0] = INT_PIN_CFG;
    cmd[1] = 0x22; //0x02  
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);
}

void MPU9250_GET_GYRO(int16_t * destination)
{
    char cmd[6];
    cmd[0] = GYRO_XOUT_H;
    i2c.write(MPU9250_ADDRESS, cmd, 1, 1);
    i2c.read(MPU9250_ADDRESS, cmd, 6, 0);
    destination[0] = (int16_t)(((int16_t)cmd[0] << 8) | cmd[1]);
    destination[1] = (int16_t)(((int16_t)cmd[2] << 8) | cmd[3]);
    destination[2] = (int16_t)(((int16_t)cmd[4] << 8) | cmd[5]);
}

void MPU9250_GET_ACCEL(int16_t * destination)
{
    char cmd[6];
    cmd[0] = ACCEL_XOUT_H;
    i2c.write(MPU9250_ADDRESS, cmd, 1, 1);
    i2c.read(MPU9250_ADDRESS, cmd, 6, 0);
    destination[0] = (int16_t)(((int16_t)cmd[0] << 8) | cmd[1]);
    destination[1] = (int16_t)(((int16_t)cmd[2] << 8) | cmd[3]);
    destination[2] = (int16_t)(((int16_t)cmd[4] << 8) | cmd[5]);
}

void gyro_bias() {
    led1 = 1;
    int16_t gyro1[3];
//    pc.printf("Please keep still 5 seconds\n\r");
    for (int i = 0; i < 100; i++) {
        MPU9250_GET_GYRO(gyro1);
        gyro_bias_f[0] += gyro1[0] / 32.8;
        gyro_bias_f[1] += gyro1[1] / 32.8;
        gyro_bias_f[2] += gyro1[2] / 32.8;
//        pc.printf("bias finding i= %d\n\r", i);

    }
    gyro_bias_f[0] /= 100.0;
    gyro_bias_f[1] /= 100.0;
    gyro_bias_f[2] /= 100.0;
//    pc.printf("bias finding completed \n\r");
    led1 = 0;

}

void get_MPU(float * gyro_f, float * accel_f){
    int16_t gyro[3] = {0};
    int16_t accel[3] = {0};
    
    MPU9250_GET_GYRO(gyro);
    for(int i=0; i<3; i++){
        gyro_f[i] = gyro[i]/32.8 - gyro_bias_f[i];
    }
    
    MPU9250_GET_ACCEL(accel);
    for(int i=0; i<3; i++){
        accel_f[i]=accel[i]/2048.0;
    }
}