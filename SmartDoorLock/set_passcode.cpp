#include "set_passcode.h"
#include "hash_util.h"
#include "config.h"
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <FlashStorage.h>

// Structure for flash storage
typedef struct {
  char hashedCode[MAX_PASSCODE_LEN];
} PasscodeStorage;

// Create flash storage region (persistent)
FlashStorage(passcode_flash, PasscodeStorage);

bool updatePasscode(String newCode) {
  String hashed = HashUtil::hashCode(newCode);
  String current = getStoredPasscode();

  if (hashed == current) {
    Serial.println("[INFO] New passcode matches current stored hash. No update needed.");
    return true;
  }

  if (savePasscode(hashed)) {
    Serial.println("[SUCCESS] Passcode updated and stored!");
    return true;
  } else {
    Serial.println("[ERROR] Failed to store new passcode!");
    return false;
  }
}

bool savePasscode(String hashedCode) {
  PasscodeStorage newData;
  memset(&newData, 0, sizeof(PasscodeStorage));   // CLEAR THE STRUCT

  hashedCode.toCharArray(newData.hashedCode, MAX_PASSCODE_LEN);

  passcode_flash.write(newData);

  Serial.println("[INFO] Passcode written to FlashStorage.");
  return true;
}


String getStoredPasscode() {
  PasscodeStorage data = passcode_flash.read();
  String stored = String(data.hashedCode);

  if (stored.length() == 0) {
    Serial.println("[WARN] No passcode found in flash.");
  } else {
    Serial.println("[INFO] Retrieved hashed passcode from flash.");
  }

  return stored;
}
