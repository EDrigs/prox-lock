#ifndef SERVO_MODULE_H
#define SERVO_MODULE_H

#include <Arduino.h>

void initServo();
void unlock();   // Move servo to unlocked position (~90°)
void lock();     // Move servo to locked position (~0°)
void keepPowerBankAwake();

#endif
