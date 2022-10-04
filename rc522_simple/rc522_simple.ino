// Code for RFID scanner & LED 

#include <SPI.h>
#include <MFRC522.h>

#define SDA_DIO 53
#define RESET_DIO 49

MFRC522 mfrc522(SDA_DIO, RESET_DIO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(13, OUTPUT); // Green LED
  pinMode(12, OUTPUT); // Red LED

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
    digitalWrite(13, HIGH);
    delay(3000);
    digitalWrite(13, LOW);    
  } else {
    Serial.println("Authorized denied");
    Serial.println();
    digitalWrite(12, HIGH);
    delay(3000);
    digitalWrite(12, LOW);       
  }


}
