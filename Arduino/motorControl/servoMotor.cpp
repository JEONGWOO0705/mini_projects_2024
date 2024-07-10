#include <Servo.h>                      // �������� ���̺귯�� �ҷ�����
#define SERVO_PIN 9                     // �������Ϳ� ����� ������ �� ����

Servo servo;                            // �������� ��ü�� ����

// set 90, 145,180
// ���� ���� ����
int motorSpeedPin = 11; // 1�� ���� ȸ�� �ӵ� 
int motorDirectionPin = 13; // ���� ��������
String input = ""; // �Է��� ������ ����

// ���ܼ� ���� ����
int Obj_Sensor = A0;
int val;

void setup() {
    Serial.begin(9600);

    // ��������:
    pinMode(motorDirectionPin, OUTPUT);  // ������������ pinmode_OUTPUT���� ����
    pinMode(motorSpeedPin, OUTPUT); // ���� �ӵ� ���� ���� OUTPUT���� ����

    // ���ܼ� ����
    pinMode(Obj_Sensor, INPUT);

    // �������
    servo.attach(SERVO_PIN);
    servo.write(90);
}

bool isMotorRunning = false;
bool isForward = true;

void loop() {
    val = analogRead(Obj_Sensor);
    //Serial.print("���ͼӵ� .. ");
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

    // ���Ͱ� �۵� ���� �� ���������� ��ü ���� Ȯ��
    if (isMotorRunning && val < 300) {
        if (isForward) {
            Detect_Forward();
        }
        else {
            Detect_Backward();
        }
    }
}


void Detect_Forward() { // ���� ���� �Լ� --> ���Ŀ� ������ �����ǰ� ����ī�� ��ȣ ������ �ڵ� �߰��ؾ���!!!!
    Serial.println("������ �����Ǿ����ϴ�.");
    motorStop();
    delay(5000);
    motorForward();
}

void Detect_Backward() {// ���� ���� �Լ� --> ���Ŀ� ������ �����ǰ� ����ī�� ��ȣ ������ �ڵ� �߰��ؾ���!!!!
    Serial.println("������ �����Ǿ����ϴ�.");
    motorStop();
    delay(400);
    motorBackward();
}



void motorForward() {
    digitalWrite(motorDirectionPin, HIGH);
    analogWrite(motorSpeedPin, 75);
    servo.write(90);
    Serial.println("���Ͱ� ���������� �����մϴ�.");
}

void motorBackward() {
    digitalWrite(motorDirectionPin, LOW);
    analogWrite(motorSpeedPin, 75);
    Serial.println("���Ͱ� ���������� �����մϴ�.");
}

void motorStop() {
    digitalWrite(motorSpeedPin, LOW); // ���� ����
    Serial.println("���Ͱ� ������ϴ�.");
    servo.write(90);
}
