# 2020 Cansat

### 사용된 마이크로컨트롤러
MBED NXP LPC1768 


### 기능
mpu9250에서 3축 가속도 자이로 2byte 정수 데이터를,
neo-6m gps에서 gps 시간, 경도, 위도, 고도 float 실수 데이터를 얻는다.
데이터 측정 간격은 0.02초
패킷 또는 문자열 형태 중 하나로 시리얼 통신을 통해 전송한다.

config.h에서 설정 변경가능, 연결 핀 정보도 여기서 확인
패킷으로 받을 시 디코딩 작업은 pc에서 따로 해야 합니다. 관련 코드 추후 업로드 예정

기본 serial 38400 baud
