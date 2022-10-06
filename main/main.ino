//Imports
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//Global Variable Definitions
#define SDA_DIO 53
#define RESET_DIO 49
#define RX_PIN 10 //RX IN from sensor (Green)
#define TX_PIN 9 //TX OUT from Arduino (White)

//Pin Definitions
const int fr = 7; //Fingerprint Red
const int fg = 8; //Fingerprint Green
const int rr = 5; //RFID Red
const int rg = 6; //RFID Green
const int br = 3; //Battery Red
const int by = 4; //Battery Yellow
const int button1 = 2; //Reset Button

//Other Variable Definitions
int buttonState = 0;
int exit_call = 0;
MFRC522 mfrc522(SDA_DIO, RESET_DIO);
SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

//Global Functions Definitions
void turn_on_LED(int pin);
void turn_off_LED(int pin);
void read_button(int pin);
uint8_t readnumber(void);
int getFingerprintIDez(void);
uint8_t getFingerprintID(void);

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

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
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
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  digitalWrite(5, HIGH);
  delay(5000);
  digitalWrite(5, LOW);
  return finger.fingerID;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(fr, OUTPUT);
  pinMode(fg, OUTPUT);
  pinMode(rr, OUTPUT);
  pinMode(rg, OUTPUT);
  // pinMode(br, OUTPUT);
  // pinMode(by, OUTPUT);
  // pinMode(button1, INPUT);
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

void loop() {
  // put your main code here, to run repeatedly:
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

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
    turn_on_LED(rg);
    delay(3000);
    turn_off_LED(rg); 
  } else {
    Serial.println("Authorized denied");
    Serial.println();
    turn_on_LED(rr);
    delay(3000);
    turn_off_LED(rr);  
  }

  while (exit_call != 1){
    getFingerprintID();
    delay(50); 
  }

  // read_button(button1);
  // if(buttonState == 1){
  //   turn_on_LED(fr);
  //   }
  // else{
  //   turn_off_LED(fr);
  //   }
}
