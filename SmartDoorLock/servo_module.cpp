#include "servo_module.h"
#include <Servo.h>
#include "config.h"

static Servo lockServo;
static const int LOCK_POS   = 0;
static const int UNLOCK_POS = 90;
static int currentAngle = LOCK_POS;

void initServo() {
    lockServo.attach(SERVO_PIN);
    lockServo.write(currentAngle);
    delay(300);
}

void unlock() {
    currentAngle = UNLOCK_POS;
    lockServo.write(currentAngle);
    delay(300);
}

void lock() {
    currentAngle = LOCK_POS;
    lockServo.write(currentAngle);
    delay(300);
}

void keepPowerBankAwake() {
    // Power banks shut off under low draw; a brief nudge every 8 s keeps them active.
    static unsigned long lastNudge = 0;
    if (millis() - lastNudge > 8000) {
        lockServo.write(currentAngle + 2);
        delay(25);
        lockServo.write(currentAngle);
        lastNudge = millis();
    }
}
