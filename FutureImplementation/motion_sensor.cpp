#include "motion_sensor.h"

static const int PIR_PIN = 14;

static bool lastMotionState = LOW;

void initMotionSensor() {
  pinMode(PIR_PIN, INPUT);
  delay(2000);
  Serial.println("Motion Sensor Initialized");
}

bool isMotionDetected() {
  bool motion = digitalRead(PIR_PIN);
  lastMotionState = motion;
  return motion == HIGH;
}
