#include <Servo.h>                      // �������� ���̺귯�� �ҷ�����
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>

#define SERVO_PIN 9                     // �������Ϳ� ����� ������ �� ����
#define PIN_LED 5              // LED ���� ��
#define NUMPIXELS 3            // LED ����

// ������ ���� ���� (R, G, B�� �ּ� �� �ִ� ��)
#define RED_MIN 0
#define RED_MAX 500
#define GREEN_MIN 0
#define GREEN_MAX 500
#define BLUE_MIN 0
#define BLUE_MAX 500

// set 90, 145,180
Servo servo;                            // �������� ��ü�� ����
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);  // �÷� ���� ��ü ����
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);  // LED ��Ʈ�� ��ü ����


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

    // LED ��Ʈ�� �ʱ�ȭ
    pixels.begin();
    pixels.setBrightness(50);
    pixels.show(); // Initialize all pixels to 'off'

    // �÷� ���� �ʱ�ȭ
    if (tcs.begin()) {
        Serial.println("TCS34725 found");
    }
    else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
    }
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
            detectColor();
            delay(5000);
        }
        else {
            Detect_Backward();
        }
    }
}


void Detect_Forward() { // ���� ���� �Լ� --> ���Ŀ� ������ �����ǰ� ����ī�� ��ȣ ������ �ڵ� �߰��ؾ���!!!!
    Serial.println("������ �����Ǿ����ϴ�.");
    motorStop();
    delay(2000);
    if (detectColor()) {
        motorForward();
    }
    else {
        motorStop();
    }
}

void Detect_Backward() {// ���� ���� �Լ� --> ���Ŀ� ������ �����ǰ� ����ī�� ��ȣ ������ �ڵ� �߰��ؾ���!!!!
    Serial.println("������ �����Ǿ����ϴ�.");
    motorStop();
    delay(400);
    motorBackward();
}



void motorForward() {
    digitalWrite(motorDirectionPin, LOW);
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

void setLEDColor(int r, int g, int b) {
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b));  // RGB ������ LED ���� ����
    }
    pixels.show();  // LED ���� ����
}

bool detectColor() {
    uint16_t clear, red, green, blue;  // �÷� ���� ������ ����

    delay(1000);  // 1�ʸ��� �����ϰ� ���
    tcs.getRawData(&red, &green, &blue, &clear);  // ���� ���� �������� ���� �� �޾ƿ���

    Serial.print("R: ");
    Serial.print(red);  // �ø��� ����Ϳ� ������ �� ���
    Serial.print(" G: ");
    Serial.print(green);  // �ø��� ����Ϳ� �ʷϻ� �� ���
    Serial.print(" B: ");
    Serial.println(blue);  // �ø��� ����Ϳ� �Ķ��� �� ���

    // ���� �Ǵ� �� LED�� �������� ����
    if (red >= RED_MIN && red <= RED_MAX && green >= GREEN_MIN && green <= GREEN_MAX && blue >= BLUE_MIN && blue <= BLUE_MAX) {
        if (red > green && red > blue && red > 200 && red < 700) {
            Serial.println("������ ����");
            setLEDColor(255, 0, 0);  // ������ LED
            servo.write(90);  // �������͸� 90�� ��ġ�� �̵�
            return true;
        }
        else if (green > red && green > blue) {
            Serial.println("�ʷϻ� ����");
            setLEDColor(0, 255, 0);  // �ʷϻ� LED
            servo.write(180);  // �������͸� 180�� ��ġ�� �̵�
            return true;

        }
        else if (blue > red && blue > green) {
            Serial.println("�Ķ��� ����");
            setLEDColor(0, 0, 255);  // �Ķ��� LED
            servo.write(145);  // �������͸� 145�� ��ġ�� �̵�
            return true;

        }
        else {
            Serial.println("�� �� ���� ����");
            setLEDColor(0, 0, 0);  // LED ����
            //servo.write(90);  // �������͸� �⺻ ��ġ�� �̵�
            return false;
        }
    }
    else {
        Serial.println("�ƹ��͵� �������� ����");
        setLEDColor(0, 0, 0);  // LED ����
        return false;

        //servo.write(90);  // �������͸� �⺻ ��ġ�� �̵�
    }
}
