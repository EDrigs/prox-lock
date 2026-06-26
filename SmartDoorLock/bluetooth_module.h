#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include <ArduinoBLE.h>

// Initialize BLE and start advertising
void setupBLE();

// Continuously handle connection state and LED feedback
void handleBLEConnection();

#endif
