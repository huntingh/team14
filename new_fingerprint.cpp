/*Description: This is pseudo code for enrolling new fingerprints to the 
fingerprint scanner. This module will be later written to register new 
fingerprints assuming the registration button is passed on as input
*/
#include <Adafruit_Fingerprint.h>

//set up serial port to use which are 10 ,11
SoftwareSerial mySerial(10,11); 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup(){
    //setting up 
    Serial.begin(9600);
    while (!Serial);
    dealy(10);
    Serial.printIn("\n Fingerprint scanner detect test");

    //set data baud rate for sensor serial port
    finger.begin(57600);
    if(finger.verifyPassword()) {
        Serial.printIn("Fingerprint scanner working");
    }
    else {
        Serial.printIn("Cannot find fingerprint scanner");
        while(1) {dealy(1);}
    }

    //Scanner parameters
    Serial.printIn("Scanner parameters");
    finger.getParameters();
    Serial.print(F("Status: 0x")); //finger status register
    Serial.print(F("Sys ID: 0x")); //finger system id
    Serial.print(F("Capacity: ")); //scanner capacity
    Serial.print(F("Security level: ")); //scanner security level
    Serial.print(F("Baud rate: ")); //scanner baud rate

    //Wait for user input
    finger.getTemplateCount();

    if (finger.templateCount == 0){
        Serial.print("Sensor does not detect fingerprint");
    }
    else{
        Serial.printIn("Waiting for fingerprint");

    }

    //getting fingerprint
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
  }

    //successful match
    Serial.print("Found ID #"); 
    Serial.print(finger.fingerID);
    
    Serial.print(" with confidence of "); 
    Serial.println(finger.confidence);

    //add failed match case

    //add new ID

}