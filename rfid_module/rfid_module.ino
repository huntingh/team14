/*
This code is not the final version.
It contains the actual code AND pseudocode of RFID module implementation.
*/
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::MIFARE_Key key; // MIFARE_Key struct 'key'. This holds the card information

int block = 2; // block number

byte blockcontent[16] = {"ECE49022_Project"}; // an array of 16 bytes to be written into one of 64 card blocks defined
byte readbackblock[18];

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC with 115,200 baud rate
  while (!Serial); // Do nothing if no serial port is opened
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println("Scan a RFID card/tag");
  
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Reset the loop if no new card is presented on the scanner.
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Select one of the cards
  }
  Serial.println("Card selected");

  /*
  TODO:
  Instructions for reading and writing data of the card
  */

}

int writeBlock(int blockNum, byte arr[]) {
  // TODO
  return;
}

int readBlock(int blockNum, byte arr[]) {
  // TODO
  return;
}
