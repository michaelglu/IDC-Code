#define Rx 15
#define Tx 14
#include <Servo.h>

// QTI pins
const int rightQTIPin = 4;
const int middleQTIPin = 8;
const int leftQTIPin = 6;

const int commLedPin = 9;
const int recLedPin = 11;

// QTI Threshold
long t = 500;
long greyC = 2000;
long blackC = 5000;

// Pause variable
//bool pausedForBlack = false;

// Booleans for order of operations
bool lineFollowB, getCourtB, sendCourtB;

int courtType = -1;
int score = 0;

// Declare left and right servos
Servo servoLeft;
Servo servoRight;

void setup() {
  // Servo setup
  servoLeft.attach(13);
  servoRight.attach(12);

  // QTI setup
  pinMode(rightQTIPin, INPUT);
  pinMode(middleQTIPin, INPUT);
  pinMode(leftQTIPin, INPUT);

  // Communication LED setup
  pinMode(commLedPin, OUTPUT);

  // Receiving LED setup
  pinMode(recLedPin, OUTPUT);

  // Serial setup
  Serial.begin(9600);
  Serial3.begin(9600);

  delay(500);

  // Start off with line following
  lineFollowB = true;
}

void loop() {
  // Each boolean correspons to the task in progress, once the task is done the boolean
  // is switched to false and the subsequent boolesn is switched to true

  if (lineFollowB)
  {
    lineFollow(rcTime(rightQTIPin), rcTime(middleQTIPin), rcTime(leftQTIPin));
  }
  if (getCourtB)
  {
    courtType = getCourt();
  }
  else if (sendCourtB) {
    sendCourt(courtType);
    receive();
  }
}

// ---------------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------------

// QTI rctime
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

// ---------------------------------------------
// Movement
void goForward()
{
  servoLeft.writeMicroseconds(1550);
  servoRight.writeMicroseconds(1450);
}
void goRight()
{
  servoLeft.writeMicroseconds(1650);
  servoRight.writeMicroseconds(1550);
}
void goLeft()
{
  servoLeft.writeMicroseconds(1450);
  servoRight.writeMicroseconds(1350);
}
void stopMotors()
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

// ---------------------------------------------
// Line following
void lineFollow(long qtiRight, long qtiMiddle, long qtiLeft)
{
  //if (qtiMiddle > t) { // If center qti on black, start movement
  if (qtiRight < t && qtiLeft < t) { // If side qtis are on white, move forward
    goForward();
  }
  if (qtiRight > t && qtiLeft < t) { // If right sees white and left sees black, turn left
    goRight();
  }
  if (qtiRight < t && qtiLeft > t) { // If right sees black and left sees white, turn right
    goLeft();
  }
  if (qtiRight > t && qtiLeft > t) { // If both see black, stop, switch getCourtB to True switch lineFollowB to False

    stopMotors();
    delay(500);
    //pausedForBlack = true;
    lineFollowB = false;
    getCourtB = true;

  }
  else {
    //pausedForBlack = false;
  }
  //}
}

// ---------------------------------------------
// Get Court Type
int getCourt()
{
  // Turn left 90 degrees in place
  goLeft();
  delay(900);
  goForward();
  delay(2000);
  stopMotors();

  // Take 10 readings of court from middle QTI
  int numberOfReadings = 10;
  int readings[numberOfReadings];
  for (int i = 0; i < numberOfReadings; i++)
  {
    long reading = rcTime(middleQTIPin);
    Serial.println(reading);
    if (reading < greyC)
    {
      Serial.println("WHITE");
      readings[i] = 0;
    }
    else if (greyC <= reading && reading < blackC)
    {
      Serial.println("GREY");
      readings[i] = 1;
    }
    else if (blackC <= reading)
    {
      Serial.println("BLACK");
      readings[i] = 2;
    }
    goRight();
    delay(20);
    stopMotors();
    delay(20);
  }

  // Return the average of the 10 readings
  int sum = 0;
  for (int i = 0; i < numberOfReadings; i++)
  {
    sum = sum + readings[i];
  }
  Serial.print("Sum: ");
  Serial.println(sum);
  int ave = sum / numberOfReadings;
  Serial.print("Ave: ");
  Serial.println(ave);

  getCourtB = false;
  sendCourtB = true;
  return ave;
}

// -----------------------------------------------
// Send value of court and flash communication LED
void sendCourt(int val)
{
  if (val == 0) // White
  {
    digitalWrite(commLedPin, HIGH);
  }
  else if (val == 1) { // Grey
    digitalWrite(commLedPin, HIGH);
    delay(50);
    digitalWrite(commLedPin, LOW);
    delay(50);
  }
  else if (val == 2) { // Black
    digitalWrite(commLedPin, HIGH);
    delay(300);
    digitalWrite(commLedPin, LOW);
    delay(300);
  }
  Serial3.print(val);
}

// -----------------------------------------------
// Receive communication
void receive() {
  if (Serial3.available()) { // receiving
    digitalWrite(11, HIGH);

    char incoming = Serial3.read();
    Serial.println(incoming);
  }
  else {
    digitalWrite(11, LOW);
  }
}

// -----------------------------------------------
// Debug via serial
void print3vals(long val1, long val2, long val3) {
  Serial.print(val1);
  Serial.print(" | ");
  Serial.print(val2);
  Serial.print(" | ");
  Serial.println(val3);
  Serial.println("________________");
}
