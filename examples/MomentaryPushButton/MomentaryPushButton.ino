#include "SimpleCode.h"

// Example: Controlling an LED using a momentary push button
// This sketch demonstrates non-blocking LED blinking triggered by button states and events.

// Connect a momentary push button to digital pin 2
Switch myButton(2);

// Connect an LED to digital pin 13
LED myLed(13);

// Flag to track whether the LED should blink
bool blinkState = false;

void setup() {
  // Initialize hardware components
  myButton.begin();
  myLed.begin();
}

void loop() {
  // Enable blinking if the button is currently being held down
  if (myButton.isPressed()){
    blinkState = true;
  }

  // Turn off blinking when a single click event is detected
  if (myButton.wasPressed()){
    myLed.stopBlink();
    blinkState = false;
  }

  // Maintain non-blocking blink pattern if the state is active
  if (blinkState == true){
    myLed.blink(100, 1000);
  }
}