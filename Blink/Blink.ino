const int led1 = 2; //what pin the LED is
const int button1 = 3;
int buttonState = 0;


void turn_on(int pin){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void turn_off(int pin){
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led1, OUTPUT);
  pinMode(button1, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  buttonState = digitalRead(button1);

  if (buttonState == HIGH) {
    turn_on(led1);
  } else {
    turn_off(led1);
  }

}
