#include "otp_manager.h"
#include "config.h"
#include "secrets.h"

#include <WiFiNINA.h>
#include <FlashStorage.h>
#include <Arduino.h>

typedef struct {
    char otp[7];           // 6 digits + null terminator
    unsigned long timestamp;
} OTPData;

FlashStorage(otpFlashStorage, OTPData);
static OTPData otpData;

static unsigned long lastOtpRequestTime = 0;

static void seedRandom() {
    byte mac[6];
    WiFi.macAddress(mac);

    unsigned long seed =
        ((unsigned long)mac[3] << 24) |
        ((unsigned long)mac[4] << 16) |
        ((unsigned long)mac[5] <<  8) |
        (unsigned long)(micros() & 0xFF);

    OTPData prev = otpFlashStorage.read();
    seed ^= prev.timestamp;

    randomSeed(seed);
}

static String generateNumericOTP(uint8_t digits = 6) {
    String otp = "";
    for (uint8_t i = 0; i < digits; i++) {
        otp += char('0' + random(0, 10));
    }
    return otp;
}

bool generateOTP() {
    if (!otpCanRequestAgain()) {
        Serial.println("[OTP] Request blocked by rate limit.");
        return false;
    }

    seedRandom();
    String otp = generateNumericOTP(6);

    otp.toCharArray(otpData.otp, 7);
    otpData.timestamp = millis();
    otpFlashStorage.write(otpData);
    lastOtpRequestTime = millis();

    Serial.println("[OTP] New OTP generated.");
    return true;
}

bool storeOTP(const String &otp) {
    if (otp.length() != 6) return false;

    otp.toCharArray(otpData.otp, 7);
    otpData.timestamp = millis();
    otpFlashStorage.write(otpData);

    Serial.println("[OTP] OTP stored from server.");
    return true;
}

String getStoredOTP() {
    otpData = otpFlashStorage.read();
    return String(otpData.otp);
}

bool deleteOTP() {
    memset(otpData.otp, 0, 7);
    otpData.timestamp = 0;
    otpFlashStorage.write(otpData);
    return true;
}

unsigned long otpGetTimestamp() {
    otpData = otpFlashStorage.read();
    return otpData.timestamp;
}

bool otpExpired() {
    otpData = otpFlashStorage.read();
    if (strlen(otpData.otp) == 0) return true;

    if (millis() - otpData.timestamp > OTP_VALID_DURATION) {
        deleteOTP();
        return true;
    }

    return false;
}

bool otpIsValid(const char* input) {
    otpData = otpFlashStorage.read();
    if (strlen(otpData.otp) == 0) return false;
    if (otpExpired()) return false;

    if (strcmp(otpData.otp, input) == 0) {
        deleteOTP();
        return true;
    }

    return false;
}

bool otpCanRequestAgain() {
    return (millis() - lastOtpRequestTime) >= OTP_REQUEST_INTERVAL;
}

void notifyServerOfOTP(const char* otp) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[OTP] WiFi not connected — cannot notify server.");
        return;
    }

    String host = SMART_LOCK_SERVER_IP;
    int port = SMART_LOCK_SERVER_PORT;

    WiFiClient client;
    if (!client.connect(host.c_str(), port)) {
        Serial.println("[OTP] Unable to reach server.");
        return;
    }

    String json =
        String("{\"otp\":\"") + otp +
        "\",\"secret\":\"" + API_SECRET + "\"}";

    client.println("POST /relay_otp HTTP/1.1");
    client.print("Host: ");
    client.print(host);
    client.print(":");
    client.println(port);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println("Connection: close");
    client.println();
    client.print(json);
    client.flush();

    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 7000) {
        if (client.available()) {
            client.readStringUntil('\n'); // drain response
        }
    }

    client.stop();
    Serial.println("[OTP] Relay complete.");
}
