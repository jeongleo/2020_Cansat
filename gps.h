#pragma once

struct GpsData {
    float time, lat, lon, alt, hdop, geoid;
    char ns, ew, unit;
    int lock, sats;
    int flag;
};

extern Serial pc;
extern Serial gps;
extern GpsData Gps;

void GET_GPS(float * gps_data);
void gps_sampling();
int isEmpty();
int isFull();
char deQueue();
int enQueue(char data);
void Receiving();
int Checker(char input);
int Decoder();