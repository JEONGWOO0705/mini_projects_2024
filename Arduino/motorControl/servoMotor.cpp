#include <Servo.h>                      // 서보모터 라이브러리 불러오기
#define SERVO_PIN 9                     // 서보모터와 연결된 보드의 핀 정의

Servo servo;                            // 서보모터 객체를 생성

// set 90, 145,180
// 모터 제어 변수
int motorSpeedPin = 11; // 1번 모터 회전 속도 
int motorDirectionPin = 13; // 모터 방향제어
String input = ""; // 입력을 저장할 변수

// 적외선 센서 변수
int Obj_Sensor = A0;
int val;

void setup() {
    Serial.begin(9600);

    // 모터제어:
    pinMode(motorDirectionPin, OUTPUT);  // 방향제어핀을 pinmode_OUTPUT으로 지정
    pinMode(motorSpeedPin, OUTPUT); // 모터 속도 제어 핀을 OUTPUT으로 설정

    // 적외선 센서
    pinMode(Obj_Sensor, INPUT);

    // 서브모터
    servo.attach(SERVO_PIN);
    servo.write(90);
}

bool isMotorRunning = false;
bool isForward = true;

void loop() {
    val = analogRead(Obj_Sensor);
    //Serial.print("모터속도 .. ");
    //Serial.println(val);

    if (Serial.available() > 0) {
        input = Serial.readString();
        input.trim();

        if (input == "go") {
            isMotorRunning = true;
            isForward = true;
            motorForward();
        }
        else if (input == "back") {
            isMotorRunning = true;
            isForward = false;
            motorBackward();
        }
        else if (input == "stop") {
            isMotorRunning = false;
            motorStop();
        }
    }

    // 모터가 작동 중일 때 지속적으로 물체 감지 확인
    if (isMotorRunning && val < 300) {
        if (isForward) {
            Detect_Forward();
        }
        else {
            Detect_Backward();
        }
    }
}


void Detect_Forward() { // 물건 감지 함수 --> 차후에 물건이 감지되고 위즈카에 신호 보내는 코드 추가해야함!!!!
    Serial.println("물건이 감지되었습니다.");
    motorStop();
    delay(5000);
    motorForward();
}

void Detect_Backward() {// 물건 감지 함수 --> 차후에 물건이 감지되고 위즈카에 신호 보내는 코드 추가해야함!!!!
    Serial.println("물건이 감지되었습니다.");
    motorStop();
    delay(400);
    motorBackward();
}



void motorForward() {
    digitalWrite(motorDirectionPin, HIGH);
    analogWrite(motorSpeedPin, 75);
    servo.write(90);
    Serial.println("모터가 정방향으로 동작합니다.");
}

void motorBackward() {
    digitalWrite(motorDirectionPin, LOW);
    analogWrite(motorSpeedPin, 75);
    Serial.println("모터가 역방향으로 동작합니다.");
}

void motorStop() {
    digitalWrite(motorSpeedPin, LOW); // 모터 멈춤
    Serial.println("모터가 멈췄습니다.");
    servo.write(90);
}
