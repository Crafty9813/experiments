// A 2-DOF solar tracker using 4 LDRs.

#include <Servo.h>

Servo yaw;
Servo pitch;

int ldrTL = A0;
int ldrBL = A1;
int ldrBR = A2;
int ldrTR = A3;

int tol = 15;

int yawVal = 90;
int pitchVal = 90;

const int minAngle = 20;
const int maxAngle = 150;

float Kp = 0.03;

void setup() {
  yaw.attach(3);
  pitch.attach(5);

  yaw.write(yawVal);
  pitch.write(pitchVal);

  Serial.begin(9600);
}

void loop() {
  int TL = readAverage(ldrTL);
  int TR = readAverage(ldrTR);
  int BL = readAverage(ldrBL);
  int BR = readAverage(ldrBR);

  int avt = (TL + TR) / 2;
  int avb = (BL + BR) / 2;

  int avl = (TL + BL) / 2;
  int avr = (TR + BR) / 2;

  int avgLight = (TL + TR + BL + BR) / 4;

  if (avgLight < 200) {
    Serial.println("Too dark");
    delay(500);
    return;
  }

  int dPitch = avt - avb;
  int dYaw = avl - avr;

  // Pitch tracking
  if (abs(dPitch) > tol) {
    int moveV = abs(dPitch) * Kp;

    moveV = constrain(moveV, 1, 5);

    if (dPitch > 0) {
      pitchVal -= moveV;
    } else {
      pitchVal += moveV;
    }

    pitchVal = constrain(pitchVal, minAngle, maxAngle);

    pitch.write(pitchVal);
  }

  // Yaw tracking
  if (abs(dYaw) > tol) {
    int moveH = abs(dYaw) * Kp;

    moveH = constrain(moveH, 1, 5);

    if (dYaw > 0) {
      yawVal -= moveH;
    } else {
      yawVal += moveH;
    }

    yawVal = constrain(yawVal, minAngle, maxAngle);

    yaw.write(yawVal);
  }

  delay(5);
}

int readAverage(int pin) {
  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(pin);
    //delay(1);
  }

  return total / 10;
}
