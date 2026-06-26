// #ifndef OTP_MANAGER_H
// #define OTP_MANAGER_H

// #include <Arduino.h>

// // Public API
// bool generateOTP();
// String getStoredOTP();
// bool otpIsValid(const char* input);
// bool deleteOTP();
// bool otpExpired();
// unsigned long otpGetTimestamp();
// bool otpCanRequestAgain();
// bool storeOTP(const String &otp);

// void notifyServerOfOTP(const char* otp);

// #endif

#ifndef OTP_MANAGER_H
#define OTP_MANAGER_H

#include <Arduino.h>

bool generateOTP();
bool storeOTP(const String &otp);
String getStoredOTP();
bool deleteOTP();

bool otpIsValid(const char* input);
bool otpExpired();
unsigned long otpGetTimestamp();

bool otpCanRequestAgain();
void otpSeedRandom();  // Exposed for early seeding if needed
void notifyServerOfOTP(const char* otp);

#endif
