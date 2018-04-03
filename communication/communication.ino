// Samuel Rabinowitz 4/3/2018
#define Rx 15
#define Tx 14

const int DIVING = 0; // DIVING: a, b, and c
const int WEIGHTLIFTING = 1; // WEIGHTLIFTING: d, e, and f
const int CYCLING = 2; // CYCLING: g, h, i
const int BASKETBALL = 3; // BASKETBALL: j, k, l
const int COURT_SPORTS = 4; // COURT_SPORTS: m, n, o
const String letters = "abcdefghijklmno";

int myTeam = COURT_SPORTS; // PUT YOUR TEAM HERE!!!
int myScore = 0; // SET THIS TO YOUR SCORE OF 0, 1, OR 2!!!
int received[5] = { -1, -1, -1, -1, -1 };
int finalScore = -1;

void setup() {
  // Serial setup
  Serial.begin(9600);
  Serial3.begin(9600);

  delay(500);
}

void loop() {
  // Set own score in received
  received[myTeam] = myScore;

  // Communicate until all scores are in and calculated
  while (finalScore == -1) {
    finalScore = communication();
  }

  Serial.println(finalScore);

  // Final action - PUT YOUR ACTIONS UNDER HERE
  switch (finalScore) {
    case 0:
      // Bronze - coordinated dance
      break;
    case 1:
      // Silver - light show
      break;
    case 2:
      // Gold - sing national anthem
      break;
  }
}

// Returns answer if done, or -1 if not done yet
int communication() {
  // Sending
  Serial3.print(letters.charAt(myTeam * 3 + myScore));

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
