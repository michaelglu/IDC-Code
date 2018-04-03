// Samuel Rabinowitz
#define Rx 15
#define Tx 14

// -------------------
// COPY THIS FOR COMMUNICATION
// -------------------
const int DIVING = 0;
const int WEIGHTLIFTING = 1;
const int CYCLING = 2;
const int BASKETBALL = 3;
const int COURT_SPORTS = 4;

int myTeam = COURT_SPORTS; // PUT YOUR TEAM HERE!!!
int myScore = -1;
int received[5] = { -1, -1, -1, -1, -1};
int finalScore = -1;
// -------------------

void setup() {
  // Serial setup
  Serial.begin(9600);
  Serial3.begin(9600);

  delay(500);
}

void loop() {
  while (finalScore == -1) {
    finalScore = communication(myScore);
  }

  switch (finalScore) {
    case 0:
      // Bronze - coordinated dance
      break;
    case 1:
    
  }
}

// Returns answer if done, or -1 if not done yet
int communication(int myScore) {
  // Sending



  // Receiving
  if (Serial3.available()) {
    char incoming = Serial3.read();

    switch (incoming) {
      // DIVING: a, b, and c
      case 'a':
        received[DIVING] = 0;
        break;
      case 'b':
        received[DIVING] = 1;
        break;
      case 'c':
        received[DIVING] = 2;
        break;

      // WEIGHTLIFTING: d, e, and f
      case 'd':
        received[WEIGHTLIFTING] = 0;
        break;
      case 'e':
        received[WEIGHTLIFTING] = 1;
        break;
      case 'f':
        received[WEIGHTLIFTING] = 2;
        break;

      // CYCLING: g, h, i
      case 'g':
        received[CYCLING] = 0;
        break;
      case 'h':
        received[CYCLING] = 1;
        break;
      case 'i':
        received[CYCLING] = 2;
        break;

      // BASKETBALL: j, k, l
      case 'j':
        received[BASKETBALL] = 0;
        break;
      case 'k':
        received[BASKETBALL] = 1;
        break;
      case 'l':
        received[BASKETBALL] = 2;
        break;

      // COURT_SPORTS: m, n, o
      case 'm':
        received[COURT_SPORTS] = 0;
        break;
      case 'n':
        received[COURT_SPORTS] = 1;
        break;
      case 'o':
        received[COURT_SPORTS] = 2;
        break;
    }
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

