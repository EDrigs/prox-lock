#ifndef SET_TIME_H
#define SET_TIME_H

// Initializes NTP client and sets the RTC from network time
void setTimeFromNTP();

// Prints the current time from the RTC to Serial
void printCurrentTime();

#endif
