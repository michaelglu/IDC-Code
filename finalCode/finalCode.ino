#define Rx 15
#define Tx 14
#include <Servo.h>

// QTI Threshold
long t = 500;

// Pause variable
//bool pausedForBlack = false;

// Booleans for order of operations
bool lineFollowB, getCourtB, bsendValsB, danceB, singB, lightShowB;

// Declare left and right servos
Servo servoLeft;
Servo servoRight;

void setup() {
  // Servo Setup
  servoLeft.attach(13);
  servoRight.attach(12);
  
  // QTI Setup
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  
  // Serial Setup
  Serial.begin(9600);
  Serial3.begin(9600);

  // Booleans for order of operations
  lineFollowB = true;
  getCourtB = false;
  sendValsB = false;
  danceB = false;
  singB = false;
  lightShowB = false;
  
  delay(500);
}

void loop() {
  if (lineFollowB)
  {
    lineFollow(rcTime(4), rcTime(5), rcTime(6));
  }
  else if (getCourtB)
  {
    getCourt();
  }
  //Each boolean correspons to the task in progress, once the task is done the boolean
  //is switched to false and the subsequent boolesn is switched to true
}

//______________________________________________
//HELPER FUNCTIONS
//______________________________________________
//QTI HELPERS
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
//________________________________________________
//SERVO HELPERS
void goForward()
{
  servoLeft.writeMicroseconds(1550);
  servoRight.writeMicroseconds(1450);
}
void goLeft()
{
  servoLeft.writeMicroseconds(1650);
  servoRight.writeMicroseconds(1550);
}
void goRight()
{
  servoLeft.writeMicroseconds(1450);
  servoRight.writeMicroseconds(1350);
}
void stopMotors()
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}
//________________________________________________
//LINE FOLLOW
void lineFollow(long qtiRight, long qtiMiddle, long qtiLeft)
{
  //if (qtiMiddle > t) { // If center qti on black, start movement
  if (qtiRight < t && qtiLeft < t) { // If side qtis are on white, move forward
    goForward();
  }
  if (qtiRight > t && qtiLeft < t) { // If right sees white and left sees black, turn left
    goLeft();
  }
  if (qtiRight < t && qtiLeft > t) { // If right sees black and left sees white, turn right
    goRight();
  }
  if (qtiRight > t && qtiLeft > t) { // If both see black, stop, switch getCourtB to True switch lineFollowB to False

    stopMotors();
    delay(200);
    pausedForBlack = true;
    lineFollowB = false;
    getCourtB = true;

  }
  else {
    pausedForBlack = false;
  }
  //}
}
}
//________________________________________________
//Get Court Type
int getCourt()
{
  //Fill In Later
}
//________________________________________________
//DEBUG VIA SERIAL
void print3vals(long val1, long val2, long val3) {
  Serial.print(val1);
  Serial.print(" | ");
  Serial.print(val2);
  Serial.print(" | ");
  Serial.println(val3);
  Serial.println("________________");
}
//________________________________________________
