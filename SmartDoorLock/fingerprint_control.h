#ifndef FINGERPRINT_CONTROL_H
#define FINGERPRINT_CONTROL_H

#include <Arduino.h>

// Enable scanning
void startFingerprintScanning();

// Disable scanning
void stopFingerprintScanning();

// Query scanning state
bool isFingerprintScanning();

#endif
