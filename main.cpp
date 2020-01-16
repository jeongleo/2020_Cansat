#include "mbed.h"
#include "config.h"
#include "mpu9250.h"
#include "packet.h"
#include "gps.h"

Ticker ticker;
Timer t;
//Serial a(USBTX, USBRX);

void process();

int main(){
//    a.baud(9600);
    pc.baud(PC_BAUD);
    gps.baud(GPS_BAUD);
    
    MPU9250_RESET();
    MPU9250_INIT();
    gyro_bias();
    
    gps.attach(&Receiving, Serial::RxIrq); //gps serial 통신 인터럽트, 수신되면 큐에 쌓는 인터럽트이다.
    ticker.attach(&process, 0.02);
    
    while(1){
        gps_sampling();
    }
}

void process(){
    int16_t gyro[3] = {0}, accel[3] = {0};
    float gps_data[4] = {0}; //time, lon, lat, alt
    uint8_t data[24] = {0};
    
    MPU9250_GET_GYRO(gyro);
    MPU9250_GET_ACCEL(accel);
    GET_GPS(gps_data);
    
    make_data(data, gyro, accel, gps_data);
    send_packet(make_packet(data, 28));
}