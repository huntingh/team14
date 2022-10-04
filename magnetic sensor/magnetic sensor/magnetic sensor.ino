// Code for Magnetic Sensor and Lock

#define MAG 48
#define LOCK 11

void setup() {
  // put your setup code here, to run once:
  pinMode(MAG, INPUT_PULLUP);
  pinMode(LOCK, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(MAG) == HIGH) {
    Serial.println("Door opened");
    delay(1000);
    digitalWrite(LOCK, HIGH);
    while (digitalRead(MAG) == HIGH) {}
  }
  else {
    Serial.println("Door closed");
    delay(1000);
    digitalWrite(LOCK, LOW);
    while (digitalRead(MAG) == LOW) {}
  }
}
