#include "mbed.h"
#include "packet.h"

#if DEBUG == 1
    Serial pc(USBTX, USBRX); // 디버깅용
#else
    Serial pc(PC_TX, PC_RX); // 실제 통신 모듈
#endif

static uint8_t packet[100] = {HEADER1, HEADER2, HEADER3}; //전송할 패킷이 저장되는 변수
static uint8_t packet_size = 0;

void make_packet(int16_t * gyro, uint8_t gyro_size, int16_t * accel, uint8_t accel_size, float * gps_data, uint8_t gps_data_size)
{
    // 매번 패킷 id 1씩 증가
    static uint16_t packet_id = 0;
    union int16to8 int_union;
    int_union.INT16 = packet_id++;
    
    int index = 3; // 패킷 헤더 3개 이후부터 데이터 채움
    *(packet + index++) = int_union.INT8[0]; // 패킷 id 2바이트
    *(packet + index++) = int_union.INT8[1];
    *(packet + index++) = gyro_size + accel_size + gps_data_size; // 패킷 size 1바이트
    
    memcpy(packet + index, gyro, gyro_size);         index += gyro_size;    // 자이로 데이터 추가
    memcpy(packet + index, accel, accel_size);       index += accel_size;   // 가속도 데이터 추가
    memcpy(packet + index, gps_data, gps_data_size); index += gps_data_size; // gps 데이터 추가
    
    // 체크섬 계산
    uint8_t checksum = 0;
    for(int i=0; i<index; i++){
        checksum ^= *(packet + i);
    }
    *(packet + index++) = checksum;
    
    // Tail : 0x00
    *(packet + index++) = 0;

    // index는 패킷의 전체 길이
    packet_size = index;
}


void send_packet(int16_t * gyro, uint8_t gyro_size, int16_t * accel, uint8_t accel_size, float * gps_data, uint8_t gps_data_size)
{
#if SEND_PACKET == 1
    make_packet(gyro, gyro_size, accel, accel_size, gps_data, gps_data_size);
    
    for(int i=0; i<packet_size; i++){
        pc.putc(*(packet + i));
    }
#else
    pc.printf("  gyro: %5d %5d %5d ||", gyro[0], gyro[1], gyro[2]);  // unit: scale_factor*degree/s
    pc.printf("  accel: %5d %5d %5d ||", accel[0], accel[1], accel[2]);  // unit: scale_factor*g
    pc.printf("  gps: %6.0f %5.4f %5.4f %4.2f \r\n", gps_data[0], gps_data[1], gps_data[2], gps_data[3]);
#endif
}
