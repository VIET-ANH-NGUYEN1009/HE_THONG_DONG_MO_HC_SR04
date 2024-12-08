#include "Arduino.h"
#include <ESP32Servo.h>

#define PIN_SG90 2        // Chân đầu ra cho servo
#define buzzer 3          // Chân cho còi
#define TrigPin 5         // Chân Trigger cho cảm biến siêu âm
#define EchoPin 18        // Chân Echo cho cảm biến siêu âm
#define Sound_speed 0.034 // Tốc độ âm thanh (cm/µs)
#define cm_to_inch 0.393701 // Hệ số chuyển đổi cm sang inch

Servo sg90;
long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(9600);

  // Cài đặt servo
  sg90.setPeriodHertz(50);          // Tần số PWM cho servo SG90
  sg90.attach(PIN_SG90, 500, 2400); // Độ rộng xung tối thiểu và tối đa (µs)

  // Cấu hình chân
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Cài đặt hoàn tất");
}

void loop() {
  // Gửi xung siêu âm
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // Đo thời gian xung Echo
  duration = pulseIn(EchoPin, HIGH);

  // Tính khoảng cách
  distanceCm = duration * Sound_speed / 2;
  distanceInch = distanceCm * cm_to_inch;

  // In khoảng cách ra màn hình Serial Monitor
  Serial.print("KHOẢNG CÁCH (CM): ");
  Serial.println(distanceCm);
  Serial.print("KHOẢNG CÁCH (INCH): ");
  Serial.println(distanceInch);

  // Kiểm tra khoảng cách và xoay servo nếu nhỏ hơn 2 cm
  if (distanceCm < 10) {
    // Kích hoạt còi
    digitalWrite(buzzer, HIGH);

    // Xoay servo từ 0° đến 90°
    for (int pos = 0; pos <= 90; pos += 1) {
      sg90.write(pos);
      delay(0); // Chuyển động mượt hơn
    }

    // Xoay servo từ 90° về 0°
    for (int pos = 90; pos >= 0; pos -= 1) {
      sg90.write(pos);
      delay(10);
    }

    // Tắt còi
    digitalWrite(buzzer, LOW);
  }

  delay(500); // Chờ giữa các lần đo
}
