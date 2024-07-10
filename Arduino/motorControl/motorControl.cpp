// 모터 제어 변수
int motorSpeedPin = 10; // 1번 모터 회전 속도 
int motorDirectionPin = 12; // 모터 방향제어
String input = ""; // 입력을 저장할 변수

void setup() {
    // put your setup code here, to run once:
    pinMode(motorDirectionPin, OUTPUT);  // 방향제어핀을 pinmode_OUTPUT으로 지정
    pinMode(motorSpeedPin, OUTPUT); // 모터 속도 제어 핀을 OUTPUT으로 설정
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    if (Serial.available() > 0) {
        input = Serial.readString(); // 시리얼 입력을 문자열로 읽음
        input.trim(); // 줄바꿈 문자 제거

        if (input == "go") {
            digitalWrite(motorDirectionPin, HIGH);
            digitalWrite(motorSpeedPin, HIGH); // 모터 정방향으로 회전
            Serial.println("모터가 정방향으로 동작합니다.");
        }
        else if (input == "back") {
            digitalWrite(motorDirectionPin, LOW);
            digitalWrite(motorSpeedPin, HIGH); // 모터 역방향으로 회전
            Serial.println("모터가 역방향으로 동작합니다.");
        }
        else if (input == "stop") {
            digitalWrite(motorSpeedPin, LOW); // 모터 멈춤
            Serial.println("모터가 멈췄습니다.");
        }
    }
}
