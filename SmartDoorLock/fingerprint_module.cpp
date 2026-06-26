#include "fingerprint_module.h"
#include "fingerprint_control.h"
#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
  // For UNO and similar boards, use SoftwareSerial
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(2, 3);  // pin #2 is IN (green), #3 is OUT (white)
#else
  // For SAMD (Arduino Nano 33 IoT, etc.)
  #define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
String input;  // For reading 'exit' command

// Initializes the fingerprint sensor
void initFingerprintModule() {
  // Serial.begin(115200); 
  // while (!Serial);
  if (!Serial) {
    Serial.begin(115200);
  }
  delay(100);
  Serial.println("\n\nInitializing fingerprint sensor...");

  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
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

  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.println("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.print("Sensor contains "); 
    Serial.print(finger.templateCount); 
    Serial.println(" templates");
  }
}

// Check if user typed "exit" in Serial Monitor
void checkForExitCommand() {
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');  // read until Enter is pressed
    input.trim();
    if (input.equalsIgnoreCase("exit")) {
      Serial.println("Exiting fingerprint module...");
      while (1) {
        // Do nothing forever
      }
    }
  }
}

// Returns fingerprint ID (>=0) or -1 for any failure
int getFingerprintID() {
  if (!isFingerprintScanning()) {
    return -1;
  }
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return -1;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return -1;
  } else {
    Serial.println("Unknown error");
    return -1;
  }

  // Found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);

  return finger.fingerID;
}
