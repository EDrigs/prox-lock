#ifndef CONFIG_H
#define CONFIG_H

// Distance sensor
#define TRIG_PIN 2
#define ECHO_PIN 3
#define REQUIRED_MISSES 10

// Servo
#define SERVO_PIN 13

// Keypad
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
#define KEYPAD_KEYS { \
  {'1','2','3'}, \
  {'4','5','6'}, \
  {'7','8','9'}, \
  {'*','0','#'}, \
}
#define ROW_PINS {4, 5, 6, 7}
#define COL_PINS {8, 9, 10}

// OLED display
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS 0x3C

// Flash storage
#define MAX_PASSCODE_LEN 70
#define OTP_STORAGE_LEN   7
#define OTP_MAX_LEN       8

// OTP timing
#define OTP_VALID_DURATION   (5UL  * 60UL * 1000UL)   // 5 minutes
#define OTP_REQUEST_INTERVAL (10UL * 60UL * 1000UL)   // 10 minutes

#endif
