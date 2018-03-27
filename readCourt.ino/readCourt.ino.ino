
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long qti1 = 0;
  long qti2 = 0;
  long qti3 = 0;

  //qti1=rcTime(4);
  qti2 = rcTime(5);
  //qti3=rcTime(6);
  print3vals(qti1,qti2,qti3);

  if (qti2 < 18)
  {
    Serial.println("GREY");
  }
  else if (18 <= qti2 && qti2 < 200)
  {
    Serial.println("WHITE");
  }
  else if (200 <= qti2)
  {
    Serial.println("BLACK");
  }

}
long rcTime(int pin) {
  /*(pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delayMicroseconds(230);
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
    long time = micros();
    while (digitalRead(pin)) {
    time = micros() - time;
    }
    return time;*/

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

