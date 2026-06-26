#include "auth_manager.h"
#include "otp_manager.h"
#include "set_passcode.h"
#include "hash_util.h"

bool authVerifyCode(const char* input) {
    return authMatchesPasscode(input) || authMatchesOTP(input);
}

bool authMatchesPasscode(const char* input) {
    String storedHash = getStoredPasscode();
    if (storedHash.length() == 0) {
        Serial.println("[AUTH] No stored passcode hash found.");
        return false;
    }

    String inputHash = HashUtil::hashCode(String(input));
    bool match = (storedHash == inputHash);

    Serial.println(match ? "[AUTH] Passcode match." : "[AUTH] Passcode mismatch.");
    return match;
}

bool authMatchesOTP(const char* input) {
    String storedOtp = getStoredOTP();
    if (storedOtp.length() == 0) {
        Serial.println("[OTP] No OTP stored.");
        return false;
    }

    if (otpExpired()) {
        Serial.println("[OTP] OTP expired.");
        deleteOTP();
        return false;
    }

    char otpBuf[OTP_STORAGE_LEN];
    storedOtp.toCharArray(otpBuf, sizeof(otpBuf));

    if (strcmp(otpBuf, input) == 0) {
        Serial.println("[OTP] OTP match — unlocking.");
        deleteOTP();
        return true;
    }

    Serial.println("[OTP] OTP mismatch.");
    return false;
}
