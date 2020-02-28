#pragma once
#include "config.h"

struct GpsData {
    float time, lat, lon, alt, hdop, geoid;
    char ns, ew, unit;
    int lock, sats;
    int flag;
};

extern GpsData Gps; //GPS 데이터가 저장되는 구조체

extern Serial pc;
extern Serial gps;

void GET_GPS(float * gps_data); //GPS 구조체에서 값을 읽어오는 함수, 입력값은 데이터를 저장할 실수형 배열 =>{time, latitude, longitude, altitude}
void gps_sampling(); //큐에서 GPS 데이터를 샘플링해서 구조체에 저장하는 함수
void Receiving(); //시리얼 수신 인터럽트 발생 시 큐에 저장하는 함수

// 아래는 원형 큐와 파싱 관련 내부 함수들
static int isEmpty(); //큐가 비었는지 확인
static int isFull(); //큐가 가득 찼는지 확인
static char deQueue(); //큐에서 한 바이트 읽어오가
static int enQueue(char data); //큐에 한 바이트 쓰기
static int Checker(char input); //한 바이트씩 읽어서 헤더 확인
static int Decoder(); //GPGGA 문자열이 다 들어오면 데이터 파싱해서 Gps구조체에 저장
