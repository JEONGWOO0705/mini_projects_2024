# mini_projects_2024
IoT 개발자 미니 프로젝트 지포지토리

## 1일차
- 조별 자리배치
- IoT 프로젝트 개요


    ![IoT프로젝트](https://raw.githubusercontent.com/JEONGWOO0705/mini_projects_2024/main/image/mp001.png)
    
    1. IoT 기기 구성 : 아두이노, 라즈베리파이 등 IoT장비들과 연결
    2. 서버 구성 : IoT기기와 통신, DB구성, 데이터 수집 앱 개발
    3. 모니터링 구성 : 실시간 모니터링/제어 앱, 전체 연결

- 조별 미니프로젝트
    - 5월 28일(40시간)
    - 구체적으로 어떤 디바이스 구성, 데이터 수집, 모니터링 계획
    - 8월말 정도에 끝나는 프로젝트 일정 계획
    - 요구사항 리스트, 기능명세, UI/UX 디자인, DB설계 문서하나로 통합
    - 5월 28일 오후 각 조별로 파워포인트/프레젠테이션 10분 발표
    - 요구사항 리스트 문서 전달
    - 기능 명세 문서
    - DB설계 ERD 또는 SSMS 물리적 DB설계
    - UI/UX 디자인 16(목) 내용전달

## 2일차
- 미니 프로젝트
    - 프로젝트 문서
    - Notion 팀 프로젝트 템플릿 활용
    - UI/UX 디자인 툴 설명
        - 피그마
            - 피그마 단축키
                - V - 선택도구
                - A - 직선도구
                - T - 텍스트도구
                - Space + Drag - 마우스 커서에 따라 원하는 영역으로 이동
                - Ctrl + R - 레이어 이름 변경
                - Ctrl + G - 객체 그룹화
                - Ctrl + Shift + G - 객체 그룹 해제
                - Ctrl + D - 선택한 객체 복사
                - Ctrl + [ - 선택한 객체 뒤로 보내기
                - Ctrl + ] - 선택한 객체 앞으로 보내기

        - https://ovenapp.io/ 카카오
        - https://www.figma.com/ 피그마
        - https://app.moqups.com/ 목업디자인 사이트
    - 프리젠테이션
        - 미리캔버스 활용하기!!
     - 라즈베리파이(RPi) 리셋팅, 네트워크 설정, VNC(OS UI작업)

- 스마트홈 연동 클래스 미니 프로젝트
    1. 요구사항 정의, 기능 명세
    2. UI/UX 디자인
    3. DB설계
    4. RPi 세팅
    5. RPio GPIO, IoT디바이스 연결(카메라, HDT센서, RGB LED, ...)
    6. RPi 데이터 전송 파이썬 프로그래밍
    7. PC(Server) 데이터 수신 C# 프로그래밍
    8. 모니터링 앱 개발(수신 및 송신)

## 3일차
- 미니프로젝트
    - 실무 프로젝트 문서
    - 조별로 진행

- 라즈베리파이 셋팅
    1. RPi 기본 구성 - RPi + MicroSD + Power
    2. RPi 기본 셋팅
        - 한글화
        - 키보드 변경
        - 화면사이즈 변경(RealVNC)
        - Pi Apps 앱설치 도우미 앱
        - Github, Desktop, VS Code
        - 네트워크 확인
        - RealVNC Server 자동실행 설정

- 스마트홈 연동 클래스 미니프로젝트
    - RPi 셋팅...

## 4일차
- 라즈베리파이 IoT 장비 설치
    [o] 라즈베리 파이 카메라
    [o] GPIO HAT
    [o] 브레드보드와 연결
    [o] DHT11 센서
    [o] RGB LED 모듈
    [ ] 서보 모터

- rasberrypi 에서 sudo nano/etc/rc.local 들어가서 sudo iwconfig wlan0 power off 하기 (무선랜 꺼짐 방지)

## 6,7일차
- 네크워크 대공사
    - 개인 공유기, PC, 라즈베리 파이

- 스마트홈 연동 클래스 미니 프로젝트
    - 온습도 센서, RGB LED
    - Rpi <--> Windows 통신(MQTT)
    - WPF 모니터링 앱

- IoT 기기간 통신 방법
    - Modbus : 시리얼 통신으로 데이터 전송(완전 구식)
    - OPC UA : Modbus 불편한 점 개선(아주 복잡)
    - **MQTT** : 가장 편리!! AWS IoT, Azure IoT  클라우드 산업계 표준으로 사용
    - Window MQTT Broker 설치 : https://mosquitto.org/

-  MQTT 설치
    - Mosquitto Broker 설치 [o]
        - mosquitto.conf : listener 1883 0.0.0.0 , allow_anonymous true
        - 방화벽 인바운드 열기
    - RPi : paho-mqtt 패키지 설치, 송신(publisher)
    - Win : MQTT.NET Nuget 패키지 설치 , 수신 (subscriber)

## 프로젝트 진행 상황

- Our Team Notion : https://www.notion.so/4-TEAM-PROJECT-96750e470f3147189e0fb3b72c9ed436


- 인공지능 IoT 물류 시스템 으로 정함
    - AI 이미지 인식 기술을 이용해 다양한 색상의 상자를 판별
    - 서보모터, 컨베이어벨트, RC카를 제어하여 자동화된 물류 시스템을 구현

    ![IoT프로젝트](https://raw.githubusercontent.com/JEONGWOO0705/mini_projects_2024/main/image/mp002.png)



- 작동 순서
    1. 먼저 분류를 원하는 색상의 상자를 컨베이어벨트에 올리는 것으로 시작
    2. 무선캠을 통한 상자의 색상 구별 및 RC 카에 출발 신호 보냄
    3. 인식된 색상에 따라 메인 코드가 서보모터를 제어하여 상자의 이동 경로를 설정하고, 컨베이어벨트가 상자를 지정된 위치로 이동
    4. RC카는 시작점에 출발후 상자를 받기 위해 정지선에서 상자를 기다림. 
    5. RC카에는 코드위즈(아두이누 + 통신 장치)가 장착되어 상자의 수령 (적외선 센서를 통해) 및 출발 신호를  받음
    6. RC카는 라인트레이싱 기술을 이용해 지정된 경로를 따라 이동
    7. 지정된 장소에 박스 내린 후, 시작 장소로 이동