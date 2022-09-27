//Global Variable Definitions
//Pin Definitions
const int fr = 2; //Fingerprint Red
const int fg = 3; //Fingerprint Green
const int rr = 4; //RFID Red
const int rg = 5; //RFID Green
const int br = 6; //Battery Red
const int by = 7; //Battery Yellow
const int button1 = 8; //Reset Button

//Other Variable Definitions
int buttonState = 0;

//Global Functions Definitions
void turn_on_LED(int pin);
void turn_off_LED(int pin);
void read_button(int pin);

void turn_on_LED(int pin){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void turn_off_LED(int pin){
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
}

void setup() {
  pinMode(fr, OUTPUT);
  pinMode(fg, OUTPUT);
  pinMode(rr, OUTPUT);
  pinMode(rg, OUTPUT);
  pinMode(br, OUTPUT);
  pinMode(by, OUTPUT);
  pinMode(button1, INPUT);
}

void read_button(int pin){
  buttonState = digitalRead(pin);
}

void loop() {
  read_button(button1);
  if(buttonState == 1){
    turn_on_LED(fr);
    }
  else{
    turn_off_LED(fr);
    }
}
