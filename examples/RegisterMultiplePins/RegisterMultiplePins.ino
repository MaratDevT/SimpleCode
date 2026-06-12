// Example: Registering a range of pins.

#include "SimpleCode.h" // Include the library

void setup() {
  // Configure pins from 2 to 6 as OUTPUT all at once
  // Arguments: (firstPin, lastPin, mode)
  regPins(2, 6, OUTPUT); 
}

void loop() {
  // Your code here
}
