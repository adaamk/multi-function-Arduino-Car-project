#include <Servo.h>
#include <AFMotor.h>

#define Echo A5
#define Trig A4
#define motor 10
#define Speed 250
#define spoint 103

#define BuzzerPin 9   // Change to a free digital pin
#define LedPin1 10    // Use an analog pin for the first LED
#define LedPin2 11    // Use an analog pin for the second LED

char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;

Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2, OUTPUT);

  servo.attach(motor);
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
}

void loop() {
   //Obstacle();
  Bluetoothcontrol();
   //voicecontrol();
}

void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);

    if (value == 'F') {
      forward();
    } else if (value == 'B') {
      backward();
    } else if (value == 'R') {
      left();
    } else if (value == 'L') {
      right();
    } else if (value == 'S') {
      Stop();
    } else if (value == 'V') {
      hornSound();  // Added command to play horn sound
    } else if (value == 'W') {
      turnOnLEDs();  // Added command to turn on both LEDs
    } else if (value == 'w') {
      turnOffLEDs();  // Added command to turn off both LEDs
    }
  }
}

void Obstacle() {
  distance = ultrasonic();
  if (distance <= 12) {
    Stop();
    backward();
    delay(100);
    Stop();
    R = leftsee();
    servo.write(spoint);
    delay(800);
    L = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    forward();
  }
}

void voicecontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
    if (value == '^') {
      forward();
    } else if (value == '-') {
      backward();
    } else if (value == '<') {
      L = leftsee();
      servo.write(spoint);
      if (L >= 10) {
        right();
        delay(500);
        Stop();
      } else if (L < 10) {
        Stop();
      }
    } else if (value == '>') {
      R = rightsee();
      servo.write(spoint);
      if (R >= 10) {
        left();
        delay(500);
        Stop();
      } else if (R < 10) {
        Stop();
      }
    } else if (value == '*') {
      Stop();
    } else if (value == '3') {
      hornSound();  // Added command to play horn sound
    } else if (value == '1') {
      turnOnLEDs();  // Added command to turn on both LEDs
    } else if (value == '2') {
      turnOffLEDs();  // Added command to turn off both LEDs
    }
  }
}

int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2; // time convert distance
  return cm;
}

void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}

int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}

int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}

void hornSound() {
  int buzzerPin = BuzzerPin;
  int duration = 220;  // Adjust duration as needed
  for (int i = 0; i < duration * 2; i++) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(400);  // Adjust frequency for the desired horn sound
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(400);  // Adjust frequency for the desired horn sound
  }
}

void turnOnLEDs() {
  digitalWrite(LedPin1, HIGH);
  digitalWrite(LedPin2, HIGH);
}

void turnOffLEDs() {
  digitalWrite(LedPin1, LOW);
  digitalWrite(LedPin2, LOW);
}