const int led1 = 2; //what pin the LED is
// const int led3 = 14;
// bool buttonState = 0;


void turn_on(int pin){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void turn_off(int pin){
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led1, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    turn_on(led1);
    delay(1000); 
    turn_off(led1);
    delay(1000); 

}
