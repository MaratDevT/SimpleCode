// A simple example showing how to use the Button & LED classes.

#include "SimpleCode.h" // Include the library

// Create a button object on pin 2 with a 50ms debounce time
Button myButton(2, 50); 
// Create an LED object on pin 3
LED myLed(3); 

void setup() {
  // Setup is empty; the classes configure the pins automatically.
}

void loop() {
  // Check if the button has been clicked
  if (myButton.isPressed()) { 
    myLed.toggle(); // Toggle the LED state
  }
}