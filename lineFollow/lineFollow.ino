#include <Servo.h>                           // Include servo library


// QTI Threshold
long t = 500;

// Pause variable
bool pausedForBlack = false;

// Declare left and right servos
Servo servoLeft;
Servo servoRight;

void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
  Serial.begin(9600);

}

void loop() {
  long qti4 = rcTime(4);
  long qti5 = rcTime(5);
  long qti6 = rcTime(6);

  print3vals(qti4, qti5, qti6);

  //if (qti5 > t) { // If center qti on black, start movement
  if (qti4 < t && qti6 < t) { // If side qtis are on white, move forward
    servoLeft.writeMicroseconds(1550);
    servoRight.writeMicroseconds(1450);
  }
  if (qti4 > t && qti6 < t) { // If 4 (right) sees white and 6 (left) sees black, turn left
    servoLeft.writeMicroseconds(1650);
    servoRight.writeMicroseconds(1550);
  }
  if (qti4 < t && qti6 > t) { // If 4 (right) sees black and 6 (left) sees white, turn right
    servoLeft.writeMicroseconds(1450);
    servoRight.writeMicroseconds(1350);
  }
  if (qti4 > t && qti6 > t) { // If both see black, stop, wait 100 milliseconds, then proceed forward
    if (!pausedForBlack) {
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
      delay(200);
      pausedForBlack = true;
    }
    else {
      servoLeft.writeMicroseconds(1550);
      servoRight.writeMicroseconds(1450);
    }
  }
  else {
    pausedForBlack = false;
  }
  //}
}

long rcTime(int pin) {
  

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(230);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  long time = micros();
  while (digitalRead(pin));
  time = micros() - time;
  return time;

}

void print3vals(long val1, long val2, long val3) {
  Serial.print(val1);
  Serial.print(" | ");
  Serial.print(val2);
  Serial.print(" | ");
  Serial.println(val3);
  Serial.println("________________");
}

