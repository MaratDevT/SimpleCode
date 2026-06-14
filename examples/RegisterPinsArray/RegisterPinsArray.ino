// Example: Registering an array of mixed pins.

#include "SimpleCode.h"

// List the pins in any random, custom, or non-sequential order
const uint8_t Pins[] = {5, 2, 9, 3, 6}; 
const uint8_t pinCount = sizeof(Pins) / sizeof(Pins[0]); // Automatically calculate the total number of pins in the array


void setup() {
  // Configure all pins inside the array as OUTPUT with a single function call
  // Arguments: (pinsArray, arraySize, mode)
  regPinsArray(Pins, pinCount, OUTPUT); 
}

void loop() {
  // Your code here
}