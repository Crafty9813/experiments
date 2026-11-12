#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int ENA = 9;
const int IN1 = 2;
const int IN2 = 3;

const int ENB = 10;
const int IN3 = 4;
const int IN4 = 5;

const int motorSpeed = 200;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  if (tcs.begin()) {
    Serial.println("TCS34725 sensor found!");
  } else {
    Serial.println("No TCS34725 found ... check your connections.");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c;
  
  // Read raw RGBC vals
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  // Check if close enough (C = brightness/proximity)
  // Adjust 150 depending on ambient lighting
  if (c > 150) {
    // Green
    if (g > r * 1.2 && g > b * 1.2) {
      Serial.println("GREEN detected -> Moving Forward");
      moveForward();
    } 
    // Red
    else if (r > g * 1.3 && r > b * 1.3) {
      Serial.println("RED detected -> Stopping");
      stopMotors();
    }
  }

  delay(100);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, motorSpeed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, motorSpeed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}
