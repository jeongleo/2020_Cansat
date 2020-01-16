#include "mbed.h"
#include "config.h"
#include "gps.h"

Serial gps(GPS_TX, GPS_RX);

struct GpsData Gps = {0};

static int Q_size =500;
static char Q[500] = {0};
static int rear = 0;
static int front = 0;
static char DATA[256] = {0};

void GET_GPS(float * gps_data){
    if(Gps.flag){
        *(gps_data) = Gps.time;
        *(gps_data + 1) = Gps.lat;
        *(gps_data + 2) = Gps.lon;
        *(gps_data + 3) = Gps.alt;
        Gps.flag = 0;
    } else {
        for(int i=0; i<4; i++){
            *(gps_data + i) = 0;
        }
    }
}

void gps_sampling()
{
    while(isEmpty()==0) {
        Checker(deQueue());
    }
}

int isEmpty()
{
    if(front == rear){
        return 1;
    } else {
        return 0;
    }
}

int isFull()
{
    if( (front+1)%Q_size == rear ){
        return 1;
    } else {
        return 0;
    }
}

char deQueue()
{
    if(isEmpty()){
        return 0;
    } else {
        rear = (rear+1)%Q_size;
        return Q[rear];
    }
}

int enQueue(char data)
{
    if(isFull()){
        return 0;
    } else {
        front = (front+1)%Q_size;
        Q[front] = data;
        return 1;
    }
}

void Receiving()
{
    enQueue(gps.getc());
}

int Checker(char input)
{
    static int header = 0;
    static int HeaderCHK = 0;
    
    DATA[header++] = input;
    
    switch(HeaderCHK){
    case 0:
        if(input=='$'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    case 1:
        if(input=='G'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    case 2:
        if(input=='P'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    case 3:
        if(input=='G'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    case 4:
        if(input=='G'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    case 5:
        if(input=='A'){
            HeaderCHK++;
        } else {
            HeaderCHK = 0;
            header = 0;
        }
        break;
    }
    
    if(HeaderCHK == 6 && input == '*'){
        HeaderCHK = 0;
        header = 0;
        return Decoder();
    }
    return 0;
}

int Decoder()
{
    int num = sscanf(DATA, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f,M,,*", &Gps.time, &Gps.lat, &Gps.ns, &Gps.lon, &Gps.ew, &Gps.lock, &Gps.sats, &Gps.hdop, &Gps.alt, &Gps.unit, &Gps.geoid);
    if(num){
        Gps.flag = 1; // 새로 업데이트 됨을 알림
        
        if(!Gps.lock) {
            //Gps.time = 0.0;
            Gps.lon = 0.0;
            Gps.lat = 0.0;
            Gps.sats = 0;
            Gps.hdop = 0.0;
            Gps.alt = 0.0;
            Gps.geoid = 0.0;
            
            return 0;
                
        } else {
            //GPGGA format according http://aprs.gids.nl/nmea/#gga
            // time (float), lat (f), (N/S) (c), long (f), (E/W) (c), fix (d), sats (d),
            // hdop (float), altitude (float), M, geoid (float), M, , ,
            //GPGGA,092010.000,5210.9546,N,00008.8913,E,1,07,1.3,9.7,M,47.0,M,,0000*5D

            //format utc time to seoul time,add 9 time zone
            //Gps.time = Gps.time + 90000.00f;
                
            return 1;
        }
    }
}

