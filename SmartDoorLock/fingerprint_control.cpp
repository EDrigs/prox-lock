#include "fingerprint_control.h"

static bool scanningEnabled = false;

void startFingerprintScanning() {
    scanningEnabled = true;
}

void stopFingerprintScanning() {
    scanningEnabled = false;
}

bool isFingerprintScanning() {
    return scanningEnabled;
}
