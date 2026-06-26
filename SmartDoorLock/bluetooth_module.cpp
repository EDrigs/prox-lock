#include "bluetooth_module.h"

const int ledPin = LED_BUILTIN; // onboard LED

// Define BLE service globally so it persists
BLEService smartLockService("19B10000-E8F2-537E-4F6C-D104768A1214");

void setupBLE() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Starting BLE...");

  if (!BLE.begin()) {
    Serial.println("Failed to start Bluetooth® Low Energy!");
    while (1);
  }

  // Configure BLE advertising details
  BLE.setLocalName("SmartLockBLE");
  BLE.setAdvertisedService(smartLockService);
  BLE.addService(smartLockService);

  // Start advertising
  BLE.advertise();

  Serial.println("BLE advertising started. Waiting for connection...");
}

void handleBLEConnection() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    Serial.println();

    digitalWrite(ledPin, HIGH); // LED solid ON while connected

    while (central.connected()) {
      BLE.poll(); // keep BLE active during connection
      delay(100);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

  // Blink LED while not connected
  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  delay(300);
}
