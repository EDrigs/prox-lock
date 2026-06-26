#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

enum ProximityState {
  PROX_IDLE,      // default / no transition
  PROX_ENTERED,   // person just entered range
  PROX_EXITED,    // person just left range
  PROX_INSIDE,    // person still within range
  PROX_OUTSIDE    // person still out of range
};

void initDistanceSensor();
float getDistanceInches();
ProximityState checkProximity(float thresholdInches);

#endif
