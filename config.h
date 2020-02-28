/* 캔위성 모듈 프로그램
 */

// 0: USB포트와 연결
// 1: 무선 통신 모듈과 연결
#define DEBUG 1 // 디버그: 1, 실제 동작: 0

// 연결된 센서 (장차되지 않은 경우 주석 처리)
#define GPS
#define MPU
// ========================================== //

// ============ 무선 통신 (HC-12) ============ //
#define PC_TX p13 //모듈 rx핀과 연결
#define PC_RX p14 //모듈 tx핀과 연결

#define PC_BAUD 38400 // **주의: 속도 변경하면 잘 작동하지 않을 수 있음, 아래 참고 (초기: 38400)

// 통신 규칙 설정
// 0: 데이터 문자열 형태로 전송
// 1: 1byte씩 패킷으로 전송
#define SENT_PACKET 0
// ========================================== //

// ============ GPS (NEO-6m) ================ //
#define GPS_TX p9 //모듈 rx핀과 연결
#define GPS_RX p10 //모듈 tx핀과 연결

#define GPS_BAUD 9600
// ========================================== //

// === 가속도 자이로 센서 (MPU9250 or 6050) === //
#define MPU_SDA p28 //모듈 sda와 연결
#define MPU_SCL p27 //모듈 scl과 연결

// 자이로계 측정범위
// FS_SEL : Full-Scale Range, (Sensitivity Scale Factor)
// 0 : +/-250 degree/s, (131.0 LSB/(degree/s))
// 1 : +/-500 degree/s, (65.5 LSB/(degree/s))
// 2 : +/-1000 degree/s, (32.8 LSB/(degree/s))
// 3 : +/-2000 degree/s, (16.4 LSB/(degree/s))
#define FS_SEL 3  // original 2

// 가속도계 측정범위
// AFS_SEL : Full-Scale Range, (Sensitivity Scale Factor)
// 0 : +/-2 g, (16384.0 LSB/g)
// 1 : +/-4 g, (8192.0 LSB/g)
// 2 : +/-8 g, (4096.0 LSB/g)
// 3 : +/-16 g, (2048.0 LSB/g)
#define AFS_SEL 3  // original 3
// ========================================== //

// 이렇게도 가능하긴 하더라..
//#define GYRO_SCALE_FACTOR (131.0 / (1 << FS_SEL) )
//#define ACCEL_SCALE_FACTOR ( (16384 >> AFS_SEL)*1.0 )

/* [통신 속도와 패킷 생성 주기]
 main.cpp 의 process()함수 실행 시간은 통신 속도가 결정한다.
 pc가 9600baud일 때 실행시간은 약 33ms정도
 38400baud일 때 실행시간은 약 9.8ms정도이다.
 ticker인터럽트가 걸리지 않는 때 gps_sampling()함수가 동작하므로
 process실행시간외에 gps_sampling이 실행될 여유시간이 더 주어져야 한다.
*/
