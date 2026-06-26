#include "distance_sensor.h"
#include "config.h"
#include <Arduino.h>

static const int trigPin = TRIG_PIN;
static const int echoPin = ECHO_PIN;

static bool lastDetected = false;
static int consecutiveMisses = 0;

void initDistanceSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delay(1000);
}

float getDistanceInches() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration / 148.0;
}

ProximityState checkProximity(float thresholdInches) {
  float distance = getDistanceInches();
  bool currentlyDetected = (distance > 0 && distance <= thresholdInches);

  if (currentlyDetected) {
    consecutiveMisses = 0;
    if (!lastDetected) {
      lastDetected = true;
      return PROX_ENTERED;
    }
    return PROX_INSIDE;
  }

  if (lastDetected) {
    consecutiveMisses++;
    if (consecutiveMisses >= REQUIRED_MISSES) {
      lastDetected = false;
      consecutiveMisses = 0;
      return PROX_EXITED;
    }
    // Transient miss — treat as still inside
    return PROX_INSIDE;
  }

  return PROX_OUTSIDE;
}
