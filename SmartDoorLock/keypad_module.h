#ifndef KEYPAD_MODULE_H
#define KEYPAD_MODULE_H

#include <Arduino.h>

// Initializes the 4x4 keypad
void initKeypad();

// Reads a key press; returns '\0' if none
char getKeypadInput();

#endif
