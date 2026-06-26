#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <Arduino.h>

bool authVerifyCode(const char* input);
bool authMatchesPasscode(const char* input);
bool authMatchesOTP(const char* input);

#endif