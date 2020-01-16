#pragma once

#define HEADER1 0xFF
#define HEADER2 0xFE
#define HEADER3 0xFD

extern Serial pc;

union int16to8{
    uint16_t INT16;
    uint8_t INT8[2];
};

void make_data(uint8_t * data, int16_t * gyro, int16_t * accel, float * gps_pos);
int make_packet(uint8_t * data, uint8_t size);
void send_packet(int p_size);
