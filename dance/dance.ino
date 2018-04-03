#include <Servo.h>

// Declare left and right servos
Servo servoLeft;
Servo servoRight;

void setup() {
  // Servo setup
  servoLeft.attach(13);
  servoRight.attach(12);

  // -----------------------------
  // COPY STUFF BELOW HERE
  // (Choreographed by Samuel Rabinowitz)
  // -----------------------------

  // Perform dance
  int beatMillis = 500;

  // Wiggle back
  for (int i = 0; i < 4; i++) {
    // Back left
    servoLeft.writeMicroseconds(1480);
    servoRight.writeMicroseconds(1650);
    delay(beatMillis / 2);

    // Back right
    servoLeft.writeMicroseconds(1350);
    servoRight.writeMicroseconds(1520);
    delay(beatMillis / 2);
  }

  // Forward-back forward-back spin x2
  for (int i = 0; i < 2; i++) {
    // Forward-back forward-back
    for (int j = 0; j < 2; j++) {
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1300);
      delay((int) (beatMillis * 0.75));
      servoLeft.writeMicroseconds(1300);
      servoRight.writeMicroseconds(1700);
      delay((int) (beatMillis * 0.25));
    }

    // Spin
    if (i == 0) { // Spin right then back
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1700);
      delay(beatMillis);
      servoLeft.writeMicroseconds(1300);
      servoRight.writeMicroseconds(1300);
      delay(beatMillis);
    }
    else { // Spin left then back
      servoLeft.writeMicroseconds(1300);
      servoRight.writeMicroseconds(1300);
      delay(beatMillis);
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1700);
      delay(beatMillis);
    }
  }

  // Turn out turn in (each side) x2
  for (int i = 0; i < 2; i++) {
    // Left turn out
    servoLeft.writeMicroseconds(1520);
    servoRight.writeMicroseconds(1300);
    delay(beatMillis / 2);

    // Left turn in
    servoLeft.writeMicroseconds(1480);
    servoRight.writeMicroseconds(1700);
    delay(beatMillis / 2);

    // Right turn out
    servoLeft.writeMicroseconds(1700);
    servoRight.writeMicroseconds(1480);
    delay(beatMillis / 2);

    // Right turn in
    servoLeft.writeMicroseconds(1300);
    servoRight.writeMicroseconds(1520);
    delay(beatMillis / 2);
  }

  // Stop
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

void loop() {


}
