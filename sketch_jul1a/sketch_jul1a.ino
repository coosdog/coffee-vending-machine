#include <Servo.h>

const int trigPin = 10;         // 초음파 센서 Trig 핀
const int echoPin = 11;         // 초음파 센서 Echo 핀
const int servoPin = 9;         // 서보모터 핀
const int relayPin = 4;         // 릴레이 IN 핀 (워터펌프 제어)
const int buzzerPin = 5;        // 부저

Servo myServo;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // 릴레이 OFF (Active LOW 릴레이 기준)

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // 부저 OFF

  myServo.attach(servoPin);
  myServo.write(0);  // 서보 초기 위치
}

void loop() {
  long duration, distance;

  // 초음파 센서 측정
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 8) {
    // 서보 동작 (커피가루 투입)
    delay(3000);
    myServo.write(70);
    delay(3000);
    myServo.write(0);
    delay(2000);

    // 워터펌프 작동 (릴레이 ON → LOW)
    digitalWrite(relayPin, LOW);  // 릴레이 ON
    delay(3000);                  // 물 붓기
    digitalWrite(relayPin, HIGH); // 릴레이 OFF
    delay(2000); 

    tone(buzzerPin, 1000); // 1kHz 톤
    delay(500);            // 0.5초 울림
    noTone(buzzerPin);     // 부저 OFF

    delay(1000); // 다음 감지까지 대기
  }

  delay(2000);
}