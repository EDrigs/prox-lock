#ifndef SET_PASSCODE_H
#define SET_PASSCODE_H

#include <Arduino.h>

bool updatePasscode(String newCode);
bool savePasscode(String hashedCode);
String getStoredPasscode();

#endif
