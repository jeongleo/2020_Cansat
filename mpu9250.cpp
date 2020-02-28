#include "mbed.h"
#include "mpu9250.h"

I2C i2c(MPU_SDA, MPU_SCL); //mpu9250
DigitalOut led1(LED1); // gyro bias 잡을때 켜지는 led

float gyro_bias_f[3] = {0};  // 소수점 bias
int16_t gyro_bias[3] = {0};  // scale factor를 나누지 않은 int16_t 정수형 bias

void MPU9250_RESET(void) {
    wait(0.1);
    char cmd[2];
    cmd[0] = PWR_MGMT_1; //status
    cmd[1] = 0x80;
    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);
}

void MPU9250_INIT(void) {
    wait(0.1);
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
    cmd[1] = ((uint8_t)FS_SEL) << 3;
//    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
    wait(0.1);

    cmd[0] = ACCEL_CONFIG; 
    cmd[1] = ((uint8_t)AFS_SEL) << 3;
//    i2c.write(MPU9250_ADDRESS, cmd, 2, 0);
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

void find_gyro_bias() {
    led1 = 1;
    int16_t gyro1[3];
    for (int i = 0; i < 100; i++) {
        MPU9250_GET_GYRO(gyro1);
        gyro_bias[0] += gyro1[0];
        gyro_bias[1] += gyro1[1];
        gyro_bias[2] += gyro1[2];
        gyro_bias_f[0] += gyro1[0] / GYRO_SCALE_FACTOR;
        gyro_bias_f[1] += gyro1[1] / GYRO_SCALE_FACTOR;
        gyro_bias_f[2] += gyro1[2] / GYRO_SCALE_FACTOR;

    }
    gyro_bias[0] = (int)(gyro_bias[0]/100.0 + 0.5); // 평균 낼 때 반올림
    gyro_bias[1] = (int)(gyro_bias[1]/100.0 + 0.5);
    gyro_bias[2] = (int)(gyro_bias[2]/100.0 + 0.5);
    gyro_bias_f[0] /= 100.0;
    gyro_bias_f[1] /= 100.0;
    gyro_bias_f[2] /= 100.0;
    
//    pc.printf("%d %d %d\r\n", gyro_bias[0], gyro_bias[1], gyro_bias[2]);
//    pc.printf("%f %f %f\r\n", gyro_bias[0]/32.8, gyro_bias[1]/32.8, gyro_bias[2]/32.8);
//    pc.printf("%f %f %f\r\n", gyro_bias_f[0], gyro_bias_f[1], gyro_bias_f[2]);
    
    led1 = 0;

}

void get_MPU_f(float * gyro_f, float * accel_f){
    int16_t gyro[3] = {0};
    int16_t accel[3] = {0};
    
    MPU9250_GET_GYRO(gyro);
    for(int i=0; i<3; i++){
        gyro_f[i] = gyro[i]/GYRO_SCALE_FACTOR - gyro_bias_f[i];
    }
    
    MPU9250_GET_ACCEL(accel);
    for(int i=0; i<3; i++){
        accel_f[i]=accel[i]/ACCEL_SCALE_FACTOR;
    }
}

void get_MPU_d(int16_t * gyro, int16_t * accel){
    MPU9250_GET_GYRO(gyro);
    MPU9250_GET_ACCEL(accel);
    
    for(int i=0; i<3; i++){
        gyro[i] -= gyro_bias[i];
    }
    
}
