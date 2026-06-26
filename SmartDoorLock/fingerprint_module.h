#ifndef FINGERPRINT_MODULE_H
#define FINGERPRINT_MODULE_H

#include <Arduino.h>

// Initializes the fingerprint sensor
void initFingerprintModule();

// Runs one fingerprint identification attempt.
// Returns: fingerprint ID (>=0) on success, or -1 on failure.
int getFingerprintID();

// Optional: simplified fast-search version (same return values)
int getFingerprintIDez();

// Allows Serial 'exit' command to stop fingerprint checking loop (for testing)
void checkForExitCommand();

#endif
