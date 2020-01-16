/* 캔위성 모듈 프로그램
 */


#define DEBUG 0 // 디버그: 1, 실제 동작: 0
// 0: USB포트와 연결
// 1: 무선 통신 모듈과 연결


// 무선 통신 (HC-12)
#define PC_TX p13 //모듈 rx핀과 연결
#define PC_RX p14 //모듈 tx핀과 연결

#define PC_BAUD 38400

// GPS (neo-6m)
#define GPS_TX p9 //모듈 rx핀과 연결
#define GPS_RX p10 //모듈 tx핀과 연결

#define GPS_BAUD 9600

// 가속도 자이로 센서 (MPU9250 or 6050)
#define MPU_SDA p28
#define MPU_SCL p27
