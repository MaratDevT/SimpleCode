#include "SimpleCode.h"

// NOTE: This example is specifically designed for Toggle / Latching switches (ON/OFF).
// Create a Switch object on pin 2
Switch myButton(2);

// Create an LED object on pin 13
LED myLED(13);

void setup() {
  // Initialize both the switch and LED
  myButton.begin();
  myLED.begin();
}

void loop() {
  // Check if the toggle switch is currently flipped UP (active / closed to GND)
  if (myButton.isPressed()) {
    // Blinks asynchronously without blocking the loop
    myLED.blink(500, 500); 
  } else {
    // When the toggle switch is flipped DOWN, stop blinking immediately
    myLED.stopBlink(); 
  }
}
