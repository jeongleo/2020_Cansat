#pragma once
#include "config.h"

// Packet Header
#define HEADER1 0xFF
#define HEADER2 0xFE
#define HEADER3 0xFD

extern Serial pc;

union int16to8{ //int16_t를 int8_t로 변환할때 쓸 구조체
    uint16_t INT16;
    uint8_t INT8[2];
};

void make_packet(int16_t * gyro, uint8_t gyro_size, int16_t * accel, uint8_t accel_size, float * gps_data, uint8_t gps_data_size); //데이터 배열을 패킷에 집어넣음
void send_packet(int16_t * gyro, uint8_t gyro_size, int16_t * accel, uint8_t accel_size, float * gps_data, uint8_t gps_data_size); //패킷 size만큼 전송
