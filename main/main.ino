//Imports
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>

//Global Variable Definitions
#define SDA_DIO 53
#define RESET_DIO 49
#define RX_PIN 10 //RX IN from sensor (Green)
#define TX_PIN 9 //TX OUT from Arduino (White)

//Pin Definitions
const int fr = 3; //Fingerprint Red
const int fg = 4; //Fingerprint Green
const int rr = 5; //RFID Red
const int rg = 6; //RFID Green
const int br = 8; //Battery Red
const int by = 7; //Battery Yellow
const int button1 = 2; //Reset Button
const int lock = 11;
const int mag = 48;
uint8_t id;
bool flag = true;

//Other Variable Definitions
int buttonState = 0;
int exit_call = 0;
int magState = 0;
MFRC522 mfrc522(SDA_DIO, RESET_DIO);
SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// uint8_t id = 0;
float voltage = 0;

//Global Functions Definitions
void turn_on_LED(int pin);
void turn_off_LED(int pin);
void read_button(int pin);
uint8_t readnumber(void);
uint8_t getFingerprintID(void);
void batteryStatusLED(void);
void findVoltage(void);
int scan_for_card(void);
int validate_card_serial(void);
void openLock(void);
void closeLock(void);
uint8_t getFingerprintEnroll(void);

void turn_on_LED(int pin){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void turn_off_LED(int pin){
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
              }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    exit_call = 1;
    //finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
    //finger.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 10);
    //should add part to open lock 
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    turn_on_LED(fr);
    delay(3000);
    turn_off_LED(fr); 
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  turn_on_LED(fg);
  delay(3000);
  turn_off_LED(fg);

  closeLock();
  delay(3000); 
  magState = digitalRead(mag); 
  while(magState == HIGH){
    read_button(button1);
    if(buttonState == 1){
       while (!  getFingerprintEnroll() );
    }
    delay(2000);
    magState = digitalRead(mag); 
  }
  openLock();
  Serial.print("Here mother forker ");
  return finger.fingerID;
}

void batteryStatusLED(){
  if(voltage<=2.75){ //2.75
    turn_off_LED(by);
    turn_on_LED(br);
  }else if(voltage <= 3.75){ //3.75
    turn_off_LED(br);
    turn_on_LED(by);
  }
  else{
    turn_off_LED(by);
    turn_off_LED(br);
  }
}
void findVoltage()
{
  float Total = 0;
  float average_value = 0;
  float total_value = 0;

  int sensorValue = analogRead(A15); //read the A0 pin value
  voltage = sensorValue * (5 / 1023.00); //convert the value to a true voltage.
  Serial.print(" Voltage: ");
  Serial.println(voltage);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(fr, OUTPUT);
  pinMode(fg, OUTPUT);
  pinMode(rr, OUTPUT);
  pinMode(rg, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(mag, INPUT_PULLUP);
  pinMode(br, OUTPUT);
  pinMode(by, OUTPUT);
  pinMode(button1, INPUT);
  while(!Serial)
  delay(100);
  Serial.println("\n\nFingerprint Scanner Finger detect test");
  finger.begin(57600);
  delay(5);
  if(finger.verifyPassword()){
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor :");
    while(1) {
      delay(1);
    }
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
}
}

void read_button(int pin){
  buttonState = digitalRead(pin);
}

int scan_for_card(){
  if ( !mfrc522.PICC_IsNewCardPresent()) { //If a card isn't present, return 0
    return 0;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) { //If it can't read the card, return 0
    return 0;
  }  

  return 1; //This only happens when it passes both if's
}
int validate_card_serial(){
  Serial.print("Input RFID Tag Serial Number: ");
  String content= "";
  byte letter;
  for (byte i =0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ?  " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ?  " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase();
  if (content.substring(1) == "13 BD 81 1F") {
    Serial.println("Authorized access");
    Serial.println();
    return 1;
    
  } else {
    Serial.println("Authorized denied");
    Serial.println();
    return 0;
  }

}

  void openLock(){
    digitalWrite(lock,LOW);
  }
  void closeLock(){
    digitalWrite(lock,HIGH);    


  }

void loop() {
  Serial.println("Loopificate");
  exit_call = 0;

  // while(1){
  //   magState = digitalRead(mag); 
  //   if(magState == HIGH){
  //     turn_on_LED(rr);
  //     delay(3000);
  //     turn_off_LED(rr);
  //   }
  //   else if(magState == LOW){
  //     turn_on_LED(rg);
  //     delay(3000);
  //     turn_off_LED(rg);
  //   }
  //   delay(1000);    
  // }


 if(scan_for_card()){//If I found a card, I will search for the serial. If not, the loop restarts. 
      if(!validate_card_serial()){ //If we can't read the serial
          turn_on_LED(rr);
          delay(3000);
          turn_off_LED(rr); 
          return; //Start void loop over
      }
    }
  else{
    delay(1000); //Wait 1/10 of a second
    return; //Start void loop over
  }
  int validated = validate_card_serial();
  if(validated){
    turn_on_LED(rg);
    delay(3000);
    turn_off_LED(rg); 
    long int t1 = millis();
    long int time = 0;
    while(time <= 10000) {
      Serial.println(time);
      Serial.println();    
      getFingerprintID();
      Serial.println("Over here.");

      if(exit_call == 1){
        break;
      }
      time = millis() - t1;
    }
  }
  else{
    turn_on_LED(rr);
    delay(3000);
    turn_off_LED(rr); 
    return;
  }
}



uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}