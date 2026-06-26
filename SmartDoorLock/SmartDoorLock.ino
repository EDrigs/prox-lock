#include "bluetooth_module.h"
#include "wifi_connect.h"
#include "client_handler.h"
#include "distance_sensor.h"
#include "fingerprint_module.h"
#include "set_time.h"
#include "keypad_module.h"
#include "oled_display.h"
#include "set_passcode.h"
#include "otp_manager.h"
#include "secrets.h"
#include "hash_util.h"
#include "auth_manager.h"
#include "fingerprint_control.h"
#include "servo_module.h"

#include <ArduinoHttpClient.h>
#include <FlashStorage.h>
#include <Keypad.h>

OLEDDisplayController oled;

enum InputMode {
  MODE_NORMAL,
  MODE_EDIT_PASSCODE
};

InputMode currentMode = MODE_NORMAL;

char keypad_buffer[MAX_PASSCODE_LEN + 1];
uint8_t keypad_index = 0;

char edit_buffer[MAX_PASSCODE_LEN + 1];
uint8_t edit_index = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  initServo();

  Wire.begin();
  if (!oled.begin()) {
    Serial.println("OLED initialization failed");
    while (1);
  }

  oled.displayClearStartup();

  bool wifiConnected = connectToWiFi();
  if (wifiConnected) {
    server.begin();
  } else {
    Serial.println("WiFi connection failed.");
  }

  initDistanceSensor();
  initKeypad();
}

void loop() {
  delay(50);

  WiFiClient client = server.available();
  if (client) handleClient(client);

  ProximityState state = checkProximity(70.0);

  switch (state) {
    case PROX_ENTERED:
      oled.displayWelcomeScreen();
      keepPowerBankAwake();
      break;

    case PROX_EXITED:
      oled.displayGoodbyeScreen();
      currentMode = MODE_NORMAL;
      keypad_index = 0;
      edit_index = 0;
      keypad_buffer[0] = '\0';
      edit_buffer[0] = '\0';
      lock();
      break;

    case PROX_INSIDE: {
      char key = getKeypadInput();

      if (key != NO_KEY) {
        if (currentMode == MODE_NORMAL) {
          static uint8_t asteriskCount = 0;

          if (key == '*') {
            asteriskCount++;
            if (keypad_index < MAX_PASSCODE_LEN) {
              keypad_buffer[keypad_index++] = key;
            }
          } else if (key == '#') {
            if (asteriskCount == 5) {
              oled.displayOTPSent();
              if (generateOTP()) {
                String otp = getStoredOTP();
                notifyServerOfOTP(otp.c_str());
              } else {
                Serial.println("[OTP] Failed to generate OTP.");
              }
              keypad_index = 0;
              keypad_buffer[0] = '\0';
              asteriskCount = 0;
              break;
            }

            if (asteriskCount >= 2) {
              currentMode = MODE_EDIT_PASSCODE;
              edit_index = 0;
              keypad_index = 0;
              asteriskCount = 0;
              keypad_buffer[0] = '\0';
              edit_buffer[0] = '\0';
              oled.displayEditModeEntered();
            } else {
              keypad_buffer[keypad_index] = '\0';
              if (authVerifyCode(keypad_buffer)) {
                oled.displayValidCode();
                unlock();
              } else {
                oled.displayInvalidCode();
              }
              keypad_index = 0;
              keypad_buffer[0] = '\0';
              asteriskCount = 0;
            }
          } else {
            asteriskCount = 0;
            if (keypad_index < MAX_PASSCODE_LEN) {
              keypad_buffer[keypad_index++] = key;
            }
          }
        } else if (currentMode == MODE_EDIT_PASSCODE) {
          if (key == '#') {
            edit_buffer[edit_index] = '\0';
            if (edit_index >= 4) {
              if (updatePasscode(String(edit_buffer))) {
                oled.displayCodeSaved();
              } else {
                oled.displayCodeSaveFailed();
              }
            } else {
              oled.displayCodeTooShort();
            }
            currentMode = MODE_NORMAL;
            edit_index = 0;
            keypad_index = 0;
            keypad_buffer[0] = '\0';
            edit_buffer[0] = '\0';
          } else if (key == '*') {
            currentMode = MODE_NORMAL;
            edit_index = 0;
            edit_buffer[0] = '\0';
            oled.displayEditCancelled();
          } else if (edit_index < MAX_PASSCODE_LEN) {
            edit_buffer[edit_index++] = key;
          }
        }
      }

      checkForExitCommand();
      break;
    }

    default:
      break;
  }
}
