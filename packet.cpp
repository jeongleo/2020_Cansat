#include "mbed.h"
#include "config.h"
#include "packet.h"

#if DEBUG==1
    Serial pc(USBTX, USBRX); // 디버깅용
#else
    Serial pc(PC_TX, PC_RX); // 실제 통신 모듈
#endif

static uint8_t packet[100] = {HEADER1, HEADER2, HEADER3};

void make_data(uint8_t * data, int16_t * gyro, int16_t * accel, float * gps_data){
    memcpy(data, gyro, 6);
    memcpy(data + 6, accel, 6);
    memcpy(data + 12, gps_data, 16);
}

int make_packet(uint8_t * data, uint8_t size){
    static uint16_t packet_id = 0;
    union int16to8 int_union;
    int_union.INT16 = packet_id++;
    
    int index = 3;
    *(packet + index++) = int_union.INT8[0];
    *(packet + index++) = int_union.INT8[1];
    *(packet + index++) = size;
    
    memcpy(packet+index, data, size);
    index += size;
    
    uint8_t checksum = 0;
    for(int i=0; i<index; i++){
        checksum ^= *(packet + i);
    }
    *(packet + index++) = checksum;
    
    return index; // 패킷 전체 길이 반환
}

void send_packet(int packet_size){
    for(int i=0; i<packet_size; i++){
        pc.putc(*(packet + i));
    }
}