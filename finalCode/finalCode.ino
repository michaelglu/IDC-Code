#define Rx 15
#define Tx 14
#include <Servo.h>

// QTI pins
const int rightQTIPin = 4;
const int middleQTIPin = 8;
const int leftQTIPin = 6;

const int commLedPin = 9;

// QTI Threshold
long t = 500;
long greyC = 2000;
long blackC = 5000;

// Pause variable
//bool pausedForBlack = false;

// Booleans for order of operations
bool lineFollowB, getCourtB, communicationB, flashMyScoreB;

// Communication
const int DIVING = 0; // DIVING: a, b, and c
const int WEIGHTLIFTING = 1; // WEIGHTLIFTING: d, e, and f
const int CYCLING = 2; // CYCLING: g, h, i
const int BASKETBALL = 3; // BASKETBALL: j, k, l
const int COURT_SPORTS = 4; // COURT_SPORTS: m, n, o
const String letters = "abcdefghijklmno";

int myTeam = COURT_SPORTS; // PUT YOUR TEAM HERE!!!
int myScore = -1; // SET THIS TO YOUR SCORE OF 0, 1, OR 2 BASED ON SENSING!!!
int received[5] = { -1, -1, -1, -1, -1 };
int finalScore = -1;

// Lightshow
int greenpin = 9;
int redpin = 10;
int yellowpin = 4;

// National Anthem
int piezoPin = 9;
#include "pitches.h"
int melodyBass[] = {
  G2, F2, E2, F2, G2, A2, B2, C3, D3,
  G2, F2, B2, F3, B3, AS2, A2, A2, D2, B2, D3, FS3,
  D3, D3, D3, C3, B2, FS2, G2, G2, C3, B2, A2, G2,
  D2, B2, B1, C2, A1, B1, C2, D2, D2, G2
};
int noteDurations5[] = {
  2, 2, 3, 1, 1, 1, 1, 1, 4,
  2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  4, 2, 2, 1, 1, 1, 2, 2, 4
};

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

  // Lightshow setup
  pinMode(greenpin, OUTPUT);     //Initialize the LED pins as outputs
  pinMode(redpin, OUTPUT);     //Initialize the LED pins as outputs
  pinMode(yellowpin, OUTPUT);     //Initialize the LED pins as outputs
  digitalWrite(greenpin, LOW);  //turn the sent light off
  digitalWrite(redpin, LOW);  //turn the sent light off
  digitalWrite(yellowpin, LOW);  //turn the sent light off

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

  if (lineFollowB) {
    lineFollow(rcTime(rightQTIPin), rcTime(middleQTIPin), rcTime(leftQTIPin));
  }
  if (getCourtB) {
    myScore = getCourt();
  }
  if (communicationB) {
    // Set own score in received
    received[myTeam] = myScore;

    // Communicate until all scores are in and calculated
    while (finalScore == -1) {
      finalScore = communication();
      flashMyScore(myScore);
    }

    Serial.println(finalScore);

    // Final action - PUT YOUR ACTIONS UNDER HERE
    switch (finalScore) {
      case 0:
        // Bronze - coordinated dance
        dance();
        break;
      case 1:
        // Silver - light show
        lightshowz();
        break;
      case 2:
        // Gold - sing national anthem
        playAnthem();
        break;
    }

    communicationB = false;
    flashMyScoreB = true;
  }
  if (flashMyScoreB) {
    flashMyScore(myScore);
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
  communicationB = true;
  return ave;
}

// -----------------------------------------------
// Flash communication LED
void flashMyScore(int val)
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
}

// -----------------------------------------------
// Communication
// Returns answer if done, or -1 if not done yet
int communication() {
  // Sending
  Serial3.print(letters.charAt(myTeam * 3 + myScore));
  delay(1000);

  // Receiving
  int maxReceives = 10;
  int receives = 0;
  while (Serial3.available() && receives++ < maxReceives) {
    char incoming = Serial3.read();

    int incomingPos = letters.indexOf(incoming);
    received[(int) (incomingPos / 3)] = incomingPos % 3;
  }

  // Check to see if done
  int sum = 0;
  for (int i = 0; i < sizeof(received) / 2; i++) {
    if (received[i] == -1) { // Not done, so return -1
      return -1;
    }
    sum += received[i];
  }

  // Done, so return mod 3 of the results
  return sum % 3;
}

// -----------------------------------------------
// Dance
void dance() {
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

// -----------------------------------------------
// Lightshow
void lightshowz() {
  flashall(50);
  delay(500);
  flashall(100);
  delay(500);
  linear(2);
  flashall(50);
  delay(500);
  flashall(100);
  delay(500);
}

void flashall(int timetoflash) {
  digitalWrite(greenpin, HIGH);  //turn the sent light off
  digitalWrite(redpin, HIGH);  //turn the sent light off
  digitalWrite(yellowpin, HIGH);  //turn the sent light off
  delay(timetoflash);
  digitalWrite(greenpin, LOW);  //turn the sent light off
  digitalWrite(redpin, LOW);  //turn the sent light off
  digitalWrite(yellowpin, LOW);  //turn the sent light off
}

void linear(int timestolinear) {
  for (int timez = 0; timez <= timestolinear; timez = timez + 1)
  {
    digitalWrite(greenpin, HIGH);  //turn the sent light off
    delay(150);
    digitalWrite(greenpin, LOW);
    digitalWrite(yellowpin, HIGH);  //turn the sent light off
    delay(150);
    digitalWrite(yellowpin, LOW);
    digitalWrite(redpin, HIGH);  //turn the sent light off
    delay(150);
    digitalWrite(redpin, LOW);
    digitalWrite(yellowpin, HIGH);  //turn the sent light off
    delay(150);
    digitalWrite(yellowpin, LOW);
    digitalWrite(greenpin, HIGH);  //turn the sent light off
    delay(150);
    digitalWrite(greenpin, LOW);
  }
}

// -----------------------------------------------
// National Anthem
void playAnthem() {
  // iterate through the notes of the anthem
  int numberOfNotes = min(sizeof(melodyBass) / 2, sizeof(noteDurations5) / 2);
  for (int thisNote = 0; thisNote < numberOfNotes/*44*/; thisNote++) {

    int noteDuration = noteDurations5[thisNote] * 250;
    tone(piezoPin, melodyBass[thisNote], noteDuration);

    // set the time between notes (duration + 30%)
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone
    noTone(piezoPin);
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
