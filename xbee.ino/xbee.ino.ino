#define  Rx 15
#define Tx 14
void setup() {
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,INPUT);
  Serial.begin(9600);
  Serial3.begin(9600);
  
  delay(500);

}

void loop() {

  if (digitalRead(9)) { // transmitting
   digitalWrite(10,HIGH);
    //char outgoing = Serial.read();
    char outgoing = 's';
    Serial3.print(outgoing);
    
  }
  else{
    digitalWrite(10,LOW);
    }

  if (Serial3.available()) { // receiving
    digitalWrite(11,HIGH);
    
    char incoming = Serial3.read();
    Serial.println(incoming);
  }
  else{
    digitalWrite(11,LOW);
    }
  
  delay(50);

}
