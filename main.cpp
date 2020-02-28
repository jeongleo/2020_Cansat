#include "mbed.h"
#include "config.h"
#include "mpu9250.h"
#include "packet.h"
#include "gps.h"

Ticker ticker; //일정 시간마다 인터럽트를 발생시키는 인터럽트 객체

void process(); //인터럽트 발생 시 수행할 함수 원형

int main(){
    pc.baud(PC_BAUD);
    
    #ifdef MPU
        MPU9250_RESET();
        MPU9250_INIT();
        find_gyro_bias();
    #endif
    
    #ifdef GPS
        gps.baud(GPS_BAUD);
        gps.attach(&Receiving, Serial::RxIrq); //gps serial 통신 인터럽트, 수신되면 큐에 쌓는 인터럽트이다.
    #endif
    
    ticker.attach(&process, 0.02); //0.02초마다 인터럽트 발생(pc baud rate와 밀접한 관련이 있음. 함부로 바꾸지 말것)
    
    while(1){
        #ifdef GPS
            gps_sampling(); //gps 데이터를 계속 샘플링한다.
        #endif
    }
}

void process(){
    int16_t gyro[3] = {0}; //gyro_x, y, z
    int16_t accel[3] = {0};  //accel_x, y, z
    float gps_data[4] = {0}; //time, lon, lat, alt
    
    // 데이터 읽어오기
    #ifdef GPS
        GET_GPS(gps_data);
    #endif
    
    #ifdef MPU
        get_MPU_d(gyro, accel);
        //MPU9250_GET_GYRO(gyro);  // 이 코드 대신 윗줄 MPU_d()에서 한번에 읽어보도록 처리했다.
        //MPU9250_GET_ACCEL(accel);
    #endif
    
    // 패킷 전송, 데이터 길이 28byte, 패킷 총길이 36byte.
    send_packet(gyro, 6, accel, 6, gps_data, 16); //데이터를 패킷에 넣고 패킷을 전송한다.
}
